#include "GreedyGc.h"

GreedyGc::GreedyGc(MapTable& map, BlockDirectory& blkDir, SimpleFtl& simpleFtl, Int64 percentOfFreeBlock)
	:map(map), blkDir(blkDir), simpleFtl(simpleFtl), percentOfFreeBlock(percentOfFreeBlock)
{
	//printf("Greedy GC\n");
	minFreeBlocdkCnt = TOTAL_SUPER_BLOCK_CNT * percentOfFreeBlock / 100;
	if (minFreeBlocdkCnt < 1)
	{
		printf("ERROR MIN_FREE_BLOCK_CNT 0\n");
		assert(0);
	}
}

bool GreedyGc::GreedyGcTrigger()
{
	if (blkDir.freeBlk.size() < minFreeBlocdkCnt)
		return true;
	else
		return false;
}

Int64 GreedyGc::GetGcVictimBlock()
{
	Int64 victimBlk, invalidBlkCnt;
	victimBlk = INV_VALUE;
	invalidBlkCnt = 0;
	BlockElement blockTemp;
	for (auto itr = blkDir.closedBlk.begin(); itr != blkDir.closedBlk.end(); itr++)
	{
		blockTemp = *itr;
		if (invalidBlkCnt < blkDir.blkState[blockTemp.blockIndex].invalidCnt)
			
		{
			invalidBlkCnt = blkDir.blkState[blockTemp.blockIndex].invalidCnt;
			victimBlk = blockTemp.blockIndex;
		}
	}
	if (victimBlk == INV_VALUE)
	{
		assert(0);
	}
	return victimBlk;
}

Int64 GreedyGc::GreedyGcInvoke()
{
	//GC victim block no, valid cnt
	Int64 vbn = GetGcVictimBlock();
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
