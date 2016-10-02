/********************************************************************************
 * cir linux os api header
 *
 *
 * Modify History
 * ------------------------------------------------------------------------------
 * 2010-06-14 22:30
 * author: zhaoming
 *
 *
 ********************************************************************************/

#ifndef		_INC_CIR_LINUX_DEBUG_H_
#define		_INC_CIR_LINUX_DEBUG_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* includes */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
//#include "cir_linux_os.h"

/****************************************
 *            Declare
 * --------------------------------------
 *
 * Here we define CLO as cir_linux_os
 *
 ****************************************/

/* constants define */

#define		cdbg_module_name_size	64

#define CDBG_DEBUG_ENABLE

/* enum define */

/* struct define */
#define	cdbg_env_str_def	"cdbg"
#define	cdbg_file_def		"/tmp/cdbg"
#define	cdbg_make_level_str(level) {level, #level}

#define  cdbg_config_priv_file_name 	"./zmdbg.conf"
#define  cdbg_config_global_file_name 	"tmp/zmdbg.conf"
#define  cdbg_config_file_len_max 		(2048)
#define  cdbg_module_num_max 			(128)
#define  cdbg_module_name_len_max 		(128)

#define  cdbg_config_flag_str_header		"cdbg"
#define  cdbg_config_flag_str_module		"module="
#define  cdbg_config_flag_str_level			"level="
#define  cdbg_config_flag_str_reread		"reread="
#define  cdbg_config_flag_str_time			"time="
#define  cdbg_config_flag_str_flf			"flf="
#define  cdbg_config_flag_str_outStd		"stdout="
#define  cdbg_config_flag_str_outfile		"file="
#define  cdbg_config_flag_str_outfileName	"logFile="
#define  cdbg_config_flag_str_outSyslog		"syslog="
#define  cdbg_config_flag_str_outServer		"server="

typedef	enum
{
	cdbg_out_type_stdout=1,
	cdbg_out_type_file=2,
	cdbg_out_type_syslog=4,
	cdbg_out_type_server=8,
	cdbg_out_type_max
}cdbg_out_type_e;

typedef		enum
{
	Enter = 0,
	Debug,
	Minor,
	Major,
	Infor,
	Error,
	Fatal,
	Leave
}cdbg_level_e;

typedef		enum
{
	cdbg_close = 0,
	cdbg_open
}cdbg_flag_e;

////////////////////////////////////////////////////

typedef	struct
{
	bool			used;
	char			moduleName[cdbg_module_name_len_max];
	cdbg_level_e 	dbgLevel;
	cdbg_out_type_e	outType;
	bool			flf;
	bool			dateTime;
	FILE			*fileHandle;
	char			*filePath;
	int				syslogHandle;
}cdbg_module_t;

typedef	struct
{
	bool				inited;
	pthread_t			pid;
	pthread_attr_t		attr;
	pthread_mutex_t 	mutex;
	pthread_mutexattr_t mutex_attr;
	bool				reread;
	int					configFileHandle;
	cdbg_module_t		modules[cdbg_module_num_max];
}cdbg_config_t;

///////////////////////////////////
typedef	struct
{
	cdbg_level_e level;
	char		*levelStr;
}cdbg_level_t;

#define	Cir_Debug_Module_Define(module) \
	static  const char	*cdbg_module_str = #module; \
	static	cdbg_flag_e cdbg_modle_flag = cdbg_close; \
	static	cdbg_level_e cdbg_modle_level = Enter;	\
	static	int	cdbg_cfg_modle_id = 0;	\
	extern	cdbg_config_t	cdbgConfig;

void	cdbg_print_ex(
	int cfgModId,
	cdbg_level_e level,
	char *file,
	int	line,
	char *function,
	const char *args,
	...
	);

void cdbg_modEnv_get(const char *module, cdbg_flag_e *flag, int *cfgModId);

int	cprintf( char *args, ... );


#define CASSERT(expression)	\
	do{				\
		if( !(expression) )		\
		{	\
			printf("\033[1;33m[Cdbg]\033[0m <\033[0;31mAbort\033[0m> \033[1;34m[ %s +%d %s ]\033[0m ...\n",__FILE__,__LINE__, __FUNCTION__);	\
			abort();	\
		}				\
	}while(0)				


#define CPCheck(expression, RET)	\
	do{				\
		if( !(expression) )		\
		{	\
			printf("\033[1;33m[CDBG]\033[0m <\033[0;31mParams error:\033[1;33m[ %s ]\033[0m> \033[1;34m[%s+%d %s]\033[0m ...\n", #expression, __FILE__, __LINE__, __FUNCTION__);	\
			return RET;	\
		}				\
	}while(0)	

#define cdbg_env_check \
	cdbg_modEnv_get(cdbg_module_str, &cdbg_modle_flag, &cdbg_cfg_modle_id);	\
	cdbg_modle_level = cdbgConfig.modules[cdbg_cfg_modle_id].dbgLevel;

#ifdef	CDBG_DEBUG_ENABLE

#define cdbg_level_printf(level, ...) \
	do	\
	{	\
		cdbg_env_check;		\
		if((cdbg_modle_flag)&&(cdbg_modle_level <= level)&&(cdbg_modle_level>=Enter)&&(cdbg_modle_level<=Leave))	\
		cdbg_print_ex(cdbg_cfg_modle_id, level, __FILE__, __LINE__, (char *)__FUNCTION__, __VA_ARGS__); \
	}while(0)

#define cdbg_InOut_printf(level, ...) \
	do	\
	{	\
		cdbg_env_check;		\
		if((cdbg_modle_flag)&&((level==Enter)||(level==Leave)))	\
		cdbg_print_ex(cdbg_cfg_modle_id, level, __FILE__, __LINE__, (char *)__FUNCTION__, __VA_ARGS__); \
	}while(0)

#else
#define cdbg_level_printf(level,args...)
#define cdbg_InOut_printf(level,args...)
#endif

int		CDBG_Init(void);
int		CDBG_Fini(void);

#define CENTER()	cdbg_InOut_printf(Enter, ">>>\n")
#define CLEAVE() 	cdbg_InOut_printf(Leave, "<<<\n")

#define CINFO(...) cdbg_level_printf(Infor, __VA_ARGS__)
#define CDEBUG(...) cdbg_level_printf(Debug, __VA_ARGS__)
#define CMINOR(...) cdbg_level_printf(Minor, __VA_ARGS__)
#define CMAJOR(...) cdbg_level_printf(Major, __VA_ARGS__)
#define CERROR(...) cdbg_level_printf(Error, __VA_ARGS__)
#define CFATAL(...) cdbg_level_printf(Fatal, __VA_ARGS__)


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* #ifndef		_INC_CIR_LINUX_DEBUG_H_ */

