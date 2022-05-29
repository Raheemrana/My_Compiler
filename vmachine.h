#ifndef _VMACHINE_H_
#define _VMACHINE_H_
#include <fstream>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

class vmachine{
    vector<char> _stream; 
    int ** quad;
    int * ds;
    int NoOFLines = 0;
    int quadCount;
    unordered_map<string, int> opCodes;
    unordered_map<string, int> symbols;
    ofstream stringfileWriter;
    int stringfileCounter;
public:
    vmachine();
    vmachine(const char filename[]);
    void execute();
    void MakeMachinecodes();
    void MakeOPcodes();
    void AddintoQuad(int a, int b, int c , int d , int e);
    void MakeSymbolHash();
    void PrintQuads();
};
#endif