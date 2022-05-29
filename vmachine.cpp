#include "vmachine.h"
#include <string.h>
#include <fstream>
using namespace std;
string err = "Something odd noticed :( \n";

//quad has 5 columns
// i <- a + b
// opcode , operand 1 , operand 2 , destination , extra bit
// extra bit 0 -> both operands are addresses
// 1 -> first operand is address second is immediate value
// 2 -> first is immediate value second operand is address
// 3 -> both are immediate values
// i <- a
// opcode , operand 1 , destination , extra bit
// extra bit 0 -> immediate value 
// 1 -> address operand

vmachine::vmachine(){
    cout << "constructor";
}
vmachine::vmachine(const char filename[])
{   
    char ch;
    fstream fin(filename, fstream::in);
    if(fin.is_open())
    {
        while (fin >> noskipws >> ch) {
            if(ch == ';')
            {
                NoOFLines++;
            }
            _stream.push_back(ch);
        }
        fin.close();
        stringfileWriter.open("Stringfile.txt");
        stringfileCounter = 0;
        quadCount = 0;
        MakeSymbolHash();
        MakeOPcodes();
        MakeMachinecodes();
    }
    else{
        cout << "File not found\n;";
        return;
    }
    

}
void vmachine::MakeOPcodes(){
    opCodes["+"] = 1;
    opCodes["-"] = 2;
    opCodes["*"] = 3;
    opCodes["/"] = 4;
    opCodes["%"] = 5;
    opCodes["if"] = 6;
    opCodes["goto"] = 7;
    opCodes["in"] = 8;
    opCodes["out"] = 9;
    opCodes["ret"] = 10;
    opCodes[">"] = 11;
    opCodes["<"] = 12;
    opCodes[">="] = 13;
    opCodes["<="] = 14;
    opCodes["="] = 15;
    opCodes["=="] = 16;
    opCodes["elif"] = 17;
}
void vmachine::AddintoQuad(int a, int b, int c , int d , int e){

    quad[quadCount][0] = a;
    quad[quadCount][1] = b;
    quad[quadCount][2] = c;
    quad[quadCount][3] = d;
    quad[quadCount][4] = e;
    quadCount++;
}
void vmachine::MakeMachinecodes(){ 
    quad = new int*[NoOFLines];
    for (int i = 0; i < NoOFLines; i++){
        quad[i] = new int[5];
    }
    string buff = "";

    string tac( _stream.begin(), _stream.end());
    int it = 0;
    try{

    while (it < tac.length())
    {
        string buff = "";
        string value = "";
        while (tac[it] != ' ')
        {
            buff += tac[it];
            it += 1;
        }
        // if it is an id
        unordered_map<string, int>:: iterator word = opCodes.find(buff);
        if(word == opCodes.end() && tac[it+1] == '=')
        {
            it++;
            it++;
            it++;
            while (tac[it] != '\n')
            {
                value += tac[it];
                it += 1;
            }
            
            bool dual = false;
            // check if intilization has 2 operands
            for (int i = 0 ; i < value.length() && (!dual); i++)
            {
                if (value[i] == ' '){
                    dual = true;
                }
            }
            if(dual == false)
            {
                value = value.substr(0, value.size()-1);
                unordered_map<string, int>:: iterator itr = symbols.find(value);
                if (itr == symbols.end())
                {
                    try{
                         // 0(4) for immediate value
                    AddintoQuad(opCodes.find("=")->second, stoi(value),symbols.find(buff)->second, 0 , -1 );
                    }
                    catch (...) {
                        cout << err;
                        exit(1);
                    }
                    
                }
                else{
                    // 1(4) for variable
                    AddintoQuad(opCodes.find("=")->second, itr->second,symbols.find(buff)->second, 1 , -1 );
                }
            }
            else
            {
                string op1 = "";
                int o1;
                string op2 = "";
                int o2;
                string operatorr = "";
                int j = 0;
                while(value[j] != ' '){
                    op1 += value[j];
                    j++;
                }
                j++;
                while(value[j] != ' '){
                    operatorr += value[j];
                    j++;
                }
                j++;
                while(value[j] != ';'){
                    op2 += value[j];
                    j++;
                }
                unordered_map<string, int>:: iterator itr_;
                itr_ = symbols.find(op1);
                int extra = 0;
                if(itr_ != symbols.end())
                {
                    o1 = itr_->second;
                }
                else{
                    extra = 2;
                    try{
                    o1 = stoi(op1);
                    }
                    catch(...){
                        cout << err;
                        exit(1);
                    }
                }
                itr_ = symbols.find(op2);
                if(itr_ != symbols.end())
                {
                    o2 = itr_->second;
                }
                else{
                    if(extra == 2)
                    {
                        extra = 3;
                    }
                    else{
                    extra = 1;
                    }
                    try{
                    o2 = stoi(op2);
                    }
                    catch(...){
                        cout << err;
                        exit(1);
                    }
                }
                AddintoQuad(opCodes.find(operatorr)->second,o1,o2,symbols.find(buff)->second,
                extra);

            }
        }
        else if(strcmp(word->first.c_str(),"out") == 0)
        {
            it++;
            if (tac.at(it) == '"')
            {
                it++;
                while (tac.at(it) != '"')
                {
                    value += tac[it];
                    it += 1;
                }
                it += 2;
                stringfileWriter << value << ";" << endl;
                //1(3) to indicate string
                AddintoQuad(word->second, stringfileCounter, 1, -1, -1);
                stringfileCounter++;
            }
            else{
                while (tac.at(it) != ';')
                {
                    value += tac[it];
                    it += 1;
                }
                it++;
                 //0(3) to indicate variable
                AddintoQuad(word->second, symbols.find(value)->second, 0, -1, -1);

            }
            
        }
        else if(strcmp(word->first.c_str(),"in") == 0){
            it++;
            while(tac.at(it) != ';')
            {
                value += tac[it];
                it++; 
            }
            it++;
            try{
            AddintoQuad(word->second,symbols.find(value)->second,-1,-1,-1);
            }
            catch(...){
                cout << err << "can only take input in already defined variable";
            }
            
        }
        else if((strcmp(word->first.c_str(),"elif") == 0) || (strcmp(word->first.c_str(),"if") == 0)){
            it++;
            while( tac.at(it) != '\n'){
                value += tac[it];
                it +=1;
            }
            string op1 = "",op2 = "",ro = "";
            int o1, o2;
            int j =0;
            int extra = 0;
            string conLineNo = "";
            while(value[j] != ' ')
            {
                op1 += value[j];
                j++;
            }
            j++;
            while(value[j] != ' ')
            {
                ro += value[j];
                j++;
            }
            j++;
            while(value[j] != ' ')
            {
                op2 += value[j];
                j++;
            }
            if (symbols.find(op1) != symbols.end())
            {
                o1 = symbols.find(op1)->second;
            }
            else{
                try{
                o1 = stoi(op1);
                extra = 2;
                }
                catch(...){
                    cout << err <<endl;
                }
            }
            if (symbols.find(op2) != symbols.end())
            {
                o2 = symbols.find(op2)->second;
            }
            else{
                
                try{
                o2 = stoi(op2);
                }
                catch(...){
                    cout << err <<endl;
                }
                if(extra == 2){
                    extra = 3;
                }
                else{
                    extra = 1;
                }
            }
            j++;
            while(value[j] != ' ')
            {
                j++;
            }
            j++;
            while(value[j] != ';')
            {
                conLineNo += value[j];
                j++;
            }
            AddintoQuad(opCodes.find(ro)->second, o1,o2,extra, stoi(conLineNo));

        }
        else if(strcmp(word->first.c_str(),"goto") == 0){
            it++;
            while(tac[it] != ';')
            {
                value += tac[it];
                it++;
            }
            if(value[value.length() -1] == ' '){ 
                value.pop_back();
            }
            AddintoQuad(word->second, stoi(value), -1, -1, -1);
            it++;
        }
        else if(strcmp(word->first.c_str(),"ret") == 0){
            it++;
            while (tac.at(it) != ';')
            {
                value += tac[it];
                it += 1;
            }
            it++;
                //0(3) to indicate variable
            AddintoQuad(word->second, symbols.find(value)->second, 0, -1, -1);
        }

        it++;
    }
    }
    catch(...){
        cout << "Error found\n";
        exit(1);
    }
}
void vmachine::PrintQuads(){
    for(int i = 0; i < quadCount; i++){
    cout << "->" << quad[i][0] << " " << quad[i][1] << " " << quad[i][2]
                 << " "<< quad[i][3] << " " << quad[i][4] << endl;
    }
}
void vmachine::MakeSymbolHash(){
    char ch;
    fstream fin("symbol_table.txt", fstream::in);
    if(fin.is_open())
    {
        while (fin >> noskipws >> ch) {
            string buff = "";
            if (ch == '('){
                fin >> noskipws >> ch;

                while (ch != ',')
                {
                    buff += ch;
                    fin >> noskipws >> ch;
                }
                string name = buff;
                buff = "";
                fin >> noskipws >> ch;
                while (ch != ',')
                {
                    fin >> noskipws >> ch;
                }
                fin >> noskipws >> ch;
                while(ch != ')'){
                    buff += ch;
                    fin >> noskipws >> ch;
                }
                string value = buff;
                symbols[name] = stoi(value);
            }
        }
        fin.close();
    }
}
void vmachine::execute(){
    ds = new int[symbols.size()];
    for(int i = 0 ; i < quadCount; i++){
        if(quad[i][0] == opCodes.find("+")->second ||
        quad[i][0] == opCodes.find("-")->second ||
        quad[i][0] == opCodes.find("*")->second ||
        quad[i][0] == opCodes.find("/")->second ||
        quad[i][0] == opCodes.find("%")->second){
            int op1, op2;
            if(quad[i][4] == 3){
                op1 = quad[i][1];
                op2 = quad[i][2];
            }
            else if (quad[i][4] == 2){
                op1 = quad[i][1];
                op2 = ds[quad[i][2] / 4];
            }
            else if (quad[i][4] == 1){
                op1 = ds[quad[i][1] / 4];
                op2 = quad[i][2];
            }
            else if (quad[i][4] == 0){
                op1 = ds[quad[i][1] / 4];
                op2 = ds[quad[i][2] / 4];
            }
            if(quad[i][0] == opCodes.find("+")->second){
                ds[quad[i][3] / 4] = op1 + op2;
            }
            else if(quad[i][0] == opCodes.find("-")->second){
                ds[quad[i][3] / 4] = op1 - op2;
            }
            else if(quad[i][0] == opCodes.find("*")->second){
                ds[quad[i][3] / 4] = op1 * op2;
            }
            else if(quad[i][0] == opCodes.find("/")->second){
                ds[quad[i][3] / 4] = op1 / op2;
            }
            else if(quad[i][0] == opCodes.find("%")->second){
                ds[quad[i][3] / 4] = op1 % op2;
            }
        }
        
        if(quad[i][0] == opCodes.find("=")->second){
            int op1;
            if(quad[i][3] == 0){
                op1 = quad[i][1];
            }
            else if (quad[i][3] == 1){
                op1 = ds[quad[i][1] / 4];
            }
            ds[quad[i][2] / 4] = op1;
        }
        
        else if (quad[i][0] == opCodes.find("out")->second){
            if(quad[i][2] == 0){
                cout << ds[quad[i][1] / 4];
            }
            else if (quad[i][2] == 1) {
                char ch;
                string stuff = "";
                int c = 0;
                fstream finn("Stringfile.txt", fstream::in);
                if(finn.is_open())
                {
                    while (c != quad[i][1] && finn >> noskipws >> ch) {
                        if(ch == ';')
                        {
                            c++;
                        }
                    }
                    finn >> noskipws >> ch;
                    if (ch !=  '\n'){
                        stuff += ch;
                    }
                    while (finn >> noskipws >> ch && ch != ';' ) {
                        
                        stuff += ch;
                    }
                    if(stuff[stuff.length() - 1] != '\n')
                    {
                    cout << stuff;
                    }
                    else{
                        cout << stuff << endl;
                    }
                    finn.close();
            }
            }
        }
        
        else if (quad[i][0] == opCodes.find("in")->second){
            int a;
            cin >> a;
            ds[quad[i][1] / 4] = a;
        }
        
        else if (quad[i][0] == opCodes.find("<")->second ||
        quad[i][0] == opCodes.find(">")->second ||
        quad[i][0] == opCodes.find("<=")->second ||
        quad[i][0] == opCodes.find(">=")->second ||
        quad[i][0] == opCodes.find("==")->second ){
            int op1, op2;
            if(quad[i][3] == 3){
                op1 = quad[i][1];
                op2 = quad[i][2];
            }
            else if (quad[i][3] == 2){
                op1 = quad[i][1];
                op2 = ds[quad[i][2] / 4];
            }
            else if (quad[i][3] == 1){
                op1 = ds[quad[i][1] / 4];
                op2 = quad[i][2];
            }
            else if (quad[i][3] == 0){
                op1 = ds[quad[i][1] / 4];
                op2 = ds[quad[i][2] / 4];
            }
            if(quad[i][0] == opCodes.find("<")->second){
                if(op1 < op2){i = quad[i][4] - 2;}
            }
            else if(quad[i][0] == opCodes.find(">")->second){
                if(op1 > op2){i = quad[i][4] - 2;}
            }
            else if(quad[i][0] == opCodes.find("<=")->second){
                if(op1 <= op2){i = quad[i][4] - 2;}
            }
            else if(quad[i][0] == opCodes.find(">=")->second){
                if(op1 >= op2){i = quad[i][4] - 2;}
            }
            else if(quad[i][0] == opCodes.find("==")->second){
                if(op1 == op2){i = quad[i][4] - 2;}
            }
        }  
    
        else if (quad[i][0] == opCodes.find("goto")->second){
            i = quad[i][1] - 2;
        }

        else if (quad[i][0] == opCodes.find("ret")->second){
            cout << endl;
            exit(1);
        }
    }

}