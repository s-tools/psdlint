#ifndef __JPGFILE_H__
#define __JPGFILE_H__
#ifdef __cplusplus
extern "C" {
#endif

#pragma pack (1)		//byte allign start


#pragma pack ()		//byte allign end

int jpg_save(char * filename,unsigned char *image_buffer,int image_width,int image_height,int quality);
unsigned char *jpg_load(char *fileName,unsigned int *width,unsigned int *height);


#ifdef __cplusplus
}
#endif
#endif