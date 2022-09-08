#include "scriptnode.h"

#include "core/nodemanager.h"

ScriptNode::ScriptNode()
{
	NodeManager::AddNode(this);
}
