#include <queue>
#define S1 20
#define S2 20
#define S3 1000
vector<int> best;
vector<int> prev;
priority_queue<pair<int,int> > q;

inline int enc(int v1, int v2, int v3) {
	return ((v1)*S2+v2)*S3+v3;
}

inline int dec(const int &v, int &v1, int &v2, int &v3) {
	v1 = v/S3/S2;
	v2 = v/S3%S2;
	v3 = v%S3;
}

inline void move(int p, int v1, int v2, int v3, int d) {
	if( v1 < 0 || v1 >= S1 ) return;
	if( v2 < 0 || v2 >= S2 ) return;
	if( v3 < 0 || v3 >= S3 ) return;
	int v = enc(v1, v2, v3);
	if( best[v] != -1 && best[v] <= d ) return;
	// check other state validity here
	best[v] = d;
	prev[v] = p;
	q.push(pair<int,int>(-d, v));
}

int dobfs(int s, int e) {
	int d, v, v1, v2, v3;
	best = prev = vector<int>(S1*S2*S3, -1);
	q = priority_queue<pair<int,int> >();
	q.push(pair<int,int>(0, s));
	best[s] = 0;
	while( !q.empty() && q.top().second != e ) {
		d = -q.top().first; v = q.top().second; q.pop();
		dec(v, v1, v2, v3);
		//add moves here, ie move(v, v1-1, v2, v3, d+3);
	}
	return best[e];
}
