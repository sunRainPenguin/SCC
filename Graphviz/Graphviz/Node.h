#pragma once
#include <string>
#include <vector>
using namespace std;

class Node
{
private:
	string ID;
	vector<Node*> transitions;

public:
	Node();
	~Node();
	string getID();
	vector<Node*> getTransitions();
	void setID(string tmpID);
	void setTransitions(vector<Node*> nodeIDs);
};

