#ifndef _STDAFX_H
#define _STDAFX_H
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <stack>
#include <assert.h>
#include <iostream>
#include <sstream>
#include <set>
#include <ctime>
#include <queue>
#include <functional>
#include <algorithm>
#include <random>
#include <list>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdint.h>
#include <bitset>
#include <stack>

#ifdef WIN32
#else
#include <sys/time.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#define  _mkdir mkdir
#define  _access access
#define  ASSERT assert
#define _stati64 stat64
#define _fseeki64 fseeko
#define _FILE_OFFSET_BITS 64  
#endif

typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;

static int INSERTED = 253;
static int DELETED = 254;
static int UNMAPPED = 255;

typedef struct verifyGraphNode
{
	u8 verifyGraphNodeID;
	u8 verifyGraphNodeStr;
	verifyGraphNode(){}
	verifyGraphNode(const verifyGraphNode &t)
	{
		*this = t;
	}
	verifyGraphNode & operator= (const verifyGraphNode &t)
	{
		if (this != &t)
		{
			this->verifyGraphNodeID = t.verifyGraphNodeID;
			this->verifyGraphNodeStr = t.verifyGraphNodeStr;
		}
		return *this;
	}
};

const std::string AIDS_Query = "d:\\data\\query\\AIDS";
const std::string AIDSH_Query = "d:\\data\\query\\HAIDS";
const std::string PROTEINS = "d:\\data\\database\\proteins";
const std::string AIDS = "d:\\data\\database\\AIDS";
const std::string AIDSH = "d:\\data\\database\\AIDSH";
const std::string DB = "E:\\project\\pattern regonitier letter\\code\\test_data\\AIDS_db";
const std::string Query = "E:\\project\\pattern regonitier letter\\code\\test_data\\AIDS_query";
const std::string OFFSET = "";
const std::string test_db = "./AIDS-10K";
const std::string CSI_AIDS_DB = "d:\\data\\database\\CSI_GED\\AIDS_10K";
const std::string CSI_AIDS_Query = "d:\\data\\database\\CSI_GED\\AIDS_query";
const std::string AIDS_1K = "D:\\data\\HAIDS_1K"; 
const std::string AIDS_1K_18 = "D:\\data\\HAIDS_1K_18";


using namespace std;
#endif
