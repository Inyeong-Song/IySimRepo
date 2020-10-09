#include "FtlConfig.h"

typedef enum _HostCmdType
{
	AdminCmd = 0,
	IoCmdRxDma,
	IoCmdTxDma
} HostCmdType;

typedef enum _NandCmdType
{
	ReadTrigger = 0,
	ReadTransfer,
	WriteTrigger,
	EraseTrigger,
	CheckStatus,
	CheckControllerBusy
} NandCmdType;

typedef enum _RequestQueueType
{
	FreeRequestQ = 0,
	SliceRequestQ,
	NvmeRequestQ,
	NandRequestQ
} RequestQueueType;

typedef struct _NvmeDmaInfo
{
	Int64 nvmeCmdSlotTag;
	Int64 nvmeDmaStartIndex;
	Int64 nvmeDmaOffset;
	Int64 nvmeBlockCnt;

} NvmeDmaInfo;

typedef struct _DataBufInfo
{
	Int64 DataBufAddr;
	Int64 DataBufIndex;

} DataBufInfo;

typedef struct _NandDmaInfo
{
	Int64 logicalSliceAddr;
	Int64 virtualSliceAddr;
	Int32 physicalCh;
	Int32 physicalWay;
	Int32 physicalBlock;
	Int32 physicalPage;

} NandDmaInfo;

class SsdRequestFormat
{
public:
	SsdRequestFormat();
	~SsdRequestFormat();



private:
	HostCmdType hostCmd;
	NandCmdType nandCmd;
	RequestQueueType reqQ;
	NvmeDmaInfo nvmeDma;
	DataBufInfo dataBuf;
	NandDmaInfo nandDma;

	Int64 parentReq;
	Int32 remainChildReqCnt;

};