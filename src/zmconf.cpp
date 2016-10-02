#include "zmconf.h"

Cir_Debug_Module_Define(zmconf);
namespace zm{

	ZAE_SINGLETON_INIT(zmConf);
	zmConf::zmConf():
		path_(""),
		company_(""),
		projName_(""),
		projPrefix_(""),
		projNotice_(""),
		projAuthor_(""),
		projSpace_("")
	{
		parser();
	}

	zmConf::~zmConf()
	{
		projDirs_.clear();
	}

	void zmConf::getConf(std::string &conf)
	{
		std::string curDir = cur_dir();
		recurse_rfind_file(curDir, ZM_CONF_NAME, path_);
		if (path_.empty())
		{
			conf = curDir + "/" + ZM_CONF_NAME;
			std::string  cpstr = "cp /etc/zmake/default.xml " + conf;
			system(cpstr.c_str());
			path_ = curDir;
			CINFO("Set defult conf: %s\n", cpstr.c_str());
			CINFO("Root path: %s\n", path_.c_str());
			return;
		}
		conf = path_ + "/" + ZM_CONF_NAME;
		CINFO("Get success conf: %s\n", conf.c_str());
		CINFO("Root path: %s\n", path_.c_str());
	}

	bool	zmConf::parser()
	{
		std::string xfile = "";
		getConf(xfile);
		pxml(new CXml(xfile.c_str()));
		if (!pxml()->isOk())
		{
			CERROR("open config file [%s] failed.\n", xfile.c_str());
			SAFE_RELEASE(pxml_);
			return false;
		}

		company_ = pxml_->getEnd(NULL, "root,company");
		projName_ = pxml_->getEnd(NULL, "root,project,name");
		projSpace_ = pxml_->getEnd(NULL, "root,project,space");
		projPrefix_ = pxml_->getEnd(NULL, "root,project,prefix");
		projNotice_ = pxml_->getEnd(NULL, "root,project,notice");
		projAuthor_ = pxml_->getEnd(NULL, "root,project,author");

		CXml::Nodes  dirNodes = pxml()->getNode(NULL, "root,project,struct,dir");
		for (int d = 0; d < dirNodes.size(); d++)
		{
			std::vector<zmConfMod>	mods;
			std::string dir = pxml_->getEnd(dirNodes[d], "dir,path");

			CXml::Nodes  modNodes = pxml()->getNode(dirNodes[d], "dir,module");
			for (int m = 0; m < modNodes.size(); m++)
			{
				zmConfMod mod;
				mod.name_ = pxml_->getEnd(modNodes[m], "module,name");
				mod.type_ = pxml_->getEnd(modNodes[m], "module,type");
	//			CINFO("ModName: %s, type: %s.\n", mod.name_.c_str(),mod.type_.c_str());

				//parser inc dirs
				CXml::Nodes  idNodes = pxml()->getNode(modNodes[m], "module,inc_dir");
				for (int ic = 0; ic < idNodes.size(); ic++)
				{
					std::string idir = pxml_->getEnd(idNodes[ic], "inc_dir");
					//		CINFO("Link lib: %s\n", link.c_str());
					mod.zcm_.inc_dirs().push_back(idir);
				}

				//parser link dirs
				CXml::Nodes  ldNodes = pxml()->getNode(modNodes[m], "module,link_dir");
				for (int l = 0; l < ldNodes.size(); l++)
				{
					std::string link = pxml_->getEnd(ldNodes[l], "link_dir");
					//		CINFO("Link lib: %s\n", link.c_str());
					mod.zcm_.link_dirs().push_back(link);
				}

				//parser link libs
				CXml::Nodes  llNodes = pxml()->getNode(modNodes[m], "module,link_lib");
				for (int l = 0; l < llNodes.size(); l++)
				{
					std::string link = pxml_->getEnd(llNodes[l], "link_lib");
					//		CINFO("Link lib: %s\n", link.c_str());
					mod.zcm_.link_libs().push_back(link);
				}

				//parser h
				CXml::Nodes  hNodes = pxml()->getNode(modNodes[m], "module,h");
				for (int h = 0; h < hNodes.size(); h++)
				{
					zmConfModFile mfile;
					mfile.type_ = "h";
					mfile.name_ = pxml_->getEnd(hNodes[h], "h");
			//		CINFO("%s: %s\n", mfile.type_.c_str(), mfile.name_.c_str());
					mod.files_.push_back(mfile);
				}

				//parser c
				CXml::Nodes  cNodes = pxml()->getNode(modNodes[m], "module,c");
				for (int c = 0; c < cNodes.size(); c++)
				{
					zmConfModFile mfile;
					mfile.type_ = "c";
					mfile.name_ = pxml_->getEnd(cNodes[c], "c");
			//		CINFO("%s: %s\n", mfile.type_.c_str(), mfile.name_.c_str());
					mod.files_.push_back(mfile);
				}

				//parser cpp
				CXml::Nodes  cppNodes = pxml()->getNode(modNodes[m], "module,cpp");
				for (int cpp = 0; cpp < cppNodes.size(); cpp++)
				{
					zmConfModFile mfile;
					mfile.type_ = "cpp";
					mfile.name_ = pxml_->getEnd(cppNodes[cpp], "cpp");
			//		CINFO("%s: %s\n", mfile.type_.c_str(), mfile.name_.c_str());
					mod.files_.push_back(mfile);
				}

				//parser class
				CXml::Nodes  classNodes = pxml()->getNode(modNodes[m], "module,class");
				for (int cl = 0; cl < classNodes.size(); cl++)
				{
					zmConfModFile mfile;
					mfile.type_ = "class";
					mfile.name_ = pxml_->getEnd(classNodes[cl], "class");
				//	CINFO("%s: %s\n", mfile.type_.c_str(), mfile.name_.c_str());
					mod.files_.push_back(mfile);
				}
				mods.push_back(mod);
			}

			projDirs_[dir] = mods;
		}

		return true;
	}

	void	zmConf::show()
	{
		CINFO("==============================================================================\n");
		CINFO("== Company: %s\n", company_.c_str());
		CINFO("== Project Name: %s\n", projName_.c_str());
		CINFO("== Project Author: %s\n", projAuthor_.c_str());
		CINFO("== Project Space: %s\n", projSpace_.c_str());
		CINFO("== Project Prefix: %s\n", projPrefix_.c_str());
		CINFO("== Project Notice: %s\n", projNotice_.c_str());

		CINFO("== Project Dir p: %p\n", &projDirs_);
		std::map<std::string, std::vector<zmConfMod> >::iterator dIter = projDirs_.begin();
		for (; dIter != projDirs_.end(); ++dIter)
		{
			CINFO("------------------------------------------------------------------------------\n");
			std::string dir = dIter->first;
			std::vector<zmConfMod>	mods = dIter->second;
			CINFO("==== Dir: %s, mods: %d, point: %p\n", dir.c_str(), mods.size(), &(dIter->second));
			std::vector<zmConfMod>::iterator mIter = mods.begin();
			for (; mIter != mods.end(); ++mIter)
			{
				std::string mName = mIter->name_;
				CINFO("======= Mod: %s, %s, %p\n", mName.c_str(), mIter->type_.c_str(), &(mIter->files_));

				std::vector<std::string> incDirs = mIter->zcm_.inc_dirs();
				std::vector<std::string>::iterator incIter = incDirs.begin();
				for (; incIter != incDirs.end(); ++incIter)
				{
					CINFO("=========== Link lib: %s\n", incIter->c_str());
				}

				std::vector<std::string> linkDirs = mIter->zcm_.link_dirs();
				std::vector<std::string>::iterator ldIter = linkDirs.begin();
				for (; ldIter != linkDirs.end(); ++ldIter)
				{
					CINFO("=========== Link lib: %s\n", ldIter->c_str());
				}

				std::vector<std::string> linkLibs = mIter->zcm_.link_libs();
				std::vector<std::string>::iterator lIter = linkLibs.begin();
				for (; lIter != linkLibs.end(); ++lIter)
				{
					CINFO("=========== Link lib: %s\n", lIter->c_str());
				}

				std::vector<zmConfModFile> files = mIter->files_;
				std::vector<zmConfModFile>::iterator fIter = files.begin();
				for (; fIter != files.end(); ++fIter)
				{
					std::string type = fIter->type_;
					std::string fName = fIter->name_;
					CINFO("=========== File: %s.%s\n", fName.c_str(), type.c_str());
				}
			}
		}

		CINFO("==============================================================================\n");
	}

	void	zmConf::save()
	{
		CXml *pnx = new CXml();
		pnx->xmlfile(pxml_->xmlfile());
		pnx->setRoot("root");
		pnx->setNode(pnx->proot(), "root,company", company_);
		pnx->setNode(pnx->proot(), "root,project,name", projName_);
		pnx->setNode(pnx->proot(), "root,project,space", projSpace_);
		pnx->setNode(pnx->proot(), "root,project,prefix", projPrefix_);
		pnx->setNode(pnx->proot(), "root,project,notice", projNotice_);
		pnx->setNode(pnx->proot(), "root,project,author", projAuthor_);


		CXml::Node projNode= pnx->getNode(pnx->proot(), "root,project").at(0);
		CXml::Node	structNode = pxml()->newNode(projNode, "struct");
		std::map<std::string, std::vector<zmConfMod> >::iterator dIter = projDirs_.begin();
		for (; dIter != projDirs_.end(); ++dIter)
		{
			CXml::Node	newDirNode = pxml()->newNode(structNode, "dir");
			pnx->setNode(newDirNode, "dir,path", dIter->first);
			std::vector<zmConfMod>	mods = dIter->second;
			std::vector<zmConfMod>::iterator mIter = mods.begin();
			for (; mIter != mods.end(); ++mIter)
			{
		//		CINFO("Save module[%p/%p] info: n[%s], t[%s], s[%d]\n", mIter, &(dIter->second), mIter->name_.c_str(), mIter->type_.c_str(), mIter->files_.size());
				CXml::Node	newModNode = pxml()->newNode(newDirNode, "module");
				pnx->setNode(newModNode, "module,name", mIter->name_);
				pnx->setNode(newModNode, "module,type", mIter->type_);

				std::vector<std::string> incDirs = mIter->zcm_.inc_dirs();
				std::vector<std::string>::iterator incIter = incDirs.begin();
				for (; incIter != incDirs.end(); ++incIter)
				{
				//	CINFO("=========== Link lib: %s\n", incIter->c_str());
					CXml::Node	newFileNode = pxml()->newNode(newModNode, "inc_dir");
					pnx->setNode(newFileNode, "inc_dir", incIter->c_str());
				}

				std::vector<std::string> linkDirs = mIter->zcm_.link_dirs();
				std::vector<std::string>::iterator ldIter = linkDirs.begin();
				for (; ldIter != linkDirs.end(); ++ldIter)
				{
				//	CINFO("=========== Link lib: %s\n", ldIter->c_str());
					CXml::Node	newFileNode = pxml()->newNode(newModNode, "link_dir");
					pnx->setNode(newFileNode, "link_dir", ldIter->c_str());
				}

				std::vector<std::string> linkLibs = mIter->zcm_.link_libs();
				std::vector<std::string>::iterator lIter = linkLibs.begin();
				for (; lIter != linkLibs.end(); ++lIter)
				{
				//	CINFO("=========== Link lib: %s\n", lIter->c_str());
					CXml::Node	newFileNode = pxml()->newNode(newModNode, "link_lib");
					pnx->setNode(newFileNode, "link_lib", lIter->c_str());
				}

				std::vector<zmConfModFile> files = mIter->files_;
				std::vector<zmConfModFile>::iterator fIter = files.begin();
				for (; fIter != files.end(); ++fIter)
				{
					std::string ftype = fIter->type_;
					CXml::Node	newFileNode = pxml()->newNode(newModNode, (char *)(fIter->type_.c_str()));
			//		CINFO("Save file name: %s, type: %s\n", fIter->name_.c_str(), fIter->type_.c_str());
					pnx->setNode(newFileNode, fIter->type_, fIter->name_);
				}
			}
		}

		pnx->save();
	}

	int	zmConf::doConf()
	{
		/* create root CMakeList.txt */
		if (path_.empty())
		{
			CERROR("Error path: %s\n", path_.c_str());
			return 0;
		}
		zcm_.isRoot() = true;
		zcm_.path() = path_ + "/" + CM_CONF_FILE;
		zcm_.name() = projName();

		CONF_DIRS::iterator dIter = projDirs().begin();
		for (; dIter != projDirs().end(); ++dIter)
		{
			std::string dir = path_ + "/" + dIter->first;
			create_dir(dir);
			std::vector<zmConfMod>	mods = dIter->second;
			std::vector<zmConfMod>::iterator mIter = mods.begin();
			for (; mIter != mods.end(); ++mIter)
			{
				std::string mod_path = dir + "/" + mIter->name_;
				create_dir(mod_path);
				/* TODO	create module CMakeList.txt */
				mIter->zcm_.isRoot() = false;
				mIter->zcm_.path() = mod_path + "/" + CM_CONF_FILE;
				mIter->zcm_.name() = mIter->name_;
				mIter->zcm_.type() = mIter->type_;
				mIter->zcm_.save();
				
				/* add info to root list */
				if (mIter->type_ == "process"){
					zcm_.proc_dirs().push_back(dIter->first + "/" + mIter->name_);
				}
				else{
					zcm_.libs_dirs().push_back(dIter->first + "/" + mIter->name_);
				}
				zcm_.inc_dirs().push_back(dIter->first);
				std::vector<zmConfModFile> files = mIter->files_;
				std::vector<zmConfModFile>::iterator fIter = files.begin();
				for (; fIter != files.end(); ++fIter)
				{
					zmFile::FPARAM fp;
					fp.module = mIter->name_;
					fp.name = fIter->name_;
					fp.type = fIter->type_;
					fp.path = mod_path;
					zmFile zf;
					zf.createFile(fp);
				}
			}
		}

		zcm_.save();
		return -1;
	}

	bool	zmConf::createModule(std::string path, std::string name)
	{
		if ((path.empty()) || (name.empty()))
		{
			return false;
		}
		int fl = name.find('.', 0);
		if (fl == std::string::npos)
		{
			name += ".lib";
			fl = name.find('.', 0);
		}
		std::string fname = name.substr(0, fl);
		std::string ftype = name.substr(fl + 1, name.length() - fl);

		if ((ftype == "library") || (ftype == "lib") || (ftype == "l") || (ftype == "L"))
		{
			ftype = "library";
		}
		else if ((ftype == "process") || (ftype == "proc") || (ftype == "p") || (ftype == "P"))
		{
			ftype = "process";
		}
		else
		{
			return false;
		}
		std::map<std::string, std::vector<zmConfMod> >::iterator dIter = projDirs_.begin();
		for (; dIter != projDirs_.end(); ++dIter)
		{
			if (path == dIter->first)
			{
				zmConfMod mod;
				mod.name_ = fname;
				mod.type_ = ftype;
				dIter->second.push_back(mod);

				std::string mod_path = path_ + "/" + path + "/" + fname;
				create_dir(mod_path);
				/* TODO	create module CMakeList.txt */
				zcmake zcm;
				zcm.isRoot() = false;
				zcm.path() = mod_path + "/" + CM_CONF_FILE;
				zcm.name() = fname;
				zcm.type() = ftype;
				zcm.save();

				return true;
			}
		}

		return false;
	}

	bool	zmConf::createFile(std::string path, std::string name)
	{
		if ((path.empty()) || (name.empty()))
		{
			return false;
		}

		int l = path.rfind('/', path.length());
		std::string npath = path.substr(0, l);
		std::string nmod = path.substr(l + 1, path.length() - l);

		int fl = name.find('.', 0);
		if (fl == std::string::npos)
		{
			name += ".class";
			fl = name.find('.', 0);
		}
		std::string fname = name.substr(0, fl);
		std::string ftype = name.substr(fl + 1, name.length() - fl);

		CINFO("Module name: %s, path: %s\n", nmod.c_str(), npath.c_str());
		std::map<std::string, std::vector<zmConfMod> >::iterator dIter = projDirs_.begin();
		for (; dIter != projDirs_.end(); ++dIter)
		{
			if (npath == dIter->first)
			{
				CINFO("Found path ok: %s\n", npath.c_str());
				std::vector<zmConfMod>	mods = dIter->second;
				std::vector<zmConfMod>::iterator mIter = dIter->second.begin();
				for (; mIter != dIter->second.end(); ++mIter)
				{
					if (mIter->name_ == nmod)
					{
						if (ftype == "inc_dir")
						{
							mIter->zcm_.inc_dirs().push_back(fname);
						}
						else if (ftype == "link_dir")
						{
							mIter->zcm_.link_dirs().push_back(fname);
						}
						else if (ftype == "link_lib")
						{
							mIter->zcm_.link_libs().push_back(fname);
						}
						else
						{
							CINFO("Module[%p/%p] info: n[%s], t[%s], s[%d]\n", mIter, &(dIter->second), mIter->name_.c_str(), mIter->type_.c_str(), mIter->files_.size());
							zmConfModFile mfile;
							mfile.type_ = ftype;
							mfile.name_ = fname;
							mIter->files_.push_back(mfile);

							zmFile::FPARAM fp;
							fp.module = nmod;
							fp.name = fname;
							fp.type = ftype;
							fp.path = path_ + "/" + npath + "/" + nmod;
							zmFile zf;
							zf.createFile(fp);
						}

						return true;
					}
				}
			}
		}

		return false;
	}

	bool	zmConf::deleteModule(std::string path, std::string name)
	{
		if ((path.empty()) || (name.empty()))
		{
			return false;
		}
		std::map<std::string, std::vector<zmConfMod> >::iterator dIter = projDirs_.begin();
		for (; dIter != projDirs_.end(); ++dIter)
		{
			if (path == dIter->first)
			{
				std::vector<zmConfMod>::iterator mIter = dIter->second.begin();
				for (; mIter != dIter->second.end(); ++mIter)
				{
					if (mIter->name_ == name)
					{
						CDEBUG("Delete module: %s, from path: %s\n", name.c_str(), dIter->first.c_str());
						dIter->second.erase(mIter);
						dir_del(path_ + "/" + path + "/" + name);
						return true;
					}
				}
			}
		}

		return false;
	}

	bool	zmConf::deleteFile(std::string path, std::string name)
	{
		if ((path.empty()) || (name.empty()))
		{
			return false;
		}

		if ((path.find("/") == std::string::npos) || (name.find(".") == std::string::npos))
		{
			return false;
		}

		int l = path.rfind('/', path.length());
		std::string npath = path.substr(0, l);
		std::string nmod = path.substr(l + 1, path.length() - l);

		int fl = name.find('.', 0);
		if (fl == std::string::npos)
		{
			name += ".class";
			fl = name.find('.', 0);
		}
		std::string fname = name.substr(0, fl);
		std::string ftype = name.substr(fl + 1, name.length() - fl);

		CINFO("Module name: %s, path: %s\n", nmod.c_str(), npath.c_str());
		std::map<std::string, std::vector<zmConfMod> >::iterator dIter = projDirs_.begin();
		for (; dIter != projDirs_.end(); ++dIter)
		{
			if (npath == dIter->first)
			{
				CINFO("Found path ok: %s\n", npath.c_str());
				std::vector<zmConfMod>::iterator mIter = dIter->second.begin();
				for (; mIter != dIter->second.end(); ++mIter)
				{
					if (mIter->name_ == nmod)
					{
						if (ftype == "inc_dir")
						{
							std::vector<std::string>::iterator iter = mIter->zcm_.inc_dirs().begin();
							for (; iter != mIter->zcm_.inc_dirs().end(); ++iter)
							{
								if (*iter == fname)
								{ 
									mIter->zcm_.inc_dirs().erase(iter);
									return true;
								}
							}
						}
						else if (ftype == "link_dir")
						{
							std::vector<std::string>::iterator iter = mIter->zcm_.link_dirs().begin();
							for (; iter != mIter->zcm_.link_dirs().end(); ++iter)
							{
								if (*iter == fname)
								{
									mIter->zcm_.link_dirs().erase(iter);
									return true;
								}
							}
						}
						else if (ftype == "link_lib")
						{
							std::vector<std::string>::iterator iter = mIter->zcm_.link_libs().begin();
							for (; iter != mIter->zcm_.link_libs().end(); ++iter)
							{
								if (*iter == fname)
								{
									mIter->zcm_.link_libs().erase(iter);
									return true;
								}
							}
						}
						else
						{
							CINFO("Found module: %s, %s, files: %d\n", nmod.c_str(), mIter->type_.c_str(), mIter->files_.size());
							std::vector<zmConfModFile>::iterator fIter = mIter->files_.begin();
							for (; fIter != mIter->files_.end(); ++fIter)
							{
								CINFO("Check file: %s/%s, %s/%s\n", fIter->type_.c_str(), fname.c_str(), fIter->type_.c_str(), ftype.c_str());
								if (fIter->name_ == fname)
								{
									CINFO("Found file: %s, %s\n", fname.c_str(), fIter->type_.c_str());
									if (fIter->type_ == "class")
									{
										if (ftype == "h")
										{
											fIter->type_ = "cpp";
											file_del(path_ + "/" + path, fname + ".h");
										}
										else if (ftype == "cpp")
										{
											fIter->type_ = "h";
											file_del(path_ + "/" + path, fname + ".cpp");
										}
										else if (ftype == "class")
										{
											file_del(path_ + "/" + path, fname + ".h");
											file_del(path_ + "/" + path, fname + ".cpp");

											mIter->files_.erase(fIter);
										}
										CINFO("Delete class file: %s\n", fname.c_str());
										return true;
									}
									else if (fIter->type_ == ftype)
									{
										mIter->files_.erase(fIter);
										file_del(path_ + "/" + path, name);
										CINFO("Delete file: %s\n", fname.c_str());
										return true;
									}
								}
							}
						}
					}
				}
			}
		}

		return false;
	}

}
