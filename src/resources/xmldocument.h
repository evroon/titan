#pragma once

#include "core/definitions.h"

#if PLATFORM == LINUX
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#else
#include "utility\rapidxml\rapidxml.hpp"
#include "utility\rapidxml\rapidxml_print.hpp"
#endif

#include "core/string.h"

class XmlDocument;
struct XmlNode;

struct XmlAttribute {
    void add_to_node(const XmlNode& p_node);

    String get_name() const;
    String get_value() const;

    rapidxml::xml_attribute<>* attr;
    XmlDocument* doc;
};

struct XmlNode {
    void add_to_node(const XmlNode& p_node);
    void add_to_document();

    String get_name() const;
    String get_value() const;

    Array<XmlNode> get_children() const;
    Array<XmlAttribute> get_attributes() const;

    rapidxml::xml_node<>* node;
    XmlDocument* doc;
};

class XmlDocument {
   public:
    XmlDocument();
    ~XmlDocument();

    void open(const String& p_source);

    XmlNode add_node(const String& p_name, const String& p_value);
    XmlAttribute add_attribute(const String& p_name, const String& p_value);

    String get_source();

    XmlNode get_root();

    rapidxml::xml_document<> doc;
};
