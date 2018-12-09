#ifndef UNTITLED1_OPERATORPRIORITY_ANALYZER_H
#define UNTITLED1_OPERATORPRIORITY_ANALYZER_H
#include <bits/stdc++.h>
#include <unistd.h>
#include <fcntl.h>
#include "Lex_analyzer.h"
using namespace std;
class Operatorpriority_analyzer: public Lex_analyzer{
private:
    char *wf;
    string blank = ""; //用于通过字符凑字符串
    vector<string> grammarList; //文法集合
    vector<char> Vn; //非终结符集合Vn
    vector<char> Vt; //终结符集合Vt
    vector<char> symbolStack; //分析栈
    vector<vector<char>> firstVT; //firstVT集
    vector<vector<char>> lastVT; //LastVT集
    char table[255][255]; //算符优先顺序表
    bool F[30][255]; //firstVT集的布尔数组
    stack<pair<char, char>> STACK; //firstVT集计算用栈
    stack<string> SEM;
    vector<vector<string>> QT;
    int i=1;
public:
    void printLine (int n);
    void printTable(vector<char> symbolStack, vector<char> inputString, int step, char left, char priority, char right, string handle);
    void printV();
    int get_VnNum(char c);
    int get_VtNum(char c);
    bool search_List(char c, vector<char> List);
    char search_Grammar(string handle);
    bool check_grammar(string grammar);
    void putinVt();
    void INSERT(char U, char b);
    void get_firstVt();
    void get_lastVt();
    void get_table();
    void print_firstlastVt();
    void Readwenfa();
    void Putinwf();
    void GEQ(string x);
    void printQT();
    void Operator_analysisRun();
};
#endif //UNTITLED1_OPERATORPRIORITY_ANALYZER_H
