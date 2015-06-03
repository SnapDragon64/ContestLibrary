template<class T=int> class UnionFind {
	map<T, T> comp;

public:
	T dofind(const T &x) {
		map<T, T>::iterator it = comp.find(x);
		if( it == comp.end() ) return comp[x]=x;
		T &ret = it->second;
		if( ret == x ) return ret;
		return ret=dofind(ret);
	}

	// if we always call with x < y, this guarantees the representative is
	//   always the least element
	void dounion(const T &x, const T &y) {
		comp[dofind(y)] = dofind(x);
	}
};
