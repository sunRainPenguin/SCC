#include "Node.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <hash_map>
using namespace std;

int main()
{
	//����ǵ����һ��ָ��
	ofstream dotFile;			//.dot�ļ�
	vector<Node*>  nodeVec;			//���ͼ�г��ֵ����нڵ�
	vector<string> tmpIDs;			//������нڵ��ID
	hash_map<string, Node*> nodeTable;			//���ڵ��ID����Ӧ��Node���һ�ű�����ͨ��ID����node
	hash_map<string, Node*>::iterator it;		//���ڱ���hash��ĵ�����
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
					cin.clear();		//��⵽��һ��������󣬾���ոýڵ������transition�ڵ�
					cin.sync();	// ��� cin ������δ��ȡ��Ϣ
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

	/************************����png**************************/
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