#pragma once

#include "core/variant/Variant.h"
#include "core/Object.h"

#define SERIALIZER Serializer::get_singleton()
#define TYPESERIALIZER TypeSerializer::get_singleton()

struct XmlNode;

class Serializer : public Object
{
	OBJ_DEFINITION(Serializer, Object);

public:
	Serializer();
	~Serializer();

	String serialize(const Variant &p_value);
	void serialize_recursively(const String &p_name, const Variant &p_value, XmlNode &p_node);
	Variant deserialize(const String &p_source);

	Array<String> deserialize_set(const String& p_value);

	static void init();
	static Serializer* get_singleton();

	static void bind_methods();

private:
	static Serializer* singleton;
};

class TypeSerializer : public Object
{
	OBJ_DEFINITION(TypeSerializer, Object);

public:
	TypeSerializer();
	~TypeSerializer();

	String serialize_all_types() const;
	void serialize_type(const VariantType& p_type, XmlNode &p_node) const;

	static void init();
	static TypeSerializer* get_singleton();

	static void bind_methods();

private:
	static TypeSerializer* singleton;
};