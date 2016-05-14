#include "Node.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <hash_map>
using namespace std;

int main()
{
	//后面记得清空一下指针
	ofstream dotFile;
	vector<Node*>  nodeVec;
	hash_map<string, Node*> nodeTable;	
	hash_map<string, Node*>::iterator it;
	dotFile.open("graph.dot");

	string strID;
	vector<string> tmpIDs;

	bool needReinput = true;
	while (needReinput)
	{
		needReinput = false;
		tmpIDs.clear();
		cout << "Please enter the nodes IDs and split the strings with spaces (e.g: '2 3 4 a'):\n";
		while (cin >> strID)
		{
			if (count(tmpIDs.begin(), tmpIDs.end(), strID)>0)
			{
				needReinput = true;
				cout << "There are some repeated Node IDs!\n";
				break;
			}
			tmpIDs.push_back(strID);
		}

	}
	cin.clear();

	for (int i = 0; i<tmpIDs.size(); i++)
	{
		Node* node = new Node();
		node->setID(tmpIDs[i]);
		nodeVec.push_back(node);
		nodeTable[tmpIDs[i]] = node;
	}
	
	vector<Node*> transitionToNode;
	string strTransition;
	vector<string> tmpTransitions;
	for (int i = 0; i < nodeVec.size(); i++)
	{		
		needReinput = true;
		while (needReinput)
		{
			needReinput = false;
			transitionToNode.clear();
			tmpTransitions.clear();
			cout << "Please enter transitions of the Node (" << i << ") (e.g: '3 5 a'):\n";
			while (cin >> strTransition)
			{
				if (find(tmpIDs.begin(), tmpIDs.end(), strTransition) == tmpIDs.end() || find(tmpTransitions.begin(), tmpTransitions.end(), strTransition) != tmpTransitions.end())
				{
					needReinput = true;
					cout << "There are some invalid Node IDs or repeated IDs!\n";
					cin.clear();		//检测到第一次输入错误，就清空该节点的所有transition节点
					cin.sync();	// 清除 cin 缓冲区未读取信息
					break;
				}
				tmpTransitions.push_back(strTransition);
			}
			for (int i = 0; i < tmpTransitions.size(); i++)
				transitionToNode.push_back(nodeTable[tmpTransitions[i]]);

		}
		nodeVec[i]->setTransitions(transitionToNode);
		cin.clear();
		cin.sync();
	}

	dotFile << "digraph srcPic{";
	vector<Node*> tmpTransions;
	for (int i = 0; i < nodeVec.size(); i++)
	{
		tmpTransions = nodeVec[i]->getTransitions();
		if (tmpTransions.size()>0)
		{
			for (int j = 0; j < tmpTransions.size(); j++)
			{
				dotFile << nodeVec[i]->getID() << "->" << tmpTransions[j]->getID() << " ";
			}
		}
		else
		{
			dotFile << nodeVec[i]->getID() << " ";
		}
	}
	dotFile << "}";
	dotFile.flush();
	dotFile.close();

	char cmdExplore[] = "D:";
	char cmdExe[] = "D:\\Graphviz2.38\\bin\\dot.exe -Tpng -o graph.png graph.dot";
	WinExec(cmdExplore, SW_NORMAL);
	WinExec(cmdExe, SW_NORMAL);

	//clear------------------------------------
	for (int i = 0; i < nodeVec.size(); i++)
	{
		delete nodeVec[i];
		nodeVec[i] = NULL;
	}
	nodeVec.clear();

	for (int i = 0; i < tmpTransions.size(); i++)
		tmpTransions[i] = NULL;
	nodeVec.clear();

	for (it = nodeTable.begin(); it != nodeTable.end(); ++it)
		(*it).second = NULL;
	nodeTable.clear();

	return 0;
}