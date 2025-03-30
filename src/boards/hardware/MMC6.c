#include "mapinc.h"

DECLFR(MMC6_read0) {
	return 0;
}

void	MMC6_syncWRAM () {
	uint16 A;
	setprg4r(0x10, 0x7000, OR <<1);
	for (A =0x7000; A <=0x7FFF; A |=0x400) {
		/* "If neither bank is enabled for reading, the $7000-$7FFF area is open bus. If only one bank is enabled for reading, the other reads back as zero." */
		SetReadHandler(A |0x000, A |0x1FF, ~index &0x20 || ~MMC3_wramEnable &0x20 && ~MMC3_wramEnable &0x80? NULL: ~MMC3_wramEnable &0x20 && MMC3_wramEnable &0x80? MMC6_read0: CartBR);
		SetReadHandler(A |0x200, A |0x3FF, ~index &0x20 || ~MMC3_wramEnable &0x20 && ~MMC3_wramEnable &0x80? NULL: ~MMC3_wramEnable &0x80 && MMC3_wramEnable &0x20? MMC6_read0: CartBR);
		/* "The write-enable bits only have effect if that bank is enabled for reading, otherwise the bank is not writable." */
		SetWriteHandler(A |0x000, A |0x1FF, index &0x20 && MMC3_wramEnable &0x20 && MMC3_wramEnable &0x10? CartWR: NULL);
		SetWriteHandler(A |0x200, A |0x3FF, index &0x20 && MMC3_wramEnable &0x80 && MMC3_wramEnable &0x40? CartWR: NULL);
	}
}
