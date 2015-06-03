#define BASE 10

class bigint {
public:
	vector<char> dig;

	bigint operator+(const bigint &b) const {
		char car = 0;
		vector<char> res;
		int i;
		for( i = 0; i < size() || i < b.size(); i++ ) {
			if( i < size() ) car += dig[i];
			if( i < b.size() ) car += b.dig[i];
			res.push_back( car%BASE );
			car /= BASE;
		}
		if( car ) res.push_back(car);
		return bigint(res);
	}
	bigint operator-(const bigint &b) const {
		char car = BASE;
		vector<char> res;
		int i;
		for( i = 0; i < size(); i++ ) {
			car += dig[i];
			if( i < b.size() ) car -= b.dig[i];
			res.push_back( car%BASE );
			car = (car/BASE)+BASE-1;
		}
		if( car < BASE || i < b.size() ) return bigint();
		while( res.size() > 1 && res.back() == 0 ) res.pop_back();
		return bigint(res);
	}
	bigint operator*(const bigint &b) const {
		if( b.zero() ) return bigint();
		bigint res, term;
		vector<bigint> dp(BASE);
		int i;
		for( i = 1; i < BASE; i++ ) dp[i] = dp[i-1] + b;
		for( i = 0; i < size(); i++ ) {
			if( !dig[i] ) continue;
			bigint &p = dp[dig[i]];
			term.dig = vector<char>(i);
			term.dig.insert(term.dig.end(), p.dig.begin(), p.dig.end());
			res = res + term;
		}
		return res;
	}
	bigint operator/(const bigint &b) const {
		if( b.zero() || b > *this ) return bigint();
		bigint cur = *this, term;
		vector<char> res;
		vector<bigint> dp(BASE);
		int i;
		char d;
		for( i = 1; i < BASE; i++ ) dp[i] = dp[i-1] + b;
		for( i = size()-b.size(); i >= 0; i-- ) {
			for( d = BASE-1; d > 0; d-- ) {
				bigint &p = dp[d];
				term.dig = vector<char>(i);
				term.dig.insert(term.dig.end(), p.dig.begin(), p.dig.end());
				if( term <= cur ) break;
			}
			res.push_back(d);
			if( d ) cur = cur - term;
		}
		reverse( res.begin(), res.end() );
		while( res.back() == 0 ) res.pop_back();
		return bigint(res);
	}
	bigint operator%(const bigint &b) const {
		return *this - (*this / b) * b;
	}
	bool operator<(const bigint &b) const {
		if( size() != b.size() ) return( size() < b.size() );
		for( int i = size()-1; i >= 0; i-- )
			if( dig[i] != b.dig[i] ) return( dig[i] < b.dig[i] );
		return false;
	}
	bool operator<=(const bigint &b) const {
		if( size() != b.size() ) return( size() < b.size() );
		for( int i = size()-1; i >= 0; i-- )
			if( dig[i] != b.dig[i] ) return( dig[i] < b.dig[i] );
		return true;
	}
	bool operator>(const bigint &b) const {return !(*this <= b);}
	bool operator>=(const bigint &b) const {return !(*this < b);}

	inline size_t size() const {return dig.size();}
	inline bool zero() const {return (dig.size() == 1 && dig[0] == 0);}
	u64 toint() const {
		u64 ret = 0;
		for( int i = size()-1; i >= 0; i-- )
			ret = ret*10 + dig[i];
	}
	string tostring() const {
		vector<char>::const_reverse_iterator it;
		string ret;
		for( it = dig.rbegin(); it != dig.rend(); it++ )
			ret += *it+'0';
		return ret;
	}

	bigint() {dig.push_back(0);}
	bigint( const vector<char> &vc ) {dig = vc;}
	bigint( u64 n ) {
		if( !n ) dig.push_back(0);
		while( n > 0 ) {
			dig.push_back(n%BASE);
			n /= BASE;
		}
	}
	bigint( const string &s ) {
		string::const_reverse_iterator it;
		for( it = s.rbegin(); it != s.rend(); it++ )
			dig.push_back(*it-'0');
	}
};
