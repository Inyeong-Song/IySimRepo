#include "VirtualBlock.h"

BlockDirectory::BlockDirectory(Int64 blockCnt)
{
	blkState = new BlockStat[blockCnt];
}
/*
bool BlockDirectory::CompEraseCnt(list<Int64>::iterator itrA, list<Int64>::iterator itrB)
{
	Int64 vbnA_EraseCnt = blkState[*itrA].eraseCnt;
	Int64 vbnB_EraseCnt = blkState[*itrB].eraseCnt;
	if ((vbnA_EraseCnt == 0) && (vbnB_EraseCnt == 0))
		return true;
	if (vbnA_EraseCnt < vbnB_EraseCnt)
		return true;
	else
		return false;
}
void BlockDirectory::SwapBlockElement(list<Int64>& blockList, list<Int64>::iterator& itrA, list<Int64>::iterator& itrB)
{
	Int64 vbnA = *itrA;
	Int64 vbnB = *itrB;
	itrA = blockList.erase(itrA);
	blockList.insert(itrA, vbnB);
	advance(itrA, -1);
	itrB = blockList.erase(itrB);
	blockList.insert(itrB, vbnA);
	advance(itrB, -1);
}
void BlockDirectory::SortBlockListByEraseCnt(list<Int64>& blockList)
{
	if (blockList.size() < 2)
		return;
	else
	{
		Int64 key;
		for (key = 1; key < blockList.size(); key++)
		{
			auto itrA = blockList.begin();
			auto itrB = blockList.begin();
			advance(itrA, key - 1);
			advance(itrB, key);
			while (1)
			{
				if (!CompEraseCnt(itrA, itrB))
				{
					SwapBlockElement(blockList, itrA, itrB);
				}
				if (itrA == blockList.begin())
					break;
				else
				{
					itrA--;
					itrB--;
				}
			}
		}
	}
	//Debug printf
	printf("Sorted Block List\n");
	Int64 i = 0;
	for (auto itr = blockList.begin(); itr != blockList.end(); itr++, i++)
	{
		printf("[%ld] vbn:%ld eraseCnt:%ld\n", i,*itr, blkState[*itr].eraseCnt);
	}
 }
*/
