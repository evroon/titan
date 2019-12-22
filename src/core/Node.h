#pragma once

#include "types/Scriptable.h"

class Node : public Scriptable
{
	OBJ_DEFINITION(Node, Scriptable);

public:
	Node();
	virtual ~Node();

	virtual void add_child(Node *p_child);
	virtual void remove_child(Node *p_child);

	void clean();

	Node* duplicate();

	Node* get_child_by_index(int p_index);
	Node* get_child(const String& p_name);
		
	template<typename T>
	inline T get_child_by_type()
	{
		for (int c = 0; c < children.size(); c++)
		{
			Node* child = get_child_by_index(c);
			T t = dynamic_cast<T>(child);

			if (t)
				return t;
		}
		return nullptr;
	}

	template<typename T>
	inline T get_parent_by_type_recursively() const
	{
		while (Node * parent = get_parent())
		{
			T p = dynamic_cast<T>(parent);

			if (p)
				return p;
		}
		return nullptr;
	}

	Node* get_parent_by_type_recursively(const String& p_typename) const;

	int get_child_count() const;

	Node* get_parent() const;

	void set_name(const String& p_name);
	String get_name() const;

	int get_index(Node* p_child) const;

	void children_changed();

	static void bind_methods();

protected:
	Node* parent;
	Vector<Node> children;

	String name;


private:
};

