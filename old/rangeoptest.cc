#include <iostream>
#include <functional>
#include <cstdlib>
#include <utility>
#include <algorithm>

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

RangeOp<int, plus<int> > r(8000);
RangeOp<double, multiplies<double> > r2(1000, 1);
int min(const int &a, const int &b) {return min(a,b);}
RangeOp<int> r3(1000, 2147483647, min);

int a[16384], b[16384];
main() {
	RangeOp<int, plus<int> > r(16384);
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
