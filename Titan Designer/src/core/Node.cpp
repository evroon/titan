#include "Node.h"

#include "core/Memory.h"

Node::Node()
{

}

Node::~Node()
{
	for (int c = 0; c < children.size(); c++)
	{
		remove_child(children[c]);
	}
}

void Node::add_child(Node* p_child)
{
	if (!p_child)
		return;

	if (p_child->name == "")
		p_child->name = p_child->get_type_name();

	children.push_back(p_child);

	p_child->parent = this;
	p_child->emit_signal("parent_changed");
}

void Node::remove_child(Node* p_child)
{
	p_child->clean();
	GC->queue_clean(p_child);
	children.clear(p_child);
	children_changed();
}

void Node::clean()
{
	int size = children.size();
	for (int c = 0; c < size; c++)
	{
		Node* child = get_child_by_index(0);
		GC->queue_clean(child);
		children.clear(child);
	}
}

Node* Node::get_child_by_index(int p_index)
{
	return children[p_index];
}

Node* Node::get_child(const String& p_name)
{
	for (int c = 0; c < children.size(); c++)
	{
		if (children[c]->name == p_name)
			return children[c];

		Node* n = children[c]->get_child(p_name);
		if (n)
			return n;
	}

	return NULL;
}

Node* Node::get_parent_by_type_recursively(const String& p_typename) const
{
	while (Node* parent = get_parent())
	{
		if (parent->get_type_name() == p_typename)
			return parent;
	}
	return nullptr;
}

int Node::get_child_count() const
{
	return children.size();
}

Node* Node::get_parent() const
{
	return parent;
}

void Node::set_name(const String& p_name)
{
	name = p_name;
}

String Node::get_name() const
{
	return name;
}

int Node::get_index(Node* p_child) const
{
	return children.getindex(p_child);
}

void Node::children_changed()
{
	emit_signal("children_changed");

	if (parent)
		parent->children_changed();
}

#undef CLASSNAME
#define CLASSNAME Node

void Node::bind_methods()
{
	REG_PROPERTY(name);
	REG_METHOD(add_child);
	REG_METHOD(get_child);
	REG_METHOD(get_parent);

	REG_SIGNAL("children_changed");
	REG_SIGNAL("parent_changed");
}
