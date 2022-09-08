#pragma once

#include <algorithm>
#include <map>

template <class KEY, class VAL>
class Map {
   private:
    typedef std::map<KEY, VAL *> M;
    M map;

   public:
    Map() = default;
    Map(M p_val) { map = p_val; }

    // Methods
    typename M::iterator begin() { return map.begin(); }
    typename M::iterator end() { return map.end(); }

    void clear() { map.clear(); }

    void clear(const KEY &p_key) { map.erase(p_key); }
    void clean(const KEY &p_key) { delete map[p_key]; }
    int size() const { return static_cast<int>(map.size()); }

    // Cleaning
    int count(const KEY &p_key) { return static_cast<int>(map.count(p_key)); }
    void clean() {
        for_each(
            map.begin(), map.end(),
            [](std::pair<const KEY &, VAL *> p_pair) { delete p_pair.second; });

        map.clear();
    }
    bool contains(const KEY &p_key) const {
        if (map.count(p_key) > 0) return true;

        return false;
    }
    bool contains(VAL *p_val) const {
        for (VAL *e : map)
            if (p_val == e) return true;

        return false;
    }

    VAL *set(const KEY &p_key, VAL *p_val) {
        map[p_key] = p_val;
        return p_val;
    }

    VAL *get(const KEY &p_key) const { return map.at(p_key); }

    // Operators
    VAL *&operator[](const KEY &p_key) { return map.at(p_key); }
    VAL *operator[](const KEY &p_key) const { return map.at(p_key); }
};

#if 0

template<class VAL> class IndexedMap
{
private:
	typedef std::map<int, VAL*> M;
	M mymap;
	int nextid = 0;

public:
	IndexedMap() = default;
	IndexedMap(M v) { mymap = v; }

	//Methods
	typename M::iterator begin() { return mymap.begin(); }
	typename M::iterator end() { return mymap.end(); }

	void push_back(VAL * e) { mymap[nextid++] = e; }
	void erase(int index) { delete mymap[index]; mymap.erase(index); }
	void remove(int index) { mymap.erase(index); }
	int getnext() const { return nextid; }
	int size() const { return mymap.size(); }
	void clean()
	{
		for_each(mymap.begin(), mymap.end(), [](pair<int, VAL*> p) { delete p.second; });
		mymap.clear();
	}
	bool contains(int k) { return mymap.count(k) > 0; }
	bool contains(VAL* v)
	{
		for (VAL* e : mymap) if (v == e) return true;
		return false;
	}

	//Operators
	VAL* operator[](int key) { return mymap.at(key); }
};

#endif
