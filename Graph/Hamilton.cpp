/*
【题目来源】
http://poj.org/problem?id=2438
【题目分析】
有敌对关系的小朋友，不能坐在一起。最后围成一个圈，吃饭。。。
将小朋友看成点，有敌对关系的看成没有边，最后构成一个回路。
哈密顿回路。 

【小小总结】 
哈密顿回路
充分条件：
无向连通图中任意2点度数之和大于等于顶点数，则必定存在哈密顿回路。 

思路分析： 
1.任意找两个相邻的节点S和T，在它们基础上扩展出一条尽量长的没有重复节点的路径。
也就是说，如果S与节点v相邻，而且v不在路径S->T上，则可以把该路径变成v->S->T，然后v成为新的S。
从S和T分别向两头扩展，直到无法扩为止，即所有与S或T相邻的节点都在路径S->T上。
2.若S与T相邻，则路径S->T形成了一个回路。
3.若S与T不相邻，可以构造出一个回路。设路径S->T上有k+2个节点，依次为S、v1、v2……vk和T。
可以证明v1到vk中必定存在vi，满足vi与T相邻，且vi+1与S相邻。（其实vi,vi+1与s t同时相邻） (怎么证明就不赘述了，反正刷题肯定不会叫你证)
找到了满足条件的节点vi以后，就可以把原路径变成S->vi+1->T->vi->S，即形成了一个回路。（自己画图就知道了） 
4.现在我们有了一个没有重复节点的回路。如果它的长度为N，则哈密顿回路就找到了。
如果回路的长度小于N，由于整个图是连通的，所以在该回路上，一定存在一点与回路以外的点相邻。
那么从该点处把回路断开，就变回了一条路径。再按照步骤1的方法尽量扩展路径，则一定有新的节点被加进来。（画图就知道了） 
接着回到步骤2。 

伪代码： 
思路清楚后主要是理解好伪代码，伪代码一懂代码就写出来了。关于下面步骤中为什么要倒置，自己画画图就清楚了。 
s为哈密顿回路起点，t为当前哈密顿回路的终点，ans[]就是哈密顿回路啦，默认不包含0顶点
1.初始化，令s=1,t为任意与s相邻的点。
2.若ans[]中的元素个数小于n，则从t开始扩展，若可扩展，则把新点v加入ans[],并令t=v,继续扩展到无法扩展。
3.将ans[]倒置，s,t互换，从t（原来的s）开始扩展，若可扩展，则把新点v加入ans[],并令t=v,继续扩展到无法扩展。
4.此时s,t两头都无法扩展了，若s,t相连，则继续步骤5。若st不相连，则遍历ans[]，必定会有2点，ans[i]与t相连，ans[i+1]与s相连，
将ans[i+1]到t倒置，t=ans[i+1](未倒置前的)
5.st相连，此时为一个环。若ans[]个数等于n,算法结束，ans[]为哈密顿回路，如需要再添加一个起点。
若ans[]个数小于n,遍历ans[]，寻找ans[i],使得ans[i]与ans[]外一点j相连，倒置ans[]中s到ans[i-1]部分，令s = ans[i-1]，
再倒置ans[]中ans[i]到t的部分，j加入ans[],t = j.继续步骤2 

下面去掉main函数，就是求解哈密顿回路的模版了。 
*/
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

#define Max 500

int map[Max][Max];
int ans[Max];
bool vis[Max];

//ans数组的index 
int index;
int n, m;
int s, t;

void init()
{
    for (int i = 0; i < Max; ++i)
        for (int j = 0; j < Max; ++j)
            if (i == j)
                map[i][j] = 0;
            else
                map[i][j] = 1;
    
    memset(ans, 0, sizeof(ans));
    memset(vis, 0 , sizeof(vis));
    index = 0;
}

void reverse(int a, int b)
{
    while (a < b)
    {
        swap(ans[a], ans[b]);
        a++; 
        b--;
    }
}

void expand()
{
    while (true)
    {
        int i;
        for (i = 1; i <= n; ++i) 
        {
            if (!vis[i] && map[i][t])//未被访问且与t相连 
            {
                ans[index++] = i;
                vis[i] = true;
                t = i;       
                break;
            }
        }
        if (i > n) break;//无法扩展 
    }
}

void Hamilton()
{
    //初始化s = 1
    s = 1; 
    
    //取任意连接s的点 
    for (int i = 1; i <= n; ++i)
    {
        if (map[i][s])
        {
            t = i;       
            break;
        }
    }
    vis[s] = true;
    vis[t] = true;
    ans[index++] = s;
    ans[index++] = t;
    
    while (true)
    {
        //从t向外扩展 
        expand(); 
        
        //t扩展完毕，倒置ans并交换s,t 
        reverse(0, index-1);
        
        swap(s, t);
        
        //从另一头，t(原来的s)继续扩展 
        expand();
        
        //若s,t不相连,处理成相连 
        if (!map[s][t])
        {
            //在ans[1]到ans[index-2]中寻找两个相邻的且与st同时相连的点（必存在） 因为涉及i+1所以i < index-2 
            for (int i = 1; i < index-2; ++i) 
            {
                if (map[ans[i+1]][s] && map[ans[i]][t])
                {
                    reverse(i+1, index-1);//倒置ans[i+1]到ans[index-1] 
                    t = ans[index-1];//更新t 
                    break;
                }
            }
        }
        
        //若ans元素有n个，说明算法完成 
        if (index == n) return;
        
        //若ans元素不满n个，ans[]中寻找与未被遍历过的点相连的点，但这一点必定不是s,t.因为s,t已经遍历到无法遍历才能走到这一步 
        for (int j = 1; j <= n; ++j)
        {
            if (!vis[j])
            {
                int i;
                for (i = 1; i < index-1; ++i)//排除st 
                {
                    if (map[ans[i]][j])
                    {
                        s = ans[i-1];
                        t = j;
                        reverse(0, i-1);
                        reverse(i,index-1);
                        ans[index++] = j;
                        vis[j] = true;       
                        break;
                    }
                }       
                if (map[ans[i]][j])break;//记得有2个循环，要break两次 
            }
        }
        //继续返回，从t扩展。。 
    }
}
    
int main()
{
    while (cin >> n >> m, n||m)
    {
        n *= 2;
        init();
        int temp1, temp2;
        for (int i = 0; i < m; ++i)
        {
            cin >> temp1 >> temp2;
            map[temp1][temp2] = 0;   
            map[temp2][temp1] = 0;
        }
        Hamilton();
        cout << ans[0];
        for (int i = 1; i < index; ++i)
            cout << ' ' << ans[i]; 
        cout << endl;
    }
}
