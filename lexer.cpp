#include "lexer.h"
#include<string.h>
using namespace std;
//for printing tokens names.c_str(), Same as the enum defined in lexer.h
string reserved[] = {
    "END_OF_FILE",
	"ERROR",
	"INC",
	"COMMENT",
	"FUNC",
	"INT",
	"ID",
	"COLON",
	"BEGIN",
	"ASSIGN",
	"NUM",
	"SEMICOLON",
	"STRING",
	"RELOP",
	"CHAR",
	"ADD",
    "SUBTRACT",
    "MULTIPLY",
    "DIVIDE",
    "MODULUS",
	"END",
	"LETTER",
	"IF",
	"ELSE",
	"ELIF",
	"CALL",
	"FOR",
	"PRINT",
	"PRINTLN",
	"RETURN",
	"IN",
    "COMMA"

	};
token::token()
{
    this->lexeme = "";
    this->tokenType = TokenType::ERROR;
}
token::token(string lexeme, TokenType tokenType)
{
    this->lexeme = lexeme;
    this->tokenType = tokenType;
}
void token::Print()
{
    cout << "{" << lexeme << " , "
        << reserved[(int)tokenType] << "}\n";
}
int lexer::getCurrentPointer()
{
    return index;
}
void lexer::setCurrentPointer(int pos)
{
    if (pos >= 0 && pos < tokens.size())
        index = pos;
    else
        index = 0;
}
bool lexer::isKeyword(string s){
    // this function checks if the received string is a keyword
    // if it is then make a token else return false
    if (strcmp(s.c_str(),"in") == 0)
    {
        
        tokens.push_back(token("",TokenType::IN));
        return true;
    }
    else if (strcmp(s.c_str(),"func") == 0)
    {
        tokens.push_back(token("",TokenType::FUNC));
        return true;
    }
    else if (strcmp(s.c_str(),"char") == 0)
    {
        tokens.push_back(token("",TokenType::CHAR));
        return true;
    }
    else if (strcmp(s.c_str(),"call") == 0)
    {
        tokens.push_back(token("",TokenType::CALL));
        return true;
    }
    else if (strcmp(s.c_str(),"if") == 0)
    {
        tokens.push_back(token("",TokenType::IF));
        return true;
    }
    else if (strcmp(s.c_str(),"elif") == 0)
    {
        tokens.push_back(token("",TokenType::ELIF));
        return true;
    }
    else if (strcmp(s.c_str(),"else") == 0)
    {
        tokens.push_back(token("",TokenType::ELSE));
        return true;
    }
    else if (strcmp(s.c_str(),"for") == 0)
    {
        tokens.push_back(token("",TokenType::FOR));
        return true;
    }
    else if (strcmp(s.c_str(),"print") == 0)
    {
        tokens.push_back(token("",TokenType::PRINT));
        return true;
    }
    else if (strcmp(s.c_str(),"println") == 0)
    {
        tokens.push_back(token("",TokenType::PRINTLN));
        return true;
    }
    else if (strcmp(s.c_str(),"return") == 0)
    {
        tokens.push_back(token("",TokenType::RETURN));
        return true;
    }
    else if (strcmp(s.c_str(),"int") == 0)
    {
        tokens.push_back(token("",TokenType::INT));
        return true;
    }
    else if (strcmp(s.c_str(),"begin") == 0)
    {
        tokens.push_back(token("",TokenType::BEGIN));
        return true;
    }
    else if (strcmp(s.c_str(),"end") == 0)
    {
        tokens.push_back(token("",TokenType::END));
        return true;
    }
    else {
        return false;
    }


}

void lexer::Tokenize()//function that tokenizes your input stream
{
    vector<char>::iterator it = stream.begin();
	//your implementation goes here
    int state = 0;
    //will start iterating over each character
    for (it = stream.begin(); it != stream.end(); it++)
    {
        //starting state is 0
        //after making token pointer will start from state 0 again
        switch(state){
            case 0:
                {
                    // these all ifs depicts the DFA working at initial state
                    // they will set their desired states then
                    if(*it == '>')
                    {
                        state = 1;
                    }
                    else if (*it == '<')
                    {
                        state = 2;
                    }
                    else if (*it == '#')
                    {
                        state = 3;
                    }
                    else if (*it == '"')
                    {
                        state = 4;
                    }
                    else if (*it == '\'')
                    {
                        state = 5;
                    }
                    else if (*it == '~')
                    {
                        state = 7;
                    }
                    else if ( *it >= 'A' && *it <= 'Z' || *it >= 'a' && *it <= 'z')
                    {
                        //check if its a letter
                        state = 16;
                    }
                    else if (*it >= '0' && *it <= '9')
                    {
                        state = 9;
                    }
                    else if (*it == '=')
                    {
                        //return type as Relational operator with Equal lexeme
                        tokens.push_back(token("Equals",TokenType::RELOP));
                    }
                    else if(*it == '+')
                    {
                        tokens.push_back(token("SUM",TokenType::ADD));
                    }
                    else if(*it == '-')
                    {
                        tokens.push_back(token("SUB",TokenType::SUBTRACT));
                    }
                    else if(*it == '/')
                    {
                        tokens.push_back(token("DIV",TokenType::DIVIDE));
                    }
                    else if(*it == '*')
                    {
                        tokens.push_back(token("MUL",TokenType::MULTIPLY));
                    }
                    else if(*it == '%')
                    {
                        tokens.push_back(token("MOD",TokenType::MODULUS));
                    }
                    else if(*it == ',')
                    {
                        //make token for special character
                        tokens.push_back(token("",TokenType::COMMA));
                    }
                    else if(*it == ':')
                    {
                        tokens.push_back(token("",TokenType::COLON));
                    }
                    else if(*it == ';')
                    {
                        tokens.push_back(token("",TokenType::SEMICOLON));
                    }
                    
                    break;
                }
            case 1:
                {
                    if (*it == '=')
                    {
                        tokens.push_back(token("GE", TokenType::RELOP));
                    }
                    else{
                        tokens.push_back(token("GT",TokenType::RELOP));
                        it--;
                    }
                    state = 0;
                    break;
                }
            case 2:
                {
                    if (*it == '-')
                    {
                        tokens.push_back(token("", TokenType::ASSIGN));
                    }
                    else if (*it == '=')
                    {
                        tokens.push_back(token("LE", TokenType::RELOP));
                    }
                    else{
                        tokens.push_back(token("LT", TokenType::RELOP));
                        it--;
                    }
                    state = 0;
                    break;
                }
            case 3:
                {
                    while(*it != '\n')
                    {
                        it++;
                    }
                    //tokens.push_back(token("", TokenType::COMMENT));
                    state = 0;
                    break;
                }
            case 4:
                {
                    string buff = "";
                    while(*it != '"')
                    {
                        buff += *it;
                        it++;
                    }
                    tokens.push_back(token('"' + buff + '"', TokenType::STRING));
                    state = 0;
                    break;
                }
            case 5:
                {
                    string buff1 = "";
                    buff1 += *it;
                    it++;
                    if (*it == '\'')
                    {
                        tokens.push_back(token(buff1, TokenType::LETTER));
                    }
                    state = 0;
                    break;
                }
            case 7:
                {
                    if(*it == '=')
                    {
                        tokens.push_back(token("NE", TokenType::RELOP));
                    }
                    state = 0;
                    break;
                }
            case 16:
                {
                    // temporary string to store statement
                    string buff2 = "";
                    it--;
                    //getting the previous character
                    buff2 += *it;
                    it++;
                    while ((*it >= 'A' && *it <= 'Z') || (*it >= 'a' && *it <= 'z'))
                    {
                        buff2 += *it;
                        it++;
                    }
                    //function called to check if read string is valid keyword
                    if (!(isKeyword(buff2)))
                    {
                        tokens.push_back(token( buff2, TokenType::ID));
                    }
                    state = 0;
                    it--;
                    break;
                }
            case 9: 
                {
                    string buff3 = "";
                    it--;
                    buff3 += *it;
                    it++;
                    while (*it >= '0' && *it <= '9')
                    {
                        buff3 += *it;
                        it++;
                    }
                    if (*it == '-' || *it == '+')
                    {
                        buff3 += *it;
                        it++;
                        if(*it == 'E')
                        {
                            buff3 += *it;
                            it++;
                            while (*it >= '0' && *it <= '9')
                            {
                                buff3 += *it;
                                it++;
                            }
                        }
                    }
                    tokens.push_back(token(buff3, TokenType::NUM)); 
                    state = 0;
                    it--;
                    break;
                }
            default:
                {
                    //it will cover spaces
                }
        }

    }
	//push_back EOF token at end to identify End of File
    tokens.push_back(token(string(""), TokenType::END_OF_FILE));
}

lexer::lexer(const char filename[])
{
    //constructors: takes file name as an argument and store all
    //the contents of file into the class varible stream
    ifstream fin(filename);
    if (!fin) //if file not found
    {
        cout << "file not found" << endl;
    }
    else
    {
        char byte = 0;
        while (fin.get(byte))
        { //store file contents into stream
            if (byte != '\r')
                stream.push_back(byte);
        }
        stream.push_back(' ');//dummy spaces appended at the end
        stream.push_back(' ');
        //Tokenize function to get all the (token,lexeme) pairs
        Tokenize();
        //assigning tokens to iterator::pointer
        index = 0;
    }
}
lexer::lexer()
{
    index = -1;
}
void lexer::printRaw()
{
    //helper function to print the content of file
    vector<char>::iterator it = stream.begin();
    for (it = stream.begin(); it != stream.end(); it++)
    {
        cout << *it;
    }
    cout << endl;
}
token lexer::getNextToken()
{
    //this function will return single (token,lexeme) pair on each call
    //this is an iterator which is pointing to the beginning of vector of tokens
    token _token;
    if (index == tokens.size())
    {                       // return end of file if
        _token.lexeme = ""; // index is too large
        _token.tokenType = TokenType::END_OF_FILE;
    }
    else
    {
        _token = tokens[index];
        index = index + 1;
    }
    return _token;
}
void lexer::resetPointer()
{
    index = 0;
}
token lexer::peek(int howFar)
{
    if (howFar <= 0)
    { // peeking backward or in place is not allowed
        cout << "LexicalAnalyzer:peek:Error: non positive argument\n";
        exit(-1);
    }

    int peekIndex = index + howFar - 1;
    if (peekIndex > (tokens.size() - 1))
    {                                                 // if peeking too far
        return token("", TokenType::END_OF_FILE); // return END_OF_FILE
    }
    else
        return tokens[peekIndex];
        
}