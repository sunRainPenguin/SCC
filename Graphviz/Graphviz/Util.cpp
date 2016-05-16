#include "Util.h"


Util::Util()
{
}


Util::~Util()
{
}

int Util::generatePNG(string picFileName, string dotFileName, string exeDir, string PNGName, vector <int> Component[], int nodeCount, int componentCount)
{
	//����ǵ����һ��ָ��
	ifstream inFile(picFileName);			//"inputPic.txt"
	ofstream dotFile;			//.dot�ļ�
	vector<Node*>  nodeVec;			//���ͼ�г��ֵ����нڵ�
	vector<string> tmpIDs;			//������нڵ��ID
	hash_map<string, Node*> nodeTable;			//���ڵ��ID����Ӧ��Node���һ�ű�����ͨ��ID����node
	hash_map<string, Node*>::iterator it;		//���ڱ���hash��ĵ�����
	dotFile.open(dotFileName);			//"graph.dot"
	int errNum = 0;

	//init color
	int Color[3] = { 255, 255, 255 };
	int colorIndex = 0;
	int currColor[3] = { 255, 255, 255 };

	string str;
	int inputType = -1;			//����"//"��ʾ�л��������ͣ�-1��ʾĿǰ������ǽڵ㣬0��ʾ��0���ڵ��transition�б�1��ʾ��1���ڵ��...
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
			cout << "The transitions of the Node (" << inputType << "): " << str << "\n";
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
	dotFile << "digraph srcPic{"<<endl;
	for (int i = 0; i < nodeVec.size(); i++)
	{
		transitionToNode = nodeVec[i]->getTransitions();
		if (transitionToNode.size()>0)
		{
			for (int j = 0; j < transitionToNode.size(); j++)
			{
				dotFile << nodeVec[i]->getID() << "->" << transitionToNode[j]->getID() << " "<<endl;
			}
		}
		else
		{
			dotFile << nodeVec[i]->getID() << " "<<endl;
		}
	}

	int componentIndex = 0;
	if (componentCount > 0)
	{
		for (int i = 0; i<nodeCount; i++)
		{
			if (Component[i].size() > 0)
			{
				Color[componentIndex] = Color[componentIndex] - 30;
				for (int j = 0; j < Component[i].size(); j++)
				{
					cout << Component[i][j] << " ";
					for (int p = 0; p < 3; p++)
					{
						if (p == componentIndex)
							currColor[p] = Color[componentIndex];
						else
							currColor[p] = 255;
					}

					int color16 = RGBTo16(currColor[0], currColor[1], currColor[2]);
					dotFile << Component[i][j] << "[ fillcolor=\"#" << hex << color16 << "\",style=filled]" << endl;
				}
				componentIndex = (componentIndex + 1) % 3;
			}
			cout << endl;
		}
	}
	
	dotFile << "}";
	dotFile.flush();
	dotFile.close();

	string root = exeDir.substr(0,2);
	const char *cmdExplore = root.c_str();
	string cmdStr = exeDir + " -Tpng -o " + PNGName + " " + dotFileName;
	const char *cmdExe = cmdStr.c_str();

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

int Util::RGBTo16(int r, int g, int b)
{
	return r << 16 | g << 8 | b;
}
