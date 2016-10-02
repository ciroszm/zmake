/***********************************************************************
* Notice:
* Copyright (c) 2003-2009, BeiJing Baustem Technology Co., LTD
* All Rights Reserved
* Confidential Property of Baustem Corporation
*
* ---------------------------------------------------------------------
* Description:
* bstm_ldpf_debug.h - baustem debug subroutine header file
* 
* Author: zhaoming
* Create: 2016-01-02
*
* ---------------------------------------------------------------------
* History:
* 2 2010.05.07 linguofu
* remove macro BSTM_DEBUG_INFO
*
* 1 2009.12.30 14:00, WangPuyang
* written.
*
***********************************************************************/

#ifndef ZM_ZMAKE_H
#define ZM_ZMAKE_H


#include "common.h"
#include "zmopt.h"
#include "zmconf.h"


namespace zm{
	class zmake
	{
	public:
		zmake(int argc, char **argv);
		~zmake();

		int	doConf();
		int	doOpts();
	private:
		PP_PROPERTY_PRIVATE_DEF(std::string, rootDir);
		PP_PROPERTY_PRIVATE_DEF(std::string, oppDir);
		PP_PROPERTY_PRIVATE_DEF_P(zmOpts, opts);
		PP_PROPERTY_PRIVATE_DEF_P(zmConf, conf);

	};
}//namespace zm

#endif