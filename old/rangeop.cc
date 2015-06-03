// RangeOp is a data structure built on an array and an associative operation.
//   It takes linear setup time, linear memory, then allows for log-time
//   updates, and log-time calculation of the operation on arbitrary ranges.
//   It also has constant amortized time when you merely increment the front
//   and/or back of ranges.
// T will be the data type for the operation.
// op will be the associative (but not necessarily commutative) binary
//   operation.
// def, the "default value" for an empty range, must be the identity under op.
//   ie, it should satisfy op(x, def) == x == op(def, x).  Be careful to
//   specify it properly depending on your operation.
//
// Example declarations:
//
//		RangeOp<int, plus<int> > r(v.size());
//		r.set(0, v.begin(), v.end());
//			Creates a RangeOp, using the addition operation, from a known
//          vector v.  Note that the default is assumed to be 0.  Also note
//          that this initialiation is nice and linear in time/memory.
//      x = r.calc(2, 5);
//			x is now equal to v[2]+v[3]+v[4]
//
//		RangeOp<double, multiplies<double> > r(1000, 1);
//			Product operation - note that default value has to be 1.  The
//			default value is ALWAYS used as a basis for range calculations;
//			if it were left as 0, all ranges would return 0.
//
//		int min(const int &a, const int &b) {return min(a,b);}
//		RangeOp<int> r(1000, 2147483647, min);
//			The easiest way to specify your own operator.  In this case, min.
//			Note the default value.
//
// Memory use: RangeOp stores an internal array of T.  Its size is the smallest
//   power of 2 that is at least twice the maximum number of elements.  (So,
//   it's never larger than 4 times a normal array size)
//
// Time complexity:
//   (n is the maximum size of the RangeOp, m is the size of any given range)
// RangeOp constructor: O(n)
// Calculating a range: O(log m)
// Getting a value: O(1)
// Setting a value: O(log n)
// Setting a range: O(m log n), and no worse than O(n)
// Also, computing a series of distinct ranges with endpoints that change
//   monotonically is never worse than O(n).  That is, calculating ranges that
//   are sliding along the array takes amortized CONSTANT time.
//
// Things to watch for:
// - Don't call calc with q < p.
// - Always set the default value properly.
// - Updating values clears the internal "cache", which clashes with the above
//    amortized-constant-time guarantee.
// - 2-D RangeOps would require some additional annoying code (you'd need to
//    change RangeOp to support ==, =, and a "lifted" version of op)

template<class T, class Op = T(*)(const T&,const T&)> struct RangeOp {
	Op op;
	T *t, c[32], def;
	int s, cs[32], ce[32], cv;

	RangeOp(int n, T def=T(), Op op=Op()) : cv(0), def(def), op(op) {
		for (s=1; s<n; s<<=1);
		t = new T[s*2];
	}
	~RangeOp() {delete t;}

	// calculates operation on [p,q)
	T calc(int p, int q) {
		int a=p+=s,b=q+=s,d=0;
		for (; a!=b && (cv<=d||a!=cs[d]||b!=ce[d]); d++)
			a=(cs[d]=a)+1>>1, b=(ce[d]=b)>>1;
		if ((cs[d]=a)==(ce[d]=b)) c[d]=def;
		if (cv<d) cv=d;
		while (d--) {
			c[d]=((a<<=1)-1<<d<p?c[d+1]:op(t[--a],c[d+1]));
			c[d]=((b<<=1)+1<<d>q?c[d]:op(c[d],t[b++]));
		}
		return c[0];
	}

	// optional: for setting a range of values at once
	template<class it> void set(int p, it i, it j) {
		int q=p+=s;
		for (; i!=j; ++i) t[q++]=*i;
		for (cv=0; q=q+1>>1,p>>=1;)
		for (int k=p; k<q; k++)
			t[k]=op(t[k*2],t[k*2+1]);
	}

	// optional: for reading/writing individual values
	inline T get(int p) {return t[p+s];}
	void set(int p, T v) {
		for (cv=0, p+=s; t[p]=v, p>>=1;)
			if ((v=op(t[p*2],t[p*2+1]))==t[p]) break;
	}
};
