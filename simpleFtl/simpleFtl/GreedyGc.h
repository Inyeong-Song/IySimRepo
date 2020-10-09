#pragma once
#include "Common.h"
#include "SectorMappingTable.h"
#include "VirtualBlock.h"
#include "SimpleFtl.h"

class GreedyGc
{
public:
	GreedyGc(MapTable& map, BlockDirectory& blkDir, SimpleFtl& simpleFtl, Int64 percentOfFreeBlock);

	bool GreedyGcTrigger();
	Int64 GetGcVictimBlock();
	Int64 GreedyGcInvoke();

private:
	MapTable& map;
	BlockDirectory& blkDir;
	SimpleFtl &simpleFtl;

	Int64 percentOfFreeBlock;
	Int32 minFreeBlocdkCnt;
};