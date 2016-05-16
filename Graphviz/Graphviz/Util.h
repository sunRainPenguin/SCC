#pragma once
#include "Node.h"
#include <string>
#include <windows.h>
#include <iostream>
#include <fstream>
#include <hash_map>
#include <sstream>
using namespace std;

class Util
{
public:
	Util();
	~Util();
	static int generatePNG(string picFileName, string dotFileName, string exeDir, string PNGName, vector <int> Component[], int nodeCount, int componentCount);
	static int RGBTo16(int r, int g, int b);
};

