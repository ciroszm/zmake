#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <ctime>

#include "zmfile.h"
#include "zmconf.h"

Cir_Debug_Module_Define(zmfile);
namespace zm{

	zmFile::zmFile()
	{
	}

	zmFile::~zmFile()
	{
	}

	bool zmFile::isDirExist(std::string path)
	{
		struct stat st;
		if (stat(path.c_str(), &st) == 0)
		{
			if (st.st_mode & S_IFDIR != 0)
			{
				return true;
			}
		}
		return false;
	}
	bool zmFile::isFileExist(std::string name)
	{
		if ((access(name.c_str(), F_OK)) != -1)
		{
			return true;
		}
		return false;
	}


	bool zmFile::createDir(std::string path)
	{
		if ((path.empty()) || (isDirExist(path)))
		{
			return false;
		}

		std::string cmd = "mkdir -p ";
		cmd += path;
		system(cmd.c_str());

		return true;
	}


	bool zmFile::createFile(FPARAM &param)
	{
		if (param.type == "h")
		{
			return createFileH(param);
		}
		else if (param.type == "cpp")
		{
			return createFileCpp(param);
		}
		else if (param.type == "class")
		{
			return createFileClass(param);
		}

		return false;
	}

	FILE *zmFile::writeFrame(FPARAM &param)
	{
		if (!isFileExist(param.path))
		{
			createDir(param.path);
		}

		std::string suffix = "." + param.type;
		std::string fullName = param.path + "/" + param.name + suffix;
		if (isFileExist(fullName))
		{
			CDEBUG("File already exist.\n");
			return NULL;
		}

		FILE *fd = NULL;
		fd = fopen(fullName.c_str(), "w+");
		if (!fd)
		{
			CDEBUG("Create file error.\n");
			return NULL;
		}

		std::string pline = "";
		fputs("/***********************************************************************\n", fd);
		fputs("* Notice:\n", fd);
		std::string projNotice = zmConf::getSingletonPtr()->projNotice();
		projNotice = "* " + str_replace(projNotice, "\\n", "\n* ");
		fputs(projNotice.c_str(), fd);
		fputs("\n", fd);

		fputs("* ---------------------------------------------------------------------\n", fd);
		fputs("* Description:\n*\n", fd);
		pline = "* " + param.name + suffix + "\n*\n";
		fputs(pline.c_str(), fd);
		pline = "* Author: " + zmConf::getSingletonPtr()->projAuthor() + "\n";
		fputs(pline.c_str(), fd);
		std::time_t now = std::time(0);
		std::string time = std::asctime(std::localtime(&now));
		pline = "* Create: " + time + "*\n";
		fputs(pline.c_str(), fd);

		fputs("* ---------------------------------------------------------------------\n", fd);
		fputs("* History:\n", fd);
		fputs("*\n", fd);

		pline = "* " + time + "* written by " + zmConf::getSingletonPtr()->projAuthor() + ".\n*\n";
		fputs(pline.c_str(), fd);

		fputs("***********************************************************************/\n\n\n", fd);

		return fd;
	}

	bool zmFile::createFileH(FPARAM &param)
	{
		FILE *fd = writeFrame(param);
		if (!fd)
		{
			CERROR("Create file frame error.\n");
			return false;
		}

		std::string pline = "";
		std::string hdef = "__" + str2upper(param.module) + "_" + str2upper(param.name) + "_H__";
		pline = "#ifndef\t" + hdef + "\n";
		fputs(pline.c_str(), fd);
		pline = "#define\t" + hdef + "\n\n\n";
		fputs(pline.c_str(), fd);

		/* include header from here. */
		fputs("/* includes */\n\n", fd);

		/* Start namespace */
		pline = "namespace " + zmConf::getSingletonPtr()->projSpace() + "{\n";
		fputs(pline.c_str(), fd);

		fputs("\n", fd);
		fputs("\n", fd);
		fputs("\n", fd);
		fputs("\n", fd);

		/* End namespace */
		pline = "} /* namespace " + zmConf::getSingletonPtr()->projSpace() + "*/\n";
		fputs(pline.c_str(), fd);

		/* End header file macro define */
		hdef = "__" + str2upper(param.module) + "_" + str2upper(param.name) + "_H__";
		pline = "#endif\t/*" + hdef + "*/\n";
		fputs(pline.c_str(), fd);
		fputs("\n", fd);
		fputs("\n", fd);
		fclose(fd);

		CINFO("Create h file success.\n");
		return true;
	}

	bool zmFile::createFileCpp(FPARAM &param)
	{
		FILE *fd = writeFrame(param);
		if (!fd)
		{
			CERROR("Create file frame error.\n");
			return false;
		}

		std::string pline = "";
		/* include header from here. */
		fputs("/* includes */\n\n", fd);

		/* Start namespace */
		pline = "namespace " + zmConf::getSingletonPtr()->projSpace() + "{\n";
		fputs(pline.c_str(), fd);

		fputs("\n", fd);
		fputs("\n", fd);
		fputs("\n", fd);
		fputs("\n", fd);

		/* End namespace */
		pline = "} /* namespace " + zmConf::getSingletonPtr()->projSpace() + "*/\n\n";
		fputs(pline.c_str(), fd);

		fclose(fd);
		return true;
	}

	bool zmFile::createFileClass(FPARAM &param)
	{
		FPARAM hp = param;
		FPARAM cp = param;

		hp.type = "h";
		cp.type = "cpp";

		/* create class header file. */
		FILE *fd = writeFrame(hp);
		if (!fd)
		{
			CERROR("Create file frame error.\n");
			return false;
		}

		/* Start to make header macro define. */
		std::string pline = "";
		std::string hdef = "__" + str2upper(param.module) + "_" + str2upper(param.name) + "_H__";
		pline = "#ifndef\t" + hdef + "\n";
		fputs(pline.c_str(), fd);
		pline = "#define\t" + hdef + "\n\n\n";
		fputs(pline.c_str(), fd);

		/* include header from here. */
		fputs("/* includes */\n\n", fd);

		/* Start namespace */
		pline = "namespace " + zmConf::getSingletonPtr()->projSpace() + "{\n\n";
		fputs(pline.c_str(), fd);

		/* Make class */
		pline = "\tclass " + hp.name + "\n\t{\n";
		fputs(pline.c_str(), fd);
		fputs("\tpublic:\n", fd);
		pline = "\t\t" + hp.name + "();\n\t\t~" + hp.name + "();\n\n";
		fputs(pline.c_str(), fd);
		fputs("\tprivate:\n\n", fd);
		fputs("\t};\n", fd);
		fputs("\n", fd);

		/* End namespace */
		pline = "} /* namespace " + zmConf::getSingletonPtr()->projSpace() + "*/\n";
		fputs(pline.c_str(), fd);

		/* End header file macro define */
		hdef = "__" + str2upper(param.module) + "_" + str2upper(param.name) + "_H__";
		pline = "#endif\t/*" + hdef + "*/\n";
		fputs(pline.c_str(), fd);
		fputs("\n", fd);
		fputs("\n", fd);
		fclose(fd);

		CINFO("Create class header file success.\n");
		

		/* create class cpp file. */
		fd = writeFrame(cp);
		if (!fd)
		{
			CERROR("Create cpp file error.\n");
			return false;
		}

		/* include header from here. */
		fputs("/* includes */\n", fd);
		/* include class header file. */
		pline = "#include \"" + hp.name + ".h\"\n\n";
		fputs(pline.c_str(), fd);

		/* Start namespace */
		pline = "namespace " + zmConf::getSingletonPtr()->projSpace() + "{\n\n";
		fputs(pline.c_str(), fd);

		/* Make class functions.*/
		pline = "\t" + cp.name + "::" + cp.name +"()\n\t{\n\n\t}\n\n";
		fputs(pline.c_str(), fd);
		pline = "\t" + cp.name + "::~" + cp.name + "()\n\t{\n\n\t}\n\n";
		fputs(pline.c_str(), fd);

		/* End namespace */
		pline = "} /* namespace " + zmConf::getSingletonPtr()->projSpace() + "*/\n\n";
		fputs(pline.c_str(), fd);

		fclose(fd);
		return false;
	}


}
