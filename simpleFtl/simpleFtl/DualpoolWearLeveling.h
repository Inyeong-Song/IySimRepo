#pragma once
#include "Common.h"
#include "SectorMappingTable.h"
#include "VirtualBlock.h"
#include "SimpleFtl.h"

class DualpoolWearLeveling
{
public:
	DualpoolWearLeveling(MapTable& map, BlockDirectory& blkDir, SimpleFtl& simpleFtl);

private:
	MapTable& map;
	BlockDirectory& blkDir;
	SimpleFtl& simpleFtl;

};