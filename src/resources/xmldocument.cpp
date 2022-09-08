#include "xmldocument.h"

#include <cstdlib>
#include <iostream>
#include <string>

using namespace rapidxml;

void XmlAttribute::add_to_node(const XmlNode &p_node) {
    p_node.node->append_attribute(attr);
}

String XmlAttribute::get_name() const { return attr->name(); }

String XmlAttribute::get_value() const { return attr->value(); }

//==========================================================

void XmlNode::add_to_node(const XmlNode &p_node) {
    p_node.node->append_node(node);
}

void XmlNode::add_to_document() { doc->doc.append_node(node); }

String XmlNode::get_name() const { return node->name(); }

String XmlNode::get_value() const { return node->value(); }

Array<XmlNode> XmlNode::get_children() const {
    Array<XmlNode> arr;

    for (xml_node<> *n = node->first_node(); n; n = n->next_sibling()) {
        arr.push_back({n, doc});
    }

    return arr;
}

Array<XmlAttribute> XmlNode::get_attributes() const {
    Array<XmlAttribute> arr;

    for (xml_attribute<> *a = node->first_attribute(); a;
         a = a->next_attribute()) {
        arr.push_back({a, doc});
    }

    return arr;
}

//==========================================================

XmlDocument::XmlDocument() {}

XmlDocument::~XmlDocument() { doc.clear(); }

void XmlDocument::open(const String &p_source) {
    doc.parse<0>(const_cast<char *>(p_source.c_str()));

    xml_node<> *decl = doc.allocate_node(node_declaration);
    decl->append_attribute(doc.allocate_attribute("version", "1.0"));
    decl->append_attribute(doc.allocate_attribute("encoding", "utf-8"));
    doc.append_node(decl);
}

XmlNode XmlDocument::add_node(const String &p_name, const String &p_value) {
    char *node_name = doc.allocate_string(p_name.c_str());
    char *node_value = doc.allocate_string(p_value.c_str());

    xml_node<> *node = doc.allocate_node(node_element, node_name, node_value);

    return {node, this};
}

XmlAttribute XmlDocument::add_attribute(const String &p_name,
                                        const String &p_value) {
    char *node_name = doc.allocate_string(p_name.c_str());
    char *node_value = doc.allocate_string(p_value.c_str());

    xml_attribute<> *attr = doc.allocate_attribute(node_name, node_value);

    return {attr, this};
}

String XmlDocument::get_source() {
    std::string s;
    print(std::back_inserter(s), doc, 0);

    return s;
}

XmlNode XmlDocument::get_root() { return {doc.first_node(), this}; }
