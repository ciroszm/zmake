#ifndef ZM_CONF_H
#define ZM_CONF_H

#include <vector>
#include <map>
#include <string>
#include "common.h"
#include "singleton.h"
#include "zxml.h"
#include "zcmake.h"
#include "zmfile.h"

#define		ZM_CONF_PATH		"./"
#define		ZM_CONF_NAME		"zmake.xml"


namespace zm{
	class zmConf : public Singleton<zmConf>
	{
	public:
		zmConf();
		~zmConf();

		class zmConfModFile
		{
		public:
			std::string type_;
			std::string name_;
		};

		class zmConfMod
		{
		public:
			std::string name_;
			std::string type_;
			std::vector<zmConfModFile>	files_;
			zcmake		zcm_;
		};
		typedef	std::map<std::string, std::vector<zmConfMod> >	CONF_DIRS;

		CONF_DIRS	&projDirs(){ return projDirs_; };
		void	show();
		void	save();
		int		doConf();

		bool	createModule(std::string path, std::string name);
		bool	createFile(std::string path, std::string name);
		bool	deleteModule(std::string path, std::string name);
		bool	deleteFile(std::string path, std::string name);
	private:
		bool	parser();
		void	getConf(std::string &conf);
		void	setConf(std::string conf);
		PP_PROPERTY_PRIVATE_DEF(std::string, path);

		PP_PROPERTY_PRIVATE_DEF(std::string, company);

		PP_PROPERTY_PRIVATE_DEF(std::string, projName);
		PP_PROPERTY_PRIVATE_DEF(std::string, projPrefix);
		PP_PROPERTY_PRIVATE_DEF(std::string, projNotice);
		PP_PROPERTY_PRIVATE_DEF(std::string, projAuthor);
		PP_PROPERTY_PRIVATE_DEF(std::string, projSpace);

		PP_PROPERTY_PRIVATE_DEF(zcmake, zcm);
		CONF_DIRS	projDirs_;
		
		PP_PROPERTY_PRIVATE_DEF(bool, isOk);
		PP_PROPERTY_PRIVATE_DEF_P(CXml, xml);
	};

}//namespace zm
#endif


