#include<bits/stdc++.h>
using namespace std;
inline int read()
{
    int x=0,f=1;
    char ch=getchar();
    if(ch==EOF) return 0;
    while(!isdigit(ch))
    {
        if(ch=='-') f=-1;
        ch=getchar();
    }
    while(isdigit(ch))
    {
        x=x*10+ch-'0';
        ch=getchar();
    }
    return x*f;
}
int main()
{
    int n=read();
    bool flag=false;
    while(true)
    {
        int a=read(),b=read();
        if(a==0) break;
        if(a==1&&flag) putchar('\n'),flag=false;
        else if(a!=1) flag=true;
        cout<<a<<' '<<b<<endl;
    }
    return 0;
}