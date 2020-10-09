#include "LoadWorkfile.h"

LoadWorkfile::LoadWorkfile(char** fileName, Int64 WorkloadSectorSize, Int64 UserSectorCnt)
	:fileName(fileName), WorkloadSectorSize(WorkloadSectorSize), UserSectorCnt(UserSectorCnt)
{
	//printf("Load file:%s\n", fileName[1]);
	importFile = fopen(fileName[1], "rb");
	if (importFile == nullptr)
	{
		printf("ERROR open file\n");
		abort();
	}
}
Int64 LoadWorkfile::ReadFileLine(cmdFormat& getCmd)
{
	char op[10];
	char appr[10];
	double disk;
	double dummy;
	double byteOffset;
	Int32 byteSize;
	Int32 timestamp;

	/*
	//WebSearch1.spc
	if (fscanf(importFile, "%*d,%ld,%d,%c,%ld", &byteOffset, &byteSize, &op, &timestamp) == 4)
	{
		assert(0);
	}
	*/
	//MSR
	//while (fscanf(importFile, "%[^,], %[^,], %[^,], %[^,], %lf, %d, %d", &timestamp, appr, &disk, op, &byteOffset, &byteSize, &dummy) != 7)
	
	if (feof(importFile) != 0)
	{
		//printf("End Of File\n");
		return INV_VALUE;
	}
	else if (fscanf(importFile, "%*[^,], %[^,], %[^,], %[^,], %lf, %d, %d", appr, &disk, op, &byteOffset, &byteSize, &dummy) != 6)
	{
		
		//printf("ERROR read file line\n");
		return 1;
	}
	else
	{
		getCmd.lba = ((Int64)byteOffset / WorkloadSectorSize) % UserSectorCnt;

		if (getCmd.lba > UserSectorCnt)
		{
			printf("Exceed user capacity\n");
			assert(0);
		}
		if (op[0] == 'W')
			getCmd.op = 'W';
		else
			getCmd.op = 'R';
		getCmd.size = byteSize;
		return 0;
	}
}