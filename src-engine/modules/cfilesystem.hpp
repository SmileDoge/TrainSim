#pragma once

#include "modules/filesystem.hpp"

#define FILE_STREAM_BUFFER_SIZE 65536

class CFileStream : public IFileStream
{
public:
	CFileStream();
	~CFileStream() override;


	virtual uint8_t  ReadUInt8();
	virtual uint16_t ReadUInt16();
	virtual uint32_t ReadUInt32();
	virtual uint64_t ReadUInt64();

	virtual int8_t  ReadInt8();
	virtual int16_t ReadInt16();
	virtual int32_t ReadInt32();
	virtual int64_t ReadInt64();

	virtual float  ReadFloat();
	virtual double ReadDouble();

	virtual size_t ReadBytes(char* buffer, size_t length);

	virtual std::string ReadString(size_t length);


	virtual void WriteUInt8(uint8_t  value);
	virtual void WriteUInt16(uint16_t value);
	virtual void WriteUInt32(uint32_t value);
	virtual void WriteUInt64(uint64_t value);

	virtual void WriteInt8(int8_t  value);
	virtual void WriteInt16(int16_t value);
	virtual void WriteInt32(int32_t value);
	virtual void WriteInt64(int64_t value);

	virtual void WriteFloat(float value);
	virtual void WriteDouble(double value);

	virtual size_t WriteBytes(char* buffer, size_t length);

	virtual size_t WriteString(const std::string& string);


	virtual size_t GetSize();
	virtual std::string GetFilename();
	virtual std::string GetPath();


	virtual size_t Tell();
	virtual void Seek(size_t pos);


	virtual void Close();

	bool InitFromFile(const std::string& path, const std::string& mode);
private:

	std::string path;
	std::string mode;

	FILE* file;

	//char* buffer;
};

class CFileSystem : public IFileSystem
{
public:
	CFileSystem();
	~CFileSystem() override;

	virtual void PostInit() {};
	virtual void PreDeinit() {};
	virtual void Update() {};

	virtual std::string ReadFileString(const std::string& path);
	virtual void WriteFileString(const std::string& path, const std::string& content);

	virtual uint8_t* ReadFile(const std::string& path, size_t* length);
	virtual size_t WriteFile(const std::string& path, uint8_t* buffer, size_t length);

	virtual size_t GetFileSize(const std::string& path);

	virtual bool FileExists(const std::string& path);

	virtual IFileStream* OpenFile(const std::string& path, const char* mode);
};

extern CFileSystem* g_FileSystem;