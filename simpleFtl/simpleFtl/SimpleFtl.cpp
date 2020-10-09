#include "SimpleFtl.h"

SimpleFtl::SimpleFtl(MapTable& map, BlockDirectory& blkDir, Int64 TOTAL_BLOCK_CNT, Int64 PAGE_PER_BLOCK)
	:map(map), blkDir(blkDir), TOTAL_BLOCK_CNT(TOTAL_BLOCK_CNT), PAGE_PER_BLOCK(PAGE_PER_BLOCK)
{
	blkDir.openBlockMap.resize(3, INV_VALUE);
	BlockElement blockE;

	//Block status inititation
	for (Int64 i = 0; i < TOTAL_BLOCK_CNT; i++)
	{
		blockE.blockIndex = i;
		blockE.eraseCnt = 0;
		blkDir.freeBlk.push_back(blockE);
		blkDir.blkState[i].eraseCnt = 0;
		blkDir.blkState[i].invalidCnt = 0;
		blkDir.blkState[i].writePoint = 0;
		blkDir.blkState[i].timeStamp = 0;
	}

}

Int64 SimpleFtl::ReadAddrTrans(Int64 lba)
{
	Int64 vba = map.GetVirtualAddr(lba);
#if (PRINT_TRANS_ADDR == 1)
	printf("Read Lba:%lld, Vba:%lld \n", lba, vba);
#endif
	return vba;
}
Int64 SimpleFtl::WriteAddrTrans(Int64 lba, OpenBlockSel openBlockNo)
{
	//over Write + invalid update
	Int64 oldVba = map.GetVirtualAddr(lba);
	if (oldVba != INV_VALUE)
	{
		map.SetVtolMap(oldVba, INV_VALUE);

		Int64 blockNo = oldVba / PAGE_PER_BLOCK;

		blkDir.blkState[blockNo].invalidCnt++;
	}

	Int64 vba = GetFreeVrtualAddr(openBlockNo);//block Allocation;
	map.SetLtovMap(lba, vba);
	map.SetVtolMap(vba, lba);
	ssdWriteCnt++;
#if (PRINT_TRANS_ADDR == 1)
	printf("Write Lba:%lld, Vba:%lld \n", lba, vba);
#endif
	return vba;
}
Int64 SimpleFtl::EraseBlock(Int64 vbn)
{
	BlockElement blockErase, blockClosed;
	/*Block Erase*/
	blkDir.blkState[vbn].invalidCnt = 0;
	blkDir.blkState[vbn].writePoint = 0;
	blkDir.blkState[vbn].eraseCnt++;
	blkDir.blkState[vbn].timeStamp = 0;
	/*Pop from closed block list*/
	for (auto itr = blkDir.closedBlk.begin(); itr != blkDir.closedBlk.end(); itr++)
	{
		blockClosed = *itr;
		if (vbn == blockClosed.blockIndex)
		{
			blkDir.closedBlk.erase(itr);
			break;
		}

	}
	/*Push to free block list*/
	blockErase.blockIndex = vbn;
	blockErase.eraseCnt = blkDir.blkState[vbn].eraseCnt;
	blkDir.freeBlk.push_back(blockErase);
	
#if (PRINT_GC == 1)
	printf("Block %d was erased\n\n", vbn);
#endif
	return vbn;
}
Int64 SimpleFtl::MigrationValidPages(Int64 vbn, OpenBlockSel openBlockNo)
{
	Int64 oldVba, newVba, validLba, openVbn;
	Int64 validPageCnt = GetValidPageCnt(vbn);
	Int64 vba = vbn * SECTOR_PER_SUPER_BLOCK;
	openVbn = blkDir.openBlockMap[openBlockNo];
	for (Int64 i = 0; i < SECTOR_PER_SUPER_BLOCK; i++)
	{
		if (validPageCnt > 0)
		{
			if (map.GetLogicalAddr(vba + i) != INV_VALUE)
			{
				validLba = map.GetLogicalAddr(vba + i);
				oldVba = ReadAddrTrans(validLba);
				if (oldVba != (vba + i))
				{
					printf("ERROR virtual address\n");
					assert(0);
				}
				map.SetVtolMap(oldVba, INV_VALUE);
				newVba = WriteAddrTrans(validLba, openBlockNo);
				validPageCnt--;
#if (PRINT_GC == 1)
				printf("Lba:%ld, oldVba:%ld, newVba:%ld\n", validLba, oldVba, newVba);
#endif
			}
		}
		else
			break;
	}
	openVbn = blkDir.openBlockMap[openBlockNo];
	return openVbn;
}
bool SimpleFtl::CompareInvalidCnt(Int64 newBlkA, Int64 newBlkB)
{
	if (blkDir.blkState[newBlkA].invalidCnt > blkDir.blkState[newBlkB].invalidCnt)
		return true;
	else
		return false;
}

Int64 SimpleFtl::GetValidPageCnt(Int64 newBlk)
{
	return PAGE_PER_BLOCK - blkDir.blkState[newBlk].invalidCnt;
}

Int64 SimpleFtl::GetFreeVrtualAddr(OpenBlockSel openBlockNo)
{
	Int64 vbn, newWritePoint;
	BlockElement blockTemp, blockFree;

	if (blkDir.openBlockMap[openBlockNo] == INV_VALUE)
	{
		/*Sort free block list*/
		blkDir.freeBlk.sort(CompEraseCnt);

		blockFree = blkDir.freeBlk.front();
		blkDir.openBlockMap[openBlockNo] = blockFree.blockIndex;
		blkDir.freeBlk.pop_front();
	}

	vbn = blkDir.openBlockMap[openBlockNo];
	

	if (blkDir.blkState[vbn].writePoint == PAGE_PER_BLOCK) //openBlockÀÇ page °¡µæÃ¡À» ¶§
	{
		if (blkDir.freeBlk.size() > 0)
		{
			/*Sort free block list*/
			blkDir.freeBlk.sort(CompEraseCnt);

			blockFree = blkDir.freeBlk.front();
			blkDir.openBlockMap[openBlockNo] = blockFree.blockIndex;
			blkDir.freeBlk.pop_front();
			blockTemp.blockIndex = vbn;
			blockTemp.eraseCnt = blkDir.blkState[vbn].eraseCnt;
			blkDir.closedBlk.push_back(blockTemp);
			vbn = blkDir.openBlockMap[openBlockNo];
#if (PRINT_TRANS_ADDR == 1)
			printf("block open %d \n", vbn);
#endif
		}
		else
		{
			printf("No remaining free block\n");
			assert(0);
		}
	}
	else if (blkDir.blkState[vbn].writePoint > PAGE_PER_BLOCK)
	{
		printf("Error write pointer even free\n");
		assert(0);
	}

	newWritePoint = (vbn * PAGE_PER_BLOCK) + blkDir.blkState[vbn].writePoint;
	blkDir.blkState[vbn].writePoint++;
	blkDir.blkState[vbn].timeStamp = clock();
	return newWritePoint;
}
Int64 SimpleFtl::GetSsdWriteCnt()
{
	return ssdWriteCnt;
}

Int64 SimpleFtl::GetPagePerBlock()
{
	return PAGE_PER_BLOCK;
}
Int64 SimpleFtl::GetTotalBlockCnt()
{
	return TOTAL_BLOCK_CNT;
}

void SimpleFtl::SetSsdWriteCnt(Int64 writeCnt)
{
	ssdWriteCnt = writeCnt;
}