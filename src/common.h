
#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string>
#include <vector>
#include <map>

#include "zmdbg.h"



#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#define			ZM_VERSION				"1.0.1"


#define         PP_PROPERTY_DEF(tp,t,np,n)                                      \
tp:     t       n##_;                                                           \
np:     t &n(){ return n##_; };                                                 \
np:     void n(t &x){ n##_ = x; };


#define         PP_PROPERTY_COMMON_DEF(t,n)     PP_PROPERTY_DEF(public,t,public,n)
#define         PP_PROPERTY_PRIVATE_DEF(t,n)     PP_PROPERTY_DEF(private,t,public,n)
#define         PP_PROPERTY_PROTECTED_DEF(t,n)     PP_PROPERTY_DEF(protected,t,public,n)

#define         PP_PROPERTY_DEF_P(tp,t,np,n)                                        \
tp:     t       *p##n##_;                                                             \
np:     t *p##n(){ return p##n##_; };                                                      \
np:     void p##n(t *p){ p##n##_ = p; };

#define     PP_PROPERTY_COMMON_DEF_P(t,n)     PP_PROPERTY_DEF_P(public,t,public,n)
#define     PP_PROPERTY_PRIVATE_DEF_P(t,n)     PP_PROPERTY_DEF_P(private,t,public,n)
#define     PP_PROPERTY_PROTECTED_DEF_P(t,n)     PP_PROPERTY_DEF_P(protected,t,public,n)


	/** 安全的释放一个指针内存 */
#define SAFE_RELEASE(i)									\
	if (i)											\
	{												\
	delete i;									\
	i = NULL;									\
}

	/** 安全的释放一个指针数组内存 */
#define SAFE_RELEASE_ARRAY(i)							\
	if (i)											\
	{												\
	delete[] i;									\
	i = NULL;									\
}

namespace zm{
	std::string str_replace(std::string strBase, std::string strSrc, std::string strDes);
	std::string str2lower(const std::string& str);
	std::string str2upper(const std::string& str);

	bool is_dir_exist(std::string path);
	bool is_file_exist(std::string name);

	void recurse_rfind_file(std::string src_path, std::string file, std::string &ret_path);
	void recurse_rfind_path(std::string src_path, std::string file, std::string &ret_path);

	const char *cur_dir();
	bool create_dir(std::string path);

	bool file_read_by_line(std::string file, std::vector<std::string> &lines);
	bool file_write_by_line(std::string file, std::vector<std::string> &lines);
	bool file_insert(std::string file, std::string flag, std::string str);

	size_t	file_size(std::string file);
	bool file_del(std::string path, std::string name);
	bool dir_del(std::string dir);

}//namespace zm
#endif

