#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <cassert>
#include <algorithm>
#include <time.h>
#include <climits>
#include <io.h>
#include <time.h>
#include <cmath>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#define INV_VALUE (-1)
#define SECTOR_SIZE_KB (4)
#define TOTAL_SUPER_BLOCK_CNT (200)
#define SECTOR_PER_SUPER_BLOCK (100)
#define PERCENT_OF_FREE_BLOCK_FOR_GC (8) //free block %
#define PERCENT_OF_OVERPROVISIONING (20) //op %
#define GC_SELECT (0) //0:Greedy 1:CAT 
#define FREE_CONDITIONING (1)
#define DUAL_POOL (0)
#define THRESH_HOLD_VALUE_FOR_DUALPOOL (4)

#define PRINT_GC (0)
#define PRINT_TRANS_ADDR (0)

using namespace std;

typedef int8_t		Int8;
typedef uint8_t		UInt8;
typedef int16_t		Int16;
typedef uint16_t	UInt16;
typedef int32_t		Int32;
typedef uint32_t	UInt32;
typedef int64_t		Int64;
typedef uint64_t	UInt64;