#ifndef ZM_OPTIONS_H
#define ZM_OPTIONS_H

#include "common.h"
#include "singleton.h"

namespace zm{

	class zmOpts : public Singleton<zmOpts>
	{
	public:
		zmOpts(int argc, char **argv);
		~zmOpts();
		typedef		std::map<std::string, std::string>	OPTS_MAP;

		void	show();
		PP_PROPERTY_PRIVATE_DEF(OPTS_MAP, opts);
		bool	get(std::string f, std::string &v);
	private:
		bool	parser(int argc, char **argv);
		void	helper();
		void	version();
	};

}//namespace zm
#endif
