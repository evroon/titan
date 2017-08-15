#pragma once

#include "resources\Resource.h"

class UBO :	public Resource
{
	OBJ_DEFINITION(UBO, Resource);

public:
	UBO();
	virtual ~UBO();

	void set_data(const Array<float>& p_buffer);
	void update_data(const Array<float>& buffer);

	void bind();

	void bindBufferBase();

	void bindBufferBase(int index);

private:
	unsigned ubo;
	int binding_point_index;
	String bindingName;
};

