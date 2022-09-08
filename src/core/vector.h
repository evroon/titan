#pragma once

#include <algorithm>
#include <vector>

template <class VAL>
class Vector {
   private:
    typedef std::vector<VAL *> V;
    V vec;

   public:
    Vector() {}
    Vector(V v) { vec = v; }

    // Constructor function
    void buildarray(VAL **arr, int size) {
        for (int c = 0; c < size; c++) push_back(arr[c]);
    }

    // Methods
    typename V::iterator begin() { return vec.begin(); }
    typename V::iterator end() { return vec.end(); }
    typename V::const_iterator begin() const { return vec.begin(); }
    typename V::const_iterator end() const { return vec.end(); }

    // Data
    VAL *at(int ind) const { return vec[ind]; }
    VAL *getlast() { return vec[size() - 1]; }
    void set(int ind, VAL *v) { vec[ind] = v; }
    void push_back(VAL *e) { vec.push_back(e); }
    void insert(int pos, VAL *e) { vec.insert(begin() + pos, e); }
    void replace(int index, VAL *e) { vec[index] = e; }
    int size() const { return (int)vec.size(); }
    int getindex(VAL *e) const {
        for (int c = 0; c < size(); c++)
            if (vec[c] == e) return c;

        return -1;
    }

    // Cleaning

    // clear
    void clear(VAL *e) {
        int i = getindex(e);

        if (i != -1) vec.erase(vec.begin() + i);
    }
    void clear(int index) { vec.erase(vec.begin() + index); }
    void clear() { vec.clear(); }
    void removelast() { clear(size() - 1); }

    // clean
    void clean(VAL *element) {
        clear(element);
        delete element;
    }
    void clean_at(int index) {
        VAL *ptr = at(index);
        clean(ptr);
    }
    void clean() {
        delete_all();
        vec.clear();
    }
    void delete_all() {
        for (int c = 0; c < size(); c++) delete vec[c];
    }

    // Helper
    bool contains(VAL v) const {
        for (VAL *e : vec)
            if (v == *e) return true;

        return false;
    }
    bool contains(VAL *v) const {
        for (VAL *e : vec)
            if (v == e) return true;

        return false;
    }
    Vector<VAL> split(int start, int end) const {
        Vector<VAL> result;
        for (int c = start; c <= end; c++) result.push_back(vec[c]);

        return result;
    }
    Vector<VAL> getrest(int start) const { return split(start, size() - 1); }

    // Operators
    VAL *operator[](const int i) const { return vec.at(i); }
    Vector &operator+=(const VAL *e) {
        vec.push_back(e);
        return *this;
    }
    bool operator==(Vector v) const {
        if (size() != v.size()) return false;

        for (int c = 0; c < size(); c++)
            if (v[c] != vec[c]) return false;
        return true;
    }
};
