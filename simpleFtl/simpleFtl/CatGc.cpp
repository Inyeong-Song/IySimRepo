#pragma once
#include "CatGc.h"

CatGc::CatGc(MapTable& map, BlockDirectory& blkDir, SimpleFtl& simpleFtl, Int64 percentOfFreeBlock)
	:map(map), blkDir(blkDir), simpleFtl(simpleFtl), percentOfFreeBlock(percentOfFreeBlock)
{
	//printf("CAT GC\n");
	minFreeBlocdkCnt = TOTAL_SUPER_BLOCK_CNT * percentOfFreeBlock / 100;
	if (minFreeBlocdkCnt < 1)
	{
		printf("ERROR MIN_FREE_BLOCK_CNT 0\n");
		assert(0);
	}
}

bool CatGc::CatGcTrigger()
{
	if (blkDir.freeBlk.size() < minFreeBlocdkCnt)
		return true;
	else
		return false;
}

Int64 CatGc::CatGcVictimBlock()
{
	Int64 vbn, eraseCnt;
	double util, age;
	vbn = INV_VALUE;
	double cost[TOTAL_SUPER_BLOCK_CNT] = { 0, };
	double lowestCost = LLONG_MAX;
	BlockElement blockTemp;
	for (auto itr = blkDir.closedBlk.begin(); itr != blkDir.closedBlk.end(); itr++)
	{
		blockTemp = *itr;
		util = GetUtilization(blockTemp.blockIndex);
		age = GetAgeFromTransformationFn(blockTemp.blockIndex);
		eraseCnt = GetEraseCnt(blockTemp.blockIndex);
		if (blkDir.blkState[blockTemp.blockIndex].invalidCnt == 0)
		{
			cost[blockTemp.blockIndex] = LLONG_MAX;
		}
		else
		{
			//cost[*itr] = (util * (double)eraseCnt) / ((double)age * (1 - util));
			cost[blockTemp.blockIndex] = (util * 1) / (age * (1 - util));
		}			

		if (cost[blockTemp.blockIndex] < lowestCost)
		{
			lowestCost = cost[blockTemp.blockIndex];
			vbn = blockTemp.blockIndex;
		}
	}
#if (PRINT_GC == 1)
	for (Int64 i = 0; i < TOTAL_SUPER_BLOCK_CNT; i++)
	{
		if (i % 5 == 0)
			printf("\n");
		printf("Blk%d cost:%lf  ", i, cost[i]);
	}
	printf("\n\n");
#endif
	return vbn;
}
Int64 CatGc::CatGcInvoke()
{
	//GC victim block no, valid cnt
	Int64 vbn = CatGcVictimBlock();
	Int64 validPageCnt = simpleFtl.GetValidPageCnt(vbn);
	Int64 victimVba, oldVba, newVba, validLba;
	victimVba = vbn * SECTOR_PER_SUPER_BLOCK;
#if (PRINT_GC == 1)
	printf("GC invoke at %ld block, Valid:%d\n", vbn, validPageCnt);
#endif
	simpleFtl.MigrationValidPages(vbn, OPEN_BLOCK_FOR_GC);
	simpleFtl.EraseBlock(vbn);
	return vbn;
}
double CatGc::GetAgeFromTransformationFn(Int64 vbn)
{
	double age, vbnTimeStamp;
	clock_t endTime = clock();
	vbnTimeStamp = (double)(endTime - blkDir.blkState[vbn].timeStamp) / CLOCKS_PER_SEC;
	
	age = TransformationFn(vbnTimeStamp);
	return age;
}
double CatGc::TransformationFn(Int64 x)
{
	double y;
	
	if (x < 10)
		y = 1;
	else if (x < 20)
		y = 2;
	else if (x < 45)
		y = 3;
	else if (x < 90)
		y = 4;
	else if (x < 180)
		y = 5;
	else if (x < 360)
		y = 6;
	else
		y = 7;
	return y;
	
	//return y = 7 * (1 / (1 + exp((-x + 215) / 120)));
	//return y = 7 * (1 / (1 + exp((-x + 129) / 72)));
	//return y = 7 * (1 / (1 + exp((-x + 30) / 15)));
	/*
	if (x = 0)
		return y = 1;
	else
		return y = log10(x) + 5;
	*/
}
double CatGc::GetUtilization(Int64 vbn)
{
	Int64 vaildCnt = SECTOR_PER_SUPER_BLOCK - blkDir.blkState[vbn].invalidCnt;
	double util = (double) vaildCnt / (double) SECTOR_PER_SUPER_BLOCK;
	return util;
}
Int64 CatGc::GetEraseCnt(Int64 vbn)
{
	Int64 eraseCnt = blkDir.blkState[vbn].eraseCnt;
	return eraseCnt;
}