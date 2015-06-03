i64 gcd(i64 a, i64 b) {
	i64 n;
	a = (a<0)?-a:a;
	b = (b<0)?-b:b;
	while( b ) {
		n = b;
		b = a%b;
		a = n;
	}
	return a;
}

class fract {
public:
	i64 numer, denom;

	inline fract operator+(const fract &b) const {
		return fract(numer*b.denom + denom*b.numer, denom*b.denom);
	};
	inline fract operator-(const fract &b) const {
		return fract(numer*b.denom - denom*b.numer, denom*b.denom);
	};
	inline fract operator*(const fract &b) const {
		return fract(numer*b.numer, denom*b.denom);
	};
	inline fract operator/(const fract &b) const {
		return fract(numer*b.denom, denom*b.numer);
	};
	inline bool operator<(const fract &b) const {
		return( numer*b.denom - denom*b.numer < 0 );
	};
	inline bool operator<=(const fract &b) const {
		return( numer*b.denom - denom*b.numer <= 0 );
	};
	inline bool operator>(const fract &b) const {
		return( numer*b.denom - denom*b.numer > 0 );
	};
	inline bool operator>=(const fract &b) const {
		return( numer*b.denom - denom*b.numer > 0 );
	};
	inline bool operator==(const fract &b) const {
		return( numer == b.numer && denom == b.denom );
	};
	inline bool operator!=(const fract &b) const {
		return( numer != b.numer || denom != b.denom );
	};

	fract() {numer = 0; denom = 1;}
	fract( const i64 &n ) {numer = n; denom = 1;}
	fract( const i64 &n, const i64 &d ) {
		numer = n;
		if( numer == 0 ) {denom = 1; return;}
		denom = d;
		if( denom < 0 ) {numer = -numer; denom = -denom;}
		i64 g = gcd(numer, denom);
		numer /= g;
		denom /= g;
	};
};
