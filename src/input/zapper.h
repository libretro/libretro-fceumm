#ifndef _ZAPPER_H_
#define _ZAPPER_H_

#include "../fceu-types.h"

typedef struct ZAPPER {
	uint32 mzx, mzy, mzb, mzs;
	int zap_readbit;
	uint8 bogo;
	int zappo;
	uint64 zaphit;
} ZAPPER;

extern ZAPPER ZD[2];

#endif
