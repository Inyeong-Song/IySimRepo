#include "SectorMappingTable.h"

MapTable::MapTable(Int64 totalKbSize, Int64 entryKbSize)
{
	TOTAL_ENTRY_CNT = totalKbSize / entryKbSize;

	mapLtoV = new Int64[TOTAL_ENTRY_CNT];
	memset(mapLtoV, INV_VALUE, sizeof(Int64)* TOTAL_ENTRY_CNT);

	mapVtoL = new Int64[TOTAL_ENTRY_CNT];
	memset(mapVtoL, INV_VALUE, sizeof(Int64)* TOTAL_ENTRY_CNT);
}
MapTable::~MapTable()
{
	delete[] mapLtoV;
	delete[] mapVtoL;
}
