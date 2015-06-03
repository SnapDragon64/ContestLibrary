#include <iostream>
#include <cstdlib>

// Structure that takes O(N) setup time, O(N) memory, then allows for O(logN)
//   calculation of a given associative operation on arbitrary ranges.  Also
//   has constant amortized time when successively incrementing the front
//   and/or back of ranges.
// op is the associative (but not necessarily commutative) function to apply
// def is the default value, and should satisfy op(x, def) == x == op(def, x)
template<class T> struct RangeOp {
	T *t, (*op)(T&,T&), def, c[32];
	int s, cs[32], ce[32], cv;

	RangeOp(int n, T (*op)(T&,T&), T def=T()) : op(op), def(def), cv(0) {
		for (s=1; s<n; s<<=1);
		t = (T *)malloc(sizeof(T)*s*2);
	}
	~RangeOp() {free(t);}
	inline T get(int p) {return t[p+s];}
	void set(int p, T v) {
		for (cv=0, p+=s; t[p]=v, p>>=1;)
			if ((v=op(t[p*2],t[p*2+1]))==t[p]) break;
	}
	template<class it> void set(int p, it i, it j) {
		int q=p+=s;
		for (; i!=j; ++i) t[q++]=*i;
		for (cv=0; q=q+1>>1,p>>=1;)
		for (int k=p; k<q; k++)
			t[k]=op(t[k*2],t[k*2+1]);
	}
	T calc(int p, int q) {
		int a=p+=s,b=q+=s,d=0;
		for (; a!=b && (d>=cv||a!=cs[d]||b!=ce[d]); d++)
			a=(cs[d]=a)+1>>1, b=(ce[d]=b)>>1;
		if ((cs[d]=a)==(ce[d]=b)) c[d]=def;
		if (cv<d) cv=d;
		while (d--) {
			c[d]=((a<<=1)-1<<d<p?c[d+1]:op(t[--a],c[d+1]));
			c[d]=((b<<=1)+1<<d>q?c[d]:op(c[d],t[b++]));
		}
		return c[0];
	}
};

int plus(int &a, int &b) {return a+b;}

int a[16384], b[16384];
main() {
	RangeOp<int> r(16384,plus);
	int i, j, k, x, y, z, nz;
	for( i = 0; i < 16384; i++ ) {a[i] = i; b[i] = -1;}
	r.set(0, a, a+16384);
	for(;;) {
		x = rand()%100;
		if( x>20 ) {
			x = rand()%16384;
			y = rand()%16384 <? 16384-x;
			z = r.calc(x, x+y);
			for( i = nz = 0; i < y; i++ ) nz += a[x+i];
			if( z != nz ) cout << "Error: " << x << ' ' << y << ' ' << z << ' ' << nz << endl;
		} else if( x ) {
			x = rand()%16384;
			y = rand()%5;
			r.set(x, r.get(x)+y);
			a[x] += y;
		} else {
			x = rand()%16384;
			y = rand()%(16384/32) <? 16384-x;
			r.set(x, b, b+y);
			for( i = 0; i < y; i++ ) a[x+i] = -1;
		}
	}
}
