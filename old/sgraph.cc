// NOTE: returns -1 for unmatched items.  nm is number of matchable items.
vector<int> bipartitematch(const vector<vector<int> > &m, int nm) {
	vector<int> mat(nm, -1), ret(m.size(), -1);
	int i, j, x, y;

	for( i = 0; i < m.size(); i++ ) {
		queue<int> q;
		vector<int> pred(m.size(), -1);
		q.push(i);
		pred[i] = -2;
		while( !q.empty() ) {
			x = q.front(); q.pop();
			for( j = 0; j < m[x].size(); j++ ) {
				y = mat[m[x][j]];
				if( y == -1 ) goto found;
				if( pred[y] != -1 ) continue;
				pred[y] = x;
				q.push(y);
			}
		}
		continue;
found:	y = m[x][j];
		while( x != -2 ) {
			mat[y] = x;
			swap(ret[x], y);
			x = pred[x];
		}
	}
	return ret;
}
