#include "cfilesystem.hpp"

#include <io.h>

CFileStream::CFileStream() : file(NULL)
{

}

CFileStream::~CFileStream()
{
	Close();
}

bool CFileStream::InitFromFile(const std::string& path, const std::string& mode)
{
	FILE* res = fopen(path.c_str(), mode.c_str());

	if (res == NULL)
		return false;

	this->path = path;
	this->mode = mode;

	this->file = res;

	return true;
}

#define READ_FUNCTION_TYPED(type, name) \
type CFileStream::Read##name##() \
{ \
	type val; \
	fread(&val, sizeof(type), 1, file); \
	return val; \
} \

#define WRITE_FUNCTION_TYPED(type, name) \
void CFileStream::Write##name##(type value) \
{ \
	fwrite(&value, sizeof(type), 1, file); \
} \

READ_FUNCTION_TYPED(uint8_t,  UInt8 )
READ_FUNCTION_TYPED(uint16_t, UInt16)
READ_FUNCTION_TYPED(uint32_t, UInt32)
READ_FUNCTION_TYPED(uint64_t, UInt64)

READ_FUNCTION_TYPED(int8_t , Int8 )
READ_FUNCTION_TYPED(int16_t, Int16)
READ_FUNCTION_TYPED(int32_t, Int32)
READ_FUNCTION_TYPED(int64_t, Int64)

READ_FUNCTION_TYPED(float , Float)
READ_FUNCTION_TYPED(double, Double)

size_t CFileStream::ReadBytes(char* buffer, size_t length)
{
	if (file == NULL) return 0;

	return fread(buffer, 1, length, file);
}

std::string CFileStream::ReadString(size_t length)
{
	char* buffer = new char[length + 1];
	
	memset(buffer, 0, length + 1);

	ReadBytes(buffer, length);

	std::string res(buffer);

	delete[] buffer;

	return res;
}


WRITE_FUNCTION_TYPED(uint8_t , UInt8 )
WRITE_FUNCTION_TYPED(uint16_t, UInt16)
WRITE_FUNCTION_TYPED(uint32_t, UInt32)
WRITE_FUNCTION_TYPED(uint64_t, UInt64)

WRITE_FUNCTION_TYPED(int8_t , Int8 )
WRITE_FUNCTION_TYPED(int16_t, Int16)
WRITE_FUNCTION_TYPED(int32_t, Int32)
WRITE_FUNCTION_TYPED(int64_t, Int64)

WRITE_FUNCTION_TYPED(float, Float)
WRITE_FUNCTION_TYPED(double, Double)

size_t CFileStream::WriteBytes(char* buffer, size_t length)
{
	if (file == NULL) return 0;

	return fwrite(buffer, length, 1, file);
}

size_t CFileStream::WriteString(const std::string& string)
{
	if (file == NULL) return 0;

	return WriteBytes(const_cast<char*>(string.c_str()), string.size());
}

size_t CFileStream::GetSize()
{
	if (file == NULL) return 0;

	long pos = ftell(file);

	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	fseek(file, pos, SEEK_SET);

	return size;
}

std::string CFileStream::GetFilename()
{
	return path.substr(path.find_last_of("/\\") + 1);
}

std::string CFileStream::GetPath()
{
	return path;
}

size_t CFileStream::Tell()
{
	if (file == NULL) return -1;

	return ftell(file);
}

void CFileStream::Seek(size_t pos)
{
	if (file == NULL) return;

	fseek(file, pos, SEEK_SET);
}

void CFileStream::Close()
{
	if (file == NULL) return;

	fclose(file);
	file = NULL;
}

CFileSystem* g_FileSystem = NULL;

CFileSystem::CFileSystem()
{
	g_FileSystem = this;
}

CFileSystem::~CFileSystem()
{

}

std::string CFileSystem::ReadFileString(const std::string& path)
{
	IFileStream* stream = OpenFile(path, "r");

	if (stream == NULL) return "";

	std::string str = stream->ReadString(stream->GetSize());

	delete stream;

	return str;
}

void CFileSystem::WriteFileString(const std::string& path, const std::string& content)
{
	IFileStream* stream = OpenFile(path, "w");

	if (stream == NULL) return;

	stream->WriteString(content);

	delete stream;
}

uint8_t* CFileSystem::ReadFile(const std::string& path, size_t* length)
{
	IFileStream* stream = OpenFile(path, "rb");

	if (stream == NULL) return 0;

	uint8_t* buffer = new uint8_t[stream->GetSize()];

	stream->ReadBytes((char*)buffer, stream->GetSize());

	if (length != NULL)
		*length = stream->GetSize();

	delete stream;

	return buffer;
}

size_t CFileSystem::WriteFile(const std::string& path, uint8_t* buffer, size_t length)
{
	IFileStream* stream = OpenFile(path, "rb");

	if (stream == NULL) return 0;

	size_t writed = stream->WriteBytes((char*)buffer, length);

	delete stream;

	return writed;
}

size_t CFileSystem::GetFileSize(const std::string& path)
{
	IFileStream* stream = OpenFile(path, "rb");

	if (stream == NULL) return 0;

	size_t size = stream->GetSize();

	delete stream;

	return size;
}

bool CFileSystem::FileExists(const std::string& path)
{
	return _access(path.c_str(), 0) != -1;
}

IFileStream* CFileSystem::OpenFile(const std::string& path, const char* mode)
{
	CFileStream* stream = new CFileStream();

	if (!stream->InitFromFile(path, mode))
	{
		delete stream;
		return NULL;
	}

	return stream;
}
