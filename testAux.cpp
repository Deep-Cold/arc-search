#include<bits/stdc++.h>
using namespace std;
const int N=15,BOUND=7;
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
    int siz;
    vector<vector<bool>> edges;
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
        int curn=edges.size()-1;
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
}orig;
vector<Graph> gs;
Graph tmp;
int n,m,bound,curn,cnt;
inline void PushGraph(Graph cur) //Need change later
{
    gs.push_back(cur);
}
inline bool CheckGraph(Graph cur)
{
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
void SearchGraph(PII cur={1,2})
{
    int curi=cur.first,curj=cur.second;
    //cout<<curi<<' '<<curj<<' '<<curn<<endl;
    if(curi>curn||curj>curn)
    {
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
    if(vi.type!=1||vj.type!=1)
        SearchGraph(GetNex(cur));
    //puts("!!!?");
    tmp.edges[curi][curj]=tmp.edges[curj][curi]=true;
    SearchGraph(GetNex(cur));
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
        n(number of vertexes), m(number of edges)
        type_i the type of vertex
        a, b two vertexes that are connected
*/ 
void SearchVertex(int cur=n)
{
    if(cur>=curn)
    {
        SearchGraph();
        return;
    }
    curv[cur].type=1;
    SearchGraph();
    curv[cur].type=2;
    SearchGraph();
}
int main()
{
    Graph G=Graph(4);
    for(int i=1;i<=4;i++)
        curv.push_back(Vertex(i));
    curv[0].type=1;
    for(int i=1;i<=3;i++)
        curv[i].type=2;
    G.edges[1][2]=G.edges[2][1]=1;
    G.edges[1][3]=G.edges[3][1]=1;
    G.edges[1][4]=G.edges[4][1]=1;
    // G.edges[2][5]=G.edges[5][2]=1;
    // G.edges[3][6]=G.edges[6][3]=1;
    // G.edges[4][7]=G.edges[7][4]=1;
    Graph aux=G.Getaux();
    G.Output();
    aux.Output();
    return 0;
}