#include <fstream>
#include <algorithm> 
#include "common.h"

Cir_Debug_Module_Define(common);
namespace zm{

	std::string str_replace(std::string strBase, std::string strSrc, std::string strDes)
	{
		std::string str = strBase;
		std::string::size_type pos = 0;
		std::string::size_type srcLen = strSrc.size();
		std::string::size_type desLen = strDes.size();
		pos = str.find(strSrc, pos);
		while ((pos != std::string::npos))
		{
			str.replace(pos, srcLen, strDes);
			pos = str.find(strSrc, (pos + desLen));
		}

		return str;
	}


	std::string str2lower(const std::string& str)
	{
		std::string t = str;
		std::transform(t.begin(), t.end(), t.begin(), tolower);
		return t;
	}

	std::string str2upper(const std::string& str)
	{
		std::string t = str;
		std::transform(t.begin(), t.end(), t.begin(), toupper);
		return t;
	}


	bool is_dir_exist(std::string path)
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
	bool is_file_exist(std::string name)
	{
		if ((access(name.c_str(), F_OK)) != -1)
		{
			return true;
		}
		return false;
	}

	void recurse_rfind_file(std::string src_path, std::string file, std::string &ret_path)
	{
		std::string spath = src_path;
		if (spath.empty())
		{
			CERROR("Error src path: %s\n", spath.c_str());
			return;
		}

		std::string fpath = spath + "/" + file;
		if (is_file_exist(fpath))
		{
			CINFO("Success find path: %s\n", spath.c_str());
			ret_path = spath;
			return;
		}

		std::string npath = spath.substr(0, spath.rfind('/', spath.length()));
		CINFO("Now find next path: %s\n", npath.c_str());
		recurse_rfind_file(npath, file, ret_path);
	}


	void recurse_rfind_path(std::string src_path, std::string file, std::string &ret_path)
	{
		std::string spath = src_path;
		if (spath.empty())
		{
			CERROR("Error src path: %s\n", spath.c_str());
			return;
		}

		std::string fpath = spath + "/" + file;
		if (is_file_exist(fpath))
		{
			CINFO("Success find path: %s\n", ret_path.c_str());
		//	ret_path = spath;
			return;
		}
		int l = spath.rfind('/', spath.length());
		std::string npath = spath.substr(0, l);
		std::string ndir = spath.substr(l + 1, spath.length() - l);

		std::string tpath = ndir;
		
		if (!ret_path.empty())
		{ 
			tpath += "/" + ret_path;
		}

		ret_path = tpath;

		CINFO("Now find next path: %s\n", npath.c_str());
		recurse_rfind_path(npath, file, ret_path);
	}

	const char *cur_dir()
	{
		static char curPwd[MAX_PATH];
		getcwd(curPwd, MAX_PATH);
		return curPwd;
	}

	bool create_dir(std::string path)
	{
		if ((path.empty()) || (is_dir_exist(path)))
		{
			CINFO("Dir: [%s] seems exist.\n", path.c_str());
			return false;
		}

		std::string cmd = "mkdir -p ";
		cmd += path;
		system(cmd.c_str());

		CINFO("Creat dir [%s] success.\n", path.c_str());
		return true;
	}

	bool file_read_by_line(std::string file, std::vector<std::string> &lines)
	{
		if (!is_file_exist(file))
		{
			CERROR("File not exist.\n");
			return false;
		}
		std::ifstream in(file.c_str());
		if (!in)
		{
			CERROR("Open file failed.\n");
			return false;
		}

		lines.clear();
		std::string line;
		while (getline(in, line))
		{
			lines.push_back(line);
		}
		in.close();

		CINFO("Read file over.\n");
		return true;
	}

	bool file_write_by_line(std::string file, std::vector<std::string> &lines)
	{
		FILE *fd = NULL;
		fd = fopen(file.c_str(), "w+");
		if (!fd)
		{
			CDEBUG("Create file error.\n");
			return false;
		}

		std::string line = "";
		std::vector<std::string>::iterator iter;
		for (iter = lines.begin(); iter != lines.end(); iter++)
		{
			line = *iter + "\n";
			fputs(line.c_str(), fd);
		}

		fclose(fd);
		CINFO("Write file over.\n");
		return true;
	}

	bool file_insert(std::string file, std::string flag, std::string str)
	{
		if (!is_file_exist(file))
		{
			CERROR("File not exist.\n");
			return false;
		}
		FILE * fp;
		char * line = NULL;
		size_t len = 0;
		size_t read;
		fp = fopen(file.c_str(), "r+");
		if (fp == NULL)
		{
			CERROR("File open failed.\n");
			exit(EXIT_FAILURE);
		}
		while ((read = getline(&line, &len, fp)) != -1)
		{
			printf("Retrieved line of length %zu :\n", read);
			printf("%s", line);
		}
		if (line)
		{
			free(line);
		}

		fclose(fp);
		CINFO("Get line over.\n");
		return true;
	}

	size_t	file_size(std::string file)
	{
		std::ifstream in(file.c_str());
		in.seekg(0, std::ios::end);
	//	std::streampos ps = in.tellg(); 
		size_t size = in.tellg();
		in.close();

		return size;
	}

	bool file_del(std::string path, std::string name)
	{
		std::string p = path + "/" + name;
		if (is_file_exist(p))
		{
			std::string cmd = "rm -rf " + p;
			system(cmd.c_str());
			return true;
		}
		return false;
	}


	bool dir_del(std::string dir)
	{
		if (is_file_exist(dir))
		{
			std::string cmd = "rm -rf " + dir;
			CINFO("Dir del: %s\n", cmd.c_str());
			system(cmd.c_str());
			return true;
		}
		return false;
	}
}
