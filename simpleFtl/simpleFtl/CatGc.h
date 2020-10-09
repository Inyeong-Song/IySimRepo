#pragma once

#include "Common.h"
#include "SectorMappingTable.h"
#include "VirtualBlock.h"
#include "SimpleFtl.h"

class CatGc
{
public:
	CatGc(MapTable& map, BlockDirectory& blkDir, SimpleFtl& simpleFtl, Int64 percentOfFreeBlock);

	bool CatGcTrigger();
	Int64 CatGcVictimBlock();
	Int64 CatGcInvoke();
	double GetAgeFromTransformationFn(Int64 vbn);
	double TransformationFn(Int64 x);
	double GetUtilization(Int64 vbn);
	Int64 GetEraseCnt(Int64 vbn);

private:
	MapTable& map;
	BlockDirectory& blkDir;
	SimpleFtl& simpleFtl;

	Int64 percentOfFreeBlock;
	Int32 minFreeBlocdkCnt;

};