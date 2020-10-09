#pragma once
#include "Common.h"

class MapTableInterface
{
public:
	virtual Int64 GetVirtualAddr(Int64 logicalAddr) = 0;
	virtual Int64 GetLogicalAddr(Int64 virtualAddr) = 0;

	virtual void SetLtovMap(Int64 logicalAddr, Int64 virtualAddr) = 0;
	virtual void SetVtolMap(Int64 virtualAddr, Int64 logicalAddr) = 0;
};

class MapTable : public MapTableInterface
{
public:
	MapTable(Int64 totalKbSize, Int64 entryKbSize);
	~MapTable();
	Int64 GetVirtualAddr(Int64 logicalAddr) override { return mapLtoV[logicalAddr]; }
	Int64 GetLogicalAddr(Int64 virtualAddr) override { return mapVtoL[virtualAddr]; }

	void SetLtovMap(Int64 logicalAddr, Int64 virtualAddr) override { mapLtoV[logicalAddr] = virtualAddr; }
	void SetVtolMap(Int64 virtualAddr, Int64 logicalAddr) override { mapVtoL[virtualAddr] = logicalAddr; }

private:
	Int64 TOTAL_ENTRY_CNT;

	Int64 *mapLtoV;
	Int64 *mapVtoL;
};
