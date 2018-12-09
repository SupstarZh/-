#include<bits/stdc++.h>
#include<fcntl.h>
#include<unistd.h>
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
            if(st[i]>='0'&&st[i]<='9')
            {

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
            if(st[i]>='0'&&st[i]<='9')
            {

            }else if(st[i]=='\0')
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
class Wenfa{
public:
    string Vnot;
    set<string> productions;
    Wenfa(char s[]){
        Vnot = s;
    }
    void print(){
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
map<string, set<char>> first, follow;
vector<map<char,string>> Predict_table;
map<string, int> Vn_dic;
vector<Wenfa> Vn_set;
bool is_used[1000];
char *wf;
void changetoLL1(){
    for(int i =0; i<Vn_set.size(); i++){
        for(auto j = Vn_set[i].productions.begin(); j!=Vn_set[i].productions.end(); j++){
            for(auto k=Vn_set[i].productions.begin(); k!=Vn_set[i].productions.end(); k++){
                if( (*j)[0]==(*k)[0] && *j!=*k ){
                    char a[100];
                    char b[100];
                    memset(a, 0, sizeof(a));
                    memset(b, 0, sizeof(b));
                    int ss = 0;
                    for(auto q:Vn_set[i].Vnot){
                        a[ss] = q;
                        b[ss+1] = q;
                        ss++;
                    }
                    a[ss] = '\'';
                    b[0] = (*j)[0];
                    b[ss+1] = a[ss];
                    if(!Vn_dic[a]){
                        Vn_set.push_back(a);
                        Vn_dic[a] = Vn_set.size();
                    }
                    int x = Vn_dic[a]-1;
                    string cc = (*j).substr(1);
                    memset(a, 0, sizeof(a));
                    int sz=0;
                    for(auto q:cc){
                        a[sz++] = q;
                    }
                    Vn_set[x].insert(a);
                    string dd = (*k).substr(1);
                    memset(a, 0, sizeof(a));
                    sz=0;
                    for(auto q:dd){
                        a[sz++] = q;
                    }
                    Vn_set[x].insert(a);
                    Vn_set[i].productions.erase((*j));
                    Vn_set[i].productions.erase((*k));
                    Vn_set[i].insert(b);
                }
            }
        }
    }
    for(int i =0; i<Vn_set.size(); i++){
        for(auto j = Vn_set[i].productions.begin(); j!=Vn_set[i].productions.end(); j++){
            string fir;
            fir+=(*j)[0];
            if((*j)[1]=='\'')
                fir+=(*j)[1];
            if(fir == Vn_set[i].Vnot){
                char a[100];
                int sz=0;
                memset(a, 0, sizeof(a));
                int k = 1;
                if((*j)[k]=='\'')
                    k++;
                for(; k<(*j).size(); k++){
                    a[sz++] = (*j)[k];
                }
                char b[10];
                int sz1 = 0;
                for(auto k:Vn_set[i].Vnot){
                     b[sz1++]=k;
                 }
                b[0]+=1;
                for(auto k = Vn_set[i].productions.begin(); k!=Vn_set[i].productions.end(); k++){
                    if(k!=j){
                        string as = *k;
                        Vn_set[i].productions.erase(*k);
                        for(int e = 0; e<sz1; e++){
                            as+=b[e];
                        }
                        int sz2=0;
                        char c[100];
                        memset(c, 0, sizeof(c));
                        for(auto z:as){
                            c[sz2++] = z;
                        }
                        Vn_set[i].insert(c);
                    }
                }
                for(int e=0; e<sz1; e++){
                    a[sz++] = b[e];
                }
                if(!Vn_dic[b]){
                    Vn_set.push_back(b);
                    Vn_dic[b] = Vn_set.size();
                }
                int x = Vn_dic[b] - 1;
                Vn_set[x].insert(a);
                Vn_set[x].insert("e");
                Vn_set[i].productions.erase((*j));
            }
        }
    }
}
void DFS(int x){
    if(is_used[x]) return ;
    is_used[x] = 1;
    string& left = Vn_set[x].Vnot;
    set<string>& right = Vn_set[x].productions;
    for(auto it = right.begin(); it!=right.end(); it++){
        for(int i=0; i<it->length(); i++){
            if(!isupper(it->at(i))&&it->at(i)!='\''){ //如果是终结符
                first[left].insert(it->at(i));
                break;
            }
            if(isupper(it->at(i))){ //如果是非终结符
                int y;
                if(i!=it->length()-1 && it->at(i+1) == '\'') //检测终结符是否带’
                    y = Vn_dic[it->substr(i,2)] - 1;
                else y = Vn_dic[it->substr(i,1)]-1;
                string& tleft = Vn_set[y].Vnot;
                DFS(y); //递归求解该非终结符的first集
                set<char>& temp = first[tleft];
                bool flag = true;
                for(auto it1 = temp.begin(); it1!=temp.end(); it1++){
                    if(*it1 == 'e') flag = false;
                    first[left].insert(*it1);
                }
                if(flag) //如果该非终结符first集合不含空符号
                    break;
            }
            else continue;
        }
    }
}
void make_first(){ //生成first集
    memset(is_used, 0, sizeof(is_used));
    for(int i=0; i<Vn_set.size(); i++)
        DFS(i);
}
void print_firstset(){
    ofstream out("first_set.txt");
    out<<"非终结符的first集合:"<<endl;
    for(auto it=first.begin(); it!=first.end(); it++){
        out<<"first("<<it->first<<")={";
        set<char>& temp = it->second;
        bool flag = false;
        for(auto it1=temp.begin(); it1!=temp.end(); it1++){
            if(flag) out<<",";
            out<<*it1;
            flag = true;
        }
        out<<"}"<<endl;
    }
    out.close();
}
void append(const string& str1, const string& str2){
    set<char>& from = follow[str1];
    set<char>& to = follow[str2];
    for(auto it = from.begin(); it!=from.end(); it++)
        to.insert(*it);
}
void make_follow(){  //生成follow集合
    string a;
    a+=wf[0];
    follow[a].insert('#');
    while(1){
        bool goon = false;
        for(int i=0; i<Vn_set.size(); i++){
            string& left = Vn_set[i].Vnot;
            set<string>& right = Vn_set[i].productions;
            for(auto it=right.begin(); it!=right.end(); it++){
                bool flag = true;
                const string& str = *it;
                for(int j = it->length()-1; j>=0; j--){
                    if(str[j] == '\''){
                        int x = Vn_dic[it->substr(j-1, 2)]-1;
                        if(flag){  //若最后非终结符不能推出空就不再执行
                            int tt = follow[it->substr(j-1,2)].size();
                            append(left, it->substr(j-1, 2));
                            int tt1 = follow[it->substr(j-1,2)].size();
                            if(tt1>tt) goon = true; //如果follow集变大了 就继续
                            if(!Vn_set[x].productions.count("e")) flag = false;
                        }
                        for(int k=j+1; k<it->length(); k++){
                            if(isupper(str[k])){
                                string id;
                                if(k!=it->length()-1 && str[k+1]=='\'')
                                    id = it->substr(k,2);
                                else id = it->substr(k,1);
                                set<char>& from = first[id];
                                set<char>& to = follow[it->substr(j-1, 2)];
                                int tt = to.size();
                                for(auto it1=from.begin(); it1!=from.end(); it1++){
                                    if(*it1!='e')
                                        to.insert(*it1);
                                }
                                int tt1= follow[it->substr(j-1, 2)].size();
                                if(tt1>tt) goon = true;
                                if(!Vn_set[Vn_dic[id]-1].productions.count("e"))
                                    break;
                            }
                            else if(str[k]!='\''){
                                int tt = follow[it->substr(j-1,2)].size();
                                follow[it->substr(j-1, 2)].insert(str[k]);
                                int tt1 = follow[it->substr(j-1,2)].size();
                                if(tt1>tt) goon = true;
                                break;
                            }
                            else continue;
                        }
                        j--;
                    }
                    else if(isupper(str[j])){
                        int x = Vn_dic[it->substr(j,1)]-1;
                        if(flag){
                            int tt = follow[it->substr(j,1)].size();
                            append(left, it->substr(j,1));
                            if(!Vn_set[x].productions.count("e")) flag = false;
                            int tt1 = follow[it->substr(j, 1)].size();
                            if(tt1>tt) goon=true;
                        }
                        for(int k=j+1; k<it->length(); k++){
                            if(isupper(str[k])){
                                string id;
                                if(k!=it->length()-1 && str[k+1]=='\'')
                                    id = it->substr(k, 2);
                                else id = it->substr(k,1);
                                set<char>& from = first[id];
                                set<char>& to = follow[it->substr(j,1)];
                                int tt = follow[it->substr(j,1)].size();
                                for(auto it1 = from.begin(); it1!=from.end(); it1++){
                                    if(*it1!='e')
                                        to.insert(*it1);
                                }
                                int tt1 = follow[it->substr(j,1)].size();
                                if(tt1>tt) goon = true;
                                if(!Vn_set[Vn_dic[id]-1].productions.count("e"))
                                    break;
                            }
                            else if(str[k]!='\''){
                                int tt = follow[it->substr(j,1)].size();
                                follow[it->substr(j,1)].insert(str[k]);
                                int tt1 = follow[it->substr(j,1)].size();
                                if(tt1>tt) goon = true;
                                break;
                            }
                            else continue;
                        }
                    }
                    else flag = false;
                }
            }
        }
        if(!goon) break;
    }
}
void print_followset(){
    ofstream out("follow.txt");
    out<<"非终结符的follow集合:"<<endl;
    for(auto it=follow.begin(); it!=follow.end(); it++){
        out<<"follow("<<it->first<<")={";
        set<char>& temp = it->second;
        bool flag = false;
        for(auto it1=temp.begin(); it1!=temp.end(); it1++){
            if(flag) out<<",";
            out<<*it1;
            flag = true;
        }
        out<<"}"<<endl;
    }
    out.close();
}
bool check_first(const string& production, char ch){ //检测字符是否在产生式的first集合里
    for(int i=0; i<production.length(); i++){
        bool hasEmpty = false;
        if(!isupper(production[i])&&production[i]!='\''){
            if(production[i]!=ch) return false;
            else return true;
        }
        else if(isupper(production[i])){
            string temp;
            if(i!=production.length()-1&&production[i+1]=='\'')
                temp = production.substr(i,2);
            else temp = production.substr(i,1);
            for(auto i:first[temp]){
                if(i == 'e') hasEmpty = true;
                if(i == ch) return true;
            }
            if(!hasEmpty) break;
        }
        else continue;
    }
    return false;
}
bool check_follow(const string& production, char ch){ //检测字符是否在产生式的follow集合里
    for(auto i: follow[production])
        if(i == ch) return true;
    return false;
}
void make_predict_table(){
    map<char, string> temp;
    vector<char>letter;
    bool vis[500];
    memset(vis, 0, sizeof(vis));
    letter.push_back('#');
    for(int i=0; i<Vn_set.size(); i++){
        set<string>& right = Vn_set[i].productions;
        for(auto it = right.begin(); it!=right.end(); it++)
            for(int j=0; j<it->length(); j++)
                if(!isupper(it->at(j)) && it->at(j)!='\''){
                    if(vis[it->at(j)]) continue;
                    vis[it->at(j)] = true;
                    if(it->at(j)!='e')
                        letter.push_back(it->at(j));
                }
    }
    for(int i=0; i<Vn_set.size(); i++){
        temp.clear();
        string& left = Vn_set[i].Vnot;
        set<string>& right = Vn_set[i].productions;
        for(auto it=right.begin(); it!=right.end(); it++)
            for(int j=0; j<letter.size(); j++)
            {
                if(check_first(*it, letter[j]))
                    temp[letter[j]] = *it;
                if(check_first(*it, 'e')&& check_follow(left, letter[j]))
                    temp[letter[j]] = *it;
            }
        Predict_table.push_back(temp);
    }
    ofstream out("select.txt");
    out<<"文法的分析表:"<<endl;
    for(int i=0; i<(letter.size()+1)*10; i++)
        out<<"-";
    out<<endl;
    out<<"|        |";
    for(int i=0; i<letter.size(); i++)
        out<<"    "<<letter[i]<<"    |";
    out<<endl;
    for(int i=0; i<(letter.size()+1)*10; i++)
        out<<"-";
    out<<endl;
    for(int i=0; i<Vn_set.size(); i++){
        out<<"|"<<setw(5)<<Vn_set[i].Vnot;
        out<<right<<"   |";
        for(int j=0; j<letter.size(); j++)
            if(Predict_table[i].count(letter[j])){
                out<<setw(7)<<Predict_table[i][letter[j]];
                out<<"  |";
            }
            else out<<"         |";
        out<<endl;
        for(int i=0; i<(letter.size()+1)*10; i++)
            out<<"-";
        out<<endl;
    }
    out.close();
}
void Readwenfa()
{
    int size = 1000;
    int fd,i;
    wf = (char *)malloc(size);
    if(!wf)
    {
        cout<<"[error] could not malloc"<<endl;
        exit(-1);
    }
    if((fd = open("wenfa.txt", 0)) < 0)
    {
        cout<<"[error] could not open Grammer file"<<endl;
        exit(-1);
    }
    if((i = read(fd, wf, size-1)) <= 0)
    {
        cout<<"[error] return "<<i<<endl;
        exit(-1);
    }
}
void Putinwf()
{
    char *p = wf;
    while(*p!='\0'){
        int i = 0;
        char a[1000];
        memset(a, 0, sizeof(a));
        while(!(*p=='-'&&*(p+1)=='>')){
            a[i++] = *p;
            p++;
        }
        if(!Vn_dic[a]){
            Vn_set.push_back(a);
            Vn_dic[a] = Vn_set.size();
        }
        int x = Vn_dic[a]-1;
        p+=2;
        memset(a, 0, sizeof(a));i=0;
        while(*p!='\n'){
            a[i++] = *p;
            p++;
        }
        Vn_set[x].insert(a);
        p++;
    }
    changetoLL1();
    for(auto i:Vn_set){
        i.print();
    }
}
bool Is_Vn(string x){
    for(auto i: Vn_set){
        if(x == i.Vnot)
            return true;
    }
    return false;
}
int Is_match(string x, string m){
    for(int i=0; i<Vn_set.size(); i++){
        if(x == Vn_set[i].Vnot){
            if(!Predict_table[i][m[0]].empty())
                return i;
            else
                return -1;
        }
    }
    return -1;
}
void Lex_analyzer_Run()
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
void LL1_Run()
{
    Readwenfa();
    Putinwf();
    make_first();
    print_firstset();
    make_follow();
    print_followset();
    cout.flush();
    make_predict_table();
    Synax_stack.push("#");
    Synax_stack.push("E");
    p = 0;
    ReadFile();
    cout<<"文件中的内容："<<endl;
    cout<<prog<<endl;
    p = 0;
    ch = prog[p++];
    scaner();
    int flag=0;
    string x, m; // x为栈顶符号，m为当前符号
    x = Synax_stack.top();
    m = Analy_stack.front();
    Synax_stack.pop();
    Analy_stack.pop();
    if(x=="e"){
        x = Synax_stack.top();
        Synax_stack.pop();
    }
    cout<<"LL(1) analysis:"<<endl;
    //cout<<"ananysis stack     x      w      remain"<<endl;
    while(1){
        cout<<x<<"  "<<m<<endl;
        if(!isupper(x[0]) && x[0] != '\'' && x[0]!='#'){
            if(x == m)
            {
                m.clear();
                x.clear();
                scaner();
                m = Analy_stack.front();
                Analy_stack.pop();
                x = Synax_stack.top();
                Synax_stack.pop();
                if(x=="e"){
                    x = Synax_stack.top();
                    Synax_stack.pop();
                }
            }
            else{
                flag=1;
                break;
            }
        }
        else if(Is_Vn(x)){
            int i = Is_match(x, m);
            if(i!=-1){
                for(auto j=Predict_table[i][m[0]].rbegin(); j!=Predict_table[i][m[0]].rend(); j++)
                {
                    if(*j == '\''){
                        string tem;
                        tem+=*(j+1);
                        tem+=*j;
                        Synax_stack.push(tem);
                        j++;
                    }
                    else{
                        string tem;
                        tem=*j;
                        Synax_stack.push(tem);
                    }
                }
                x.clear();
                x = Synax_stack.top();
                Synax_stack.pop();
                if(x=="e"){
                    x = Synax_stack.top();
                    Synax_stack.pop();
                }
            }
            else{
                flag=1;
                break;
            }
        }
        else if(m!="#"){
            flag = 1;
            break;
        }
        else if(m=="#"){
            flag = 0;
            break;
        }
    }
    if(flag==0)
        cout<<"RIGHT!"<<endl;
    else
        cout<<"ERROR!"<<endl;
}
int main()
{
    LL1_Run();
    return 0;
}
