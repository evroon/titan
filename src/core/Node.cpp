#include "Node.h"

#include "Memory.h"

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
	children.push_back(p_child);

	p_child->parent = this;
}

void Node::remove_child(Node* p_child)
{
	GC->queue_clean(p_child);
	children.clear(p_child);
}

void Node::clean()
{
	int size = children.size();
	for (int c = 0; c < size; c++)
	{
		Node* child = get_child(0);
		GC->queue_clean(child);
		children.clear(child);
	}
}

Node* Node::get_child(int p_index)
{
	return children[p_index];
}

Node* Node::get_child(const String& p_name)
{
	for (int c = 0; c < children.size(); c++)
	{
		if (children[c]->name == p_name)
			return children[c];
	}

	return NULL;
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

int Node::get_index(Node * p_child) const
{
	return children.getindex(p_child);
}
