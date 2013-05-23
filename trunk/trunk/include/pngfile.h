#ifndef __PNGFILE_H__
#define __PNGFILE_H__
#ifdef __cplusplus
extern "C" {
#endif
#include "png.h"

#pragma pack (1)		//byte allign start



#pragma pack ()		//byte allign end

extern unsigned char *png_load(char *filename,unsigned int *width,unsigned int *height);
extern int png_save(char * filename,unsigned char *img_buf,int width,int height);

extern int png2png(char *inname, char *outname);


#ifdef __cplusplus
}
#endif
#endif
