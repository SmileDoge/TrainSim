#pragma once

// Taken from OpenRails (https://github.com/openrails/openrails/blob/master/Source/ORTS.Common/Coordinates.cs)

#define WORLD_TILE_SIZE 2048.0f

#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

struct WorldLocation
{
public:
	WorldLocation(const WorldLocation& copy)
	{
		TileX = copy.TileX;
		TileZ = copy.TileZ;
		Location = copy.Location;
	}

	WorldLocation(int tileX, int tileZ, float x, float y, float z)
	{
		TileX = tileX;
		TileZ = tileZ;
		Location = glm::vec3(x, y, z);
	}

	WorldLocation(int tileX, int tileZ, glm::vec3& location)
	{
		TileX = tileX;
		TileZ = tileZ;
		Location = location;
	}

	void Normalize()
	{
		while (Location.x >= 1024) { Location.x -= 2048; TileX++; }
		while (Location.x < -1024) { Location.x += 2048; TileX--; }
		while (Location.z >= 1024) { Location.z -= 2048; TileZ++; }
		while (Location.z < -1024) { Location.z += 2048; TileZ--; }
		int xTileDistance = round((int)(Location.x / 1024.0f) / 2.0f);
		int zTileDistance = round((int)(Location.z / 1024.0f) / 2.0f);
		if (xTileDistance == 0 && zTileDistance == 0) return;
		else
		{
			TileX += xTileDistance;
			TileZ += zTileDistance;
			Location.x = (float)(Location.x - (xTileDistance * WORLD_TILE_SIZE));
			Location.z = (float)(Location.z - (zTileDistance * WORLD_TILE_SIZE));
		}
	}

	void NormalizeTo(int tileX, int tileZ)
	{
		while (TileX < tileX) { Location.x -= 2048; TileX++; }
		while (TileX > tileX) { Location.x += 2048; TileX--; }
		while (TileZ < tileZ) { Location.z -= 2048; TileZ++; }
		while (TileZ > tileZ) { Location.z += 2048; TileZ--; }
		int xDiff = TileX - tileX;
		int zDiff = TileZ - tileZ;
		if (xDiff == 0 && zDiff == 0) return;
		else
		{
			TileX = tileX;
			TileZ = tileZ;
			Location.x = (float)(Location.x + (xDiff * WORLD_TILE_SIZE));
			Location.z = (float)(Location.z + (zDiff * WORLD_TILE_SIZE));
		}
	}

	static bool Within(WorldLocation& location1, WorldLocation& location2, float distance)
	{
		return GetDistanceSquared(location1, location2) < distance * distance;
	}

	static float GetDistanceSquared(WorldLocation& location1, WorldLocation& location2)
	{
		double dx = location1.Location.x - location2.Location.x;
		double dy = location1.Location.y - location2.Location.y;
		double dz = location1.Location.z - location2.Location.z;
		dx += 2048 * (location1.TileX - location2.TileX);
		dz += 2048 * (location1.TileZ - location2.TileZ);
		return (float)(dx * dx + dy * dy + dz * dz);
	}

	static float GetDistanceSquared2D(WorldLocation& location1, WorldLocation& location2)
	{
		double dx = location1.Location.x - location2.Location.x;
		double dz = location1.Location.z - location2.Location.z;
		dx += WORLD_TILE_SIZE * (location1.TileX - location2.TileX);
		dz += WORLD_TILE_SIZE * (location1.TileZ - location2.TileZ);
		return (float)(dx * dx + dz * dz);
	}

	static glm::vec3 GetDistance(WorldLocation& locationFrom, WorldLocation& locationTo)
	{
		return glm::vec3((float)(locationTo.Location.x - locationFrom.Location.x + (locationTo.TileX - locationFrom.TileX) * WORLD_TILE_SIZE), (float)(locationTo.Location.y - locationFrom.Location.y),
			(float)(locationTo.Location.z - locationFrom.Location.z + (locationTo.TileZ - locationFrom.TileZ) * WORLD_TILE_SIZE));
	}
	
	static float ApproximateDistance(WorldLocation& a, WorldLocation& b)
	{
		float dx = a.Location.x - b.Location.x;
		float dz = a.Location.z - b.Location.z;
		dx += (a.TileX - b.TileX) * 2048;
		dz += (a.TileZ - b.TileZ) * 2048;
		return abs(dx) + abs(dz);
	}

	bool operator==(const WorldLocation& other) const {
		return this->TileX == other.TileX && this->TileZ == other.TileZ && this->Location == other.Location;
	}

	bool operator!=(const WorldLocation& other) const {
		return !(*this == other);
	}

	int TileX, TileZ;

	glm::vec3 Location;
};

class WorldPosition
{
public:
	WorldPosition() : TileX(0), TileZ(0), Matrix(1.0f) {};

	WorldPosition(const WorldPosition& copy)
	{
		TileX = copy.TileX;
		TileZ = copy.TileZ;
		Matrix = copy.Matrix;
	}

	WorldLocation WLocation()
	{
		return WorldLocation(TileX, TileZ, glm::vec3(Matrix[3]));
	}

	glm::vec3 Location()
	{
		return glm::vec3(Matrix[3]);
	}

	void SetLocation(glm::vec3& location)
	{
		Matrix[3] = glm::vec4(location, 1.0f);
	}

	int TileX, TileZ;

	glm::mat4 Matrix;
};