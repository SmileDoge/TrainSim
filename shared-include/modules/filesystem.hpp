#pragma once

#include "module.hpp"

#include <string>

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
	virtual std::string ReadCString() = 0;
	

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
	virtual size_t WriteCString(const std::string& string) = 0;


	virtual size_t GetSize() = 0;
	virtual std::string GetFilename() = 0;
	virtual std::string GetPath() = 0;

	virtual size_t Tell() = 0;
	virtual void Seek(size_t pos) = 0;


	virtual void Close() = 0;
};

class IFileSystem : public IModule
{
public:
	virtual ~IFileSystem() = default;

	virtual std::string ReadFileString(const std::string& path) = 0;
	virtual void WriteFileString(const std::string& path, const std::string& content) = 0;

	virtual size_t GetFileSize(const std::string& path) = 0;
	
	virtual bool FileExists(const std::string& path) = 0;

	virtual IFileStream* OpenFile(const std::string& path, const char* mode) = 0;
};