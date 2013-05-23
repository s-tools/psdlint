/*
linux file utility includes user space and kernel space file operation.
Author: Yingcai.Tan

fflush（更新缓冲区） 
相关函数  write，fopen，fclose，setbuf
表头文件  #include<stdio.h>
定义函数  int fflush(FILE* stream);
函数说明  fflush()会强迫将缓冲区内的数据写回参数stream指定的文件中。如果参数stream为NULL，fflush()会将所有打开的文件数据更新。
返回值  成功返回0，失败返回EOF，错误代码存于errno中。
错误代码  EBADF 参数stream 指定的文件未被打开，或打开状态为只读。其它错误代码参考write（）。
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "logfile.h"

//module global varible here
static tLogPara gLogSetting;

static unsigned long fGetSize(char *file_name)
{
	unsigned long offset, size;
	FILE *file=fopen(file_name, "rb+");

	if (file==NULL)
		return 0;

	offset = ftell(file);
	fseek(file, 0, SEEK_END);
	size = ftell(file);
	fseek(file, 0, SEEK_SET);
	fseek(file, offset, SEEK_CUR);

	fclose(file);
	return size;
}

static void LogCfgInit(void)
{
	memset((void *)&gLogSetting,0,sizeof(gLogSetting));
	strcpy(gLogSetting.sSaveMediaType,"HDISK");
	strcpy(gLogSetting.sSaveLogPath,"");
	gLogSetting.MaxLogFileSize=MAX_LOGFILE_INFO_SIZE;
}

DLL_API void Log_MsgNew(char *LogFileName)
{
	FILE *fp;
	static char FullFileName[128];

	LogCfgInit();			//init as default

	strcpy(FullFileName,gLogSetting.sSaveLogPath);
	strcat(FullFileName,LogFileName);
	
	fp = fopen(FullFileName, "w+");
}

/*
Func: append a line to the log file. if the log file does not exist, create it. if its size exceeds MAX_LOGFILE_SIZE, delete and create it again.
Example:
	char line[256];
	sprintf(line,"Error in ioctl of %s\n",__FUNCTION__);
	Log_MsgLine("drvlib.log",line);	
*/
void Log_MsgLine(char *LogFileName,char *sLine)
{
	FILE *fp;
	unsigned long fSize;
	static char FullFileName[128];

	LogCfgInit();			//init as default

	//printf("MediaType is:%s\n",gLogSetting.sSaveMediaType);
	//printf("log path is:%s\n",gLogSetting.sSaveLogPath);
	//printf("log file size is:%dKB\n",gLogSetting.MaxLogFileSize);

	strcpy(FullFileName,gLogSetting.sSaveLogPath);
	strcat(FullFileName,LogFileName);		//merge two string as one
	
	//printf("full log name is:%s\n",FullFileName);

	fSize=fGetSize(FullFileName);
	
	if ((fSize > gLogSetting.MaxLogFileSize) ||(fSize==0))		//too large or no logfile exist, create it
		fp = fopen(FullFileName, "w+");		//reset to zero
	else
		fp = fopen(FullFileName, "a+");		//append to the end of logfile
	
	if (fp)
	{
		fputs(sLine,fp);		//write a line to file
		fflush(fp);		//only block device has buffer(cache) requiring flush operation.
		fclose(fp);
	}
}











#ifdef __cplusplus
}
#endif 

