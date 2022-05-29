#ifndef _PARSER_H_
#define _PARSER_H_
#include "lexer.h"
//for future assignments leave it as it is
class parser
{
    lexer _lexer;
    ofstream fout;
    ofstream foutST;
public:
    void syntax_error();
    token expect(TokenType expected_type);
    parser(const char filename[]);
    void readAndPrintAllInput();
    void resetPointer();
    /*Terminal functions goes here use peek and expect*/
    /*use TokenType:: for token names for example
        expect(TokenType::ASSIGN);   //example function call
      */
	//all your parser function goes here
    void parse();
    void functionState();
    void declare();
    void declaration();
    void functionWithAssignment();
    void LA();
    void type();
    void inputState();
    string assignment();
    void loopAssign();
    void assignmentState();
    string E();
    string E_1();
    string T();
    string T_1();
    string G();
    void printState();
    string stuff();
    string condition();
    void ifElseState();
    void next(int, vector<int>, int);
    void Else(int,  vector<int>, int);
    void Elif(int,  vector<int>, int);
    void forLoop();
    void param();
    void callState();
    int callParam();
    int moreParam();
    void statements();
    void nextParam();
    void addToSymbolTable(string type, string symbol);
};
#endif
