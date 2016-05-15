#include "Node.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <hash_map>
using namespace std;

int main()
{
	//后面记得清空一下指针
	ifstream inFile("inputPic.txt");
	ofstream dotFile;			//.dot文件
	vector<Node*>  nodeVec;			//存放图中出现的所有节点
	vector<string> tmpIDs;			//存放所有节点的ID
	hash_map<string, Node*> nodeTable;			//将节点的ID与相应的Node组成一张表，便于通过ID查找node
	hash_map<string, Node*>::iterator it;		//用于遍历hash表的迭代器
	dotFile.open("graph.dot");
	int errNum=0;

	string str;
	int inputType=-1;			//遇到"//"表示切换输入类型，-1表示目前输入的是节点，0表示第0个节点的transition列表，1表示第1个节点的...
	vector<Node*> transitionToNode;
	vector<string> tmpTransitions;
	while (inFile >> str)
	{
		/*cout << "Read from file: " << str << endl;*/
		if (str == "//")
		{
			if (inputType == -1)			//节点列表输入完毕
			{
				for (int i = 0; i<tmpIDs.size(); i++)
				{
					Node* node = new Node();
					node->setID(tmpIDs[i]);
					nodeVec.push_back(node);
					nodeTable[tmpIDs[i]] = node;
				}
			}
			if (inputType>-1 && inputType < nodeVec.size())
			{
				for (int i = 0; i < tmpTransitions.size(); i++)
					transitionToNode.push_back(nodeTable[tmpTransitions[i]]);
				nodeVec[inputType]->setTransitions(transitionToNode);
				transitionToNode.clear();
				tmpTransitions.clear();
			}
			inputType++;
			continue;
		}
		if (inputType == -1)
		{
			if (count(tmpIDs.begin(), tmpIDs.end(), str)>0)
			{
				cout << "There are some repeated Node IDs!\n";
				errNum = 1;
				return errNum;
			}
			else
			{
				tmpIDs.push_back(str);
			}
		}
		
		if (inputType > -1 && inputType < nodeVec.size())
		{
			cout << "The transitions of the Node (" << inputType << "): "<<str<<"\n";
			if (find(tmpIDs.begin(), tmpIDs.end(), str) == tmpIDs.end() || find(tmpTransitions.begin(), tmpTransitions.end(), str) != tmpTransitions.end())
			{
				cout << "There are some invalid transitions or repeated transitions!\n";
				errNum = 2;
				return errNum;
			}
			else
			{
				tmpTransitions.push_back(str);
			}
		}

	}
	cout << "complete reading!\n";

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
	cout << "complete the picture!\n";

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