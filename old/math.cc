int gcd(int a, int b) {
	int n;
	while( b ) {
		n = b;
		b = a%b;
		a = n;
	}
	return abs(a);
}

// Returned values will satisfy x*a+y*b = d.  |x|+|y| will be as small as
//   possible.  Ties are broken by x <= y.
// All other solutions are of the form (x+n*(b/d))*a + (y-n*(a/d))*b = d.
int extendedgcd(int a, int b, int &x, int &y) {
	if( !b ) {
		x = (a==0)?0:(a<0)?-1:1; y = 0;
		return abs(a);
	}
	int ret = extendedgcd(b, a%b, x, y);
	x -= y*(a/b);
	swap(x, y);
	return ret;
}

unsigned modinv(unsigned a, unsigned n) {
	unsigned on=n;
	unsigned ax=1, nx=0, d, tmp;
	while( n ) {
		d = a/n;
		tmp = n; n = a - d*n; a = tmp;
		tmp = nx; nx = (ax + d*(i64)(on-nx))%on; ax = tmp;
	}
	return ax%(on/a);
}

bool isprime(int n) {
	n = abs(n);
	if( n <= 3 ) return( n >= 2 );
	if( n%2 == 0 ) return false;
	if( n%3 == 0 ) return false;
	for( unsigned i = 5; i*i <= n; i += 6 ) {
		if( n%i == 0 ) return false;
		if( n%(i+2) == 0 ) return false;
	}
	return true;
}

vector<int> factorize(int n) {
	vector<int> ret;

	n = abs(n);
	if( n < 2 ) return ret;
	while( n%2 == 0 ) {ret.push_back(2); n /= 2;}
	while( n%3 == 0 ) {ret.push_back(3); n /= 3;}
	for( int i = 5; i*i <= n; i += 6 ) {
		while( n%i == 0 ) {ret.push_back(i); n /= i;}
		while( n%(i+2) == 0 ) {ret.push_back(i+2); n /= i+2;}
	}
	if( n > 1 ) ret.push_back(n);
	return ret;
}

i64 num_permutations(int n, int r) {
	if( r < 0 || n < r ) return 0;
	i64 ret = 1;
	for( int i = n; i > n-r; i-- ) ret *= i;
	return ret;
}

i64 num_combinations(int n, int r) {
	if( r < 0 || n < r ) return 0;
	if( n-r < r ) r = n-r;
	vector<int> fact(r);
	int i, j, k, x;
	for( i = 0; i < r; i++ ) fact[i] = n-i;
	for( i = 1; i <= r; i++ ) {
		x = i;
		for( j = 2; j <= x; j++ ) while( x%j == 0 ) {
			x /= j;
			for( k = 0; k < r; k++ ) if( fact[k]%j == 0 ) break;
			fact[k] /= j;
		}
	}
	i64 ret = 1;
	for( i = 0; i < r; i++ ) ret *= fact[i];
	return ret;
}

inline unsigned modmult(unsigned a, unsigned b, unsigned n) {
	return (unsigned)(((i64)a*(i64)b)%n);
}

inline unsigned modadd(unsigned a, unsigned b, unsigned n) {
	return (unsigned)(((i64)a+(i64)b)%n);
}

unsigned modpow(unsigned a, unsigned e, unsigned n) {
	vector<unsigned> p;
	unsigned b = a%n, c = 1, i;
	i64 x;

	for( i = 0, x = 1; x <= e; x *= 2, i++ ) {
		p.push_back(b);
		b = modmult(b, b, n);
	}
	while( e ) {
		x /= 2; i--;
		if( e >= x ) {
			c = modmult(c, p[i], n);
			e -= x;
		}
	}
	return c;
}

unsigned eulerphi(unsigned n) {
	unsigned phi = n, p;
	for( p = 2; p*p <= n; p++ ) if( n%p == 0 ) {
		phi = (phi/p)*(p-1);
		while( n%p == 0 ) n /= p;
	}
	if( n >= 2 ) phi = (phi/n)*(n-1);
	return phi;
}

// Fills array with partition function p(n) for 0 <= n <= N
int part[122];	// 121 is max partition that will fit into int
void getpartitions(int N) {
	part[0] = 1;
	for( int n = 1; n <= N; n++ ) {
		part[n] = 0;
		for( int k = 1, x; k <= n; k++ ) {
			x = n-k*(3*k-1)/2;
			if( x < 0 ) break;
			if( k&1 ) part[n] += part[x]; else part[n] -= part[x];
			x = n-k*(3*k+1)/2;
			if( x < 0 ) break;
			if( k&1 ) part[n] += part[x]; else part[n] -= part[x];
		}
	}
}
