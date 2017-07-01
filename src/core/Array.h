#pragma once

#include <vector>
#include <algorithm>

template<class VAL> class Array
{
private:
	typedef std::vector<VAL> V;
	V vec;

public:
	Array() { }
	Array(const VAL &v_0)
	{
		push_back(v_0);
	}
	Array(const VAL &v_0, const VAL &v_1)
	{
		push_back(v_0);
		push_back(v_1);
	}
	Array(const VAL &v_0, const VAL &v_1, const VAL &v_2)
	{
		push_back(v_0);
		push_back(v_1);
		push_back(v_2);
	}
	Array(const VAL &v_0, const VAL &v_1, const VAL &v_2, const VAL &v_3)
	{
		push_back(v_0);
		push_back(v_1);
		push_back(v_2);
		push_back(v_3);
	}
	Array(const VAL &v_0, const VAL &v_1, const VAL &v_2, const VAL &v_3, const VAL &v_4)
	{
		push_back(v_0);
		push_back(v_1);
		push_back(v_2);
		push_back(v_3);
		push_back(v_4);
	}
	Array(const VAL &v_0, const VAL &v_1, const VAL &v_2, const VAL &v_3, const VAL &v_4, const VAL &v_5)
	{
		push_back(v_0);
		push_back(v_1);
		push_back(v_2);
		push_back(v_3);
		push_back(v_4);
		push_back(v_5);
	}

	//Constructor function
	void buildarray(VAL *arr, int size)
	{
		for (int c = 0; c < size; c++)
			push_back(arr[c]);
	}

	//Constructor function
	static Array<VAL> build_array(VAL *arr, int size)
	{
		Array<VAL> result;
		for (int c = 0; c < size; c++)
			result.push_back(arr[c]);

		return result;
	}

	//Methods
	typename V::iterator begin() { return vec.begin(); }
	typename V::iterator end() { return vec.end(); }

	//Data
	VAL& at(int ind) const { return vec[ind]; }
	VAL& get(int ind) { return vec[ind]; }
	VAL& getlast() { return vec[size() - 1]; }
	void set(int ind, const VAL &v) { vec[ind] = v; }
	void push_back(const VAL &e) { vec.push_back(e); }
	void push_back_ref(const VAL &e) { vec.push_back(e); }
	void push_back(const Array<VAL> &a) { for (int c = 0; c < a.size(); c++) push_back(a[c]); }
	void push_backref(const VAL &e) { vec.push_back(e); }
	void emplace(const VAL &e) { vec.emplace(vec.begin(), e); }
	void replace(int index, const VAL &e) { vec[index] = e; }
	void insert(int index, const VAL &e) { vec.insert(begin() + index, e); }
	int size() const { return static_cast<int>(vec.size()); }
	void reserve(int p_size) { vec.reserve(p_size); }

	//Cleaning
	void clear(int index) { vec.erase(vec.begin() + index); }
	void clear() { vec.clear(); }
	void removelast() { clear(size() - 1); }

	//Helper
	bool contains(const VAL &v) const
	{
		for (VAL e : vec)
			if (v == e)
				return true;
		return false;
	}
	Array<VAL> split(const int start, const int end) const
	{
		Array<VAL> result;
		for (int c = start; c <= end; c++)
			result.push_back(vec[c]);

		return result;
	}
	Array<VAL> getrest(const int start) const
	{
		return split(start, size() - 1);
	}

	//Operators
	VAL& operator[](int i)
	{
		return vec[i];
	}
	const VAL& operator[](const int i) const
	{
		return vec[i];
	}
	Array& operator+=(const VAL &e)
	{
		vec.push_back(e);
		return *this;
	}
	bool operator==(const Array<VAL> &v) const
	{
		if (size() != v.size())
			return false;

		for (int c = 0; c < size(); c++)
			if (v[c] != vec[c])
				return false;

		return true;
	}
};