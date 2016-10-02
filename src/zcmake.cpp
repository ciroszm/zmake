#include "zcmake.h"
#include "zmfile.h"

Cir_Debug_Module_Define(zcmake);
namespace zm{

	zcmake::zcmake(std::string cmk, bool bRoot) :
		path_(cmk),
		isRoot_(bRoot),
		name_(""),
		type_(""),
		src_dir_("."),
		out_dir_("build"),
		install_dir_("/")
	{

	}
	zcmake::zcmake() :
		path_("."),
		isRoot_(false),
		name_(""),
		type_(""),
		src_dir_("."),
		out_dir_("build"),
		install_dir_("/")
	{

	}

	zcmake::~zcmake()
	{
	}

	bool	zcmake::parser()
	{
		if (!is_file_exist(path_))
		{
			CERROR("File[%s] not exist.\n", path_.c_str());
			return false;
		}

		FILE* fp = fopen(path_.c_str(), "rb");
		if (!fp)
		{
			CERROR("File open failed.\n");
			return false;
		}

		size_t size = file_size(path_);
		char *fbuff = (char *)malloc(size + 1);
		int len = fread(fbuff, size+1, 1, fp);
		CDEBUG("Read: ========================< %d/%d >====================\n\n", len, size);


		std::string str = fbuff;
		while (true)
		{
			if ((str.empty())||(str.find(')', 0) == std::string::npos))
			{
				CERROR("Not fine ).\n");
				break;
			}
			int pos = str.find(')', 0);
			std::string sstr = str.substr(0, pos);
			str = str.substr(pos+1, str.length() - pos-1);

			int fpos = sstr.find('(', 0);
			std::string flag = sstr.substr(0, fpos);
			std::string value = sstr.substr(fpos + 1, sstr.length() - fpos - 1);

			/* deal flag */
			flag = str_replace(flag, " ", "");
			flag = flag.erase(0, flag.rfind('\n', flag.length()) + 1);
			flag = str_replace(flag, "\t", "");

			/* deal value */
			value = str_replace(value, "\n", " ");
			value = str_replace(value, "\t", "");
			
			CM_VALUE v;
			v.flag = flag;
			v.value = value;

			settings_.push_back(v);
			CDEBUG("flag:[%s], value:[%s]\n", flag.c_str(), value.c_str());
		}

		fclose(fp);
		return true;
	}

	void	zcmake::makeHeader()
	{
		/* project not null, is root cmakelist.txt */
		if (isRoot_)
		{
			addValue("PROJECT", name_);
			addValue("CMAKE_MINIMUM_REQUIRED", "VERSION 2.9.8");
			addValue("SET", "CMAKE_INSTALL_PREFIX " + install_dir_);
			addValue("SET", "PROJECT_OUT_DIR ${PROJECT_SOURCE_DIR}/" + out_dir_);
			addValue("SET", "LIBRARY_OUTPUT_PATH ${PROJECT_OUT_DIR}");
			addValue("SET", "EXECUTABLE_OUTPUT_PATH ${PROJECT_OUT_DIR}");
			addValue("LINK_DIRECTORIES", "${PROJECT_OUT_DIR}");

			return;
		}

		/* add include dirs */
		VALUES::iterator iter;
		for (iter = inc_dirs_.begin(); iter != inc_dirs_.end(); ++iter)
		{
			std::string idir = *iter;
			if (idir.find('/', 0) != 0)
			{
				idir = "${PROJECT_SOURCE_DIR}/" + idir;
			}
			addValue("INCLUDE_DIRECTORIES", idir);
		}

		/* add link lib dirs */
		for (iter = link_dirs_.begin(); iter != link_dirs_.end(); ++iter)
		{
			std::string idir = *iter;
			if (idir.find('/', 0) != 0)
			{
				idir = "${PROJECT_SOURCE_DIR}/" + idir;
			}
			addValue("LINK_DIRECTORIES", idir);
		}

		/* add libs sub dirs */
		for (iter = libs_dirs_.begin(); iter != libs_dirs_.end(); ++iter)
		{
			addValue("ADD_SUBDIRECTORY", *iter);
		}

		/* add proc sub dirs */
		for (iter = proc_dirs_.begin(); iter != proc_dirs_.end(); ++iter)
		{
			addValue("ADD_SUBDIRECTORY", *iter);
		}

	}

	void	zcmake::makeFooter()
	{

		/* module is a library, to make .so and .a */
		if ("library" == str2lower(type_))
		{
			addValue("AUX_SOURCE_DIRECTORY", src_dir_ + " SRC_DIR");
			addValue("ADD_LIBRARY", name_ + "_shared SHARED ${SRC_DIR}");
			addValue("ADD_LIBRARY", name_ + "_static STATIC ${SRC_DIR}");
			addValue("SET_TARGET_PROPERTIES", name_ + "_shared PROPERTIES OUTPUT_NAME " + name_);
			addValue("SET_TARGET_PROPERTIES", name_ + "_static PROPERTIES OUTPUT_NAME " + name_);
		}
		/* module is a process, to make proc */
		else if ("process" == str2lower(type_))
		{
			addValue("AUX_SOURCE_DIRECTORY", src_dir_ + " SRC_DIR");
			addValue("ADD_EXECUTABLE", name_ + " ${SRC_DIR}");
			addValue("TARGET_LINK_LIBRARIES", name_ + " pthread");
		}

		/* if project have no sub dir, make project is a module. */
		if ((libs_dirs_.size() == 0) && (proc_dirs_.size() == 0) && (isRoot_ == true))
		{
			addValue("AUX_SOURCE_DIRECTORY", ". SRC_DIR");
			addValue("SET", "EXECUTABLE_OUTPUT_PATH ${PROJECT_OUT_DIR}");
			addValue("ADD_EXECUTABLE", name() + " ${SRC_DIR}");
			addValue("TARGET_LINK_LIBRARIES", name() + " pthread");
			addValue("TARGET_LINK_LIBRARIES", name() + " rt");
		}
		
		/* add link libs */
		VALUES::iterator iter;
		for (iter = link_libs_.begin(); iter != link_libs_.end(); ++iter)
		{
			addValue("TARGET_LINK_LIBRARIES", name_ + " " + *iter);
		}
	}
	void	zcmake::createCMakeList()
	{
		/* project not null, is root cmakelist.txt */
		if (isRoot_)
		{
			addValue("PROJECT", name_);
			addValue("CMAKE_MINIMUM_REQUIRED", "VERSION 2.9.8");
			addValue("SET", "CMAKE_INSTALL_PREFIX " + install_dir_);
			addValue("SET", "PROJECT_OUT_DIR ${PROJECT_SOURCE_DIR}/" + out_dir_);
			addValue("SET", "LIBRARY_OUTPUT_PATH ${PROJECT_OUT_DIR}");
			addValue("SET", "EXECUTABLE_OUTPUT_PATH ${PROJECT_OUT_DIR}");
			addValue("LINK_DIRECTORIES", "${PROJECT_OUT_DIR}");

			return;
		}

		/* module is a library, to make .so and .a */
		if ("library" == str2lower(type_))
		{
			addValue("AUX_SOURCE_DIRECTORY", src_dir_ + " SRC_DIR");
			addValue("ADD_LIBRARY", name_ + "_shared SHARED ${SRC_DIR}");
			addValue("ADD_LIBRARY", name_ + "_static STATIC ${SRC_DIR}");
			addValue("SET_TARGET_PROPERTIES", name_ + "_shared PROPERTIES OUTPUT_NAME " + name_);
			addValue("SET_TARGET_PROPERTIES", name_ + "_static PROPERTIES OUTPUT_NAME " + name_);
		}
		/* module is a process, to make proc */
		else if ("process" == str2lower(type_))
		{
			addValue("AUX_SOURCE_DIRECTORY", src_dir_ + " SRC_DIR");
			addValue("ADD_EXECUTABLE", name_ + " ${SRC_DIR}");
			addValue("TARGET_LINK_LIBRARIES", name_ + " pthread");
		}

	}

	void	zcmake::addValue(std::string flag, std::string value)
	{
		CM_SETTINGS::iterator iter;
		for (iter = settings_.begin(); iter != settings_.end(); ++iter)
		{
			if ((iter->flag == flag) && (iter->value == value))
			{
				return;
			}
		}
		CM_VALUE v;
		v.flag = flag;
		v.value = value;
		settings_.push_back(v);
	}

	void	zcmake::save()
	{
		FILE *fd = NULL;
		fd = fopen(path_.c_str(), "w+");
		if (!fd)
		{
			CDEBUG("Create file error.\n");
			return;
		}

		makeHeader();
		makeFooter();

		/* Start to make header macro define. */
		std::string line = "";
		CM_SETTINGS::iterator siter;
		for (siter = settings_.begin(); siter != settings_.end(); ++siter)
		{
			line = str2upper(siter->flag) + "(" + siter->value + ")\n";
			fputs(line.c_str(), fd);
		}

		fputs("\n", fd);

		fclose(fd);
	}



}

