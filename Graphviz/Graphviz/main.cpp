#include "Node.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <hash_map>
using namespace std;

int main()
{
	//后面记得清空一下指针
	ofstream dotFile;			//.dot文件
	vector<Node*>  nodeVec;			//存放图中出现的所有节点
	vector<string> tmpIDs;			//存放所有节点的ID
	hash_map<string, Node*> nodeTable;			//将节点的ID与相应的Node组成一张表，便于通过ID查找node
	hash_map<string, Node*>::iterator it;		//用于遍历hash表的迭代器
	dotFile.open("graph.dot");

	string strID;
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
	cin.sync();

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
			if (needReinput == false)
			{
				for (int i = 0; i < tmpTransitions.size(); i++)
					transitionToNode.push_back(nodeTable[tmpTransitions[i]]);
			}
			
		}
		nodeVec[i]->setTransitions(transitionToNode);
		cin.clear();
		cin.sync();
	}

	/************************生成png**************************/
	dotFile << "digraph srcPic{";
	for (int i = 0; i < nodeVec.size(); i++)
	{
		transitionToNode = nodeVec[i]->getTransitions();
		if (transitionToNode.size()>0)
		{
			for (int j = 0; j < transitionToNode.size(); j++)
			{
				dotFile << nodeVec[i]->getID() << "->" << transitionToNode[j]->getID() << " ";
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

	for (int i = 0; i < transitionToNode.size(); i++)
		transitionToNode[i] = NULL;
	nodeVec.clear();

	for (it = nodeTable.begin(); it != nodeTable.end(); ++it)
		(*it).second = NULL;
	nodeTable.clear();

	return 0;
}