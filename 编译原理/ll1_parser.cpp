#include<bits/stdc++.h>
#include<fcntl.h>
#include<unistd.h>
#include <cctype>
#define MAX 507
using namespace std;
char *prog;
string token;
char ch;
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
    if((fd = open("com.txt", 0)) < 0)
    {
        cout<<"[error] could not open file"<<endl;
        exit(-1);
    }
    if((i = read(fd, prog, size-1)) <= 0)
    {
        cout<<"[error] return "<<i<<endl;
        exit(-1);
    }
    prog[i] = '#';
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
    }
    else if(ch=='#')
        syn=100;
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
    }
}

class WF
{
public:
    string left;
    set<string> right;
    WF ( char s[] )
    {
        left = s;
    }
    void print ( )
    {
        cout<<left<<"->";
        set<string>::iterator it = right.begin();
        if ( it!= right.end() )
        {
            cout<<*it;
            it++;
        }
        for(; it != right.end() ; it++ )
            cout<<"|"<<*it;
        cout<<endl;
    }
    void insert ( char s[] )
    {
        right.insert ( s );
    }
};

map<string,set<char> > first;
map<string,set<char> > follow;
map<string,int> VN_dic;
vector<WF> VN_set;
bool used[MAX];

void dfs ( int x )
{
    if ( used[x] ) return;
    used[x] = 1;
    string& left = VN_set[x].left;
    set<string>& right = VN_set[x].right;
    for (auto it = right.begin(); it!= right.end(); it++)
        for ( int i = 0 ; i < it->length() ; i++ )
        {
            if (!isupper( it->at(i) ) && it->at(i) != '\'')//如果是终结符
            {
                first[left].insert ( it->at(i) );
                break;
            }
            if (isupper( it->at(i)))  //如果是非终结符
            {
                int y;
                if ( i != it->length()-1 && it->at(i+1) == '\'' )
                    y = VN_dic[it->substr(i,2)]-1;
                else y = VN_dic[it->substr(i,1)]-1;
                string& tleft = VN_set[y].left;
                dfs (y);
                set<char>& temp = first[tleft];
                bool flag = true;
                for (auto it1 = temp.begin(); it1 != temp.end() ; it1++)
                {
                    if (*it1 == '~') flag = false;
                    first[left].insert(*it1);
                }
                if ( flag ) break;
            }
            else continue;
        }
}
void make_first ( )
{
    memset ( used , 0 , sizeof ( used ) );
    for ( int i = 0 ; i < VN_set.size() ; i++ )
        dfs ( i );
#define DEBUG
#ifdef DEBUG
        puts ("***************FIRST集***********************");
        for (auto it = first.begin() ; it != first.end() ; it++ )
        {
            printf ( "FIRST(%s)={" , it->first.c_str() );
            set<char> & temp = it->second;
            bool flag = false;
            for (auto it1 = temp.begin() ; it1 != temp.end() ; it1++ )
            {
                if ( flag ) printf ( "," );
                printf ( "%c" , *it1 );
                flag = true;
            }
            cout<<"}"<<endl;
        }
#endif
}
void append ( const string& str1 , const string& str2 )
{
    set<char>& from = follow[str1];
    set<char>& to = follow[str2];
    for (auto it = from.begin(); it != from.end() ; it++)
        to.insert ( *it );
}
void make_follow()
{
    follow["E"].insert('#');
    while (1)
    {
        bool goon = false;
        for (int i = 0 ; i < VN_set.size() ; i++ )
        {
            string& left = VN_set[i].left;
            set<string>& right = VN_set[i].right;
            for (auto it = right.begin(); it!= right.end() ; it++)
            {
                bool flag = true;
                const string& str = *it;
                for (int j = it->length()-1 ; j >= 0 ; j--)
                {
                    if (str[j] == '\'')
                    {
                        int x = VN_dic[it->substr(j-1,2)]-1;
                        if (flag)
                        {
                            int tt = follow[it->substr(j-1,2)].size();
                            append ( left , it->substr(j-1,2) );
                            int tt1 = follow[it->substr(j-1,2)].size();
                            if ( tt1 > tt ) goon = true;
                            if ( !VN_set[x].right.count("~" ) )
                                flag = false;
                        }
                        for ( int k = j+1 ; k < it->length() ; k++ )
                        {
                            if ( isupper(str[k]) )
                            {
                                string id;
                                if ( k != it->length()-1 && str[k+1] == '\'' )
                                    id = it->substr(k,2);
                                else id = it->substr(k,1);
                                set<char>& from = first[id];
                                set<char>& to = follow[it->substr(j-1,2)];
                                int tt = to.size();
                                set<char>::iterator it1 = from.begin();
                                for ( ; it1 != from.end() ; it1++ )
                                    if ( *it1 != '~' )
                                        to.insert ( *it1 );
                                int tt1 = follow[it->substr(j-1,2)].size();
                                if ( tt1 > tt ) goon = true;
                                if ( !VN_set[VN_dic[id]-1].right.count("~") )
                                    break;
                            }
                            else if ( str[k] != '\'' )
                            {
                                int tt = follow[it->substr(j-1,2)].size();
                                follow[it->substr(j-1,2)].insert ( str[k] );
                                int tt1 = follow[it->substr(j-1,2)].size();
                                if ( tt1 > tt )
                                    goon = true;
                                break;
                            }
                            else continue;
                        }
                        j--;
                    }
                    else if ( isupper(str[j] ) )
                    {
                        int x = VN_dic[it->substr(j,1)]-1;
                        if ( flag )
                        {
                            int tt = follow[it->substr(j,1)].size();
                            append ( left , it->substr(j,1) );
                            if ( !VN_set[x].right.count("~") )
                                flag = false;
                            int tt1 = follow[it->substr(j,1)].size();
                            if ( tt1 > tt ) goon = true;
                        }
                        for ( int k = j+1 ; k < it->length() ; k++ )
                        {
                            if ( isupper( str[k] ) )
                            {
                                string id;
                                if ( k != it->length()-1 && str[k+1] == '\'' )
                                    id = it->substr(k,2);
                                else id = it->substr(k,1);
                                set<char>& from = first[id];
                                set<char>& to = follow[it->substr(j,1)];
                                set<char>::iterator it1 = from.begin();
                                int tt = follow[it->substr(j,1)].size();
                                for ( ; it1 != from.end() ; it1++ )
                                    if ( *it1 != '~' )
                                        to.insert( *it1 );
                                int tt1 = follow[it->substr(j,1)].size();
                                if ( tt1 > tt ) goon = true;
                                if ( !VN_set[VN_dic[id]-1].right.count("~") )
                                    break;
                            }
                            else if ( str[k] != '\'' )
                            {
                                int tt = follow[it->substr(j,1)].size();
                                follow[it->substr(j,1)].insert ( str[k] );
                                int tt1 = follow[it->substr(j,1)].size();
                                if ( tt1 > tt ) goon = true;
                                break;
                            }
                            else continue;
                        }
                    }
                    else flag = false;
                }
            }
        }
        if ( !goon ) break;
    }
#define DEBUG
#ifdef DEBUG
    puts ("****************FOLLOW集**********************" );
    map<string,set<char> >::iterator it = follow.begin();
    for ( ; it != follow.end() ; it++ )
    {
        printf ( "FOLLOW(%s)={" , it->first.c_str() );
        set<char> & temp = it->second;
        //temp.insert('#');
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for ( ; it1 != temp.end() ; it1++ )
        {
            if ( flag ) printf ( "," );
            printf ( "%c" , *it1 );
            flag = true;
        }
        puts ("}");
    }
#endif
}
vector<map<char,string> > predict_table;
//检查一个字符是否属于一个字符串的FIRST集合
bool check_first ( const string& text , char ch )
{
    for ( int i = 0 ; i < text.length() ; i++ )
    {
        bool hasEmpty = false;
        if ( !isupper(text[i]) && text[i] != '\'' )
        {
            if ( text[i] != ch ) return false;
            else return true;
        }
        else if ( isupper(text[i] ) )
        {
            string temp;
            if ( i != text.length()-1 && text[i+1] == '\'' )
                temp = text.substr(i,2);
            else
                temp = text.substr(i,1);
            set<char>& dic = first[temp];
            set<char>::iterator it = dic.begin();
            for (auto it:first[temp])
            {
                if ( it == '~' ) hasEmpty = true;
                if ( it == ch ) return true;
            }
            if ( !hasEmpty) break;
        }
        else continue;
    }
    return false;
}
//检查一个字符是否属于一个字符串的FOLLOW集合
bool check_follow ( const string& text , char ch )
{
    set<char>& dic = follow[text];
    set<char>::iterator it = dic.begin();
    for ( ; it != dic.end() ; it++ )
        if ( *it == ch ) return true;
    return false;
}
void make_table ()
{
    map<char,string> temp;
    vector<char> letter;
    bool vis[500];
    memset ( vis , 0 , sizeof ( vis ) );
    letter.push_back('#');
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
        set<string>& right = VN_set[i].right;
        set<string>::iterator it = right.begin();
        for ( ; it != right.end() ; it++ )
            for ( int j = 0 ; j < it->length() ; j++ )
                if ( !isupper(it->at(j)) && it->at(j) != '\'' )
                {
                    if ( vis[it->at(j)] ) continue;
                    vis[it->at(j)] = true;
                    if(it->at(j) != '~')
                        letter.push_back ( it->at(j) );
                }
    }
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
        temp.clear();
        string& left = VN_set[i].left;
        set<string>& right = VN_set[i].right;
        set<string>::iterator it = right.begin();
        for ( ; it != right.end() ; it++ )
            for ( int j = 0 ; j < letter.size() ; j++ )
            {
                //cout << *it << " " <<  letter[j] << endl;
                if ( check_first ( *it , letter[j] ) )
                {
                    //cout << "YES" << endl;
                    temp[letter[j]] = *it;
                }
                if ( check_first(*it, '~') && check_follow ( left, letter[j] ))
                    temp[letter[j]] = *it;
            }
        predict_table.push_back ( temp );
    }
#define DEBUG
#ifdef DEBUG
    for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
        printf ( "-" );
    puts ("");
    printf ( "|%9s" , "|" );
    for ( int i = 0 ; i < letter.size() ; i++ )
        printf ( "%5c%5s" , letter[i] , "|" );
    puts("");
    for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
        printf ( "-" );
    puts("");
    for ( int i = 0 ; i < VN_set.size() ; i++ )
    {
        printf ( "|%5s%4s" , VN_set[i].left.c_str() , "|" );
        for ( int j = 0 ; j < letter.size() ; j ++ )
            if ( predict_table[i].count(letter[j] ) )
                printf ( "%7s%3s" , predict_table[i][letter[j]].c_str() , "|" );
            else printf ( "%10s" , "|" );
        puts("");
        for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
            printf ( "-" );
        puts ("");
    }
#endif
}
int main ( )
{
    freopen("wenfa.txt","r",stdin);
    int n=8;
    char s[MAX];
    for ( int i = 0 ; i < n ; i++ )
    {
        scanf ( "%s" , s );
        int len = strlen ( s ),j;
        for (j = 0 ; j < len ; j++)
            if ( s[j] == '-' ) break;
        s[j] = 0;
        if (!VN_dic[s])
        {
            VN_set.push_back ( s );
            VN_dic[s] = VN_set.size();
        }
        int x = VN_dic[s]-1;
        VN_set[x].insert ( s+j+2 );
    }
    freopen("select.txt","w",stdout);
    for(auto i:VN_set){
        i.print();
    }
    make_first();
    make_follow();
    make_table();

}
