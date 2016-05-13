#include "Node.h"


Node::Node()
{
	ID = "";
	transitions.empty();
}


Node::~Node()
{

}

string Node::getID()
{
	return ID;
}
vector<Node*> Node::getTransitions()
{
	return transitions;
}
void Node::setID(string tmpID)
{
	ID = tmpID;
}
void Node::setTransitions(vector<Node*> nodeIDs)
{
	transitions = nodeIDs;
}