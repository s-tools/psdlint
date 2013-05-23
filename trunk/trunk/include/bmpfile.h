#ifndef __BMPFILE_H__
#define __BMPFILE_H__
#ifdef __cplusplus
extern "C" {
#endif

#pragma pack (1)		//byte allign start
#define BMP_HEADERSIZE 54
typedef struct
{
	char bmpid[2];  		//BM
	unsigned int filesize; 	//file size = header len+datasize
	unsigned int reserved1; //must be 0L
	unsigned int imgoff; 	//general 54L
	unsigned int headsize; 	//40L
	unsigned int imgwidth;
	unsigned int imgheight;
	unsigned short planes; 	//must be 1L
	unsigned short bitcount;
	unsigned int compression;
	unsigned int datasize;	//raw data len
	unsigned int hres;
	unsigned int vres;
	unsigned int colors;
	unsigned int impcolors; 
}BMP_HEADER;
//}__attribute__((packed,aligned(1)));

typedef struct tagRGBQUAD{     //定义每个像素的数据类型
        unsigned char  rgbBlue;
        unsigned char  rgbGreen;
        unsigned char  rgbRed;
}RGBQUAD;

#pragma pack ()		//byte allign end

int bmp_save(char* filename,unsigned char *buffer,int width,int height,int bpp);
int bmp_load(char* filename,BMP_HEADER **pBmpHeader,unsigned char **pBmpBuff);




#ifdef __cplusplus
}
#endif
#endif
