#pragma once

#include "module.hpp"

#include "engine.hpp"

#include <string>
#include <vector>
#include <sstream>

class IFileStream
{
public:
	virtual ~IFileStream() = default;
	
	virtual uint8_t  ReadUInt8()  = 0;
	virtual uint16_t ReadUInt16() = 0;
	virtual uint32_t ReadUInt32() = 0;
	virtual uint64_t ReadUInt64() = 0;

	virtual int8_t  ReadInt8()    = 0;
	virtual int16_t ReadInt16()   = 0;
	virtual int32_t ReadInt32()   = 0;
	virtual int64_t ReadInt64()   = 0;

	virtual float  ReadFloat()    = 0;
	virtual double ReadDouble()   = 0;

	virtual size_t ReadBytes(char* buffer, size_t length) = 0;

	virtual std::string ReadString(size_t length) = 0;
	

	virtual void WriteUInt8 (uint8_t  value) = 0;
	virtual void WriteUInt16(uint16_t value) = 0;
	virtual void WriteUInt32(uint32_t value) = 0;
	virtual void WriteUInt64(uint64_t value) = 0;

	virtual void WriteInt8 (int8_t  value)   = 0;
	virtual void WriteInt16(int16_t value)   = 0;
	virtual void WriteInt32(int32_t value)   = 0;
	virtual void WriteInt64(int64_t value)   = 0;

	virtual void WriteFloat(float value)     = 0;
	virtual void WriteDouble(double value)   = 0;

	virtual size_t WriteBytes(char* buffer, size_t length) = 0;

	virtual size_t WriteString(const std::string& string) = 0;


	virtual size_t GetSize() = 0;
	virtual std::string GetFilename() = 0;
	virtual std::string GetPath() = 0;


	virtual size_t Tell() = 0;
	virtual void Seek(size_t pos) = 0;


	virtual void Close() = 0;
};

enum FindFileFrom
{
	FIND_FILE_FROM_ROUTE,
	FIND_FILE_FROM_GLOBAL,
	FIND_FILE_FROM_TRAIN,

	FIND_FILE_FROM_CURRENT_DIRECTORY = FIND_FILE_FROM_TRAIN,
};

enum FileType
{
	FILE_TYPE_MODEL,
	FILE_TYPE_TEXTURE,
	FILE_TYPE_MATERIAL,
	FILE_TYPE_SOUND,
	FILE_TYPE_OTHER,
};

class IFileSystem : public IModule
{
public:
	virtual ~IFileSystem() = default;

	virtual std::string ReadFileString(const std::string& path) = 0;
	virtual void WriteFileString(const std::string& path, const std::string& content) = 0;

	virtual uint8_t* ReadFile(const std::string& path, size_t* length) = 0;
	virtual size_t WriteFile(const std::string& path, uint8_t* buffer, size_t length) = 0;

	virtual size_t GetFileSize(const std::string& path) = 0;
	
	virtual bool FileExists(const std::string& path) = 0;

	virtual IFileStream* OpenFile(const std::string& path, const char* mode) = 0;

	ENGINE_EXPORT static std::string GetGameDirectory();

	static std::string GetDataDirectory()
	{
		return Combine(GetGameDirectory(), "data");
	}

	static std::string GetDirectoryName(const std::string& path)
	{
		size_t found = path.find_last_of("/\\");
		if (found != std::string::npos) {
			return path.substr(0, found);
		}
		else {
			return "";
		}
	}

	static std::string Combine(const std::string& path1, const std::string& path2)
	{
		if (path1.empty() || path2.empty())
			return path1 + path2;

		if (path1.back() == '/' || path1.back() == '\\')
			return path1 + path2;
		else
			return path1 + '/' + path2;
	}

	static std::string ChangeExtension(const std::string& path, const std::string& newExtension)
	{
		size_t dotIndex = path.find_last_of('.');
		if (dotIndex != std::string::npos) {
			return path.substr(0, dotIndex) + newExtension;
		}
		else {
			return path + newExtension;
		}
	}

	static std::string GetExtension(const std::string& filename)
	{
		size_t pos = filename.find_last_of('.');

		if (pos != std::string::npos)
			return filename.substr(pos + 1);

		return "";
	}

	static std::string GetFileName(const std::string& path)
	{
		size_t slashIndex = path.find_last_of("/\\");
		if (slashIndex != std::string::npos) {
			return path.substr(slashIndex + 1);
		}
		else {
			return path;
		}
	}

	static std::string GetFileNameWithoutExtension(const std::string& path)
	{
		std::string fileName = GetFileName(path);
		size_t dotIndex = fileName.find_last_of('.');
		if (dotIndex != std::string::npos) {
			return fileName.substr(0, dotIndex);
		}
		else {
			return fileName;
		}
	}

	static std::string GetFullPath(const std::string& path)
	{
		std::stringstream ss(path);
		std::string token;
		std::vector<std::string> directories;

		while (std::getline(ss, token, '/')) {
			if (token == "..") {
				if (!directories.empty()) {
					directories.pop_back();
				}
			}
			else if (token != ".") {
				directories.push_back(token);
			}
		}

		std::string full_path;
		for (const auto& dir : directories) {
			full_path += "/" + dir;
		}

		if (full_path.empty()) {
			full_path = "/";
		}

		return full_path.substr(1);
	}

	static FileType GetFileType(const std::string& filename)
	{
		auto& ext = GetExtension(filename);

		if (ext == "ts_model")
			return FILE_TYPE_MODEL;

		if (ext == "ts_tex")
			return FILE_TYPE_TEXTURE;

		if (ext == "ts_mat")
			return FILE_TYPE_MATERIAL;

		return FILE_TYPE_OTHER;
	}

	static std::string GetResourceFilepath(const std::string& filename, const std::string& root_path, FindFileFrom from)
	{
		auto file_type = GetFileType(filename);

		auto data_directory = GetGameDirectory();

		std::string result_path = "";

		switch (from)
		{
		case FIND_FILE_FROM_ROUTE:
			if (file_type == FILE_TYPE_MODEL)
				result_path = Combine(Combine(root_path, "models"), filename);

			if (file_type == FILE_TYPE_TEXTURE)
				result_path = Combine(Combine(root_path, "textures"), filename);

			if (file_type == FILE_TYPE_MATERIAL)
				result_path = Combine(Combine(root_path, "materials"), filename);
			break;
		case FIND_FILE_FROM_GLOBAL:
			if (file_type == FILE_TYPE_MODEL)
				result_path = Combine(Combine(data_directory, "models"), filename);

			if (file_type == FILE_TYPE_TEXTURE)
				result_path = Combine(Combine(data_directory, "textures"), filename);

			if (file_type == FILE_TYPE_MATERIAL)
				result_path = Combine(Combine(data_directory, "materials"), filename);
			break;
		case FIND_FILE_FROM_TRAIN:
			result_path = Combine(root_path, filename);
			break;
		default:
			break;
		}

		if (result_path != "")
			return GetFullPath(result_path);

		return result_path;
	}

	std::string FindResourcePath(const std::string& filename, const std::string& root_path, FindFileFrom from)
	{
		std::string result_path = "";

		if (from == FIND_FILE_FROM_ROUTE)
		{
			result_path = GetResourceFilepath(filename, root_path, FIND_FILE_FROM_ROUTE);

			if (!FileExists(result_path))
				result_path = GetResourceFilepath(filename, root_path, FIND_FILE_FROM_GLOBAL);
		}

		if (from == FIND_FILE_FROM_TRAIN)
		{
			result_path = GetResourceFilepath(filename, root_path, FIND_FILE_FROM_TRAIN);
		}

		return result_path;
	}
};

/*
std::string IFileSystem::GetDirectoryName(const std::string& path)
{
}

std::string IFileSystem::Combine(const std::string& path1, const std::string& path2)
{
}

std::string IFileSystem::ChangeExtension(const std::string& path, const std::string& newExtension)
{
}

std::string IFileSystem::GetFileName(const std::string& path)
{
}

std::string IFileSystem::GetFileNameWithoutExtension(const std::string& path)
{
}
*/