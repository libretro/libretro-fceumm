#ifndef FDS_APU_H
#define FDS_APU_H

void FDSSoundReset(void);
void FDSSoundStateAdd(void);

/* Used for fds conversion-based mappers to allow access to fds apu registers */
void FDSSoundPower(void); 
DECLFR(FDSSoundRead);   /* $4040-$407F, $4090-$4092 */
DECLFW(FDSSoundWrite);  /* $4040-$407F, $4080-$408A */

#endif /* FDS_APU_H */