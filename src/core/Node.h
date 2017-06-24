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

	Node* get_child(int p_index);
	Node* get_child(const String& p_name);

	int get_child_count() const;

	Node* get_parent() const;

	void set_name(const String& p_name);
	String get_name() const;

	int get_index(Node* p_child) const;

protected:
	Node* parent;
	Vector<Node> children;

	String name;


private:
};

