#pragma once
#include "Common.h"
#include "SectorMappingTable.h"
#include "VirtualBlock.h"

enum OpenBlockSel
{
	OPEN_BLOCK_FOR_HOST = 0,
	OPEN_BLOCK_FOR_GC,
	OPEN_BLOCK_FOR_DUALPOOL
};

class SimpleFtl
{
public:
	SimpleFtl(MapTable& map, BlockDirectory& blkDir, Int64 TOTAL_BLOCK_CNT, Int64 PAGE_PER_BLOCK);

	Int64 ReadAddrTrans(Int64 lba);
	Int64 WriteAddrTrans(Int64 lba, OpenBlockSel openBlockNo);
	Int64 EraseBlock(Int64 vbn);
	Int64 MigrationValidPages(Int64 vbn, OpenBlockSel openBlockNo);
	bool CompareInvalidCnt(Int64 newBlkA, Int64 newBlkB);
	Int64 GetValidPageCnt(Int64 newBlk);
	Int64 GetSsdWriteCnt();
	Int64 GetPagePerBlock();
	Int64 GetTotalBlockCnt();

	void SetSsdWriteCnt(Int64 writeCnt);

private:
	Int64 GetFreeVrtualAddr(OpenBlockSel openBlockNo);

	MapTable& map;
	BlockDirectory& blkDir;

	Int64 PAGE_PER_BLOCK; //super
	Int64 TOTAL_BLOCK_CNT; //super

	Int32 percentOfFreeBlock;

	Int64 ssdWriteCnt = 0;
};