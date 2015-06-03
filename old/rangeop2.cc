// Structure that takes O(N) setup time, O(N) memory, then allows for O(logN)
//   calculation of a given associative operation on arbitrary ranges.  Also
//   has constant amortized time when successively incrementing the front
//   and/or back of ranges.
template<class T = int>
class RangeOp {
public:
	vector<vector<T> > tree;
	T (*op)(const T &, const T &);
	T def;	// should have the property that op(x, def) == x == op(def, x)

	// internal cache that allows for amortized runtimes
	vector<int> cs, ce;
	vector<T> cache;

	// pass in start/end iterators, the (associative) operation, and (if
	//   necessary) a default (identity) T value for empty ranges
	template<class it>
	RangeOp(it s, it e, T (*op)(const T &, const T &), T def = T())
														: op(op), def(def) {
		tree.push_back(vector<T>());
		for( ; s != e; s++ ) tree[0].push_back(*s);
		for( int depth = 1, i; tree[depth-1].size() > 1; depth++ ) {
			tree.push_back(vector<T>((tree[depth-1].size()+1)/2));
			for( i = 0; i+1 < tree[depth-1].size(); i += 2 )
				tree[depth][i>>1] = op(tree[depth-1][i], tree[depth-1][i+1]);
			if( i < tree[depth-1].size() ) tree[depth][i>>1] = tree[depth-1][i];
		}
		cs = ce = vector<int>(tree.size()+1);
		cache = vector<T>(tree.size()+1, def);
	}

	// takes O(log(e-s)) time (and possibly constant time, amortized)
	T dorange(int s, int e) {
		if( s >= e ) return def;
		int depth, curs, cure;
		for( depth = 0; ; depth++ ) {
			curs = ((s+(1<<depth)-1)>>depth);
			cure = (e>>depth);
			if( curs == cure ) {
				cache[depth] = def;
				cs[depth] = curs;
				ce[depth] = cure;
			}
			if( curs == cs[depth] && cure == ce[depth] ) break;
		}
		for( depth--; depth >= 0; depth-- ) {
			curs <<= 1; cure <<= 1;
			int &v = cache[depth];
			v = cache[depth+1];
			if( ((curs-1)<<depth) >= s ) v = op(tree[depth][--curs], v);
			if( ((cure+1)<<depth) <= e ) v = op(v, tree[depth][cure++]);
			cs[depth] = curs;
			ce[depth] = cure;
		}
		return cache[0];
	}
};
