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

#ifndef ZM_ZCMAKE_H
#define ZM_ZCMAKE_H

#include "common.h"

#define		CM_CONF_FILE		"CMakeLists.txt"

namespace zm{

	class zcmake
	{
	public:
		zcmake(std::string path, bool bRoot = false);
		zcmake();
		~zcmake();

		struct CM_VALUE
		{
			std::string flag;
			std::string value;
		};
		typedef	std::vector< CM_VALUE > CM_SETTINGS;
		typedef	std::vector< std::string > VALUES;

		void	save();
		void	createCMakeList();

	private:
		void	makeHeader();
		void	makeFooter();
		bool	parser();
		void	addValue(std::string flag, std::string value);
	private:
		PP_PROPERTY_PRIVATE_DEF(bool, isRoot);
		PP_PROPERTY_PRIVATE_DEF(std::string, path);

		PP_PROPERTY_PRIVATE_DEF(std::string, name);
		PP_PROPERTY_PRIVATE_DEF(std::string, type);
		PP_PROPERTY_PRIVATE_DEF(std::string, src_dir);
		PP_PROPERTY_PRIVATE_DEF(std::string, out_dir);
		PP_PROPERTY_PRIVATE_DEF(std::string, install_dir);

		PP_PROPERTY_PRIVATE_DEF(VALUES, inc_dirs);
		PP_PROPERTY_PRIVATE_DEF(VALUES, libs_dirs);
		PP_PROPERTY_PRIVATE_DEF(VALUES, proc_dirs);

		PP_PROPERTY_PRIVATE_DEF(VALUES, link_dirs);
		PP_PROPERTY_PRIVATE_DEF(VALUES, link_libs);

		PP_PROPERTY_PRIVATE_DEF(CM_SETTINGS, settings);
	};
}//namespace zm

#endif