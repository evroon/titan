#pragma once

#include "core/Object.h"

class Referenced : public Object
{
	OBJ_DEFINITION(Referenced, Object);

public:
	Referenced();

	void increase_ref_count();
	void decrease_ref_count();


private:
	int ref_count;
};

//T derives from Referenced
template<typename T>
class Ref
{
public:

	inline Ref(T* p_ref)
	{
		referenced = p_ref;
	}

	inline Ref() : Ref(NULL)
	{

	}

	inline ~Ref()
	{
		if (referenced)
		{
			Referenced* ref = reinterpret_cast<Referenced*>(referenced);

			ref->decrease_ref_count();
		}
	}

	inline void ref(const Ref& p_new)
	{

	}

	inline void operator=(const Ref<T>& p_r)
	{
		referenced = p_r.referenced;

		if (!referenced)
			return;

		Referenced* ref = reinterpret_cast<Referenced*>(referenced);

		ref->increase_ref_count();
	}

	inline bool operator<(const Ref<T>& p_r) const
	{
		return referenced < p_r.referenced;
	}

	inline bool operator==(const Ref<T>& p_r) const
	{
		return referenced == p_r.referenced;
	}

	inline bool operator!=(const Ref<T>& p_r) const
	{
		return referenced != p_r.referenced;
	}

	inline T* operator->()
	{
		return referenced;
	}

	inline T* operator*()
	{
		return referenced;
	}

	inline const T* operator->() const
	{
		return referenced;
	}

	inline const T* ptr() const
	{
		return referenced;
	}

	inline T* ptr()
	{
		return referenced;
	}

	inline const T* operator*() const
	{
		return referenced;
	}

	inline operator T*() const
	{
		return referenced;
	}

private:
	T* referenced;
};