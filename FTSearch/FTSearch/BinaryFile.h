#pragma once

#include <direct.h> 
#include "stdafx.h"

class BinaryFilePage
{
public:
	BinaryFilePage()
	{
		CountReads = 0;

		NeedSave = false;

		memset(pContent, 0, BIN_FILE_RIGHT_MASK + 1);
	}

	uint32 CountReads;
	bool NeedSave;
	char pContent[BIN_FILE_RIGHT_MASK + 1];
};

class BinaryFile
{
private:
	char m_fileName[1024];

	bool m_isWritable;
	bool m_isOverwrite;

	uint32 m_pagesSize;
	uint32 m_countPages;
	uint32 m_maxCountPages;

	uint32 m_countReads;

	BinaryFilePage** pPages;

	FILE* m_file;

public:
	BinaryFile(const char* fileName, bool isWritable, bool isOverwrite);
	BinaryFile(const char* fileName, bool isWritable, bool isOverwrite, uint32 bufferSize);

	void init(const char* fileName, bool isWritable, bool isOverwrite, uint32 bufferSize);
	bool open();
	void close();

	uint32 read(void* pData, ulong64 position, uint32 length);
	uint32 readBuffered(void* pData, ulong64 position, uint32 length);

	uint32 read(void* pData, uint32 length);

	uint32 write(const void* pData, ulong64 position, const uint32 length);
	uint32 writeBuffered(const void* pData, ulong64 position, const uint32 length);

	uint32 write(const void* pData, const uint32 length);

	bool readInt(uint32* pValue, ulong64 position);
	bool readInt(uint32* pValue);
	
	bool readLong(ulong64* pValue, ulong64 position);
	bool readLong(ulong64* pValue);

	bool readInts(uint32* pValues, uint32 length);

	void writeInt(const uint32* pValue);
	void writeInt(const uint32* pValue, const ulong64 position);

	void writeLong(const ulong64* pValue);
	void writeLong(const ulong64* pValue, const ulong64 position);

	void writeInts(const uint32* pValues, const uint32 length);

	void reallocatePages(uint32 page);
	void deletePages(bool isAll);

	void flush()
	{
		deletePages(true);

		fflush(m_file);
	}

	void allocate(uint32 size, ulong64 fromPosition)
	{
		size /= MAX_SHORT;

		uchar8* buff = new uchar8[MAX_SHORT];
		memset(buff, 0, MAX_SHORT);
		
		setPosition(fromPosition);

		for(uint32 i=0; i<size; i++)
		{
			write(buff, MAX_SHORT);
		}

		//setPosition(0);

		delete[] buff;
	}

	ulong64 getFileSize()
	{
		_fseeki64(m_file, 0L, SEEK_END);
		return _ftelli64(m_file);
	}

	static bool existsFile(const char* fileName)
	{
		if (FILE * file = fopen(fileName, "r"))
		{
			fclose(file);

			return true;
		}

		return false;
	}

	static bool createDirectory(const char* dirPath)
	{
		int retCode = _mkdir(dirPath);

		return retCode == 0;
	}

	static int deleteFile(const char* fileName)
	{
		return remove(fileName);
	}

	static void copyFile(BinaryFile* pSourceFile,
						 BinaryFile* pDestFile)
	{
		char buffer[4096];

		uint32 size;
		while (size = pSourceFile->read(buffer, 4096)) 
		{
			pDestFile->write(buffer, size);
		}
	}

	static int renameFile(const char* oldFileName, const char* newFileName)
	{
		return rename(oldFileName, newFileName);
	}

	bool clear();
	
	void setPosition(ulong64 position);

	ulong64 getPosition();

	const char* getFilePath();

	~BinaryFile();
};

