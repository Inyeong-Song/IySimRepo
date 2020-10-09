#include "DualPool.h"

DualPool::DualPool(MapTable& map, BlockDirectory& blkDir, SimpleFtl& simpleFtl, Int64 threshHoldValue)
	:map(map), blkDir(blkDir), simpleFtl(simpleFtl), threshHoldValue(threshHoldValue)
{
	dpOption.opColdPoolResize = false;
	dpOption.opDritySwap = false;
	dpOption.opHotPoolResize = false;
	blockErasedFlag = false;
	DualPoolBlock blockTemp;
	/* Block list vector initialization*/
	dualPoolBlockTable.resize(TOTAL_SUPER_BLOCK_CNT);
	dualPoolTable.resize(TOTAL_SUPER_BLOCK_CNT);
	for (Int64 i = 0; i < TOTAL_SUPER_BLOCK_CNT; i++)
	{
		blockTemp.blockIndex = i;
		blockTemp.eraseCnt = 0;
		blockTemp.effectiveEraseCnt = 0;
		dualPoolBlockTable[i] = blockTemp;
		if (i % 2)
		{
			hotPool.push_back(blockTemp);
			hotPoolEec.push_back(blockTemp);
			dualPoolTable[i] = hotPoolSect;
		}
		else
		{
			coldPool.push_back(blockTemp);
			coldPoolEec.push_back(blockTemp);
			dualPoolTable[i] = coldPoolSect;
		}
	}
}
void DualPool::DualPoolTrigger()
{
	DualPoolBlock hpH, cpH, hpT, cpT;
	DualPoolBlock hpeH, cpeH, hpeT, cpeT;
	hpH = hotPool.front();
	hpT = hotPool.back();
	cpH = coldPool.front();
	cpT = coldPool.back();
	hpeH = hotPoolEec.front();
	hpeT = hotPoolEec.back();
	cpeH = coldPoolEec.front();
	cpeT = coldPoolEec.back();

	hotPool.sort(DualPoolCompEraseCnt);
	coldPool.sort(DualPoolCompEraseCnt);
	hotPoolEec.sort(DualPoolCompEffEraseCnt);
	coldPoolEec.sort(DualPoolCompEffEraseCnt);

	if(hpH.eraseCnt - cpT.eraseCnt > threshHoldValue )
	{
		dpOption.opDritySwap = true;
		DirtySwap();
	}
	if(hpH.eraseCnt - hpT.eraseCnt > 2 * threshHoldValue )
	{
		dpOption.opHotPoolResize = true;
		HotPoolResize();
	}
	if (cpeH.effectiveEraseCnt - cpeT.effectiveEraseCnt > threshHoldValue)
	{
		dpOption.opColdPoolResize = true;
		ColdPoolResize();
	}

}
void DualPool::DirtySwap()
{
	DualPoolBlock hpH, cpH, hpT, cpT;
	DualPoolBlock hpeH, cpeH, hpeT, cpeT;
	hpH = hotPool.front();
	hpT = hotPool.back();
	cpH = coldPool.front();
	cpT = coldPool.back();
	hpeH = hotPoolEec.front();
	hpeT = hotPoolEec.back();
	cpeH = coldPoolEec.front();
	cpeT = coldPoolEec.back();

	Int64 vbaHPH = hpH.blockIndex * SECTOR_PER_SUPER_BLOCK;
	Int64 vbaCPT = cpT.blockIndex * SECTOR_PER_SUPER_BLOCK;
	Int64 validPageCntHPH = simpleFtl.GetValidPageCnt(hpH.blockIndex);
	Int64 validPageCntCPT = simpleFtl.GetValidPageCnt(cpT.blockIndex);
	Int64 validLba, oldVba, newVba;
	
	/* DS Step 1 */
	simpleFtl.MigrationValidPages(hpH.blockIndex, OPEN_BLOCK_FOR_DUALPOOL);

	/* DS Step 2 */
	DualPoolEraseBlk(hpH);

	/* DS Step 3 */
	simpleFtl.MigrationValidPages(cpT.blockIndex, OPEN_BLOCK_FOR_DUALPOOL);

	/* DS Step 4 */
	DualPoolEraseBlk(cpT);

	/* DS Step 5 */
	hotPool.pop_front();
	coldPool.pop_back();
	hotPool.push_back(cpT);
	coldPool.push_front(hpH);

	/* DS Step 6 */
	dualPoolBlockTable[hpH.blockIndex].effectiveEraseCnt = 0;
	dualPoolBlockTable[cpT.blockIndex].effectiveEraseCnt = 0;

	dpOption.opDritySwap = false;
}
void DualPool::HotPoolResize()
{
	DualPoolBlock hpT = hotPool.back();
	hotPool.pop_back();
	coldPool.push_front(hpT);

	dpOption.opHotPoolResize = false;
}
void DualPool::ColdPoolResize()
{
	DualPoolBlock cpH = coldPool.front();
	coldPool.pop_front();
	hotPool.push_back(cpH);

	dpOption.opColdPoolResize = false;
}
void DualPool::DualPoolEraseBlk(DualPoolBlock dpBlock)
{
	simpleFtl.EraseBlock(dpBlock.blockIndex);
	dualPoolBlockTable[dpBlock.blockIndex].eraseCnt = blkDir.blkState[dpBlock.blockIndex].eraseCnt;
	dualPoolBlockTable[dpBlock.blockIndex].effectiveEraseCnt++;
}
/*
void DualPool::MigrationPoolElement(Int64 vbn)
{
	DualPoolBlock blockTemp;
	if (dualPoolTable[vbn] != hotPoolSect)
	{
		for (auto itr = coldPool.begin(); itr != coldPool.end(); itr++)
		{
			blockTemp = *itr;
			if (vbn == blockTemp.blockIndex)
			{
				coldPool.erase(itr);
				hotPool.push_back(blockTemp);
				break;
			}
		}
	}
}
*/
void DualPool::SetSdEraseCnt()
{
	Int64 totalEraseCnt = 0;
	Int64 variation[TOTAL_SUPER_BLOCK_CNT];
	Int64 totalVariation = 0;
	double meanEraseCnt, sd;

	for (Int64 i = 0; i < TOTAL_SUPER_BLOCK_CNT; i++)
	{
		totalEraseCnt += blkDir.blkState[i].eraseCnt;
	}
	meanEraseCnt = (double)totalEraseCnt / TOTAL_SUPER_BLOCK_CNT;
	for (Int64 i = 0; i < TOTAL_SUPER_BLOCK_CNT; i++)
	{
		variation[i] = meanEraseCnt - blkDir.blkState[i].eraseCnt;
		variation[i] = variation[i] * variation[i];
		totalVariation += variation[i];
	}
	sd = totalVariation / TOTAL_SUPER_BLOCK_CNT;
	sdEraseCnt = sqrt(sd);
}
double DualPool::GetSdEraseCnt()
{
	return sdEraseCnt;
}