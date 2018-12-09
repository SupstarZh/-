#include"Grammer_analyzer.h"
using namespace std;
void Grammer_analyzer::DFS(int x){ //通过深度优先查找每个非终结符的first集合
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
void Grammer_analyzer::changetoLL1(){ //识别并改正非LL1文法
    for(int i =0; i<Vn_set.size(); i++){
        for(auto j = Vn_set[i].productions.begin(); j!=Vn_set[i].productions.end(); j++){
            for(auto k=Vn_set[i].productions.begin(); k!=Vn_set[i].productions.end(); k++){
                if( (*j)[0]==(*k)[0] && *j!=*k ){ //如果两个文法产生式首部相同
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
    for(int i =0; i<Vn_set.size(); i++){ //消除左递归文法
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
void Grammer_analyzer::make_first(){ //生成first集
    memset(is_used, 0, sizeof(is_used));
    for(int i=0; i<Vn_set.size(); i++)
        DFS(i);
}
void Grammer_analyzer::print_firstset(){ //将first集合输出到文件
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
void Grammer_analyzer::append(const string& str1, const string& str2){ //将str1的follow复制到str2follow集合中
    set<char>& from = follow[str1];
    set<char>& to = follow[str2];
    for(auto it = from.begin(); it!=from.end(); it++)
        to.insert(*it);
}
void Grammer_analyzer::make_follow(){  //生成follow集合
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
void Grammer_analyzer::print_followset(){//将follow集合打印到文件中去
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
bool Grammer_analyzer::check_first(const string& production, char ch){ //检测字符是否在产生式的first集合里
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
bool Grammer_analyzer::check_follow(const string& production, char ch){ //检测字符是否在产生式的follow集合里
    for(auto i: follow[production])
        if(i == ch) return true;
    return false;
}
void Grammer_analyzer::translateto(vector<char> letter){
    map<char, string> temp;
    for(int i=0; i<Vn_set.size(); i++){
        temp.clear();
        for(int j=0; j<letter.size(); j++)
            if(Predict_table[i].count(letter[j])){
                string tp = Predict_table[i][letter[j]];
                string tp1, tp2, str;
                bool flag=1;
                str+=tp[0];
                if(str=="+"||str=="-"||str=="/"||str=="*"){
                    tp1+=tp[0];
                    tp1+=tp[1];
                    if(tp[2]=='\''){
                        tp1+=tp[2];
                        flag = 0;
                    }
                    int k = 2;
                    if(!flag)
                        k = 3;
                    for(; k<tp.size(); k++)
                        tp2+=tp[k];
                    string ss = tp1+'@'+tp[0]+tp2;
                    temp[letter[j]] = ss;
                }
                else if(str == "i"){
                    string tp = Predict_table[i][letter[j]];
                    tp += '$';
                    temp[letter[j]] = tp;
                }
                else{
                    temp[letter[j]] = Predict_table[i][letter[j]];
                }
            }
        Translationtable.push_back(temp);
    }
}
void Grammer_analyzer::make_predict_table(){ //生成分析表并输出到文件中
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
    translateto(letter);
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
            if(Translationtable[i].count(letter[j])){
                out<<setw(7)<<Translationtable[i][letter[j]];
                out<<"  |";
            }
                //printf("%7s%3s", Predict_table[i][letter[j]].c_str(), "|");
            else out<<"         |";
        out<<endl;
        for(int i=0; i<(letter.size()+1)*10; i++)
            out<<"-";
        out<<endl;
    }
    out.close();
}
void Grammer_analyzer::Readwenfa() {
    int size = 1000;
    int fd,i;
    wf = (char *)malloc(size);
    if(!wf)
    {
        cout<<"[error] could not malloc"<<endl;
        exit(-1);
    }
    cout<<endl;
    if((fd = open("/home/zhuowenjie/CLionProjects/untitled1/wenfa.txt", 0)) < 0)
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
void Grammer_analyzer::Putinwf() {
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
bool Grammer_analyzer::Is_Vn(string x){
    for(auto i: Vn_set){
        if(x == i.Vnot)
            return true;
    }
    return false;
}
int Grammer_analyzer::Is_match(string x, string m){ //查找分析表
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
void Grammer_analyzer::LL1_Run() {
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
    string x, m, tem; // x为栈顶符号，m为当前符号
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
        if(!isupper(x[0]) && x[0] != '\'' && x[0]!='#' && x!="$" && x!="@"){
            if(x == m)
            {
                tem = token;
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
        else if(x == "@"){
            x.clear();
            x = Synax_stack.top();
            Synax_stack.pop();
            GEQ(x);
            x.clear();
            x = Synax_stack.top();
            Synax_stack.pop();
        }
        else if(x == "$"){
            SEM.push(tem);
            tem.clear();
            x.clear();
            x = Synax_stack.top();
            Synax_stack.pop();
        }
        else if(Is_Vn(x)){
            int i = Is_match(x, m);
            if(i!=-1){
                for(auto j=Translationtable[i][m[0]].rbegin(); j!=Translationtable[i][m[0]].rend(); j++)
                {
                    string tem;
                    tem=*j;
                    Synax_stack.push(tem);
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
    printQT();
}//LL1分析器
void Grammer_analyzer::GEQ(string x){ //翻译动作
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
void Grammer_analyzer::printQT(){ //打印四元式序列
    for(auto i:QT){
        cout<<"( ";
        for(auto j:i){
            cout<<j<<" ";
        }
        cout<<")"<<endl;
    }
}
void Grammer_analyzer::PUSH(string x){ //将变量压入语义栈
    SEM.push(x);
}
void Grammer_analyzer::E()
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
void Grammer_analyzer::T()
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
void Grammer_analyzer::F()
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
void Grammer_analyzer::main_procedure() //递归子程序主函数
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
void Grammer_analyzer::recursive_subroutineRun() //递归子程序分析器
{
    p = 0;
    ReadFile();
    cout << "文件中的内容：" << endl;
    cout << prog << endl;
    p = 0;
    ch = prog[p++];
    main_procedure();
    if(TF==true)
        cout<<"RIGHT!"<<endl;
    else
        cout<<"WRONG!"<<endl;
    printQT();
    cout<<"The Grammer_analyzer has finished the task"<<endl;
}