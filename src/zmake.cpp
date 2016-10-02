#include "zmake.h"
#include "zmfile.h"
#include "zcmake.h"

Cir_Debug_Module_Define(zmake);
namespace zm{

	zmake::zmake(int argc, char **argv):
		rootDir_(""),
		oppDir_(""),
		popts_(NULL),
		pconf_(NULL)
	{
		popts_ = new zmOpts(argc, argv);
		pconf_ = new zmConf;
		recurse_rfind_file(cur_dir(), ZM_CONF_NAME, rootDir_);
		recurse_rfind_path(cur_dir(), ZM_CONF_NAME, oppDir_);
	}

	zmake::~zmake()
	{
		SAFE_RELEASE(pconf_);
		SAFE_RELEASE(popts_);
	}

	int	zmake::doConf()
	{
		return pconf_->doConf();
	}

	int	zmake::doOpts()
	{
		//popts_->show();
		std::string opt0, opt1, opt2, opt3;
		opt0 = opt1 = opt2 = opt3 = "";

		/* create a module */
		/* xx.process, xx.proc, xx.p, xx.P */
		/* yy.library, yy.lib, yy.l, yy.L */
		if ((popts_->get("a", opt0)) && (popts_->get("M", opt1)))
		{
			if (!opt1.empty())
			{
				CINFO("Add Module: %s.\n", opt1.c_str());
				pconf_->createModule(oppDir_, opt1);
			}
		}

		/* create a file */
		/* xx.class, xx.h, xx.cpp,  */
		if ((popts_->get("a", opt0)) && (popts_->get("F", opt1)))
		{
			if (!opt1.empty())
			{
				CINFO("Add file: %s.\n", opt1.c_str());
				pconf_->createFile(oppDir_, opt1);
			}
		}

		/* add inc dir */
		if ((popts_->get("a", opt0)) && (popts_->get("i", opt1)) && (popts_->get("D", opt2)))
		{
			if (!opt2.empty())
			{
				CINFO("Add include dir: %s.\n", opt2.c_str());
				pconf_->createFile(oppDir_, opt2 + ".inc_dir");
			}
		}

		/* add inc dir */
		if ((popts_->get("a", opt0)) && (popts_->get("l", opt1)) && (popts_->get("D", opt2)))
		{
			if (!opt2.empty())
			{
				CINFO("Add link dir: %s.\n", opt2.c_str());
				pconf_->createFile(oppDir_, opt2 + ".link_dir");
			}
		}

		/* add inc dir */
		if ((popts_->get("a", opt0)) && (popts_->get("l", opt1)) && (popts_->get("L", opt2)))
		{
			if (!opt2.empty())
			{
				CINFO("Add link lib: %s.\n", opt2.c_str());
				pconf_->createFile(oppDir_, opt2 + ".link_lib");
			}
		}
		
		/* delete zmake module */
		if ((popts_->get("d", opt0)) && (popts_->get("M", opt1)))
		{
			if (!opt1.empty())
			{
				CINFO("Del module: %s.\n", opt1.c_str());
				pconf_->deleteModule(oppDir_, opt1);
			}
		}

		/* delete zmake file */
		if ((popts_->get("d", opt0)) && (popts_->get("F", opt1)))
		{
			if (!opt1.empty())
			{
				CINFO("Del file: %s.\n", opt1.c_str());
				pconf_->deleteFile(oppDir_, opt1);
			}
		}

		/* delete zmake inc dir */
		if ((popts_->get("d", opt0)) && (popts_->get("i", opt1)) && (popts_->get("D", opt2)))
		{
			if (!opt2.empty())
			{
				CINFO("Del include dir: %s.\n", opt2.c_str());
				pconf_->deleteFile(oppDir_, opt2 + ".inc_dir");
			}
		}

		/* delete zmake link dir */
		if ((popts_->get("d", opt0)) && (popts_->get("l", opt1)) && (popts_->get("D", opt2)))
		{
			if (!opt2.empty())
			{
				CINFO("Del link dir: %s.\n", opt2.c_str());
				pconf_->deleteFile(oppDir_, opt2 + ".link_dir");
			}
		}

		/* delete zmake link library */
		if ((popts_->get("d", opt0)) && (popts_->get("l", opt1)) && (popts_->get("L", opt2)))
		{
			if (!opt2.empty())
			{
				CINFO("Del link library: %s.\n", opt2.c_str());
				pconf_->deleteFile(oppDir_, opt2 + ".link_lib");
			}
		}

		/* delete cmake files */
		if ((popts_->get("d", opt0)) && (popts_->get("a", opt1)))
		{
			CINFO("Delete cmake files.\n");
			system("rm CMake CMakeFiles CMakeCache.txt CMakeLists.txt src build Makefile cmake_install.cmake -rf");
			return -1;
		}

		pconf_->save();
		return 0;
	}
}

