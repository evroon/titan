#pragma once

#include <map>
#include <algorithm>

template<class KEY, class VAL>
class Dictionary
{
private:
	typedef std::map<KEY, VAL> M;
	M mymap;

public:
	Dictionary() = default;
	Dictionary(M v) { mymap = v; }
	~Dictionary() { clear(); }

	//Methods
	typename M::iterator begin() { return mymap.begin(); }
	typename M::iterator end() { return mymap.end(); }

	void clear() { mymap.clear(); }
	void clear(const KEY &k) { mymap.erase(k); }
	void clean(const KEY &k) { delete mymap[k]; }
	int size() const { return mymap.size(); }

	//Cleaning
	bool contains(const KEY &k) const
	{
		if (mymap.count(k) > 0)
			return true;

		return false;
	}
	bool contains(const VAL &v) const
	{
		for (VAL &e : mymap)
			if (v == e)
				return true;

		return false;
	}

	VAL set(const KEY &key, const VAL &val)
	{
		mymap[key] = val;
		return val;
	}

	VAL& get(const KEY &key)
	{
		return mymap[key];
	}

	//Operators
	const VAL& operator[](const KEY &key) const
	{
		return mymap[key];
	}
	VAL& operator[](const KEY &key)
	{
		return mymap[key];
	}
};