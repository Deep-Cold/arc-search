#include<bits/stdc++.h>
using namespace std;
const int N=15,BOUND=9;
typedef long long ll;
typedef pair<int,int> PII;
struct Vertex
{
    int id;
    int type; //blue node: 1, pink node: 2, white node: 0
    Vertex()
    {

    }
    Vertex(int vid)
    {
        id=vid;
    }
}oriv[N];
vector<Vertex> curv;
struct Graph
{
    int siz,etot;
    vector<vector<bool>> edges;
    vector<int> esiz;
    Graph()
    {
        edges.clear();
    }
    Graph(int n)
    {
        siz=n;
        edges.resize(n+1);
        for(int i=1;i<=n;i++)
        {
            edges[i].resize(n+1);
            for(int j=1;j<=n;j++)
                edges[i][j]=false;
        }
    }
    inline void Output()
    {
        for(int i=1;i<=siz;i++)
        {
            for(int j=1;j<=siz;j++)
                if(edges[i][j])
                    printf("%d %d | ",i,j);
            putchar('\n');
        }
    }
    inline Graph Getaux() const // need update: only consider orin
    {
        int curn=siz;
        Graph aux=Graph(curn);
        for(int i=1;i<=curn;i++)
        {
            Vertex vi=curv[i-1];
            for(int j=i+1;j<=curn;j++)
            {
                Vertex vj=curv[j-1];
                ll vic=1<<i-1,vjc=1<<j-1;
                for(int k=1;k<=curn;k++)
                {
                    if(edges[i][k]) vic|=1<<k-1;
                    if(edges[j][k]) vjc|=1<<k-1;
                }
                if(vi.type==1&&vj.type==1)
                {
                    assert(edges[i][j]);
                    if((vic|vjc)!=((1<<curn)-1)) aux.edges[i][j]=aux.edges[j][i]=true; 
                }
                else if(vi.type==1||vj.type==1)
                {
                    if(vi.type==2) swap(vic,vjc);
                    if((vjc|vic)!=vic) aux.edges[i][j]=aux.edges[j][i]=true;
                }
                else aux.edges[i][j]=aux.edges[j][i]=edges[i][j];
            }
        }
        return aux;
    }
    inline bool CheckConnect() const
    {
        ll vis=1;
        queue<int> q;
        int n=siz;
        while(q.size()) q.pop();
        q.push(1);
        while(q.size())
        {
            auto t=q.front();
            q.pop();
            for(int i=1;i<=n;i++)
            {
                if(!edges[t][i]) continue;
                if(vis>>i-1&1^1) q.push(i),vis|=1<<i-1;
            }
        }
        return vis==(1<<n)-1;
    }
    inline void UpdateESiz()
    {
        int n=siz;
        etot=0;
        esiz.clear();
        esiz.resize(n+1,0);
        for(int i=1;i<=siz;i++)
            for(int j=1;j<=siz;j++)
                if(edges[i][j]) esiz[i]++,etot++;
    }
    inline bool CheckMatch(const vector<int> &mat,Graph &t)
    {
        for(int i=1;i<=t.siz;i++)
        {
            if(!mat[i]) continue;
            for(int j=1;j<=t.siz;j++)
            {
                if(!mat[j]) continue;
                if(edges[mat[i]][mat[j]]&&!t.edges[i][j]) return false;
            }
        }
        return true;
    }
    bool SearchVMatch(int cur,vector<int> &mat,Graph &t)
    {
        if(cur==siz+1) return CheckMatch(mat,t);
        for(int i=1;i<=t.siz;i++)
            if(!mat[i]&&(t.esiz[i]>=esiz[cur]))
            {
                mat[i]=cur;
                if(SearchVMatch(cur+1,mat,t)) return true;
                mat[i]=0;
            }
        return false;
    }
    inline bool CheckContain(Graph &t) 
    {
        vector<int> mat;
        UpdateESiz(),t.UpdateESiz();
        mat.clear();
        mat.resize(max(siz,t.siz)+1);
        if(siz>t.siz||(siz==t.siz&&etot>t.etot)) return t.SearchVMatch(1,mat,*this);
        return SearchVMatch(1,mat,t);
    }
}orig,sct;
vector<Graph> gs;
Graph tmp;
int n,m,bound,curn,cnt,sm;
inline void PushGraph(Graph cur) //Need change later
{
    for(auto x : gs)
        if(cur.CheckContain(x)) return;
    gs.push_back(cur);
}
inline bool CheckGraph(Graph cur)
{
    if(!cur.CheckConnect()) return false;
    Graph aux=cur.Getaux();
    for(int i=1;i<=n;i++)
        for(int j=1;j<=n;j++)
            if(orig.edges[i][j]!=aux.edges[i][j])
                return false;
    return true;
}
inline PII GetNex(PII cur)
{
    if(cur.second>=curn) return {cur.first+1,cur.first+2};
    return {cur.first,cur.second+1};
}
inline bool CheckStrict(int i,int j)
{
    if(i>n||j>n) return false;
    if(curv[i].type==2&&curv[j].type==2) return sct.edges[i][j]||orig.edges[i][j];
    return (!orig.edges[i][j])||sct.edges[i][j];    
}
void SearchGraph(PII cur={1,2})
{
    int curi=cur.first,curj=cur.second;
    //cout<<curi<<' '<<curj<<' '<<curn<<endl;
    if(curi>curn||curj>curn)
    {
        //tmp.Output();
        cnt++;
        if(CheckGraph(tmp))
        {
            //puts("???");
            PushGraph(tmp);
            //tmp.Output();
        }
        return;
    }
    Vertex vi=curv[curi-1],vj=curv[curj-1];
    //puts(".A.");
    if((vi.type!=1||vj.type!=1)&&!CheckStrict(curi,curj))
    {
        SearchGraph(GetNex(cur));
        if(curi<=n&&curj<=n&&!orig.edges[curi][curj]) return;
    }
    //puts("!!!?");
    tmp.edges[curi][curj]=tmp.edges[curj][curi]=true;
    SearchGraph(GetNex(cur));
    tmp.edges[curi][curj]=tmp.edges[curj][curi]=false;
    //puts("?!?!");
}
inline void Output()
{
    printf("There are %ld candidates graphs.\n",gs.size());
    for(auto x : gs)
        x.Output();
}
/*
    Input: 
        n(number of vertexes), m(number of edges), sm(number of strict edges)
        type_i the type of vertex
        a, b two vertexes that are connected
        a, b edges are strict in orig
*/ 
void SearchVertex(int cur=n)
{
    if(cur>=curn)
    {
        SearchGraph();
        return;
    }
    // curv[cur].type=1;
    // SearchVertex(cur+1);
    curv[cur].type=2;
    SearchVertex(cur+1);
}
int main()
{
    scanf("%d%d%d",&n,&m,&sm);
    for(int i=1;i<=n;i++)
        scanf("%d",&oriv[i].type);
    orig=Graph(n),sct=Graph(n);
    for(int i=1;i<=m;i++)
    {
        static int a,b;
        scanf("%d%d",&a,&b);   
        orig.edges[a][b]=orig.edges[b][a]=true;
    }
    for(int i=1;i<=sm;i++)
    {
        static int a,b;
        scanf("%d%d",&a,&b);   
        sct.edges[a][b]=sct.edges[b][a]=true;
    }
    for(int i=1;i<=n;i++) curv.push_back(oriv[i]);
    curn=n;
    while(curn<=BOUND)
    {
        tmp=Graph(curn);
        if(curn==n) SearchGraph();
        else
        {
            //puts("!!!");
            Vertex cur=Vertex(curn);
            curv.push_back(cur);
            SearchVertex();
        }
        curn++;
    }
    Output();
    return 0;
}