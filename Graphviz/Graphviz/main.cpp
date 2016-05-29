#include "fstream" 
#include "strstream" 
#include <stdio.h>
#include "Util.h"
using namespace std;

#define orgInputName "orgPic.txt"
#define orgDotName "orgDot.dot"
#define SCCDotName "SCCDot.dot"
#define dotExeDir "D:\\Graphviz2.38\\bin\\dot.exe"
#define orgPNGName "orgPNG.png"
#define SCCPNGName "SCCPNG.png"




void Tarjan(int i, int DFN[], int Low[], int& Index, bool InStack[], int STACK[], int& top, vector<int> Edge[], int& ComponetNumber, vector<int> Component[])
{
	int j;
	DFN[i] = Low[i] = Index++;
	InStack[i] = true;
	STACK[++top] = i;
	for (int e = 0; e<Edge[i].size(); e++)
	{
		j = Edge[i][e];
		if (DFN[j] == -1)
		{
			Tarjan(j, DFN, Low, Index, InStack, STACK, top, Edge, ComponetNumber, Component);
			Low[i] = min(Low[i], Low[j]);
		}
		else if (InStack[j])
			Low[i] = min(Low[i], DFN[j]);
	}
	if (DFN[i] == Low[i])
	{
		/*cout << "TT    " << i << "   " << Low[i] << endl;*/
		ComponetNumber++;
		do
		{
			j = STACK[top--];
			InStack[j] = false;
			Component[ComponetNumber-1].push_back(j);
		} while (j != i);
	}
}


int main()
{
	ifstream inFile(orgInputName);
	string str;
	int nodeCount = 0;              //题目中可能的最大点数

	//************************计算有多少节点************************************
	//遇到"//"表示切换输入类型，-2表示输入节点数，-1表示目前输入的是节点，0表示第0个节点的transition列表，1表示第1个节点的...
	while (inFile >> str)		//由于这里输入0到nodeCount的数字，所以取消对nodeID的检查
	{
		if (str == "//")
			break;
		else
			nodeCount++;
	}
	//根据节点数量声明的一些变量
	int* STACK = new int[nodeCount];		//Tarjan 算法中的栈 
	int top = -1;
	bool* InStack = new bool[nodeCount];		 //检查是否在栈中 
	int* DFN = new int[nodeCount];		//深度优先搜索访问次序 
	int* Low = new int[nodeCount];		 //能追溯到的最早的次序 
	int ComponetNumber = 0;       //有向图强连通分量个数 
	int Index = 0;					 //索引号 
	vector<int>* Edge = new vector <int>[nodeCount];		//邻接表表示 
	vector<int>* Component = new vector<int>[nodeCount];		//获得强连通分量结果
	
	int nodeIndex = 0;
	while (inFile >> str)
	{
		if (str == "//"){
			nodeIndex++;
			continue;
		}
		int n = atoi(str.c_str());
		if (nodeIndex > -1 && nodeIndex < nodeCount)
		{
			Edge[nodeIndex].push_back(n);
		}
	}
	cout << "complete reading the original picture!\n";

	/*************************生成原图**********************************/
	cout << endl;
	cout << "生成原图..." << endl;
	Util::generatePNG(orgInputName, orgDotName, dotExeDir, orgPNGName, Component, nodeCount, 0);

	/******************************输出强连通分量***************************************/
	cout << endl;
	cout << "计算强连通分量..." << endl;
	for (int i = 0; i < nodeCount; i++)
	{
		STACK[i] = -1;
		InStack[i] = false;
		DFN[i] = -1;
		Low[i] = -1;
	}
	for (int i = 0; i<nodeCount; i++)
	 if (DFN[i] == -1)
		Tarjan(i,DFN, Low,Index, InStack,STACK,top,Edge,ComponetNumber,Component);
	cout << "ComponetNumber is " << ComponetNumber << endl;

	int maxSize = 0;
	vector<int> maxComponent;
	for (int i = 0; i<nodeCount; i++)
	{
		if (Component[i].size() > 0 )
		{
			if (Component[i].size() >= maxSize)
				maxSize = Component[i].size();
		}
	}
	for (int i = 0; i < nodeCount; i++)
	{
		if (Component[i].size() == maxSize)
			maxComponent.push_back(i);
	}
	cout << "Largest component:" << endl;
	while (maxComponent.empty() == false)
	{
		int i = maxComponent.back();
		maxComponent.pop_back();
		for (int j = 0; j < Component[i].size(); j++)
			cout << Component[i][j] << " ";
		cout << endl;
	}

	/*****************************输出强连通图片*****************************************/
	cout << endl;
	cout << "输出强连通分量图..." << endl;
	Util::generatePNG(orgInputName, SCCDotName, dotExeDir, SCCPNGName, Component, nodeCount, ComponetNumber);

	string pngOrg = orgPNGName;
	const char* png1 = pngOrg.c_str();
	string pngSCC = SCCPNGName;
	const char* png2 = pngSCC.c_str();

	system(png1);
	system(png2);
	return 0;
}