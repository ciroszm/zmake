#ifndef ZM_FILE_H
#define ZM_FILE_H


#include <string>
#include "common.h"

namespace zm{
	class zmFile
	{
	public:
		zmFile();
		~zmFile();

		struct FPARAM
		{
			std::string module;
			std::string path;
			std::string name;
			std::string	type;
			std::string notice;
		};

		struct CMPARAM
		{
			std::string module;
			std::string	type;
			std::string path;
			std::string name;
			std::string notice;
		};

		bool createDir(std::string path);
		bool createFile(FPARAM &param);
	private:
		bool createFileH(FPARAM &param);
		bool createFileCpp(FPARAM &param);
		bool createFileClass(FPARAM &param);
		bool isDirExist(std::string path);
		bool isFileExist(std::string name);
		FILE *writeFrame(FPARAM &param);
	};

}//namespace zm
#endif