#pragma once

#include "propertycontrol.h"

#include "container.h"
#include "numberfield.h"

#include "core/string.h"

class Vec2Field : public PropertyControl
{
	OBJ_DEFINITION(Vec2Field, PropertyControl);

public:
	Vec2Field();
	Vec2Field(const vec2& p_variant);
	Vec2Field(const Variable& p_variable);

	void init();

	void value_changed() override;
	void value_input() override;

	vec2 get_required_size() const override;

	void position_items();

	static void bind_methods();

private:
	Container container;
	NumberField x, y;

	float margin[3];
};

class Vec3Field : public PropertyControl
{
	OBJ_DEFINITION(Vec3Field, PropertyControl);

public:
	Vec3Field();
	Vec3Field(const vec3& p_variant);
	Vec3Field(const Variable& p_variable);

	void init();

	void value_changed() override;
	void value_input() override;

	vec2 get_required_size() const override;

	void position_items();

	static void bind_methods();

private:
	Container container;
	NumberField x, y, z;

	float margin[4];
};

class Vec4Field : public PropertyControl
{
	OBJ_DEFINITION(Vec4Field, PropertyControl);

public:
	Vec4Field();
	Vec4Field(const vec4& p_variant);
	Vec4Field(const Variable& p_variable);

	void init();

	void value_changed() override;
	void value_input() override;

	vec2 get_required_size() const override;

	void position_items();

	static void bind_methods();

private:
	Container container;
	NumberField x, y, z, w;

	float margin[5];
};

class TransformField : public PropertyControl
{
	OBJ_DEFINITION(TransformField, PropertyControl);

public:
	TransformField();
	TransformField(const Transform& p_variant);
	TransformField(const Variable& p_variable);

	void init();

	void value_changed() override;
	void value_input() override;

	vec2 get_required_size() const override;

	void position_items();

	static void bind_methods();

private:
	Vec3Field pos;
	Vec3Field size;
	Vec3Field rotation;

	float margin[4];
};