#include "Config.h"
#include "XmlNode.h"
#include "utils/File.h"
#include <mxml.h>
#include <cstdlib>

namespace Text
{
	XmlNode::XmlNode()
	{
		_parent = NULL;
		_data = NULL;
	}

	XmlNode::XmlNode( const char * filename )
	{
		_parent = NULL;
		_data = NULL;
		Utils::FileStream fs;
		if(!fs.Open(filename))
			return;
		uint size = (uint)fs.Size();
		if(size == 0)
			return;
		std::string buf;
		buf.resize(size);
		fs.Read(&buf[0], size);
		fs.Close();
		mxml_node_t * root = mxmlLoadString(NULL, &buf[0], NULL);
		_data = root;
	}

	XmlNode * XmlNode::GetChild( const char * n )
	{
		mxml_node_t * root = (mxml_node_t *)_data;
		mxml_node_t * node = mxmlFindElement(root, root, n, NULL, NULL, MXML_DESCEND_FIRST);
		if(node == NULL)
			return NULL;
		XmlNode * r = new XmlNode;
		r->_parent = this;
		r->_data = (void *)node;
		return r;
	}

	XmlNode * XmlNode::GetNext(const char * n)
	{
		mxml_node_t * root = (_parent == NULL) ? NULL : (mxml_node_t *)_parent->_data;
		mxml_node_t * node = (mxml_node_t *)_data;
		node = mxmlFindElement(node, root, n, NULL, NULL, MXML_NO_DESCEND);
		if(node == NULL)
			return NULL;
		XmlNode * r = new XmlNode;
		r->_parent = _parent;
		r->_data = (void *)node;
		return r;
	}

	XmlNode::~XmlNode()
	{
		if(_parent == NULL && _data != NULL)
		{
			mxmlDelete((mxml_node_t*)_data);
			_data = NULL;
		}
	}

	const char * XmlNode::GetAttrString( const char * name, const char * def )
	{
		if(_data == NULL)
			return def;
		mxml_node_t * node = (mxml_node_t *)_data;
		const char * result = mxmlElementGetAttr(node, name);
		if(result == NULL)
			return def;
		return result;
	}

	int XmlNode::GetAttrInteger( const char * name, int def )
	{
		const char * result = GetAttrString(name, NULL);
		if(result == NULL)
			return def;
		return atoi(result);
	}

	double XmlNode::GetAttrReal( const char * name, double def )
	{
		const char * result = GetAttrString(name, NULL);
		if(result == NULL)
			return def;
		return atof(result);
	}

	void XmlNode::ReadArray( std::vector<std::vector<std::string> >& result, std::vector<std::string>& keys, const char * name )
	{
		result.clear();
		if(_data == NULL || keys.size() == 0)
			return;
		mxml_node_t * node = (mxml_node_t *)_data;
		if(node == NULL)
			return;
		mxml_node_t * child = mxmlFindElement(node, node, name, NULL, NULL, MXML_DESCEND_FIRST);
		size_t c = keys.size();
		while(child != NULL)
		{
			std::vector<std::string> item;
			item.resize(c);
			for(size_t i = 0; i < c; ++ i)
			{
				const char * str = mxmlElementGetAttr(child, keys[i].c_str());
				if(str == NULL)
					continue;
				item[i] = str;
			}
			result.push_back(item);
			child = mxmlFindElement(child, node, name, NULL, NULL, MXML_NO_DESCEND);
		}
	}
}
