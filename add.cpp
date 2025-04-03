#include<bits/stdc++.h>
using namespace std;
const int N=15,BOUND=9;
typedef long long ll;
typedef pair<int,int> PII;
struct Vertex
{
    int id;
    int type; //blue node: 1, pink node: 2, white node: 0
    vector<int> linkv;
    Vertex()
    {   
        linkv.clear();
    }
    Vertex(int vid)
    {
        id=vid;
        linkv.clear();
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
    inline bool operator<(Graph &t)
    {
        UpdateESiz(),t.UpdateESiz();
        if(siz==t.siz) return etot<t.etot;
        return siz<t.siz;
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
    inline bool CheckMatch(const vector<int> &mat,Graph &t)
    {
        for(int i=1;i<=t.siz;i++)
        {
            if(!mat[i]) continue;
            for(int j=1;j<=t.siz;j++)
            {
                if(!mat[j]) continue;
                if(edges[i][j]&&!t.edges[mat[i]][mat[j]]) return false;
            }
        }
        return true;
    }
    bool SearchVMatch(int cur,vector<int> &mat,Graph &t)
    {
        if(cur==siz+1) return CheckMatch(mat,t);
        for(int i=1;i<=t.siz;i++)
            if(!mat[cur]&&(t.esiz[i]>=esiz[cur]))
            {
                mat[cur]=i;
                if(SearchVMatch(cur+1,mat,t)) return true;
                mat[cur]=0;
            }
        return false;
    }
    inline bool CheckContain(Graph &t) 
    {
        vector<int> mat;
        UpdateESiz(),t.UpdateESiz();
        mat.clear();
        mat.resize(max(siz,t.siz)+1);
        if(t<*this) return t.SearchVMatch(1,mat,*this);
        return SearchVMatch(1,mat,t);
    }
}orig,sct;
int orin,m,sm;
struct ExtendGraph
{
    int curn;
    vector<vector<int>> oriState;
    ExtendGraph()
    {
        oriState.resize(orin+1);
        for(int i=1;i<=orin;i++) oriState[i].resize(orin+1);
        for(int i=1;i<=orin;i++)
        {
            for(int j=i+1;j<=orin;j++)
            {
                if(curv[i-1].type==2&&curv[j-1].type==2)
                {
                    if(orig.edges[i][j]) oriState[i][j]=oriState[j][i]=1;
                    else oriState[i][j]=oriState[j][i]=-1;
                }
                else if(sct.edges[i][j]) oriState[i][j]=oriState[j][i]=1;
                else if(curv[i-1].type==1||curv[j-1].type==1)
                {
                    if(!orig.edges[i][j])
                    {
                        if(curv[i-1].type==1) curv[j-1].linkv.push_back(i);
                        else curv[i-1].linkv.push_back(j);
                        oriState[i][j]=oriState[j][i]=1;
                    }                    
                    else oriState[i][j]=oriState[j][i]=0;
                }
                else oriState[i][j]=oriState[j][i]=0;
            }
        }
        for(int i=1;i<=orin;i++)
            for(int j=i+1;j<=orin;j++)
            {
                if(curv[i-1].type!=2||curv[j-1].type!=2||oriState[i][j]!=1) continue;
                for(auto x : curv[i-1].linkv)
                {
                    assert(oriState[x][j]!=-1);
                    oriState[x][j]=oriState[j][x]=1;
                }
                for(auto x : curv[j-1].linkv)
                {
                    assert(oriState[x][i]!=-1);
                    oriState[x][i]=oriState[i][x]=1;
                }
            }
        curn=orin;
    }
    inline void Output()
    {
        int siz=curv.size();
        for(int i=1;i<=siz;i++)
        {
            for(int j=1;j<=siz;j++)
                cout<<oriState[i][j]<<"\t";
            putchar('\n');
        }
    }
    inline Graph Convert() const
    {
        int n=curv.size();
        Graph t = Graph(n);
        for(int i=1;i<=n;i++)
            for(int j=1;j<=n;j++)
            {
                if(i==j) continue;
                if(oriState[i][j]==1||(curv[i-1].type==1&&curv[j-1].type==1))
                    t.edges[i][j]=t.edges[j][i]=true;
            }
        return t;
    }
}exg;
vector<Graph> gs,out;
inline void PushGraph(Graph cur) //Need change later
{
    for(auto x : gs)
        if(cur.CheckContain(x)) return;
    //if(gs.size()>=3) return;
    gs.push_back(cur);
}
inline PII GetNex(PII cur)
{
    if(cur.second>=orin) return {cur.first+1,cur.first+2};
    return {cur.first,cur.second+1};
}
void ExtendSearch(PII cur)
{
    int i=cur.first,j=cur.second;
    if(i>orin||j>orin)
    {
        int n=curv.size();
        for(int i=1;i<=n;i++)
            for(int j=i+1;j<=n;j++)
            {
                if(curv[i-1].type!=1||curv[j-1].type!=1) continue;
                ll vic=1<<i-1,vjc=1<<j-1;
                for(int k=1;k<=n;k++)
                {
                    if(exg.oriState[i][k]==1) vic|=1<<k-1;
                    if(exg.oriState[j][k]==1) vjc|=1<<k-1;
                }
                if(!(orig.edges[i][j]^((vic|vjc)==((1<<n)-1)))) return;
            }
        Graph t=exg.Convert();
        if(!t.CheckConnect())
        {
            //return;//test
            t.siz++;
            for(int i=1;i<=n;i++)
            {
                t.edges[i].push_back(true);
            }
            vector<bool> nw;
            nw.resize(n+2);
            for(int i=1;i<=n;i++) nw[i]=true;
            t.edges.push_back(nw);
        }
        PushGraph(t);
        return;
    }
    if((curv[i-1].type==curv[j-1].type)||(!orig.edges[i][j])) ExtendSearch(GetNex(cur));
    else
    {
        if(exg.oriState[i][j]==-1)
        {
            ExtendSearch(GetNex(cur));
            return;
        }
        else
        {
            int oriS=exg.oriState[i][j];
            if(exg.oriState[i][j]==0)
            {
                exg.oriState[i][j]=exg.oriState[j][i]=-1;
                ExtendSearch(GetNex(cur));
            }
            exg.oriState[i][j]=exg.oriState[j][i]=1;
            int u = curv[i-1].type==1 ? i : j, v = i^j^u;
            for(int k=1;k<=curv.size();k++)
            {
                if(curv[k-1].type==2&&exg.oriState[u][k]==-1&&exg.oriState[v][k]==1)
                {
                    ExtendSearch(GetNex(cur));
                    exg.oriState[i][j]=exg.oriState[j][i]=oriS;
                    return;
                }
            }
            for(int k=1;k<=curv.size();k++)
            {
                if(k==i||k==j) continue;
                if(curv[k-1].type==1||exg.oriState[u][k]==1||exg.oriState[v][k]==-1) continue;
                int tmp1 = exg.oriState[u][k],tmp2 = exg.oriState[v][k];
                exg.oriState[u][k]=exg.oriState[k][u]=-1;
                exg.oriState[v][k]=exg.oriState[k][v]=1;
                ExtendSearch(GetNex(cur));
                exg.oriState[v][k]=exg.oriState[k][v]=tmp2;
                exg.oriState[u][k]=exg.oriState[k][u]=tmp1;
            }
            Vertex nw(curv.size());
            nw.type=2;
            curv.push_back(nw);
            vector<int> nwln;
            nwln.clear();
            nwln.resize(curv.size()+1,0);
            exg.oriState.push_back(nwln);
            for(int k=1;k<curv.size();k++)
                exg.oriState[k].push_back(0);
            for(int k=0;k<=curv.size();k++)
                exg.oriState.back()[k]=0;
            exg.oriState[u][curv.size()]=exg.oriState[curv.size()][u]=-1;
            exg.oriState[v][curv.size()]=exg.oriState[curv.size()][v]=1;
            ExtendSearch(GetNex(cur));
            for(int k=1;k<curv.size();k++)
                exg.oriState[k].pop_back();
            exg.oriState.pop_back();
            curv.pop_back();
            exg.oriState[i][j]=exg.oriState[j][i]=oriS;
        }
    }
}
inline void Output()
{
    out.clear();
    for(auto x : gs)
        out.push_back(x);
    sort(out.begin(),out.end());
    gs.clear();
    for(auto x : out)
        PushGraph(x);
    printf("There are %ld candidates graphs.\n",gs.size());
    for(auto x : gs)
        x.Output();
}
int main()
{
    scanf("%d%d%d",&orin,&m,&sm);
    for(int i=1;i<=orin;i++)
        scanf("%d",&oriv[i].type);
    orig=Graph(orin),sct=Graph(orin);
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
    for(int i=1;i<=orin;i++) curv.push_back(oriv[i]);
    exg=ExtendGraph();
    //exg.Output();
    ExtendSearch({1,2});
    Output();
}