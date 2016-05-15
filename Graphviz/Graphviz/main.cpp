#include "Node.h"
#include <windows.h>
#include <iostream>
#include <fstream>
#include <hash_map>
using namespace std;

int main()
{
	//����ǵ����һ��ָ��
	ifstream inFile("inputPic.txt");
	ofstream dotFile;			//.dot�ļ�
	vector<Node*>  nodeVec;			//���ͼ�г��ֵ����нڵ�
	vector<string> tmpIDs;			//������нڵ��ID
	hash_map<string, Node*> nodeTable;			//���ڵ��ID����Ӧ��Node���һ�ű�����ͨ��ID����node
	hash_map<string, Node*>::iterator it;		//���ڱ���hash��ĵ�����
	dotFile.open("graph.dot");
	int errNum=0;

	string str;
	int inputType=-1;			//����"//"��ʾ�л��������ͣ�-1��ʾĿǰ������ǽڵ㣬0��ʾ��0���ڵ��transition�б�1��ʾ��1���ڵ��...
	vector<Node*> transitionToNode;
	vector<string> tmpTransitions;
	while (inFile >> str)
	{
		/*cout << "Read from file: " << str << endl;*/
		if (str == "//")
		{
			if (inputType == -1)			//�ڵ��б��������
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