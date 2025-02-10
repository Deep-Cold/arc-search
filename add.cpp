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
                if(edges[mat[i]][mat[j]]!=t.edges[i][j]) return false;
            }
        }
        return true;
    }
    bool SearchVMatch(int cur,vector<int> &mat,Graph &t)
    {
        if(cur==siz+1) return CheckMatch(mat,t);
        for(int i=1;i<=t.siz;i++)
            if(!mat[i]&&(t.esiz[i]==esiz[cur]||(siz<t.siz&&t.esiz[i]>=esiz[cur])))
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
        mat.resize(siz+1);
        return t.SearchVMatch(1,mat,*this);
    }
    inline bool CheckMatch(Graph &t)
    {
        if(siz!=t.siz) return false;
        vector<int> mat;
        UpdateESiz(),t.UpdateESiz();
        mat.clear();
        mat.resize(siz+1);
        if(etot!=t.etot) return false;
        return t.SearchVMatch(1,mat,*this);
    }
}orig,sct;
int orin;
struct ExtendGraph
{
    int curn;
    vector<vector<int>> oriState;
    ExtendGraph()
    {
        oriState.resize(orin+1);
        for(int i=1;i<=orin;i++)
        {
            oriState[i].resize(orin+1);
            for(int j=i+1;j<=orin;j++)
            {
                if(curv[i].type==2&&curv[j].type==2)
                {
                    if(orig.edges[i][j]) oriState[i][j]=oriState[j][i]=1;
                    else oriState[i][j]=oriState[j][i]=-1;
                }
                else if(curv[i].type==1||curv[j].type==1)
                {
                    if(!orig.edges[i][j])
                    {
                        if(curv[i].type==1) curv[j].linkv.push_back(i);
                        else curv[i].linkv.push_back(j);
                        oriState[i][j]=oriState[j][i]=1;
                    }                    
                    else oriState[i][j]=oriState[j][i]=0;
                }
                else if(sct.edges[i][j]) oriState[i][j]=oriState[j][i]=1;
                else oriState[i][j]=oriState[j][i]=0;
            }
        }
        for(int i=1;i<=orin;i++)
            for(int j=i+1;j<=orin;j++)
            {
                if(curv[i].type!=2||curv[j].type!=2||oriState[i][j]!=1) continue;
                for(auto x : curv[i].linkv)
                {
                    assert(oriState[x][j]!=-1);
                    oriState[x][j]=oriState[j][x]=1;
                }
                for(auto x : curv[j].linkv)
                {
                    assert(oriState[x][i]!=-1);
                    oriState[x][i]=oriState[i][x]=1;
                }
            }
        curn=orin;
    }
}exg;
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
        return;
    }
    if((curv[i].type==curv[j].type)||(!orig.edges[i][j])) ExtendSearch(GetNex(cur));
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
            int u = curv[i].type==1 ? i : j, v = i^j^u;
            for(int k=1;k<curv.size();k++)
            {
                if(k==i||k==j) continue;
                if(k<=orin)
                {
                    if(curv[k].type==1||exg.oriState[u][k]==1||exg.oriState[v][k]==-1) continue;
                    int tmp = exg.oriState[u][k];
                    exg.oriState[u][k]=exg.oriState[k][u]=-1;
                    ExtendSearch(GetNex(cur));
                    exg.oriState[u][k]=exg.oriState[k][u]=tmp;
                }
            }
            Vertex nw(curv.size());
            nw.type=2;
            curv.push_back(nw);
            vector<int> nwln;
            nwln.clear();
            nwln.resize(curv.size()+1,0);
            exg.oriState.push_back(nwln);
            for(int k=1;k<curv.size()-1;k++)
                exg.oriState[k].push_back(0);
            for(int k=0;k<curv.size();k++)
                exg.oriState.back()[k]=0;
            exg.oriState[u][curv.size()-1]=exg.oriState[curv.size()-1][u]=-1;
            ExtendSearch(GetNex(cur));
            for(int k=1;k<curv.size()-1;k++)
                exg.oriState[k].pop_back();
            exg.oriState.pop_back();
            curv.pop_back();
            exg.oriState[i][j]=exg.oriState[j][i]=oriS;
        }
    }
}
int main()
{
    
}