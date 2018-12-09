#ifndef UNTITLED1_LEX_ANALYZER_H
#define UNTITLED1_LEX_ANALYZER_H
#include<bits/stdc++.h>
#include<fcntl.h>
#include<unistd.h>
using namespace std;
class Lex_analyzer{
private:
    vector<string> kt={"int","main","void","if","else","char"};  //关键字集合
    vector<string> pt={">=","<=","==","=",">","<","+","-","*","/","{","}",",",";","(",")","[","]","+=","-=","*=","/=","%="
            ,"++","--","!="}; //界符集合
public:
    char *prog, ch;
    vector<string> it,ct,st, Ct; //ct常数表,Ct字符表,st字符串表
    string token, type;
    string leibie;
    int syn, p, m=0, row=1, YN=0;
    queue<string> Analy_stack; //读入的字符串
    stack<string> Synax_stack; // 语法栈
    vector<char> inputString; //剩余输入串
    void ReadFile();
    void initprog();
    int search_string(string tem, vector<string> &st);//寻找字符串string在不在这个字符串表里
    bool Is_Num(char c);
    bool Is_Letter(char c);
    bool Is_constant(string st);//自动机判断这个字符串是不是常数
    void scaner();
    void Showfile();
    void PutTokens();
    void Lex_analyzer_Run();
};
#endif //UNTITLED1_LEX_ANALYZER_H
