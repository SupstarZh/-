#ifndef UNTITLED1_GRAMMER_ANALYZER_H
#define UNTITLED1_GRAMMER_ANALYZER_H
#include<bits/stdc++.h>
#include<fcntl.h>
#include<unistd.h>
#include"Lex_analyzer.h"
using namespace std;
class Wenfa{
public:
    string Vnot; //非终结符集合
    set<string> productions; //产生式
    Wenfa(char s[]){
        Vnot = s;
    }
    void print(){  //打印
        cout<<Vnot<<"->";
        auto it = productions.begin();
        if(it!=productions.end()){
            cout<<*it;
            it++;
        }
        for(; it!=productions.end(); it++){
            cout<<"|"<<*it;
        }
        cout<<endl;
    }
    void insert(char s[]){
        productions.insert(s);
    }
};
class Grammer_analyzer: public Lex_analyzer
{
private:
    map<string, set<char>> first, follow;  //文法的first集合 follow集合
    vector<map<char,string>> Predict_table; //LL1的分析表
    vector<map<char,string>> Translationtable;
    map<string, int> Vn_dic; //非终结符的对应序号
    vector<Wenfa> Vn_set; //产生式集合
    bool is_used[1000];
    char *wf;
    vector<vector<string>> QT;
    stack<string> SEM;
public:
    bool TF=true;
    int i=1;
    void changetoLL1();
    void DFS(int x);
    void make_first();
    void print_firstset();
    void append(const string& str1, const string& str2);
    void make_follow();
    void print_followset();
    bool check_first(const string& production, char ch);
    bool check_follow(const string& production, char ch);
    void translateto(vector<char> letter);
    void make_predict_table();
    void Readwenfa();
    void Putinwf();
    bool Is_Vn(string x);
    int Is_match(string x, string m);
    void LL1_Run();
    void GEQ(string x);
    void printQT();
    void PUSH(string x);
    void E();
    void T();
    void F();

    void main_procedure();
    void recursive_subroutineRun();
};
#endif //UNTITLED1_GRAMMER_ANALYZER_H
