#include "parser.h"
#include <string.h>
// line counter for selective statements
int conditionCounter = 1;
// counter for new variables
int variableCounter = 1;
// A string to store address code
string TAC = "";
// to store the declaring variables when in a list
vector<string> decVar;
// Adress variable for symbol table
int addressCounter = 0;

void parser::syntax_error()
{
    cout << _lexer.getCurrentPointer() << endl;
    cout << "SYNTAX ERROR\n";
    exit(1);
}
string getTemp()
{
    int buff = variableCounter;
    variableCounter++;
    return "T" + to_string(buff);
}
token parser::expect(TokenType expected_type)
{
    token t = _lexer.getNextToken();
    if (t.tokenType != expected_type)
        syntax_error();
    return t;
}
parser::parser(const char filename[])
{
    _lexer = lexer(filename);
    fout.open("TAC.txt");
    foutST.open("symbol_table.txt");
}
void parser::readAndPrintAllInput() // read and print allinputs (provided)
{
    token t;
    t = _lexer.getNextToken();
    while (t.tokenType != TokenType::END_OF_FILE)
    {
        t.Print();
        t = _lexer.getNextToken();
    }
}
void parser::resetPointer()
{
    _lexer.resetPointer();
}
void backPatching(int a, int b)
{
    // on line a append b at the end

    int lineCount = 0;
    for (int i = 0; i < TAC.size(); i++)
    {
        if (TAC[i] == '\n')
        {
            lineCount++;
            string uv = "" + to_string(b);
            if (lineCount == a)
            {
                if (b <= 9)
                {
                    TAC[i - 3] = uv[0];
                }
                else
                {
                    TAC[i - 3] = uv[0];
                    TAC[i - 2] = uv[1];
                }
                return;
            }
        }
    }
}
void parser::parse()
{
    functionState();
    fout << TAC;
    fout.close();
}
void parser::functionState()
{
    if (_lexer.peek(1).tokenType == TokenType::FUNC)
    {
        expect(TokenType::FUNC);
        type();
        expect(TokenType::ID);
        param();
        expect(TokenType::COLON);
        expect(TokenType::BEGIN);
        statements();
        expect(TokenType::END);
        // write three assembly code in file
        functionState();
    }
}
string parser::E()
{
    string k = "";
    k = T();
    k = k + E_1();
    return k;
}
string parser::T()
{
    string k = "";
    k = k + G();
    k = k + T_1();
    return k;
}
string parser::E_1()
{
    if (_lexer.peek(1).tokenType == TokenType::ADD)
    {
        expect(TokenType::ADD);
        string k = " + ";
        string buff = "" + T();
        k = k + buff;
        string temp = "" + E_1();
        if (strcmp(temp.c_str(), "") != 0)
        {
            string v = getTemp();
            addToSymbolTable(v,"int");
            TAC += v + " = " + buff + temp + ";\n";
            conditionCounter++;
            return " + " + v;
        }
        return k + temp;
    }
    else if (_lexer.peek(1).tokenType == TokenType::SUBTRACT)
    {
        expect(TokenType::SUBTRACT);
        string k = " - ";
        k = k + T();
        k = k + E_1();
        return k;
    }
}
string parser::T_1()
{
    if (_lexer.peek(1).tokenType == TokenType::MULTIPLY)
    {
        expect(TokenType::MULTIPLY);
        string k = "*";
        k = k + G();
        k = k + T_1();
        return k;
    }
    else if (_lexer.peek(1).tokenType == TokenType::DIVIDE)
    {
        expect(TokenType::DIVIDE);
        string k = "/";
        k = k + G();
        k = k + T_1();
        return k;
    }
    else if (_lexer.peek(1).tokenType == TokenType::MODULUS)
    {
        expect(TokenType::MODULUS);
        string k = "%";
        k = k + G();
        k = k + T_1();
        return k;
    }
}
string parser::G()
{
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        string k = _lexer.peek(1).lexeme;
        expect(TokenType::ID);
        return k;
    }
    else if (_lexer.peek(1).tokenType == TokenType::NUM)
    {
        string k = _lexer.peek(1).lexeme;
        expect(TokenType::NUM);
        return k;
    }
    else
    {
        string k = _lexer.peek(1).lexeme;
        expect(TokenType::CHAR);
        return k;
    }
}
void parser::printState()
{
    string k = "";
    if (_lexer.peek(1).tokenType == TokenType::PRINT)
    {
        k = k + "out ";
        expect(TokenType::PRINT);
        k = k + stuff();
    }
    else
    {
        k = k + "out ";
        expect(TokenType::PRINTLN);
        k = k + stuff();
    }
    expect(TokenType::SEMICOLON);
    // cout << k << endl;
    TAC += k + ";\n";
    conditionCounter++;
}

string parser::stuff()
{
    string k = "";
    if (_lexer.peek(1).tokenType == TokenType::STRING)
    {
        k = _lexer.peek(1).lexeme;
        expect(TokenType::STRING);
    }
    else if (_lexer.peek(1).tokenType == TokenType::CHAR || _lexer.peek(1).tokenType == TokenType::NUM || _lexer.peek(1).tokenType == TokenType::ID)
    {
        string v = getTemp();
        TAC += v  + " = " + E() + ";\n";
        conditionCounter++;
        addToSymbolTable(v,"int");
        k = v;

    }
    return k;
}
string parser::condition()
{
    string k = E();

    string op = _lexer.peek(1).lexeme;
    string sign = "";
    if (strcmp(op.c_str(), "LT") == 0)
    {
        sign = sign + "<";
    }
    else if (strcmp(op.c_str(), "GT") == 0)
    {
        sign = sign + ">";
    }
    else if (strcmp(op.c_str(), "LE") == 0)
    {
        sign = sign + "<=";
    }
    else if (strcmp(op.c_str(), "GE") == 0)
    {
        sign = sign + ">=";
    }
    else if (strcmp(op.c_str(), "NE") == 0)
    {
        sign = sign + "~=";
    }
    else if (strcmp(op.c_str(), "Equals") == 0)
    {
        sign = sign + "==";
    }

    k = k + " " + sign + " ";
    expect(TokenType::RELOP);
    k = k + E();
    return k;
}
void parser::inputState()
{
    expect(TokenType::IN);
    string k = "in " + _lexer.peek(1).lexeme;
    expect(TokenType::ID);
    expect(TokenType::SEMICOLON);
    // cout << k << endl;
    TAC += k + ";\n";
    conditionCounter++;
}
// x <- y + z;
void parser::assignmentState()
{
    string k = _lexer.peek(1).lexeme;
    expect(TokenType::ID);
    k = k + " = ";
    expect(TokenType::ASSIGN);

    k = k + E();
    expect(TokenType::SEMICOLON);
    TAC += k + ";\n";
    conditionCounter++;
}
// x int; x <-0 int;
string parser::assignment()
{
    string k = "";
    k = k + _lexer.peek(1).lexeme;
    expect(TokenType::ID);
    decVar.push_back(k);
    if (_lexer.peek(1).tokenType == TokenType::ASSIGN)
    {
        k = k + " = ";
        expect(TokenType::ASSIGN);
        k = k + E();
        return k;
    }
    else{
        if(_lexer.peek(1).tokenType == TokenType::INT)
        {
            for (auto i = decVar.begin(); i != decVar.end(); ++i){
                addToSymbolTable(*i, "int");
            }
            decVar.clear();
        }
        if(_lexer.peek(1).tokenType == TokenType::CHAR)
        {
            for (auto i = decVar.begin(); i != decVar.end(); ++i){
                addToSymbolTable(*i, "char");
            }
            decVar.clear();
        }
    }
    return "";
}
void parser::declaration()
{
    string k = assignment();
    if (strcmp(k.c_str(), "") != 0)
    {
        TAC += k + ";\n";
        conditionCounter++;
    }
    LA();
    type();
    expect(TokenType::SEMICOLON);
}

void parser::LA()
{

    if (_lexer.peek(1).tokenType == TokenType::COMMA)
    {
        expect(TokenType::COMMA);
        if (_lexer.peek(1).tokenType == TokenType::ID)
        {
            string k = assignment();
            if (strcmp(k.c_str(), "") != 0)
            {
                TAC += k + ";\n";
                conditionCounter++;
            }
            LA();
        }
    }
}
void parser::type()
{
    string k = "";
    if (_lexer.peek(1).tokenType == TokenType::INT)
    {
        expect(TokenType::INT);
    }
    else
    {
        expect(TokenType::CHAR);
    }
}
void parser::functionWithAssignment()
{
    expect(TokenType::ID);
    expect(TokenType::ASSIGN);
    callState();
}

void parser::callState()
{
    expect(TokenType::CALL);
    string k = _lexer.peek(1).lexeme;
    expect(TokenType::ID);
    int param_count = callParam();
    TAC += "call " + k + ", " + to_string(param_count) + ", " + getTemp() + "; \n";
    expect(TokenType::SEMICOLON);
}
int parser::callParam()
{
    int param_count = 0;
    if (_lexer.peek(1).tokenType == TokenType::CHAR || _lexer.peek(1).tokenType == TokenType::NUM || _lexer.peek(1).tokenType == TokenType::ID)
    {
        string k = E();
        TAC += "param " + k + ";\n";
        param_count = moreParam() + 1;
    }
    return param_count;
}
int parser::moreParam()
{
    int param_count = 0;
    if (_lexer.peek(1).tokenType == TokenType::COMMA)
    {
        expect(TokenType::COMMA);
        string k = E();
        TAC += "param " + k + ";\n";
        param_count++;
        moreParam();
    }
    return param_count;
}
void parser::forLoop()
{
    string k = "";
    expect(TokenType::FOR);
    k = assignment();

    TAC += k + ";\n";
    conditionCounter++;

    int v = conditionCounter;
    expect(TokenType::COMMA);
    k = condition();
    k = "if " + k;

    expect(TokenType::COMMA);
    int t = conditionCounter;
    TAC += k + " goto   ;\n";
    conditionCounter++;

    int f = conditionCounter;
    TAC += "goto   ;\n";
    conditionCounter++;
    // cout << "put " << conditionCounter << "on line no." << t << endl;
    // TAC += "put " + to_string(conditionCounter) + "on line no." + to_string(t) + " \n";
    backPatching(t, conditionCounter);
    string k2 = assignment();
    expect(TokenType::COLON);
    expect(TokenType::BEGIN);
    statements();

    TAC += k2 + ";\n";
    conditionCounter++;

    TAC += "goto " + to_string(v) + ";\n";
    conditionCounter++;

    // TAC += "put " + to_string(conditionCounter) + " on line no. " + to_string(f) + " \n";
    backPatching(f, conditionCounter);
    // cout << "put " << conditionCounter << " on line no." << f << endl;

    expect(TokenType::END);
}
void parser::ifElseState()
{
    expect(TokenType::IF);
    string k = condition();
    k = "if " + k;
    expect(TokenType::COLON);

    int t = conditionCounter;
    TAC += k + " goto   ;\n";
    conditionCounter++;

    int f = conditionCounter;
    TAC += "goto   ;\n";
    conditionCounter++;

    backPatching(t, conditionCounter);

    expect(TokenType::BEGIN);
    statements();
    vector<int> nex;
    nex.push_back(conditionCounter);
    expect(TokenType::END);
    next(f, nex, t);
}
void parser::next(int f,  vector<int> nex,int t)
{

    if (_lexer.peek(1).tokenType == TokenType::ELSE)
    {
        backPatching(f, conditionCounter + 1);
        Else(f, nex, t);
    }
    else if (_lexer.peek(1).tokenType == TokenType::ELIF)
    {
        backPatching(f, conditionCounter + 1);
        Elif(f,nex,t);
    }
    else
    {
        backPatching(f, conditionCounter);
    }
}
void parser::Else(int f,  vector<int> nex, int t)
{
    expect(TokenType::ELSE);
    TAC += "goto   ;\n";
    conditionCounter++;
    backPatching(f, conditionCounter);
    expect(TokenType::COLON);
    expect(TokenType::BEGIN);
    statements();

    for (auto i = nex.begin(); i != nex.end(); ++i){
        backPatching(*i, conditionCounter);
    }

    expect(TokenType::END);
}
void parser::Elif(int f ,  vector<int> nex, int t)
{
    expect(TokenType::ELIF);
    TAC += "goto   ;\n";
    conditionCounter++;
    string k = condition();
    k = "elif " + k;
    expect(TokenType::COLON);

    t = conditionCounter;
    TAC += k + " goto   ;\n";
    conditionCounter++;

    f = conditionCounter;
    TAC += "goto   ;\n";
    conditionCounter++;

    backPatching(t, conditionCounter);

    expect(TokenType::BEGIN);
    statements();
    nex.push_back(conditionCounter);
    expect(TokenType::END);
    next(f, nex,t);
}
void parser::param()
{
    if (_lexer.peek(1).tokenType == TokenType::ID)
    {
        expect(TokenType::ID);
        type();
        nextParam();
    }
}
void parser::nextParam()
{
    if (_lexer.peek(1).tokenType == TokenType::COMMA)
    {
        expect(TokenType::COMMA);
        expect(TokenType::ID);
        type();
        nextParam();
    }
}
void parser::statements()
{
    if (_lexer.peek(1).tokenType == TokenType::IF)
    {
        // if statement
        ifElseState();
        statements();
    }
    else if (_lexer.peek(1).tokenType == TokenType::IN)
    {
        // input statement
        inputState();
        statements();
    }
    else if (_lexer.peek(1).tokenType == TokenType::CALL)
    {
        callState();
        statements();
    }
    else if (_lexer.peek(1).tokenType == TokenType::FOR)
    {
        forLoop();
        statements();
    }
    else if (_lexer.peek(1).tokenType == TokenType::PRINT || _lexer.peek(1).tokenType == TokenType::PRINTLN)
    {
        printState();
        statements();
    }
    else if (_lexer.peek(1).tokenType == TokenType::ID)
    {

        // peek till semi colon to get the 2nd last token of statement
        int i;
        for (i = 1; _lexer.peek(i).tokenType != TokenType::SEMICOLON; ++i)
            ;
        --i;
        if (_lexer.peek(2).tokenType == TokenType::ASSIGN && _lexer.peek(3).tokenType == TokenType::CALL)
        {
            functionWithAssignment();
        }
        else if (_lexer.peek(i).tokenType == TokenType::INT || _lexer.peek(i).tokenType == TokenType::CHAR)
        {
            declaration();
        }
        else
        {
            assignmentState();
        }

        statements();
    }
    else if (_lexer.peek(1).tokenType == TokenType::RETURN)
    {
        string k = "ret ";
        expect(TokenType::RETURN);
        string v = getTemp();
        addToSymbolTable(v, "int");
        TAC += v + " = " + E() + ";\n";
        conditionCounter++;
        k = k + v;
        expect(TokenType::SEMICOLON);
        // cout << k << endl;
        TAC += k + ";\n";
        conditionCounter++;
        statements();
    }
}
void parser::addToSymbolTable(string name, string type)
{
    foutST << "(" << name << ", " << type << ", " << to_string(addressCounter)  << ")" << endl;
    if (strcmp(type.c_str(),"int") == 0){
        addressCounter += 4;
    }
    else{
         addressCounter += 1;
    }
}
