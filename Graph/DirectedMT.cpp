#include<stdio.h>
#include<string.h>
#include<algorithm>
#include<math.h>
using namespace std;
const int MAXNODE = 30100;
const int MAXEDGE = 30100;
typedef long long Type;
const Type INF = 0x3f3f3f3f3f3f3f;
struct Edge {
    int u, v;
    Type dis;
    Edge() {}
    Edge(int u, int v, Type dis): u(u), v(v), dis(dis) {}
};
struct Directed_MT{
    int n, m;
    Edge edges[MAXEDGE];
    int vis[MAXNODE];
    int pre[MAXNODE];
    int id[MAXNODE];
    Type in[MAXNODE];
    void init(int n) {
        this->n = n;
        m = 0;
    }
    void AddEdge(int u, int v, Type dis) {
        edges[m++] = Edge(u, v, dis);
    }
    Type DirMt(int root) {
        Type ans = 0;
        while (1) {
            //初始化
            for (int i = 0; i < n; i++) in[i] = -INF;
            for (int i = 0; i < m; i++) {
                int u = edges[i].u;
                int v = edges[i].v;
                //找寻最大入边，删除自环
                if (edges[i].dis > in[v] && u != v) {
                    in[v] = edges[i].dis;
                    pre[v] = u;
                }
            }
            //如果没有最大入边，表示该点不连通，则最小树形图形成失败
            for (int i = 0; i < n; i++) {
                if (i == root) continue;
                if (in[i] == -INF) 
        {
        printf("i:%d\n",i);
         return -1;
         
        }
            }
            int cnt = 0;//记录缩点
            memset(id, -1, sizeof(id));
            memset(vis, -1, sizeof(vis));
            in[root] = 0;//树根不能有入边
            for (int i = 0; i < n; i++) {
                ans += in[i];
                int v = i;
                //找寻自环
                while (vis[v] != i && id[v] == -1 && v != root) {
                    vis[v] = i;
                    v = pre[v];
                }
                //找到自环
                if (v != root && id[v] == -1) {
                    //这里不能从i开始找，因为i有可能不在自环内
                    for (int u = pre[v]; u != v; u = pre[u]) 
                        id[u] = cnt;
                    id[v] = cnt++;
                }
            }
            //如果没有自环了，表示最大树形图形成成功了
            if (cnt == 0) break;
            //找到那些不是自环的，重新给那些点进行标记
            for (int i = 0; i < n; i++) 
                if (id[i] == -1) id[i] = cnt++;
            for (int i = 0; i < m; i++) {
                int v = edges[i].v;
                edges[i].v = id[edges[i].v];
                edges[i].u = id[edges[i].u];
                if (edges[i].u != edges[i].v) 
                    edges[i].dis -= in[v];
            }
            //缩点完后，点的数量就边了
            n = cnt;
            root = id[root];
        }
        return ans;
    }
}MT;
int main()
{
  int t;
  scanf("%d",&t);
  while(t--)
  {
    long long ans=0;
    int n,m;
    scanf("%d%d",&n,&m);
    for(int i=1;i<=n;i++)
    {
      long long x;
      scanf("%lld",&x);
      ans+=x;
    }
    
    MT.init(n+1);
    long long r=0;
    for(int i=1;i<=m;i++)
    {
      int u,v;
      long long dis;
      scanf("%d%d%lld",&u,&v,&dis);
      u--;
      v--;
      r+=dis;
      MT.AddEdge(v,u,dis);
    }
    for(int i=0;i<n;i++)
    {
      MT.AddEdge(n,i,0);
    }
    ans+=MT.DirMt(n);
    printf("%lld\n",ans); 
  }
  return 0;
}