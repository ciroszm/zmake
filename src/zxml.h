#ifndef ZAE_CXML_H
#define ZAE_CXML_H


#include <vector>
#include <string>
#include "tinyxml.h"

	// windows include	
#if ZAE_PLATFORM == PLATFORM_WIN32
	//#include <Windows.h>
#else
	// linux include
#endif

namespace zm{
	class CXml
	{
	public:
		CXml();
		CXml(const char* pFile);
		~CXml();

		typedef	std::vector< TiXmlNode * >	Nodes;
		typedef	TiXmlNode*	Node;
		void		setRoot(const char *root);

		void		findNode(TiXmlNode* pParent, std::vector< std::string >	&nds, int &level, Nodes &ret);
		void		fillNode(TiXmlNode* pParent, std::vector< std::string >	&nds, int &level, std::string value);

		bool		hasNode(std::string forward);
		void		printNodes(Nodes &nodes);
		Nodes		getNode(Node start, std::string forward);
		std::string	getEnd(Node start, std::string forward);
		void		delNode(Node start, std::string forward);

		/*
		eg: setNode(pxml()->newNode(n, "type"), "type", "");
		*/
		void		setNode(Node from, std::string forward, std::string value);
		Node		newNode(Node parent, char *name);

		void		save(const char* pFilename);
		void		save();

		void			xmlfile(std::string f){ xmlfile_ = f; };
		std::string		&xmlfile(){ return xmlfile_; }
		bool			isOk(){ return isOk_; };
		TiXmlDocument	*pdoc(){ return pdoc_; };
		TiXmlElement	*proot(){ return proot_; };
	private:
		bool			isOk_;
		std::string		xmlfile_;
		TiXmlDocument	*pdoc_;
		TiXmlElement	*proot_;

		std::vector< std::string > splits(const std::string& s, const std::string& delim, const bool keep_empty);
	};


}//namespace zm
#endif
