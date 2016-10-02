/********************************************************************************
 * cir main fun
 *
 *
 * Modify History
 * ------------------------------------------------------------------------------
 * 2010-06-14 22:30
 * author: zhaoming
 *
 *
 ********************************************************************************/

/* includes */

#include <string.h>
#include <time.h>
#include <stdarg.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#include "zmdbg.h"

/* constants define */

#define	cdbg_module_name

/* global define */
//static pthread_mutex_t bstm_mutexValue = PTHREAD_MUTEX_INITIALIZER;
cdbg_config_t	cdbgConfig;

/* funs here */
static	cdbg_level_t cdbg_mgr[]={
	cdbg_make_level_str(Enter),
	cdbg_make_level_str(Debug),
	cdbg_make_level_str(Minor),
	cdbg_make_level_str(Major),
	cdbg_make_level_str(Infor),
	cdbg_make_level_str(Error),
	cdbg_make_level_str(Fatal),
	cdbg_make_level_str(Leave)
};

//static	int	cdbg_init_flag = 0 ;
//static 	cint8	cdbg_mod[cdbg_module_name_size];
int	cprintf( char *args, ... )
{
	va_list ap;

	va_start( ap , args );
	vprintf( args , ap );
	va_end( ap );
	
	return 0;
}

int cdbg_cfgValueHandle_set(int modLineNum)
{
	cdbg_out_type_e ot = cdbgConfig.modules[modLineNum].outType;

	if(ot&cdbg_out_type_stdout)
	{
	//	printf("Module <%s> will out to stdout....\n", cdbgConfig.modules[modLineNum].moduleName);
	}

	if(ot&cdbg_out_type_file)
	{
	//	printf("Module <%s> will out to file[%s]....\n", cdbgConfig.modules[modLineNum].moduleName,cdbgConfig.modules[modLineNum].filePath);
		if(cdbgConfig.modules[modLineNum].fileHandle == NULL)
		{
			cdbgConfig.modules[modLineNum].fileHandle = fopen(cdbgConfig.modules[modLineNum].filePath, "w+");
			if(cdbgConfig.modules[modLineNum].fileHandle == NULL)
			{
				cdbgConfig.modules[modLineNum].outType ^= cdbg_out_type_file;
			}
		}
	}

	if(ot&cdbg_out_type_syslog)
	{
	//	printf("Module <%s> will out to syslog....\n", cdbgConfig.modules[modLineNum].moduleName);

	}

	if(ot&cdbg_out_type_server)
	{
	//	printf("Module <%s> will out to server....\n", cdbgConfig.modules[modLineNum].moduleName);

	}
	return 0;
}

int cdbg_cfgValue_set(char *p, int modLineNum)
{
	char *str = NULL;
	int i;

	if((str = strstr(p, cdbg_config_flag_str_module))!= NULL)
	{
	//	printf("== module = %s   \n", (char *)(str + sizeof(cdbg_config_flag_str_module) - 1));
		strcpy(cdbgConfig.modules[modLineNum].moduleName, (char *)(str + sizeof(cdbg_config_flag_str_module) - 1));
	}

	if((str = strstr(p, cdbg_config_flag_str_level))!= NULL)
	{
//		printf("==lever  = %s   \n", (char *)(str + sizeof(cdbg_config_flag_str_level) - 1));
		for(i=Enter+1; i<Leave; i++)
		{
			if(!strcasecmp(cdbg_mgr[i].levelStr, (char *)(str + sizeof(cdbg_config_flag_str_level) - 1)))
			{
				cdbgConfig.modules[modLineNum].dbgLevel = i;
				break;
			}
		}
	}

	if((str = strstr(p, cdbg_config_flag_str_reread))!= NULL)
	{
		if(!strcmp("true", (char *)(str + sizeof(cdbg_config_flag_str_reread) - 1)))
		{
			cdbgConfig.reread =  true;
		}
		else
		{
			cdbgConfig.reread = false;
		}
	}

	if((str = strstr(p, cdbg_config_flag_str_time))!= NULL)
	{		
		if(!strcmp("true", (char *)(str + sizeof(cdbg_config_flag_str_time) - 1)))
		{
			cdbgConfig.modules[modLineNum].dateTime =  true;
		}
		else
		{
			cdbgConfig.modules[modLineNum].dateTime = false;
		}
	}

	if((str = strstr(p, cdbg_config_flag_str_flf))!= NULL)
	{		
		if(!strcmp("true", (char *)(str + sizeof(cdbg_config_flag_str_flf) - 1)))
		{
			cdbgConfig.modules[modLineNum].flf=  true;
		}
		else
		{
			cdbgConfig.modules[modLineNum].flf= false;
		}
	}

	if((str = strstr(p, cdbg_config_flag_str_outStd))!= NULL)
	{		
		if(!strcmp("true", (char *)(str + sizeof(cdbg_config_flag_str_outStd) - 1)))
		{
			cdbgConfig.modules[modLineNum].outType |= cdbg_out_type_stdout;
		}
		else
		{
			if((cdbgConfig.modules[modLineNum].outType)&cdbg_out_type_stdout)
			{
				cdbgConfig.modules[modLineNum].outType ^= cdbg_out_type_stdout;
			}
		}
	}

	if((str = strstr(p, cdbg_config_flag_str_outfile))!= NULL)
	{		
		if(!strcmp("true", (char *)(str + sizeof(cdbg_config_flag_str_outfile) - 1)))
		{
			cdbgConfig.modules[modLineNum].outType |= cdbg_out_type_file;
		}
		else
		{
			if((cdbgConfig.modules[modLineNum].fileHandle != NULL)&&
				((cdbgConfig.modules[modLineNum].outType)&cdbg_out_type_file))
			{
				fclose(cdbgConfig.modules[modLineNum].fileHandle);
			}
			if((cdbgConfig.modules[modLineNum].outType)&cdbg_out_type_file)
			{
				cdbgConfig.modules[modLineNum].outType ^= cdbg_out_type_file;
			}
			cdbgConfig.modules[modLineNum].fileHandle = NULL;
		}
	}

	if((str = strstr(p, cdbg_config_flag_str_outfileName))!= NULL)
	{		
		cdbgConfig.modules[modLineNum].filePath = (char *)(str + sizeof(cdbg_config_flag_str_outfileName) - 1);
	}

	if((str = strstr(p, cdbg_config_flag_str_outSyslog))!= NULL)
	{		
		if(!strcmp("true", (char *)(str + sizeof(cdbg_config_flag_str_outSyslog) - 1)))
		{
			cdbgConfig.modules[modLineNum].outType |= cdbg_out_type_syslog;
		}
		else
		{
			if((cdbgConfig.modules[modLineNum].outType)&cdbg_out_type_syslog)
			{
				cdbgConfig.modules[modLineNum].outType ^= cdbg_out_type_syslog;
			}
		}
	}

	if((str = strstr(p, cdbg_config_flag_str_outServer))!= NULL)
	{		
		if(!strcmp("true", (char *)(str + sizeof(cdbg_config_flag_str_outServer) - 1)))
		{
			cdbgConfig.modules[modLineNum].outType |= cdbg_out_type_server;
		}
		else
		{
			if((cdbgConfig.modules[modLineNum].outType)&cdbg_out_type_server)
			{
				cdbgConfig.modules[modLineNum].outType ^= cdbg_out_type_server;
			}
		}
	}

	return 0;
}
int cdbg_cfgLine_read(char *line, int modLineNum)
{
	int size;
	if(line == NULL)
	{
		return 0;
	}
	
	char *del = ";,";
	char *p;

	size = 0;
	p = strtok(line, del);
	if(p == NULL)
	{
		return 0;
	}
	cdbg_cfgValue_set(p, modLineNum);
	while((p = strtok(NULL, del)))
	{
		cdbg_cfgValue_set(p, modLineNum);
	}

	return 0;
}

int cdbg_cfgFreeMod_get(void)
{
	int i;
	for(i=0; i<cdbg_module_num_max; i++)
	{
		if(!cdbgConfig.modules[i].used)
		{
			return i;
		}
	}
	cprintf("[CDBG]<Error>: Not Free Module Position!!!\n");
	return -1;
}

int cdbg_cfgMod_del(int cfgModId)
{
//	printf("[CDBG]<Error>: Del the [%s]\n", cdbgConfig.modules[cfgModId].moduleName);
	cdbg_out_type_e ot = cdbgConfig.modules[cfgModId].outType;

	if(ot&cdbg_out_type_stdout)
	{
	//	printf("Remove the Module <%s> STDOUT ....\n", cdbgConfig.modules[cfgModId].moduleName);
		//--- TODO remove the stdout opened param
	}

	if(ot&cdbg_out_type_file)
	{
	//	printf("Remove the Module <%s> FILE ....\n", cdbgConfig.modules[cfgModId].moduleName);
		if(cdbgConfig.modules[cfgModId].fileHandle != NULL)
		{
			fclose(cdbgConfig.modules[cfgModId].fileHandle);
		}
		cdbgConfig.modules[cfgModId].fileHandle = NULL;
	}

	if(ot&cdbg_out_type_syslog)
	{
	//	printf("Remove the Module <%s> SYSLOG ....\n", cdbgConfig.modules[cfgModId].moduleName);
		//--- TODO remove the syslog opened param
	}

	if(ot&cdbg_out_type_server)
	{
	//	printf("Remove the Module <%s> SERVER ....\n", cdbgConfig.modules[cfgModId].moduleName);
		//--- TODO remove the server opened param
	}

	memset(cdbgConfig.modules[cfgModId].moduleName, 0, cdbg_module_name_len_max);
	cdbgConfig.modules[cfgModId].dbgLevel = 0;
	cdbgConfig.modules[cfgModId].outType = 0;
	cdbgConfig.modules[cfgModId].flf = false;
	cdbgConfig.modules[cfgModId].dateTime = false;
	cdbgConfig.modules[cfgModId].fileHandle = NULL;
	cdbgConfig.modules[cfgModId].syslogHandle = 0;

	cdbgConfig.modules[cfgModId].used = false;
	return 0;
}

int cdbg_cfgFile_read(char *privCfgPath, char *globalCfgPath)
{
	int	fd = -1;
	int	modFree = -1;

//	memset(&cdbgConfig.modules, 0x00, sizeof(cdbg_module_t)*cdbg_module_num_max);
	fd = open(privCfgPath, O_RDONLY);
	if(fd == -1)
	{
		return -1;
	}
	cdbgConfig.configFileHandle = fd;
	char *buff = (char *)malloc(cdbg_config_file_len_max);
	memset(buff, 0, cdbg_config_file_len_max);
	if(buff == NULL)
	{
		close(fd);
		return -2;
	}

	int size = read(fd, buff, cdbg_config_file_len_max);
	if((size == cdbg_config_file_len_max)||(size == 0))
	{
		free(buff);
		close(fd);
		return -3;
	}
	
	char *del = "\n";
	char *p;
	char *str[cdbg_module_num_max], tmp[cdbg_module_name_len_max];
	unsigned int i=0, j=0, x;
	
	p = strtok(buff, del);
	while(p != NULL)
	{
		str[i] = p;
		p = strtok(NULL, del);
		i++;
	}

	for(x=0; x<cdbg_module_num_max; x++)
	{
		if(strlen(cdbgConfig.modules[x].moduleName) == 0)
		{
			continue;
		}
		memset(tmp, 0, cdbg_module_name_len_max);
		strcpy(tmp, cdbg_config_flag_str_module);
		strcat(tmp, cdbgConfig.modules[x].moduleName);
		for(j=0; j<i; j++)
		{
			if(strstr(str[j], tmp))
			{
		//		printf("[CDBG]<Error>: com str [%d:%d][%s]\n", j, x, str[j]);
				break;
			}
		}
		if(j==i)
		{
			cdbg_cfgMod_del(x);
		}
	}

	for(j=0; j<i; j++)
	{
		if(strstr(str[j], cdbg_config_flag_str_module) != NULL)
		{
			modFree = -1;
			for(x=0; x<cdbg_module_num_max; x++)
			{
				if(cdbgConfig.modules[x].used == false)
				{
					continue;
				}
				memset(tmp, 0, cdbg_module_name_len_max);
				strcpy(tmp, cdbg_config_flag_str_module);
				strcat(tmp, cdbgConfig.modules[x].moduleName);
			//	printf("CMP str [%s][%s]\n", tmp, str[j]);
				if(strstr(str[j], tmp))
				{
					modFree = x;
					break;
				}
			}
			if(modFree == -1)
			{
				modFree = cdbg_cfgFreeMod_get();
			}
			cdbg_cfgLine_read(str[j], modFree);
			cdbg_cfgValueHandle_set(modFree);
			cdbgConfig.modules[modFree].used =  true;
		}
		else
		{
			cdbg_cfgLine_read(str[j], -1);
		}
	}

	free(buff);
	close(fd);

//----------for test
#if 0
//	int x;
	printf("\n\n\n\n=====================================\n");
	printf("==  inited			= %d.\n", cdbgConfig.inited);
	printf("==  reread			= %d.\n", cdbgConfig.reread);
	printf("==  cfghdl			= %d.\n", cdbgConfig.configFileHandle);
	for(x=0; x<cdbg_module_num_max; x++)
	{
		if(cdbgConfig.modules[x].used)
		{
			printf("***********************************\n");
			printf("==  used			= %d.\n", cdbgConfig.modules[x].used);
			printf("==  modName			= %s.\n", cdbgConfig.modules[x].moduleName);
			printf("==  level			= %d.\n", cdbgConfig.modules[x].dbgLevel);
			printf("==  date			= %d.\n", cdbgConfig.modules[x].dateTime);
			printf("==  flf				= %d.\n", cdbgConfig.modules[x].flf);
			printf("==  outtype			= %d.\n", cdbgConfig.modules[x].outType);
			printf("==  fileHandle		= %p.\n", cdbgConfig.modules[x].fileHandle);
		}
	}
	printf("=====================================\n");

#endif
	return 0;
}

void *cfgChkFun(void * arg)
{
	while(1)
	{
		pthread_mutex_lock(&cdbgConfig.mutex);
		if(cdbgConfig.reread == false)
		{
			pthread_mutex_unlock(&cdbgConfig.mutex);
			pthread_cancel(cdbgConfig.pid);
		}

		cdbg_cfgFile_read(cdbg_config_priv_file_name, cdbg_config_global_file_name);
		pthread_mutex_unlock(&cdbgConfig.mutex);
		sleep(1);
	}
}

int	CDBG_Init(void)
{
	int ret = -1;
	memset(&cdbgConfig, 0x00, sizeof(cdbg_config_t));

	// create mutex
	ret = pthread_mutex_init(&cdbgConfig.mutex, &cdbgConfig.mutex_attr);
	if(ret != 0)
	{
		cprintf("pthread_mutex_init error...\n");
		return 0;
	}

	pthread_mutex_lock(&cdbgConfig.mutex);
	cdbg_cfgFile_read(cdbg_config_priv_file_name, cdbg_config_global_file_name);

	if(cdbgConfig.reread ==  true)
	{
		// create thread
		ret = pthread_attr_init(&cdbgConfig.attr);
		if(ret != 0)
		{
			cprintf("pthread_attr_init failed...\n");
			return 0;
		}

		ret = pthread_create(&cdbgConfig.pid, &cdbgConfig.attr, cfgChkFun, NULL);
		if(ret != 0)
		{
			cprintf("pthread_create failed...\n");
			return 0;
		}

	//	ret = pthread_attr_setstacksize(&cdbgConfig.attr, (size_t)1024);
		if(ret != 0)
		{
			cprintf("pthread_attr_setstacksize failed...\n");
			return 0;
		}
	}

	cdbgConfig.inited =  true;
	pthread_mutex_unlock(&cdbgConfig.mutex);

//	while(1);
	return 0;
}

int	CDBG_Fini(void)
{
	pthread_mutex_lock(&cdbgConfig.mutex);
	
	if(cdbgConfig.reread ==  true)
	{
		cdbgConfig.reread = false;
		pthread_cancel(cdbgConfig.pid);
	}

	memset(&cdbgConfig, 0x00, sizeof(cdbg_config_t));
	pthread_mutex_unlock(&cdbgConfig.mutex);
	pthread_mutex_destroy(&cdbgConfig.mutex);
	return 0;
}

void cdbg_print_ex(
	int cfgModId,
	cdbg_level_e level,
	char *file,
	int	line,
	char *function,
	const char *args,
	...
	)
{
	pthread_mutex_lock(&cdbgConfig.mutex);
	cdbg_out_type_e ot = cdbgConfig.modules[cfgModId].outType;

	if(ot&cdbg_out_type_stdout)
	{
		va_list ap;
		char fmt[128];
		char fm[128];
		int lnum = 7;
		int n = 7;
		unsigned int	second = 0;
		time_t timep;
		struct tm *p;
	    struct timespec now;
		
		lnum = lnum - level;
		memset(fmt, 0, 128);
		sprintf(fmt, "\033[1;33m[CDBG]\033[0m<\033[0;3%dm", lnum);

		va_start( ap , args );

		cprintf(fmt);
		if(lnum == 7)
		{
			cprintf("\033[1;32m\033[44m");
		}
		if(lnum == 0)
		{
			cprintf("\033[1;32m\033[44m");
		}
		cprintf("%s", cdbg_mgr[level].levelStr);
		cprintf("\033[0m>");

		cprintf("(\033[1;32m\033[41m");
		cprintf("%s", cdbgConfig.modules[cfgModId].moduleName);
		cprintf("\033[0m)");

		if(cdbgConfig.modules[cfgModId].dateTime)
		{
			clock_gettime(CLOCK_MONOTONIC, &now);
			time(&timep);
			p=localtime(&timep);	/* 获取本地时间 */
		    second = now.tv_nsec / 1000000;		/* 默认的是毫秒 */

			cprintf("[%d-%d-%d ", (1900+p->tm_year), (1+p->tm_mon),p->tm_mday);
			cprintf("%d:%d:%d.", p->tm_hour, p->tm_min, p->tm_sec);
			cprintf("%d] ", second);
		}

		if(cdbgConfig.modules[cfgModId].flf)
		{
			memset(fm, 0, 128);
			n = strlen(function);
			n = (n%6) + 31;
			sprintf(fm, "\033[%d;%dm[", (line%2), n);
			cprintf(fm);
			cprintf("%s+%d %s]\033[0m ", file, line, function );
		}
		vprintf( args , ap );
		va_end( ap );
	}

	if(ot&cdbg_out_type_file)
	{
		if(cdbgConfig.modules[cfgModId].fileHandle != NULL)
		{
			va_list ap;
			char fmt[256];
			unsigned int	second = 0;
			time_t timep;
			struct tm *p;
		    struct timespec now;
			
			memset(fmt, 0, 256);
			sprintf(fmt, "[CDBG]<%s>(%s)", cdbg_mgr[level].levelStr, cdbgConfig.modules[cfgModId].moduleName);
			fputs(fmt, cdbgConfig.modules[cfgModId].fileHandle);

			if(cdbgConfig.modules[cfgModId].dateTime)
			{
				clock_gettime(CLOCK_MONOTONIC, &now);
				time(&timep);
				p=localtime(&timep);	/* 获取本地时间 */
			    second = now.tv_nsec / 1000000;		/* 默认的是毫秒 */

				memset(fmt, 0, 256);
				sprintf(fmt, "[%d-%d-%d %d:%d:%d.%d] ",
					(1900+p->tm_year), (1+p->tm_mon),p->tm_mday,
					p->tm_hour, p->tm_min, p->tm_sec, second);
				fputs(fmt, cdbgConfig.modules[cfgModId].fileHandle);
			}

			if(cdbgConfig.modules[cfgModId].flf)
			{
				memset(fmt, 0, 256);
				sprintf(fmt, "[%s+%d %s] ", file, line, function );
				fputs(fmt, cdbgConfig.modules[cfgModId].fileHandle);
			}
			
			va_start(ap, args);
			vfprintf(cdbgConfig.modules[cfgModId].fileHandle, args, ap);
			va_end(ap);
		}
	}

	if(ot&cdbg_out_type_syslog)
	{
	//	printf("Module <%s> will out to syslog....\n", cdbgConfig.modules[cfgModId].moduleName);
	}

	if(ot&cdbg_out_type_server)
	{
	//	printf("Module <%s> will out to server....\n", cdbgConfig.modules[cfgModId].moduleName);
	}

	pthread_mutex_unlock(&cdbgConfig.mutex);
}

////////////////////////////////////////

void cdbg_modEnv_get(const char *module, cdbg_flag_e *flag, int *cfgModId)
{
	pthread_mutex_lock(&cdbgConfig.mutex);
	int cdbg_mdole_i;
	for(cdbg_mdole_i=0; cdbg_mdole_i<cdbg_module_num_max; cdbg_mdole_i++)
	{
		if(cdbgConfig.modules[cdbg_mdole_i].used)
		{
			if(!strcmp(cdbgConfig.modules[cdbg_mdole_i].moduleName, module))
			{
				*flag = cdbg_open;
				*cfgModId = cdbg_mdole_i;
				break;
			}
		}
	}
	pthread_mutex_unlock(&cdbgConfig.mutex);
}


