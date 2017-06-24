#include "Serializer.h"

#include "resources/XmlDocument.h"

#include "Node.h"

Serializer *Serializer::singleton;
TypeSerializer *TypeSerializer::singleton;

Serializer::Serializer()
{

}

Serializer::~Serializer()
{

}

String Serializer::serialize(const Variant & p_value)
{
	XmlDocument doc;
	doc.open("");

	XmlNode node = doc.add_node("root", "");
	serialize_recursively("root", p_value, node);
	node.add_to_document();

	String s = doc.get_source();

	return s;
}

void Serializer::serialize_recursively(const String &p_name, const Variant &p_value, XmlNode &p_node)
{
	VariantType type = p_value.get_type();
	Variant::Type var_type = type;

	XmlDocument &doc = *p_node.doc;
	XmlNode node = doc.add_node(p_name, "");
	doc.add_attribute("type", type.get_type_name()).add_to_node(node);

	if (var_type == Variant::UNDEF)
	{

	}
	else if (var_type != Variant::OBJECT)
	{
		doc.add_attribute("value", p_value.ToString()).add_to_node(node);
	}
	else
	{
		Node* n = dynamic_cast<Node*>(p_value.o);
		if (n)
		{
			doc.add_attribute("name", type.get_type_name()).add_to_node(node);

			String s = n->get_child_count();
			XmlNode children_node = p_node.doc->add_node("children", "count = " + s);

			for (int c = 0; c < n->get_child_count(); c++)
				serialize_recursively(n->get_name(), n->get_child(c), children_node);

			children_node.add_to_node(node);
		}
		//else
		{
			Array<StringName> properties = MMASTER->list_property_names(type);

			String s = properties.size();
			XmlNode properties_node = p_node.doc->add_node("properties", "count = " + s);

			for (int c = 0; c < properties.size(); c++)
				serialize_recursively(properties[c].get_source(), MMASTER->get_property(type, properties[c])->get->operator() (p_value), properties_node);

			properties_node.add_to_node(node);
		}
	}
	node.add_to_node(p_node);
}

Variant Serializer::deserialize(const String& p_source)
{
	Array<String> sources = p_source.split(':');

	String prefix = sources[0];
	String value = sources[1];

	VariantType type = prefix;
	Variant result;

	switch (type.operator Variant::Type())
	{
	case Variant::BOOL:
		result = value == "true" ? true : false;
		break;

	case Variant::INT:
		result = value.operator int();
		break;

	case Variant::FLOAT:
		result = value.operator float();
		break;

	case Variant::STRING:
		result = value;
		break;

	case Variant::VEC2:
	case Variant::VEC3:
	case Variant::VEC4:
	case Variant::TRANSFORM:
		result = value;
		break;
	}

	return result;
}

Array<String> Serializer::deserialize_set(const String &p_value)
{
	return p_value.split(',');
}

void Serializer::init()
{
	singleton = new Serializer;
}

Serializer* Serializer::get_singleton()
{
	return singleton;
}

#undef CLASSNAME
#define CLASSNAME Serializer

void Serializer::bind_methods()
{
	REG_SINGLETON(SERIALIZER);
}


//=========================================================================
//TypeSerializer
//=========================================================================

TypeSerializer::TypeSerializer()
{
}

TypeSerializer::~TypeSerializer()
{
}

String TypeSerializer::serialize_all_types() const
{
	XmlDocument doc;

	for (std::pair<StringName, VariantType> p : TYPEMAN->types)
	{
		XmlNode node = doc.add_node(p.first, "");

		serialize_type(p.second, node);

		node.add_to_document();
	}
	return doc.get_source();
}

void TypeSerializer::serialize_type(const VariantType& p_type, XmlNode &p_node) const
{
	Array<StringName> properties = MMASTER->list_property_names(p_type);
	Array<StringName> methods = MMASTER->list_method_names(p_type);

	XmlNode properties_node = p_node.doc->add_node("properties", "");

	for (int c = 0; c < properties.size(); c++)
	{
		Property *p = MMASTER->get_property(p_type, properties[c]);

		XmlNode property_node = p_node.doc->add_node(properties[c], "");
		p_node.doc->add_attribute("type", p->var_type.get_type_name()).add_to_node(property_node);

		property_node.add_to_node(properties_node);
	}

	properties_node.add_to_node(p_node);

	XmlNode methods_node = p_node.doc->add_node("methods", "");

	for (int c = 0; c < methods.size(); c++)
	{
		Method *m = MMASTER->get_method(p_type, methods[c]);

		XmlNode method_node = p_node.doc->add_node(m->name, "");

		if (m->returns_variant)
			p_node.doc->add_attribute("return_type", reinterpret_cast<ReturnMethod*>(m)->return_type.get_type_name()).add_to_node(method_node);
		else
			p_node.doc->add_attribute("return_type", "void").add_to_node(method_node);

		p_node.doc->add_attribute("const", m->is_const ? "true" : "false").add_to_node(method_node);
		
		XmlNode parameters_node = p_node.doc->add_node("parameters", "");

		for (int i = 0; i < m->param_types.size(); i++)
			p_node.doc->add_node("type", m->param_types[i]).add_to_node(parameters_node);

		parameters_node.add_to_node(method_node);

		method_node.add_to_node(methods_node);
	}

	methods_node.add_to_node(p_node);
}

void TypeSerializer::init()
{
}

TypeSerializer * TypeSerializer::get_singleton()
{
	return nullptr;
}

void TypeSerializer::bind_methods()
{
}
