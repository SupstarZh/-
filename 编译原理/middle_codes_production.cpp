#include<bits/stdc++.h>
#include<fcntl.h>
#include<unistd.h>
#include<sstream>
using namespace std;
char *prog, ch;
string token;
queue<string> Analy_stack;
stack<string> Synax_stack;
int syn, p, m=0, row=1;
vector<string> kt={"int","main","void","if","else","char"};
vector<string> pt={">=","<=","==","=",">","<","+","-","*","/","{","}",",",";","(",")","[","]","+=","-=","*=","/=","%="
                        ,"++","--","!="};
vector<string> it,ct,st, Ct; //ct常数表,Ct字符表,st字符串表
string leibie;
void ReadFile()
{
    int size = 1000;
    int fd,i;
    prog = (char *)malloc(size);
    if(!prog)
    {
        cout<<"[error] could not malloc"<<endl;
        exit(-1);
    }
    if((fd = open("equation.txt", 0)) < 0)
    {
        cout<<"[error] could not open file"<<endl;
        exit(-1);
    }
    if((i = read(fd, prog, size-1)) <= 0)
    {
        cout<<"[error] return "<<i<<endl;
        exit(-1);
    }
    prog[i-1] = '#';
    prog[i] = '\0';
}
int search_string(string tem, vector<string> &st)//寻找字符串string在不在这个字符串表里
{
    int j=0;
    for(auto i=st.begin();i!=st.end();i++)
    {
        if(*i==tem)
            return j+1; ;//找到，返回位置
        j++;
    }
    return -1;//未找到
}
bool Is_Num(char c)
{
    if(c>='0'&&c<='9')
        return true;
    else
        return false;
}
bool Is_Letter(char c)
{
    if((ch>='a'&&c<='z')||(c>='A'&&c<='Z'))
        return true;
    else
        return false;
}
bool Is_constant(string st)//判断这个字符串是不是常数，利用状态转移
{
    int state=1;
    int i=0;
    while(1)
    {
        if(state==1)
        {
            if(st[i]>='0'&&st[i]<='9')
            {
                state=2;
            }else
            {
                return false;
            }
        }else if(state==2)
        {
            if(st[i]>='0'&&st[i]<='9'){}
            else if(st[i]=='.')
            {
                state=3;
            }else if(st[i]=='e')
            {
                state=5;
            }else if(st[i]=='\0')
            {
                return true;
            }else
            {
                return false;
            }
        }else if(state==3)
        {
            if(st[i]>='0'&&st[i]<='9')
            {
                state=4;
            }else
            {
                return false;
            }
        }else if(state==4)
        {
            if(st[i]>='0'&&st[i]<='9'){}
            else if(st[i]=='e')
            {
                state=5;
            }else if(st[i]=='\0')
            {
                return true;
            }else
            {
                return false;
            }
        }else if(state==5)
        {
            if(st[i]>='0'&&st[i]<='9')
            {
                state=7;
            }else if(st[i]=='+'||st[i]=='-')
            {
                state=6;
            }
        }else if(state==6)
        {
            if(st[i]>='0'&&st[i]<='9')
            {
                state=7;
            }else
            {
                return false;
            }
        }else if(state==7)
        {
            if(st[i]>='0'&&st[i]<='9'){}
            else if(st[i]=='\0')
            {
                return true;
            }else
            {
                return false;
            }
        }
        i++;
    }
    return false;
}
void scaner()
{
    token.clear();
    leibie.clear();
    while(ch == ' '||ch =='\n'||ch=='\r'){
        if(ch=='\n')
            row++;
        ch = prog[p++];
    }
    if(Is_Letter(ch))
    {
        while(Is_Letter(ch) || Is_Num(ch))
        {
            token += ch;
            ch = prog[p++];
        }
        syn = search_string(token, kt);
        if(syn!=-1)
            leibie="keyword";
        if(syn == -1){
            syn = search_string(token, it);
            if(syn == -1){
                it.push_back(token);
                syn = search_string(token, it);
            }
            if(syn!=-1)
                leibie = "identyword";
            Analy_stack.push("i");
        }
    }
    else if(Is_Num(ch))
    {
        token+=ch;
        ch = prog[p++];
        while((ch>='0'&&ch<='9')||ch=='e'||ch=='.')
        {
            if(ch == 'e'){
                token+=ch;
                ch = prog[p++];
                if(ch=='+'||ch=='-'){
                    token+=ch;
                    ch = prog[p++];
                }
            }
            else{
                token+=ch;
                ch = prog[p++];
            }
        }
        if(Is_Letter(ch)){syn=-1;}
        token+='\0';
        if(Is_constant(token)){
            syn = search_string(token, ct);
            if(syn == -1){
                ct.push_back(token);
                syn = search_string(token, ct);
            }
            if(syn!=-1)
                leibie = "constant";
            Analy_stack.push("i");
        }
    }
    else if(ch=='\'')
    {
        token += ch;
        ch = prog[p++];
        token += ch;
        ch = prog[p++];
        if(ch=='\'')
        {
            token += ch;
            ch = prog[p++];
            syn = search_string(token, Ct);
            if(syn == -1){
                Ct.push_back(token);
                syn = search_string(token, Ct);
            }
        }
        else{
            token += ch;
            syn = -1;
        }
        if(syn>=0)
            leibie = "Cword";
        Analy_stack.push("3");
    }
    else if(ch=='"')
    {
        token += ch;
        ch = prog[p++];
        while(ch!='"')
        {
            token += ch;
            ch = prog[p++];
        }
        if(ch=='"')
        {
            token += ch;
            ch = prog[p++];
            syn = search_string(token, st);
            if(syn==-1){
                st.push_back(token);
                syn = search_string(token, st);
            }
        }
        else
        {
            syn=-1;
        }
        if(syn!=-1)
            leibie="string";
        Analy_stack.push("4");
    }
    else if(ch=='#'){
        syn=100;
        Analy_stack.push("#");
    }
    else{
        token+=ch;
        char cc = ch;
        ch = prog[p++];
        if((ch == '=' &&(cc=='>'||cc=='<'||cc=='='||cc=='+'||cc=='-'||cc=='*'||cc=='/'||cc=='!'))||((cc==ch)&&(cc=='+'||cc=='-')))
        {
            token+=ch;
            ch = prog[p++];
        }
        syn = search_string(token, pt);
        if(syn!=-1)
            leibie="jiefu";
        Analy_stack.push(token);
    }
}
void E();
void T();
void F();
bool TF=true;
vector<vector<string>> QT;
stack<string> SEM;
int i=1;
void GEQ(string x){
    string b = SEM.top();SEM.pop();
    string a = SEM.top();SEM.pop();
    string c = "t";
    c += (i++ + '0');
    vector<string> temp;
    temp.push_back(x);
    temp.push_back(a);
    temp.push_back(b);
    temp.push_back(c);
    QT.push_back(temp);
    SEM.push(c);
}
void printQT(){
    for(auto i:QT){
        cout<<"( ";
        for(auto j:i){
            cout<<j<<" ";
        }
        cout<<")"<<endl;
    }
}
void PUSH(string x){
    SEM.push(x);
}
void E()
{
    //cout<<"E"<<endl;
    if(TF==true){
        T();
        while(true){
            if(Analy_stack.front()=="+"){
                //cout<<"match "<<token<<endl;
                string x = token;
                Analy_stack.pop();
                scaner();
                T();
                GEQ(x);
            }
            else if(Analy_stack.front()=="-"){
                //cout<<"match "<<token<<endl;
                string x = token;
                Analy_stack.pop();
                scaner();
                T();
                GEQ(x);
            }
            else return ;
        }
    }
}
void T()
{
    //cout<<"T"<<endl;
    if(TF==true){
        F();
        while(true){
            if(Analy_stack.front()=="*"){
            //    cout<<"match "<<token<<endl;
                string x = token;
                Analy_stack.pop();
                scaner();
                F();
                GEQ(x);
            }
            else if(Analy_stack.front()=="/"){
                //cout<<"match "<<token<<endl;
                string x = token;
                Analy_stack.pop();
                scaner();
                F();
                GEQ(x);
            }
            else return ;
        }
    }
}
void F()
{
//    cout<<"F"<<endl;
    if(Analy_stack.front()=="i"){
        //cout<<"match "<<token<<endl;
        Analy_stack.pop();
        PUSH(token);
        scaner();
    }
    else if(Analy_stack.front()=="("){
        //cout<<"match ("<<endl;
        Analy_stack.pop();
        scaner();
        E();
        if(Analy_stack.front()==")"){
        //    cout<<"match )"<<endl;
            scaner();
            Analy_stack.pop();
        }
        else{
            TF = false;
        }
    }
    else
        TF = false;
}
void main_procedure()
{
    scaner();
    E();
    if(TF==true){
        if(Analy_stack.back()=="#"){
            //cout<<"match #"<<endl;
            Analy_stack.pop();
        }
        else {
            TF = false;
        }
    }
}
void recursive_subroutine()
{
    p = 0;
    ReadFile();
    cout<<"文件中的内容："<<endl;
    cout<<prog<<endl;
    p = 0;
    ch = prog[p++];
    i = 1;
    main_procedure();
    if(TF==true)
        cout<<"RIGHT!"<<endl;
    else
        cout<<"WRONG!"<<endl;
    printQT();
}
int main()
{
    recursive_subroutine();
    return 0;
}
