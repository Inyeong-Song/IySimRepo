#pragma once
#include "Common.h"
#include "SectorMappingTable.h"
#include "VirtualBlock.h"
#include "SimpleFtl.h"

enum DualPoolSection
{
	hotPoolSect = 0,
	coldPoolSect
};

struct DualPoolBlock
{
	Int64 blockIndex;
	Int64 eraseCnt;
	Int64 effectiveEraseCnt;
};

struct DualPoolOption
{
	bool opDritySwap;
	bool opHotPoolResize;
	bool opColdPoolResize;
};
static bool DualPoolCompEraseCnt(DualPoolBlock& blkA, DualPoolBlock& blkB)
{
	if (blkA.eraseCnt < blkB.eraseCnt)
		return true;
	else
		return false;
}
static bool DualPoolCompEffEraseCnt(DualPoolBlock& blkA, DualPoolBlock& blkB)
{
	if (blkA.effectiveEraseCnt < blkB.effectiveEraseCnt)
		return true;
	else
		return false;
}
class DualPool
{
public:
	DualPool(MapTable& map, BlockDirectory& blkDir, SimpleFtl& simpleFtl, Int64 threshHoldValue);

	void DualPoolTrigger();
	void DirtySwap();
	void HotPoolResize();
	void ColdPoolResize();
	//void MigrationPoolElement(Int64 vbn);
	void DualPoolEraseBlk(DualPoolBlock dpBlock);
	
	void SetSdEraseCnt();
	double GetSdEraseCnt();

private:
	MapTable& map;
	BlockDirectory& blkDir;
	SimpleFtl& simpleFtl;

	list< DualPoolBlock > hotPool;
	list< DualPoolBlock > coldPool;
	list< DualPoolBlock > hotPoolEec;
	list< DualPoolBlock > coldPoolEec;
	vector< DualPoolBlock >dualPoolBlockTable;
	vector< DualPoolSection > dualPoolTable;
	Int64 sdEraseCnt;
	Int64 threshHoldValue;
	DualPoolOption dpOption;
	bool blockErasedFlag;
};