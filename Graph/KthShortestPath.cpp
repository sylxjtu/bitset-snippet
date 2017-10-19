//bits
//n vertex , m edge , k shortest
//read (a , b, c) directional
//from a to b cost c
#define N 11000
#define M 110000
#define pr pair<ll,heap*>
using namespace std;
typedef long long ll;
struct heap
{
	heap *l, *r;
	ll val;
	int dis, no;
	heap();
}*null = new heap(), *H[N];
heap::heap()
{
	l = r = null;
	val = no = 0;
}
heap* Insert(heap* x, heap* y)
{
	if (x == null || y == null)
		return x == null ? y : x;
	if (x->val > y->val)
		swap(x, y);
	x->r = Insert(x->r, y);
	if (x->l->dis < x->r->dis)
		swap(x->l, x->r);
	x->dis = x->r->dis + 1;
	return x;
}
heap* merge(heap* x, heap* y)
{
	if (x == null || y == null)
		return x == null ? y : x;
	if (x->val > y->val)
		swap(x, y);
	heap* p = new heap();
	*p = *x;
	p->r = merge(p->r, y);
	if (p->l->dis < p->r->dis)
		swap(p->l, p->r);
	p->dis = p->r->dis + 1;
	return p;
}
ll dis[N], val[M];
int next[M], from[M], to[M], head[N], pre[N], n, tot;
bool inq[N], isT[M];
queue<int>q;
priority_queue<pr, vector<pr>, greater<pr> >poq;
void add(int x, int y, ll v)
{
	to[++tot] = y;
	val[tot] = v;
	from[tot] = x;
	next[tot] = head[x];
	head[x] = tot;
}
void Spfa()
{
	int x, y, i;
	memset(dis, 0x3f, sizeof(dis));
	q.push(n);
	dis[n] = 0, inq[n] = 1;
	while (!q.empty())
	{
		x = q.front(), q.pop();
		inq[x] = 0;
		for (i = head[x]; i; i = next[i])
		{
			if (dis[y = to[i]] > dis[x] + val[i])
			{
				dis[y] = dis[x] + val[i];
				isT[pre[y]] = 0;
				isT[pre[y] = i] = 1;
				if (!inq[y])
					inq[y] = 1, q.push(y);
			}
		}
	}
}
void Build()
{
	q.push(n);
	int x, y, i;
	for (i = 1; i <= tot; i++)
	{
		if (isT[i])	continue;
		y = to[i];
		heap* temp = new heap();
		temp->no = i, temp->val = val[i] + dis[from[i]] - dis[to[i]];
		H[y] = Insert(H[y], temp);
	}
	while (!q.empty())
	{
		x = q.front(), q.pop();
		for (i = head[x]; i; i = next[i])
		{
			y = to[i];
			if (isT[i])
			{
				H[y] = merge(H[y], H[x]);
				q.push(y);
			}
		}
	}
}
void init()
{
	null->l = null->r = null;
	null->val = null->no = 0;
	null->dis = -1;
	for (int i = 1; i <= n; i++)
		H[i] = null;
}
int main()
{
	int m, i, j, k, x, y;
	ll v, ans;
	heap* now;
	scanf("%d%d%d", &n, &m, &k);
	init();
	for (i = 1; i <= m; i++)
	{
		scanf("%d%d%lld", &x, &y, &v);
		add(y, x, v);
	}
	Spfa();
	Build();
	poq.push(pr(H[1]->val, H[1]));
	for (i = 1, ans = 0; i < k; i++)
	{
		ans = poq.top().first, now = poq.top().second;
		poq.pop();
		v = now->val;
		heap* temp = merge(now->l, now->r);
		if (temp != null)
		{
			poq.push(pr(ans - v + temp->val, temp));
		}
		poq.push(pr(ans + H[from[now->no]]->val, H[from[now->no]]));
	}
	printf("%lld\n", ans + dis[1]);
	return 0;
}
