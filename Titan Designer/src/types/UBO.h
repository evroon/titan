#pragma once

#include "resources\Resource.h"

class UBO :	public Resource
{
	OBJ_DEFINITION(UBO, Resource);

public:
	UBO();
	virtual ~UBO();

	void set_data(const Array<float>& p_buffer);
	void set_data(const Array<mat4>& p_buffer);

	void update_data(const Array<float>& p_buffer);
	void update_data(const Array<mat4>& p_buffer);

	void bind();

	void bind_at_index(int p_index);
	int get_bound_index() const;

private:
	void init();

	unsigned ubo;
	unsigned bound_index;

	float* data;
	int data_size;
};