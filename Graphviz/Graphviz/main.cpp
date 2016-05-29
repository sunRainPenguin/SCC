#include "fstream" 
#include "strstream" 
#include <stdio.h>
#include "Util.h"
using namespace std;

#define orgInputName "orgPic.txt"
#define orgDotName "orgDot.dot"
#define SCCDotName "SCCDot.dot"
#define dotExeDir "F:\\Graphviz2.38\\bin\\dot.exe"
#define orgPNGName "orgPNG.png"
#define SCCPNGName "SCCPNG.png"


#define  nodeCount 4              //题目中可能的最大点数       
int STACK[nodeCount], top = 0;          //Tarjan 算法中的栈 
bool InStack[nodeCount];             //检查是否在栈中 
int DFN[nodeCount];                  //深度优先搜索访问次序 
int Low[nodeCount];                  //能追溯到的最早的次序 
int ComponetNumber = 0;        //有向图强连通分量个数 
int Index = 0;                 //索引号 
vector <int> Edge[nodeCount];        //邻接表表示 
vector <int> Component[nodeCount];   //获得强连通分量结果

void Tarjan(int i)
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
			Tarjan(j);
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
			Component[ComponetNumber].push_back(j);
		} while (j != i);
	}
}

void solve(int N)     //此图中点的个数，注意是0-indexed！ 
{
	memset(STACK, -1, sizeof(STACK));
	memset(InStack, 0, sizeof(InStack));
	memset(DFN, -1, sizeof(DFN));
	memset(Low, -1, sizeof(Low));
	for (int i = 0; i<N; i++)
	if (DFN[i] == -1)
		Tarjan(i);
}


int main()
{
	ifstream inFile(orgInputName);
	string str;

	int nodeIndex = -1;		//遇到"//"表示切换输入类型，-2表示输入节点数，-1表示目前输入的是节点，0表示第0个节点的transition列表，1表示第1个节点的...
	while (inFile >> str)		//由于这里输入0到nodeCount的数字，所以取消对nodeID的检查
	{
		if (str != "//")
			continue;
		else
		{
			nodeIndex++;
			break;
		}
	}
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
	solve(nodeCount);
	cout << "ComponetNumber is " << ComponetNumber << endl;

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