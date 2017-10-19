#include<cstdio>
double f[91][91];
int n;
//高斯消元的非回代法 
bool Gauss()
{
    for(int i=1;i<=n;++i)//枚举当前行，在当前行保留当前列，在其他行消去该列 
        for(int j=1;j<=n;++j)//枚举其他行 
            if(i!=j)
            {
                double dev=f[i][i]/f[j][i];
                for(int k=1;k<=n+1;++k)
                    f[j][k]*=dev,f[j][k]-=f[i][k];
            }
    for(int i=1;i<=n;++i)
        if(f[i][i]==0)
            return 0;
    return 1;
}
main()
{
    scanf("%d",&n);
    for(int i=1;i<=n;++i)
        for(int j=1;j<=n+1;++j)
            scanf("%lf",&f[i][j]);
    bool exist=Gauss();
    if(exist)
        for(int i=1;i<=n;++i)
            printf("%.2lf\n",f[i][n+1]/f[i][i]);
        else
            printf("No Solution");
}
