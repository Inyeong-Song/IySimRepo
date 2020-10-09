#include "VirtualBlock.h"

class VirtualDie: public VirtualBlock
{
public:
	VirtualDie();
	~VirtualDie();



	Int64 GetFreeBlock();
	void PutToFreeBlock(Int64 dieNo, Int64 blockNo);

private:
	Int64 prevDie;
	Int64 nextDie;
	Int64 currentBlock;
	Int64 freeBlockCnt;
	Int64 headFreeBlock;
	Int64 tailFreeBlock;
};