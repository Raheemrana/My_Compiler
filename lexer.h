#ifndef _LEXER_H_
#define _LEXER_H_
#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <cctype>
using namespace std;

//all your tokens goes here
enum class TokenType
{
	END_OF_FILE = 0,
	ERROR = 1,
	INC = 2,
	COMMENT = 3,
	FUNC = 4,
	INT = 5,
	ID = 6,
	COLON = 7,
	BEGIN = 8,
	ASSIGN = 9,
	NUM = 10,
	SEMICOLON = 11,
	STRING = 12,
	RELOP = 13,
	CHAR = 14,
	ADD = 15,
	SUBTRACT = 16,
	MULTIPLY = 17,
	DIVIDE = 18,
	MODULUS = 19,
	END = 20,
	LETTER = 21,
	IF = 22,
	ELSE = 23,
	ELIF = 24,
	CALL = 25,
	FOR = 26,
	PRINT = 27,
	PRINTLN = 28,
	RETURN = 29,
	IN = 30,
	COMMA = 31
};
//structure of a token 
struct token
{
	string lexeme;
	TokenType tokenType;//enum type
	//constructor
	token(string lexeme, TokenType tokenType);
	//constructor default
	token();
	void Print();
};
class lexer
{
	vector<char> stream;  //used for storing file sample_code.cc content
	vector<token> tokens; //vector to store all (tokens,lexeme) pairs
	void Tokenize();//populates tokens vector
	int index;

public:
	lexer();
	lexer(const char filename[]);
	bool isKeyword(string str);
	void printRaw();//just print everything as it is present in file
	token getNextToken();//get next token
	void resetPointer();//reset pointer to start getting tokens from start
	int getCurrentPointer();//get where current pointer in tokens vector is
	void setCurrentPointer(int pos);//move current pointer to wherever
	token peek(int);//peek the next token
};

#endif // !_LEXER_H