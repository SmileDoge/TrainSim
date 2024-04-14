#include "cmodelresource.hpp"

#include "modules/clogmodule.hpp"
#include "modules/crendermodule.hpp"
#include "modules/cfilesystem.hpp"

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
}

TSModel& CTSModelResource::GetData()
{
	return model;
}

#include "modules/render/materials/mstsstandard.hpp"

/*
struct TSModel_Primitive
{
	int iHierarchy;
	IMesh* mesh;
	IMaterial* material;
};

struct TSModel_Lod
{
	float distance;
	std::vector<TSModel_Primitive> primitives;
};

struct TSModel
{
	std::vector<TSModel_Lod> lods;
	std::vector<glm::mat4> matrices;
	std::vector<std::string> matrices_name;
};
*/

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

	int matrices_count = stream->ReadInt32();

	for (int iMat = 0; iMat < matrices_count; iMat++)
	{
		/*
		glm::vec4 row1 = ReadVector4(stream);
		glm::vec4 row2 = ReadVector4(stream);
		glm::vec4 row3 = ReadVector4(stream);
		glm::vec4 row4 = ReadVector4(stream);

		glm::mat4 mat(row1, row2, row3, row4);
		*/
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
		TSModel_Lod lod = TSModel_Lod();

		lod.distance = stream->ReadFloat();
		//lod.sphere_radius = stream->ReadFloat();

		int subobjects_count = stream->ReadInt32();

		for (int iSubObj = 0; iSubObj < subobjects_count; iSubObj++)
		{
			int vertices_count = stream->ReadInt32(); // vertex size, not float

			float* vertices = new float[vertices_count * (3 + 3 + 2)];

			int readed = stream->ReadBytes((char*)vertices, vertices_count * (3 + 3 + 2) * 4);

			int primitives_count = stream->ReadInt32();

			for (int iPrim = 0; iPrim < primitives_count; iPrim++)
			{
				TSModel_Primitive primitive = TSModel_Primitive();

				primitive.iHierarchy = stream->ReadInt32();

				primitive.sort_index = sort_index;

				int triangles_count = stream->ReadInt32();

				USHORT* indices = new USHORT[triangles_count];

				stream->ReadBytes((char*)indices, triangles_count * sizeof(USHORT));

				auto texture_name = stream->ReadString(stream->ReadUInt16());
				auto material_name = stream->ReadString(stream->ReadUInt16());

				int mat_options = stream->ReadInt32();

				primitive.mesh = g_Render->GetMeshManager()->CreateMesh();

				primitive.mesh->SetName(stream->GetFilename() + model.matrices_name[primitive.iHierarchy]);
				primitive.mesh->SetData(vertices, vertices_count, indices, triangles_count);

				if (texture_name != "")
				{
					auto texture_path = g_FileSystem->FindResourcePath(texture_name, root_path, options->TextureLoadFrom);

					auto texture_load_options = TextureResourceLoadOptions();

					TSTextureResource* texture = g_ResourceManager->LoadResource<TSTextureResource>(texture_path, RESOURCE_LOAD_FLAG_DEFAULT, &texture_load_options);

					primitive.material = g_Render->GetMaterialManager()->CreateMaterial(material_name, "msts_standard_shader");

					if (texture != NULL)
					{
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


	// create model


	/*
	for (auto& lod_file : _model.lods)
	{
		TSModel_Lod lod = TSModel_Lod();

		lod.distance = lod_file.distance;

		lod.hierarchy = lod_file.hierarchy;

		for (auto& subobject_file : lod_file.subobjects)
		{
			auto& vertices = subobject_file.vertices;

			
			std::vector<float> vertices_float;

			for (auto& vertex : vertices)
			{
				vertices_float.push_back(vertex.position.x);
				vertices_float.push_back(vertex.position.y);
				vertices_float.push_back(vertex.position.z);

				vertices_float.push_back(vertex.normal.x);
				vertices_float.push_back(vertex.normal.y);
				vertices_float.push_back(vertex.normal.z);

				vertices_float.push_back(vertex.texcoord.x);
				vertices_float.push_back(vertex.texcoord.y);
			}

			float* vertices_float = reinterpret_cast<float*>(vertices.data());

			for (auto& primitive_file : subobject_file.primitives)
			{
				auto& indices = primitive_file.triangles;
			
				TSModel_Primitive primitive = TSModel_Primitive();

				primitive.iHierarchy = primitive_file.iHierarchy;

				primitive.mesh = g_Render->GetMeshManager()->CreateMesh();

				primitive.mesh->SetName(model.matrices_name[primitive.iHierarchy]);
				primitive.mesh->SetData(vertices_float, vertices.size(), indices.data(), indices.size());
				//primitive.mesh->SetData(vertices_float.data(), vertices.size(), indices.data(), indices.size());

				auto mat_path = IFileSystem::GetFullPath(IFileSystem::Combine(IFileSystem::GetDirectoryName(stream->GetPath()), primitive_file.material)).substr(1);

				//g_Log->LogWarn("material: %s", mat_path.c_str());

				//IResource* texture_resource;

				//TSResult result = g_ResourceManager->LoadResourceInternal(mat_path, RESOURCE_LOAD_FLAG_DEFAULT, texture_resource);
				TSTextureResource* texture = g_ResourceManager->LoadResource<TSTextureResource>(mat_path, RESOURCE_LOAD_FLAG_DEFAULT);

				ITexture* real_texture = texture->GetData();

				primitive.material = g_Render->GetMaterialManager()->CreateMaterial(primitive_file.material);

				primitive.material->SetShader(g_Render->GetShader("exampleshader"));
				primitive.material->SetTexture(MATERIAL_ALBEDO_TEXTURE, real_texture);

				lod.primitives.push_back(primitive);
			}
		} 

		model.lods.push_back(lod);
	}
	*/

	/*
	int lods_count = stream->ReadInt32();

	for (int iLod = 0; iLod < lods_count; iLod++)
	{
		TSModel_Lod lod = TSModel_Lod();

		lod.distance = stream->ReadFloat();

		int subobjects_count = stream->ReadInt32();

		for (int iSubObj = 0; iSubObj < subobjects_count; iSubObj++)
		{
			TSModel_SubObject subobject = TSModel_SubObject();

			int primitives_count = stream->ReadInt32();

			for (int iPrim = 0; iPrim < primitives_count; iPrim++)
			{
				TSModel_Primitive primitive = TSModel_Primitive();

				primitive.iHierarchy = stream->ReadInt32();
				
				int triangles_count = stream->ReadInt32();

				for (int iTrig = 0; iTrig < triangles_count; iTrig++)
					primitive.triangles.push_back(stream->ReadUInt16());

				primitive.material = stream->ReadString(stream->ReadUInt16());
				primitive.options = stream->ReadInt32();

				subobject.primitives.push_back(primitive);
			}

			int vertices_count = stream->ReadInt32();

			for (int iVert = 0; iVert < vertices_count; iVert++)
			{
				TSModel_Vertex vertex = TSModel_Vertex();

				vertex.position = glm::highp_vec3(stream->ReadFloat(), stream->ReadFloat(), stream->ReadFloat());
				vertex.normal = glm::highp_vec3(stream->ReadFloat(), stream->ReadFloat(), stream->ReadFloat());
				vertex.texcoord = glm::highp_vec2(stream->ReadFloat(), stream->ReadFloat());

				subobject.vertices.push_back(vertex);
			}
		
			lod.subobjects.push_back(subobject);
		}

		int hierarchy_count = stream->ReadInt32();

		for (int iHier = 0; iHier < hierarchy_count; iHier++)
			lod.hierarchy.push_back(stream->ReadInt32());

		model.lods.push_back(lod);
	}

	int matrices_count = stream->ReadInt32();

	for (int iMat = 0; iMat < matrices_count; iMat++)
	{
		glm::mat4 mat(
			stream->ReadFloat(), stream->ReadFloat(), stream->ReadFloat(), stream->ReadFloat(),
			stream->ReadFloat(), stream->ReadFloat(), stream->ReadFloat(), stream->ReadFloat(),
			stream->ReadFloat(), stream->ReadFloat(), stream->ReadFloat(), stream->ReadFloat(),
			stream->ReadFloat(), stream->ReadFloat(), stream->ReadFloat(), stream->ReadFloat()
		);

		model.matrices.push_back(mat);
	}

	for (int iMat = 0; iMat < matrices_count; iMat++)
		model.matrices_name.push_back(stream->ReadString(stream->ReadUInt16()));

	*/

	return TS_OK;
}
