#pragma once
#include "Common.h"

struct cmdFormat
{
	char op;
	Int64 lba;
	Int64 size;
};

struct ssdSpcFormat
{
	Int64 blockCnt;
	Int64 pageCnt;
	Int64 pageSize;
};

class LoadWorkfile
{
public:
	LoadWorkfile(char** fileName, Int64 WorkloadSectorSize, Int64 UserSectorCnt);
	Int64 ReadFileLine(cmdFormat& getCmd);

private:
	FILE* importFile;
	char** fileName;
	Int64 WorkloadSectorSize;
	Int64 UserSectorCnt;
};