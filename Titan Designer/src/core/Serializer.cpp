#include "Serializer.h"

#include "resources/XmlDocument.h"

#include "Node.h"

#include "ContentManager.h"
#include "resources\Texture.h"

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
			String s = n->get_child_count();
			XmlNode children_node = p_node.doc->add_node("children", "");
			doc.add_attribute("count", s).add_to_node(children_node);

			for (int c = 0; c < n->get_child_count(); c++)
			{
				Node* child = n->get_child_by_index(c);
				serialize_recursively(child->get_name(), child, children_node);
			}

			children_node.add_to_node(node);
		}
		//else
		{
			Array<StringName> properties = MMASTER->list_property_names(type);

			String s = properties.size();
			XmlNode properties_node = p_node.doc->add_node("properties", "");
			doc.add_attribute("count", s).add_to_node(properties_node);

			for (int c = 0; c < properties.size(); c++)
				serialize_recursively(properties[c].get_source(), MMASTER->get_property(type, properties[c])->get->operator() (p_value), properties_node);

			properties_node.add_to_node(node);
		}
	}
	node.add_to_node(p_node);
}

Variant Serializer::deserialize(const String& p_source)
{
	XmlDocument doc;
	doc.open(p_source);

	return deserialize_recursively(doc.get_root().get_children()[0]);
}

Variant Serializer::deserialize_recursively(const XmlNode& p_node)
{
	Array<XmlNode> nodes = p_node.get_children();

	if (nodes.size() == 0 && p_node.get_attributes().size() == 1)
	{
		if (p_node.get_attributes()[0].get_value() != "NULL")
			T_ERROR("Corrupt");
		
		return NULL_VAR;
	}
	else if (nodes.size() == 0 || p_node.get_attributes().size() == 0)
	{
		T_ERROR("Corrupt");
		return NULL_VAR;
	}

	Array<XmlNode> children;
	Array<XmlNode> properties;

	if (nodes.size() == 1)
	{
		properties = nodes[0].get_children();
	}
	else
	{
		children = nodes[0].get_children();
		properties = nodes[1].get_children();
	}

	Variant result;
	VariantType type = p_node.get_attributes()[0].get_value();
	if (type.derives_from_type<Resource>())
	{
		XmlNode file_property;
		for (XmlNode p : properties) {
			if (p.get_name() == "file")
				file_property = p;
		}
		Serializer::Property p = deserialize_property(file_property);
		result = CONTENT->Load(p.value.ToString());
		return result;
	}
	if (MMASTER->constructor_exists(type, 0))
		result = reinterpret_cast<CSTR_0*>(MMASTER->get_constructor(type, 0))->operator() ();
	else
	{
		T_ERROR("type " + type.get_type_name() + " has no default constructor that takes one parameter");
		return NULL_VAR;
	}
	
	for (int c = 0; c < properties.size(); c++)
	{
		Serializer::Property p = deserialize_property(properties[c]);

		::Property* pr = MMASTER->get_property(type, p.name);

		if (pr && pr->set)
			pr->set->operator() (result, p.value);
	}

	for (int c = 0; c < children.size(); c++)
	{
		Variant child = deserialize_recursively(children[c]);
		(result.operator Node* ())->add_child(child);
	}

	return result;
}

Serializer::Property Serializer::deserialize_property(const XmlNode& p_node)
{
	Serializer::Property result;

	Array<XmlAttribute> attributes = p_node.get_attributes();
	XmlAttribute a_type = attributes[0];
	result.name = p_node.get_name();
	result.type = a_type.get_value();

	if (attributes.size() < 2)
	{
		//is object
		result.value = deserialize_recursively(p_node);
		return result;
	}

	XmlAttribute s_value = attributes[1];
	String value = s_value.get_value();

	Variant::Type type = result.type.operator Variant::Type();

	switch (type)
	{
	case Variant::UNDEF:
		result.value = NULL_VAR;
		break;
		
	case Variant::BOOL:
		result.value = value == "true" ? true : false;
		break;

	case Variant::INT:
		result.value = value.operator int();
		break;

	case Variant::FLOAT:
		result.value = value.operator float();
		break;

	case Variant::STRING:
		result.value = value;
		break;

	case Variant::VEC2:
		result.value = deserialize_vec2(value);
		break;

	case Variant::VEC3:
		result.value = deserialize_vec3(value);
		break;

	case Variant::VEC4:
		result.value = deserialize_vec4(value);
		break;

	case Variant::COLOR:
		result.value = deserialize_color(value);
		break;

	case Variant::TRANSFORM:
		result.value = deserialize_transform(value);
		break;

	case Variant::ARRAY:
	case Variant::MAT4:
		result.value = NULL_VAR;
		break;
	}
	
	return result;
}

vec2 Serializer::deserialize_vec2(const String& p_source) const
{
	Array<String> a = p_source.split(' ');

	String x = a[1];
	String y = a[2];

	return vec2(x.substr(0, x.size() - 1), y);
}

vec3 Serializer::deserialize_vec3(const String& p_source) const
{
	Array<String> a = p_source.split(' ');

	String x = a[1];
	String y = a[2];
	String z = a[3];

	return vec3(x.substr(0, x.size() - 1), y.substr(0, y.size() - 1), z);
}

vec4 Serializer::deserialize_vec4(const String& p_source) const
{
	Array<String> a = p_source.split(' ');

	String x = a[1];
	String y = a[2];
	String z = a[3];
	String w = a[4];

	return vec4(x.substr(0, x.size() - 1), y.substr(0, y.size() - 1), z.substr(0, z.size() - 1), w);
}

Color Serializer::deserialize_color(const String & p_source) const
{
	Array<String> a = p_source.split(' ');

	String x = a[3];
	String y = a[6];
	String z = a[9];
	String w = a[12];

	return vec4(x.substr(0, x.size() - 1), y.substr(0, y.size() - 1), z.substr(0, z.size() - 1), w);
}

Transform Serializer::deserialize_transform(const String& p_source) const
{
	Array<int> starts = p_source.find(Char('{'));
	Array<int> ends = p_source.find(Char('}'));

	String pos = p_source.substr(starts[1], ends[0] - starts[1]);
	String size = p_source.substr(starts[2], ends[1] - starts[2]);
	String rot = p_source.substr(starts[3], ends[2] - starts[3]);

	return Transform(deserialize_vec3(pos), deserialize_vec3(size), deserialize_vec3(rot));
}

Object * Serializer::deserialize_object(const String& p_source) const
{
	return nullptr;
}

Array<String> Serializer::deserialize_set(const String& p_value)
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
