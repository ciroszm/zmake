#include <algorithm>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "zxml.h"
#include "common.h"

Cir_Debug_Module_Define(cxml);
namespace zm{


	CXml::CXml() :
		isOk_(false),
		pdoc_(NULL),
		proot_(NULL)
	{
		pdoc_ = new TiXmlDocument;
		pdoc()->LinkEndChild(new TiXmlDeclaration("1.0", "UTF-8", ""));
	}

	CXml::~CXml()
	{
		if (pdoc_)
		{
			pdoc_->Clear();
			delete pdoc_;
			pdoc_ = NULL;
			proot_ = NULL;
		}
	}

	CXml::CXml(const char* pFile) :
		isOk_(false),
		pdoc_(NULL),
		proot_(NULL)
	{
		pdoc_ = new TiXmlDocument(pFile);
		if (pdoc()->LoadFile(TIXML_ENCODING_UTF8))
		{
			xmlfile_ = pFile;
			isOk_ = true;
			proot_ = pdoc()->RootElement();
		}
	}

	std::string	CXml::getEnd(Node start, std::string forward)
	{
		if (forward.empty())return "";
		Nodes end;
		if (start)
			end = getNode(start, forward);
		else
			end = getNode(proot_, forward);

		if (end.size()){ return end.at(0)->FirstChild()->Value(); }
		return "";
	}

	void		CXml::setRoot(const char *root)
	{
		proot_ = new TiXmlElement(root);
		pdoc()->LinkEndChild(proot());
		isOk_ = true;
	}

	void		CXml::printNodes(Nodes &nodes)
	{
		if (!isOk_)return;
		for (int i = 0; i < nodes.size(); i++)
		{
			printf("Node: %s, %d, %d\n", nodes[i]->Value()), nodes[i]->Type(), nodes[i]->NoChildren();
			TiXmlNode* pChild;
			for (pChild = nodes[i]->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
			{
				if (pChild->Type() == TiXmlNode::TINYXML_TEXT)
				{
					printf("pChild: %s\n", pChild->ToText()->Value());
				}
			}
		}
	}

	void		CXml::findNode(TiXmlNode* pParent, std::vector< std::string >	&nds, int &level, Nodes &ret)
	{
		if (!pParent) return;
		if (level >= (int)nds.size())return;
		if (nds[level].compare(pParent->Value()) != 0)return;

		if ((level == (nds.size() - 1)) && (nds[level].compare(pParent->Value()) == 0))
		{
			ret.push_back(pParent);
			return;
		}

		TiXmlNode* pChild;
		level++;
		for (pChild = pParent->FirstChild(); pChild != 0; pChild = pChild->NextSibling())
		{
			findNode(pChild, nds, level, ret);
		}
		level--;
	}

	void		CXml::fillNode(TiXmlNode* pParent, std::vector< std::string >	&nds, int &level, std::string value)
	{
		if (!pParent) return;
		if (level > (int)nds.size())return;
		if (level == (int)nds.size())
		{
			if (pParent->NoChildren())
			{
				pParent->LinkEndChild(new TiXmlText(value.c_str()));
			}
			else
			{
				pParent->FirstChild()->SetValue(value.c_str());
			}
			return;
		}
		else
		{
			TiXmlElement *e;
			e = new TiXmlElement(nds[level].c_str());
			pParent->LinkEndChild(e);
			level++;
			fillNode(e, nds, level, value);
		}
	}

	bool		CXml::hasNode(std::string forward)
	{
		if (!isOk_)return false;
		int level = 0;
		Nodes	nodes;
		std::vector< std::string >	nds = splits(forward, ",", false);
		findNode(proot(), nds, level, nodes);
		if (nodes.size())return true;
		return false;
	}

	CXml::Nodes		CXml::getNode(Node start, std::string forward)
	{
		Nodes nodes;
		nodes.clear();
		if (isOk_)
		{
			int level = 0;
			std::vector< std::string >	nds = splits(forward, ",", false);
			if (start)
			{
				findNode(start, nds, level, nodes);
			}
			else
			{
				findNode(proot(), nds, level, nodes);
			}
		}
		return nodes;
	}



	void		CXml::delNode(Node start, std::string forward)
	{
		Nodes ns = getNode(start, forward);
		for (int i = 0; i < ns.size(); i++)
		{
			TiXmlElement *pParent = ns[i]->Parent()->ToElement();
			if (pParent) pParent->RemoveChild(ns[i]);
		}
	}

	void		CXml::setNode(Node from, std::string forward, std::string value)
	{
		if (!isOk_)return;
		int l = 0;
		Nodes	n;
		std::vector< std::string >	s, t, x;
		t = s = splits(forward, ",", false);
		x.clear();
		for (int i = (int)s.size() - 1; i >= 0; i--)
		{
			n.clear();
			l = 0;
			findNode(from, t, l, n);
			if (n.size())break;
			t.pop_back();
			x.insert(x.begin(), s[i].c_str());
		}

		for (int i = 0; i < n.size(); i++)
		{
			l = 0;
			fillNode(n[i], x, l, value);
		}
	}


	CXml::Node		CXml::newNode(Node parent, char *name)
	{
		if (!isOk_)return NULL;
		Node	n(new TiXmlElement(name));
		parent->LinkEndChild(n);
		return n;
	}

	void		CXml::save(const char* pFilename)
	{
		pdoc()->SaveFile(pFilename);
	}

	void		CXml::save()
	{
		if (!xmlfile_.empty())
		{
			pdoc()->SaveFile(xmlfile_.c_str());
		}
	}


	std::vector< std::string > CXml::splits(const std::string& s, const std::string& delim, const bool keep_empty)
	{
		std::vector< std::string > result;

		if (delim.empty()) {
			result.push_back(s);
			return result;
		}

		std::string::const_iterator substart = s.begin(), subend;

		while (true) {
			subend = std::search(substart, s.end(), delim.begin(), delim.end());
			std::string temp(substart, subend);
			if (keep_empty || !temp.empty()) {
				result.push_back(temp);
			}
			if (subend == s.end()) {
				break;
			}
			substart = subend + delim.size();
		}

		return result;
	}


}
