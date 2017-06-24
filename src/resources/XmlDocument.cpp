#include "XmlDocument.h"

#include <string>
#include <cstdlib>
#include <iostream>

using namespace rapidxml;

void XmlAttribute::add_to_node(const XmlNode &p_node)
{
	p_node.node->append_attribute(attr);
}

//==========================================================

void XmlNode::add_to_node(const XmlNode &p_node)
{
	p_node.node->append_node(node);
}

void XmlNode::add_to_document()
{
	doc->doc.append_node(node);
}

//==========================================================

XmlDocument::XmlDocument()
{
}


XmlDocument::~XmlDocument()
{
	doc.clear();
}

void XmlDocument::open(const String& p_source)
{
	doc.parse<0>(const_cast<char*>(p_source.c_str()));

	xml_node<>* decl = doc.allocate_node(node_declaration);
	decl->append_attribute(doc.allocate_attribute("version", "1.0"));
	decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
	doc.append_node(decl);
}

XmlNode XmlDocument::add_node(const String& p_name, const String& p_value)
{
	char *node_name = doc.allocate_string(p_name.c_str());
	char *node_value = doc.allocate_string(p_value.c_str());

	xml_node<> *node = doc.allocate_node(node_element, node_name, node_value);

	return { node, this };
}

XmlAttribute XmlDocument::add_attribute(const String& p_name, const String& p_value)
{
	char *node_name = doc.allocate_string(p_name.c_str());
	char *node_value = doc.allocate_string(p_value.c_str());

	xml_attribute<> *attr = doc.allocate_attribute(node_name, node_value);

	return { attr, this };
}

String XmlDocument::get_source()
{
	std::string s;
	print(std::back_inserter(s), doc, 0);

	return s;
}