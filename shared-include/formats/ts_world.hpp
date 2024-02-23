#pragma once

#include <string>
#include <vector>

#include "glm/vec3.hpp"
#include "glm/gtx/quaternion.hpp"

#include "modules/filesystem.hpp"

#include "nlohmann/json.hpp"

struct TSWorld_Object
{
	std::string Type;
	std::string Filename;
	int UID;
	glm::vec3 Position;
	glm::quat Direction;
};

struct TSWorld
{
	int TileX;
	int TileZ;
	std::vector<TSWorld_Object> Objects;

	static TSWorld CreateFromStream(IFileStream* stream)
	{
		return CreateFromText(stream->ReadString(stream->GetSize()));
	}

	static TSWorld CreateFromText(std::string content)
	{
		auto world = TSWorld();

		auto json = nlohmann::json::parse(content);

		world.TileX = json["TileX"].get<int>();
		world.TileZ = json["TileZ"].get<int>();

		for (auto& obj_json : json["Objects"])
		{
			auto obj = TSWorld_Object();

			obj.Type = obj_json["Type"].get<std::string>();
			obj.UID = obj_json["UID"].get<int>();

			if (!obj_json["Filename"].is_null())
				obj.Filename = obj_json["Filename"].get<std::string>();

			obj.Position = glm::vec3();
			obj.Direction = glm::quat();

			obj.Position.x =  obj_json["Position"].at(0).get<float>();
			obj.Position.y =  obj_json["Position"].at(1).get<float>();
			obj.Position.z =  obj_json["Position"].at(2).get<float>();

			obj.Direction.x = -obj_json["Direction"].at(0).get<float>();
			obj.Direction.y = -obj_json["Direction"].at(1).get<float>();
			obj.Direction.z = -obj_json["Direction"].at(2).get<float>();
			obj.Direction.w = obj_json["Direction"].at(3).get<float>();

			world.Objects.push_back(obj);
		}

		return world;
	}
};