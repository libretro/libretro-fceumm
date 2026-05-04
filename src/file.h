#ifndef _FCEU_FILE_H
#define _FCEU_FILE_H

typedef struct {
   uint8_t *data_int;
   const uint8_t *data;
   uint32_t size;
   uint32_t location;
} MEMWRAP;

typedef struct {
   MEMWRAP *fp;
} FCEUFILE;

FCEUFILE *FCEU_fopen(const char *path, const uint8_t *buffer, size_t bufsize);
int FCEU_fclose(FCEUFILE*);
uint64_t FCEU_fread(void *ptr, size_t size, size_t nmemb, FCEUFILE*);
int FCEU_fseek(FCEUFILE*, long offset, int whence);
uint64_t FCEU_ftell(FCEUFILE*);
int FCEU_read32le(uint32_t *Bufo, FCEUFILE*);
int FCEU_fgetc(FCEUFILE*);
uint64_t FCEU_fgetsize(FCEUFILE*);

#endif
