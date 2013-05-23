/*
linux file utility includes user space and kernel space file operation.
Author: Yingcai.Tan
*/
#ifndef __LOGFILE_INFO_H__
#define __LOGFILE_INFO_H__

#ifdef __cplusplus
       extern "C" {
#endif

#ifdef WIN32
#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif
#else
#define DLL_API 
#endif

#define	LOG_CONFIG_FILE	"/mnt/mtd/log.cfg"
#define	MAX_LOGFILE_INFO_SIZE	(5000*1024)

#define	DBG_MSG	1	//debug message switch, use to printf to screen
#define	LOG_MSG	1	//use to save to logfile, save to logfile, add by freeman
#define	LOGFILE_INFO		"libpsd.log"
#define	LOGFILE_DEBUG		"libpsd_debug.log"


struct LogCfgPara
{
	char sSaveMediaType[16];
	char sSaveLogPath[128];
	unsigned long MaxLogFileSize;
};
//}__attribute__((__packed__));

typedef struct LogCfgPara tLogPara;

DLL_API void Log_MsgNew(char *LogFileName);		//create a log file

/*
Func: append a line to the log file. if the log file does not exist, create it. if its size exceeds MAX_LOGFILE_INFO_SIZE, delete and create it again.
Example:
	char line[256];
	sprintf(line,"Error in ioctl of %s\n",__FUNCTION__);
	Log_MsgLine("drvlib.log",line);	
*/
void Log_MsgLine(char *LogFileName,char *sLine);






#ifdef __cplusplus
}
#endif 

#endif			//__LOGFILE_INFO_H__

