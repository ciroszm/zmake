#include "zmopt.h"
#include <getopt.h>

Cir_Debug_Module_Define(zmopts);
namespace zm{
	
	ZAE_SINGLETON_INIT(zmOpts);
	static	const char *short_options = "hvadilM:F:L:D:";
	static struct option long_options[] =
	{
		{ "version", 0, NULL, 'v' },
		{ "helper", 0, NULL, 'h' },
		{ "add", 0, NULL, 'a' },
		{ "delete", 0, NULL, 'd' },
		{ "include", 0, NULL, 'i' },
		{ "link", 0, NULL, 'l' },
		{ "module", 1, NULL, 'M' },
		{ "file", 1, NULL, 'F' },
		{ "library", 1, NULL, 'L' },
		{ "directory", 1, NULL, 'D' }
	};

	zmOpts::zmOpts(int argc, char **argv)
	{
		parser(argc, argv);
	}

	zmOpts::~zmOpts()
	{
	}

	void	zmOpts::helper()
	{
		printf("zmake: zm project make tools.\n");
		printf("Usage: zmake operation [ arg ] .\n\n");
		printf(" -h                                       helper.\n");
		printf(" -v                                       version.\n\n");
		printf(" -aM    module_name.[proc,p,P,lib,l,L]    add module to current dir.\n");
		printf(" -aF    file_name.[class,h,cpp]           add file to current module.\n");
		printf(" -aiD   path_name                         add include dir to current module.\n");
		printf(" -alD   path_name                         add link dir to current module.\n");
		printf(" -alL   lib_name                          add link library to current module.\n\n");
		printf(" -dM    module_name.[proc,p,P,lib,l,L]    del module from current dir.\n");
		printf(" -dF    file_name.[class,h,cpp]           del file from current module.\n");
		printf(" -diD   path_name                         del include dir from current module.\n");
		printf(" -dlD   path_name                         del link dir from current module.\n");
		printf(" -dlL   lib_name                          del link library from current module.\n");
		printf(" -da                                      del all except zmake config file.\n");
		printf("\nVersion: ");
		printf(ZM_VERSION);
		printf("\n");
	}
	void	zmOpts::version()
	{
		printf("Version: ");
		printf(ZM_VERSION);
		printf("\n");
	}

	bool zmOpts::parser(int argc, char **argv)
	{
		CINFO("Opts parse start...\n");
		int opt = 0;
		while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) != -1)
		{
			switch (opt)
			{
			case 'h':
				opts_["h"] = "";
				CINFO("Opt: h\n");
				helper();
				break;
			case 'v':
				opts_["v"] = "";
				CINFO("Opt: v\n");
				version();
				break;
			case 'a':
				opts_["a"] = "";
				CINFO("Opt: a\n");
				break;
			case 'd':
				opts_["d"] = "";
				CINFO("Opt: d\n");
				break;
			case 'i':
				opts_["i"] = "";
				CINFO("Opt: i\n");
				break;
			case 'l':
				opts_["l"] = "";
				CINFO("Opt: l\n");
				break;
			case 'M':
				if (optarg)
				{
					opts_["M"] = optarg;
					CINFO("Opt: M\n");
				}
				break;
			case 'F':
				if (optarg)
				{
					opts_["F"] = optarg;
					CINFO("Opt: F\n");
				}
				break;
			case 'L':
				if (optarg)
				{
					opts_["L"] = optarg;
					CINFO("Opt: L\n");
				}
				break;
			case 'D':
				if (optarg)
				{
					opts_["D"] = optarg;
					CINFO("Opt: D\n");
				}
				break;
			default:
				helper();
				return false;
			}
		}
		return true;
	}

	void	zmOpts::show()
	{
		OPTS_MAP::iterator iter = opts_.begin();
		for (; iter != opts_.end(); iter++)
		{
			CINFO("Opt: %s = %s\n", iter->first.c_str(), iter->second.c_str());
		}
	}

	bool	zmOpts::get(std::string f, std::string &v)
	{
		OPTS_MAP::iterator iter = opts_.find(f);
		if (iter != opts_.end())
		{
			v = iter->second;
			return true;
		}
		v = "";
		return false;
	}

}
