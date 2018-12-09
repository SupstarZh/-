#include "Lex_analyzer.h"
using namespace std;
void Lex_analyzer::ReadFile()
{
    int size = 1000;
    int fd,i;
    prog = (char *)malloc(size);
    if(!prog)
    {
        cout<<"[error] could not malloc"<<endl;
        exit(-1);
    }
    if((fd = open("/home/zhuowenjie/CLionProjects/untitled1/com.txt", 0)) < 0)
    {
        cout<<"[error] could not open file"<<endl;
        exit(-1);
    }
    if((i = read(fd, prog, size-1)) <= 0)
    {
        cout<<"[error] return "<<i<<endl;
        exit(-1);
    }
    prog[i-1] = '#';  //扫描结束的符号
    YN=1;
    prog[i] = '\0';
}
void Lex_analyzer::initprog(){
    p = 0;
    ReadFile();
}
int Lex_analyzer::search_string(string tem, vector<string> &st)//寻找字符串string在不在这个字符串表里
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
bool Lex_analyzer::Is_Num(char c)
{
    if(c>='0'&&c<='9')
        return true;
    else
        return false;
}
bool Lex_analyzer::Is_Letter(char c)
{
    if((ch>='a'&&c<='z')||(c>='A'&&c<='Z'))
        return true;
    else
        return false;
}
bool Lex_analyzer::Is_constant(string st)//判断这个字符串是不是常数，利用状态转移
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
            }
            else{
                return false;
            }
        }
        else if(state==2)
        {
            if(st[i]>='0'&&st[i]<='9'){}
            else if(st[i]=='.')
            {
                state=3;
            }
            else if(st[i]=='e')
            {
                state=5;
            }
            else if(st[i]=='\0')
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if(state==3)
        {
            if(st[i]>='0'&&st[i]<='9')
            {
                state=4;
            }
            else
            {
                return false;
            }
        }
        else if(state==4)
        {
            if(st[i]>='0'&&st[i]<='9')
            {

            }
            else if(st[i]=='e')
            {
                state=5;
            }
            else if(st[i]=='\0')
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else if(state==5)
        {
            if(st[i]>='0'&&st[i]<='9')
            {
                state=7;
            }
            else if(st[i]=='+'||st[i]=='-')
            {
                state=6;
            }
        }
        else if(state==6)
        {
            if(st[i]>='0'&&st[i]<='9')
            {
                state=7;
            }
            else
            {
                return false;
            }
        }
        else if(state==7)
        {
            if(st[i]>='0'&&st[i]<='9')
            {

            }
            else if(st[i]=='\0')
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        i++;
    }
    return false;
}
void Lex_analyzer::scaner()//扫描单词串
{
    token.clear();  //每次扫描前清空token串
    leibie.clear();
    while(ch == ' '||ch =='\n'||ch=='\r'){
        if(ch=='\n')
            row++;
        ch = prog[p++];
    }
    if(Is_Letter(ch)) //判断首字符是否是字母
    {
        while(Is_Letter(ch) || Is_Num(ch))
        {
            token += ch;
            ch = prog[p++];
        }
        syn = search_string(token, kt);//在关键字表中查找
        if(syn!=-1)
            leibie="keyword";
        if(syn == -1){
            syn = search_string(token, it);//在标识符表中查找
            if(syn == -1){ //若没找到就加入到标识符表中
                it.push_back(token);
                syn = search_string(token, it);
            }
            if(syn!=-1)
                leibie = "identyword";
            Analy_stack.push("i");
            inputString.push_back('i');
        }
    }
    else if(Is_Num(ch)) //判断常数
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
            inputString.push_back('i');
        }
        token.pop_back();
    }
    else if(ch=='\'') //判断字符
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
    else if(ch=='"') //判断字符串
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
    else if(ch=='#'){ //是否是结束符
        syn=100;
        Analy_stack.push("#");
        inputString.push_back('#');
    }
    else{ //是否是界符
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
        inputString.push_back(token[0]);
        Analy_stack.push(token);
    }
}
void Lex_analyzer::Showfile(){
    p = 0;
    cout<<"文件中的内容："<<endl;
    cout<<prog<<endl;
}
void Lex_analyzer::PutTokens(){
    p = 0;
    ch = prog[p++];
    do {
        scaner();
        switch(syn)
        {
            case -1: cout<<"Error in row"<<row<<"!"<<endl;
                break;
                break;
            case 100: cout<<"The Lex_analyzer has finished the task"<<endl;
                break;
            default: cout<<"("<<token<<","<<type<<","<<syn<<")"<<endl;
                break;
        }
    } while(syn!=100);
}
void Lex_analyzer::Lex_analyzer_Run()//运行词法分析器
{
    p = 0;
    ReadFile();
    cout<<"文件中的内容："<<endl;
    cout<<prog<<endl;
    p = 0;
    ch = prog[p++];
    do {
        scaner();
        switch(syn)
        {
            case -1: cout<<"Error in row"<<row<<"!"<<endl;
                break;
                break;
            case 100: cout<<"The Lex_analyzer has finished the task"<<endl;
                break;
            default: cout<<"("<<token<<","<<leibie<<","<<syn<<")"<<endl;
                break;
        }
    } while(syn!=100);
}