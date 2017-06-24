#pragma once

#include "Utility\RapidXml\rapidxml.hpp"
#include "Utility\RapidXml\rapidxml_print.hpp"

#include "core/String.h"

class XmlDocument;
struct XmlNode;

struct XmlAttribute
{
	void add_to_node(const XmlNode &p_node);

	rapidxml::xml_attribute<> *attr;
	XmlDocument *doc;
};

struct XmlNode
{
	void add_to_node(const XmlNode &p_node);

	void add_to_document();

	rapidxml::xml_node<>* node;
	XmlDocument *doc;
};

class XmlDocument
{
public:
	XmlDocument();
	~XmlDocument();

	void open(const String& p_source);

	XmlNode add_node(const String& p_name, const String& p_value);
	XmlAttribute add_attribute(const String& p_name, const String& p_value);

	String get_source();

	rapidxml::xml_document<> doc;
};