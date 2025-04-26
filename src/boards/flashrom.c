#include "flashrom.h"

uint8		flashrom_manufacturerID;
uint8		flashrom_modelID;
int		flashrom_state;
uint32		flashrom_sectorSize;
int             flashrom_timeOut;
uint32		flashrom_magicAddr1;
uint32		flashrom_magicAddr2;
uint32		flashrom_magicMask;

void flashrom_init (uint8 manufacturerID, uint8 modelID, uint32 sectorSize, uint32 magicAddr1, uint32 magicAddr2, uint32 magicMask) {
	flashrom_manufacturerID =manufacturerID;
	flashrom_modelID =modelID;
	flashrom_sectorSize =sectorSize;
	flashrom_magicAddr1 =magicAddr1;
	flashrom_magicAddr2 =magicAddr2;
	flashrom_magicMask =magicMask;
}

uint8 flashrom_read (uint32 A) {
	if (flashrom_state ==0x90) /* software ID */
		return A &1? flashrom_modelID: flashrom_manufacturerID;
	else
	if (flashrom_timeOut)
		return (CartBR(A) ^(flashrom_timeOut &1? 0x40: 0x00)) &~0x88;
	else
		return CartBR(A);
}

void flashrom_write (uint32 fullAddress, uint8 V) {
	uint32 i;
	uint8* sector =NULL;
	uint32 compare =fullAddress &flashrom_magicMask;
	fullAddress &=PRGsize[0] -1;
	switch (flashrom_state) {
		/* command start */
		default:   if (compare ==flashrom_magicAddr1 && V ==0xAA) flashrom_state++; break;
		case 0x01: if (compare ==flashrom_magicAddr2 && V ==0x55) flashrom_state++; break;
		case 0x02: if (compare ==flashrom_magicAddr1) flashrom_state =V; break;
		/* sector or chip erase */
		case 0x80: if (compare ==flashrom_magicAddr1 && V ==0xAA) flashrom_state++; break;
		case 0x81: if (compare ==flashrom_magicAddr2 && V ==0x55) flashrom_state++; break;
		case 0x82: if (V ==0x30) { /* sector erase */
				/* Determine the actual sector start by masking with the sector size */
				fullAddress &=~(flashrom_sectorSize -1);
				sector =PRGptr[0] +fullAddress;
				for (i =0; i <flashrom_sectorSize; i++) sector[i] =0xFF;
				flashrom_timeOut =flashrom_sectorSize;
			   } else
			   if (V ==0x10 && compare ==flashrom_magicAddr1) { /* chip erase */
				for (i =0; i<=PRGsize[0]; i++) PRGptr[0][i] =0xFF;
				flashrom_timeOut =PRGsize[0];
			   } else
			   if (V ==0xF0) flashrom_state =0;
			   break;
		/* software ID */
		case 0x90: if (V ==0xF0) flashrom_state =0; break;
		/* byte program */
		case 0xA0: PRGptr[0][fullAddress] =V;
			   flashrom_state =0;
			   break;
	}
}

void FP_FASTAPASS(1) flashrom_cpuCycle(int a) {
	while (a--) {
		if (flashrom_timeOut && !--flashrom_timeOut) flashrom_state =0;
	}
}
