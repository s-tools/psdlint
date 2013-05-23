#include <stdio.h>
#include <stdlib.h>
#include "bmpfile.h"
#include "psd_system.h"

#if 0
#define PIXPLINE  320

int bmp_read(unsigned char *image, int xsize, int ysize, char *filename) 
{
    char fname_bmp[128];
    FILE *fp;
    unsigned char header[54];
    sprintf(fname_bmp, "%s.bmp", filename);
    
    if (!(fp = fopen(fname_bmp, "rb"))) 
      return -1;
      
    
    fread(header, sizeof(unsigned char), 54, fp);
    fread(image, sizeof(unsigned char), (size_t)(long)xsize * ysize * 3, fp);
    
    fclose(fp);
    return 0;
}

int bmp_write(unsigned char *image, int xsize, int ysize, char *filename) 
{
    unsigned char header[54] = {
      0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
        54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 24, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0
    };
    long file_size,width,height;
    char fname_bmp[128];
    FILE *fp;
    
    file_size = (long)xsize * (long)ysize * 3 + 54;
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
    
    width = xsize;
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;
    
    height = ysize;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;
  
    sprintf(fname_bmp, "%s.bmp", filename);
    
    if (!(fp = fopen(fname_bmp, "wb"))) 
      return -1;
      
    fwrite(header, sizeof(unsigned char), 54, fp);
    fwrite(image, sizeof(unsigned char), (size_t)(long)xsize * ysize * 3, fp);
    
    fclose(fp);
    return 0;
}

void clonebmp(unsigned char *image,int xsize,int ysize)
{
    bmp_read(image, xsize, ysize, "orgbmp");  //orgbmpΪ��ǰĿ¼�µ�bmp�ļ���
    bmp_write(image, xsize, ysize, "clone_bmp");//clone_bmpΪ��¡��bmp�ļ���
}


    /**//****************************************************************************
* ���ƣ�youwritetobmp()
* ���ܣ�д��bmp�ļ�
* ��ڲ�����RGBQUAD *pixarr ---- Ҫд�����������ָ��, int xsize ---- ͼ����, int ysize ---- ͼ��߶�, char *filename --ͼ������
* ���ڲ�������
* ����ֵ��-1:����  ��0����ȷ
****************************************************************************/
int youwritetobmp(RGBQUAD *pixarr, int xsize, int ysize, char *filename) {
    unsigned char header[54] = {
      0x42, 0x4d, 0, 0, 0, 0, 0, 0, 0, 0,
        54, 0, 0, 0, 40, 0, 0, 0, 0, 0, 
        0, 0, 0, 0, 0, 0, 1, 0, 24, 0, 
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
        0, 0, 0, 0
    };
    int i;
    int j;
    long width,height;
    char fname_bmp[128];
    FILE *fp;
    long file_size = (long)xsize * (long)ysize * 3 + 54;
    RGBQUAD zero={0,0,0};  //�����ֽڣ���zero���
    
    header[2] = (unsigned char)(file_size &0x000000ff);
    header[3] = (file_size >> 8) & 0x000000ff;
    header[4] = (file_size >> 16) & 0x000000ff;
    header[5] = (file_size >> 24) & 0x000000ff;
    
   
    if(!(xsize%4))    width=xsize;
    else            width= xsize+(4-xsize%4);    //�粻��4�ı�������ת����4�ı���
    header[18] = width & 0x000000ff;
    header[19] = (width >> 8) &0x000000ff;
    header[20] = (width >> 16) &0x000000ff;
    header[21] = (width >> 24) &0x000000ff;
    
    height = ysize;
    header[22] = height &0x000000ff;
    header[23] = (height >> 8) &0x000000ff;
    header[24] = (height >> 16) &0x000000ff;
    header[25] = (height >> 24) &0x000000ff;

    
    sprintf(fname_bmp, "%s.bmp", filename);
    
    
    if (!(fp = fopen(fname_bmp, "wb"))) 
      return -1;
      
    fwrite(header, sizeof(unsigned char), 54, fp);

    
    
    for(j=0;j<ysize;j++){
        if(!(xsize%4)){
            for(i=0;i<xsize;i++){ 
                 fwrite(pixarr+i, sizeof(RGBQUAD),1, fp);
            }
        }
        else
        {
            for(i=0;i<xsize;i++){ 
                 fwrite(pixarr+i, sizeof(RGBQUAD),1, fp);
            }
            for(i=xsize;i<xsize+(4-xsize%4);i++){
                fwrite(&zero, sizeof(RGBQUAD),1, fp);
            }
        }
    }
    
    fclose(fp);
    return 0;

}

int TestBmpio() 
{
    unsigned char *image;
    int xsize = 320;
    int ysize = 163;

    RGBQUAD pixarray[PIXPLINE]; //һ������ֵ����

    image = (unsigned char *)malloc((size_t)xsize * ysize * 3);
    if (image == NULL) 
      return -1;
    clonebmp(image,xsize,ysize);    //����һ��ʵ��һ��ͼ��Ŀ���

    youwritetobmp(pixarray,xsize,ysize,"yourimage");//���ܶ���ʵ�����صľ�ȷ����,����ֵ��pixarray������
    
    free(image);
    return 0;
}

#endif

//bpp: bit per pixel, only support 24 bits per pixel. 
int bmp_save(char* filename,unsigned char *buffer,int width,int height,int bpp)
{
	FILE *fp=NULL;
	BMP_HEADER bmphead;
	int Byte_per_pixel=3;

	if (bpp==8)
		Byte_per_pixel=1;
	else if (bpp==24)
		Byte_per_pixel=3;
	else
		Byte_per_pixel=3;

	memset(&bmphead,0,sizeof(bmphead));
	bmphead.bmpid[0]='B';
	bmphead.bmpid[1]='M';
	bmphead.filesize=width*height*Byte_per_pixel+sizeof(bmphead);
	bmphead.imgoff=BMP_HEADERSIZE;
	bmphead.headsize=40;
	bmphead.imgwidth=width;
	bmphead.imgheight=height;
	bmphead.planes=1;
	bmphead.bitcount=bpp;
	bmphead.compression=0;  

	fp=fopen(filename,"wb");
	if(fp!=NULL)
	{
		rewind(fp);
		fwrite(&bmphead,sizeof(bmphead),1,fp);			//write header
		fwrite(buffer,sizeof(char),width*height*Byte_per_pixel,fp);  	//write data
		fclose(fp);
	}

	return 0;
 }

//bpp: bit per pixel, only support 24 bits per pixel. 
int bmp_load(char* filename,BMP_HEADER **pBmpHeader,unsigned char **pBmpBuff)
{
	FILE *fp=NULL;
	long rc=0;
	long BufferLen=0;
	int ret=-1;
	BMP_HEADER *pHeader;
	unsigned char *pBuff;
	
	pHeader=psd_malloc(sizeof(BMP_HEADER));
	
	fp=fopen(filename,"rb");
	if(fp!=NULL)
	{
		rewind(fp);

		rc = fread(pHeader,1,BMP_HEADERSIZE,fp);
		if (rc < 0)
		{
			printf("Read bmp file fail!!!\n");
			
			goto End_Err1;
		}

		if ((pHeader->bmpid[0]!='B')||(pHeader->bmpid[1]!='M'))
		{
			printf("Bmp file is crupted!!!\n");
			goto End_Err1;
		}

		//start to read data in
		switch (pHeader->bitcount)
		{
			case 24:
				BufferLen = pHeader->imgwidth*pHeader->imgheight*3;
				pBuff = psd_malloc(BufferLen);
				if (pBuff)
				{
					rc = fread(pBuff,1,BufferLen,fp);	//continue to read
					//need swap bgr to rgb??
					if (rc < 0)
					{
						goto End_Err2;						
					}
					else
					{
						*pBmpHeader = pHeader;
						*pBmpBuff = pBuff;
					}
				}
				else
				{
					goto End_Err1;
				}
			default:
			//	printf("we only support 24 bit bmpfile\n");
				break;
		}
		fclose(fp);
	}

	return 0;
End_Err1:
	free(pHeader);
	pHeader = NULL;
	fclose(fp);
	return ret;
End_Err2:
	free(pHeader);
	pHeader = NULL;
	free(pBuff);		
	pBuff = NULL;
	fclose(fp);
	return ret;	
 }

