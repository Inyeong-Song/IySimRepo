#pragma once
#include "Common.h"

enum BlockListState
{
	isFreeList = 0,
	isClosedList,
	isOpenBlock
};
struct BlockStat
{
	Int64 eraseCnt;
	Int64 writePoint;
	Int64 invalidCnt;
	clock_t timeStamp;
};
struct BlockElement
{
	Int64 blockIndex;
	Int64 eraseCnt;
};
static bool CompEraseCnt(BlockElement& blkA, BlockElement& blkB)
{
	if (blkA.eraseCnt < blkB.eraseCnt)
		return true;
	else
		return false;
}
class BlockDirectory
{
public:
	BlockDirectory(Int64 blockCnt);
	
	BlockStat* blkState;
	list<BlockElement> freeBlk;
	list<BlockElement> closedBlk;
	vector<Int64> openBlockMap;
};

