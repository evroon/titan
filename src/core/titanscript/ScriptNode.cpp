#include "ScriptNode.h"

#include "core/NodeManager.h"

ScriptNode::ScriptNode()
{
	NodeManager::AddNode(this);
}