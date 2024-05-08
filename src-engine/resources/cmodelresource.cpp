#include "cmodelresource.hpp"

#include "modules/clogmodule.hpp"
#include "modules/crendermodule.hpp"
#include "modules/cfilesystem.hpp"

#include "modules/render/cmesh.hpp"

#include "resources/textureresource.hpp"

#include "global.hpp"

#include <string>
#include <vector>

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

ModelResourceFactory::ModelResourceFactory()
{
}

ModelResourceFactory::~ModelResourceFactory()
{
}

TSResult ModelResourceFactory::CreateResourceFromStream(IFileStream* stream, IResource*& resource, ResourceLoadOptions* options)
{
	CTSModelResource* res_model = new CTSModelResource();

	TSResult result = res_model->LoadFromStream(stream, (ModelResourceLoadOptions*)(void*)options);

	resource = res_model;

	return result;
}

CTSModelResource::CTSModelResource()
{
	SetType(RESOURCE_TYPE_MODEL);
}

CTSModelResource::~CTSModelResource()
{
	for (auto& texture : textures)
		g_ResourceManager->DecrementRefResource(texture);

	for (auto& lod : model.lods)
		for (auto& primitive : lod.primitives)
			g_MeshManager->DeleteMesh(primitive.mesh);
}

TSModel& CTSModelResource::GetData()
{
	return model;
}

#include "modules/render/materials/mstsstandard.hpp"

glm::vec4 ReadVector4(IFileStream* stream)
{
	float x, y, z, w;

	x = stream->ReadFloat();
	y = stream->ReadFloat();
	z = stream->ReadFloat();
	w = stream->ReadFloat();

	return glm::vec4(x, y, z, w);
}

#include <filesystem>

TSResult CTSModelResource::LoadFromStream(IFileStream* stream, ModelResourceLoadOptions* options)
{
	std::string root_path = options->RootPath;

	if (root_path == "")
		root_path = IFileSystem::GetDirectoryName(stream->GetPath());

	int model_version = stream->ReadInt32();

	if (model_version != TS_MODEL_VERSION)
		return TS_MISMATCH_VERSION;

	int matrices_count = stream->ReadInt32();

	for (int iMat = 0; iMat < matrices_count; iMat++)
	{
		glm::mat4 mat(1.0f);

		stream->ReadBytes((char*)&mat[0][0], 64);

		model.matrices.push_back(mat);
	}

	for (int iMat = 0; iMat < matrices_count; iMat++)
		model.matrices_name.push_back(stream->ReadString(stream->ReadUInt16()));

	int lods_count = stream->ReadInt32();

	unsigned int sort_index = 0;

	for (int iLod = 0; iLod < lods_count; iLod++)
	{
		TSModel_Lod lod;

		lod.distance = stream->ReadFloat();
		lod.sphere_radius = stream->ReadFloat();

		int subobjects_count = stream->ReadInt32();

		for (int iSubObj = 0; iSubObj < subobjects_count; iSubObj++)
		{
			int vertices_count = stream->ReadInt32(); // vertex count, not float

			// 3 xyz pos, 3 xyz normal, 2 uv

			float* vertices = new float[vertices_count * (3 + 3 + 2)];

			stream->ReadBytes((char*)vertices, vertices_count * (3 + 3 + 2) * sizeof(float));

			int primitives_count = stream->ReadInt32();

			for (int iPrim = 0; iPrim < primitives_count; iPrim++)
			{
				TSModel_Primitive primitive;

				primitive.iHierarchy = stream->ReadInt32();

				primitive.sort_index = sort_index;

				int triangles_count = stream->ReadInt32();

				USHORT* indices = new USHORT[triangles_count];

				stream->ReadBytes((char*)indices, triangles_count * sizeof(USHORT));

				auto texture_name = stream->ReadString(stream->ReadUInt16());
				auto material_name = stream->ReadString(stream->ReadUInt16());

				int mat_options = stream->ReadInt32();

				primitive.mesh = g_Render->GetMeshManager()->CreateMesh();

				primitive.mesh->SetName(model.matrices_name[primitive.iHierarchy]);
				primitive.mesh->SetData(vertices, vertices_count, indices, triangles_count);

				if (texture_name != "")
				{
					auto texture_path = g_FileSystem->FindResourcePath(texture_name, root_path, options->TextureLoadFrom);

					auto texture_load_options = TextureResourceLoadOptions();

					TSTextureResource* texture = g_ResourceManager->LoadResource<TSTextureResource>(texture_path, RESOURCE_LOAD_FLAG_DEFAULT, &texture_load_options);

					primitive.material = g_Render->GetMaterialManager()->CreateMaterial(material_name, "msts_standard_shader");

					if (texture != NULL)
					{
						if (std::find(textures.begin(), textures.end(), texture) == textures.end())
						{
							g_ResourceManager->IncrementRefResource(texture);

							textures.push_back(texture);
						}

						primitive.material->SetTexture(MATERIAL_ALBEDO_TEXTURE, texture->GetData());
						primitive.material->SetInt(MATERIAL_MSTS_OPTIONS, mat_options);
					}
					else
					{
						primitive.material->SetTexture(MATERIAL_ALBEDO_TEXTURE, g_Render->GetGrayTexture());
						primitive.material->SetInt(MATERIAL_MSTS_OPTIONS, 0);
					}
				}
				else
				{
					primitive.material = g_Render->GetMaterialManager()->CreateMaterial(material_name, "msts_standard_shader");
					primitive.material->SetTexture(MATERIAL_ALBEDO_TEXTURE, g_Render->GetGrayTexture());
					primitive.material->SetInt(MATERIAL_MSTS_OPTIONS, 0);
				}


				delete[] indices;

				sort_index++;

				lod.primitives.push_back(primitive);
			}

			delete[] vertices;
		}

		int hierarchy_count = stream->ReadInt32();

		for (int iHier = 0; iHier < hierarchy_count; iHier++)
			lod.hierarchy.push_back(stream->ReadInt32());

		model.lods.push_back(lod);
	}

	if (stream->GetSize() == stream->Tell())
	{
		g_Log->LogError("model has no animations %s", stream->GetFilename().c_str());

		return TS_OK;
	}

	int anim_count = stream->ReadInt32();

	for (int iAnim = 0; iAnim < anim_count; iAnim++)
	{
		TSAnimation anim;

		anim.frame_count = stream->ReadInt32();
		anim.frame_rate = stream->ReadInt32();

		int nodes_count = stream->ReadInt32();

		for (int iNode = 0; iNode < nodes_count; iNode++)
		{
			TSAnimNode node;

			node.name = stream->ReadString(stream->ReadUInt16());

			int controllers_count = stream->ReadInt32();

			for (int iCont = 0; iCont < controllers_count; iCont++)
			{
				TSController controller;

				int controller_type = stream->ReadInt8();

				int key_count = stream->ReadInt32();

				for (int iKey = 0; iKey < key_count; iKey++)
				{
					TSKeyPosition key;

					key.frame = stream->ReadInt32();
					key.type = (TSKeyPositionType)stream->ReadUInt8();

					key.x = stream->ReadFloat();
					key.y = stream->ReadFloat();
					key.z = stream->ReadFloat();

					if (key.type == TS_KEY_POSITION_SLERP_ROT || key.type == TS_KEY_POSITION_TCB_KEY)
						key.w = stream->ReadFloat();

					controller.key_positions.push_back(key);
				}


				node.controllers.push_back(controller);
			}

			anim.anim_nodes.push_back(node);
		}

		model.animations.push_back(anim);
	}

	return TS_OK;
}
