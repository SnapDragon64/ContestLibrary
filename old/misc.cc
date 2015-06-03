// SORT TOOLS
// Comparison function
class cmp : binary_function<T, T, bool> {
public:
bool operator()(T a, T b) const {
}
};

// Functions to convert two vectors into a vector of pairs, and vice versa
template <class T1, class T2>
vector<pair<T1,T2> > topair(const vector<T1> &v1, const vector<T2> &v2) {
	vector<pair<T1,T2> > ret;
	vector<T1>::const_iterator it1;
	vector<T2>::const_iterator it2;
	for( it1 = v1.begin(), it2 = v2.begin();
		 it1 != v1.end() && it2 != v2.end(); it1++, it2++ )
		ret.push_back(pair<T1,T2>(*it1, *it2));
	return ret;
}

template <class T1, class T2>
void frompair(const vector<pair<T1,T2> > &v, vector<T1> &v1, vector<T2> &v2) {
	v1.clear(); v2.clear();
	for( vector<pair<T1,T2> >::const_iterator it = v.begin();
		 it != v.end(); it++ ) {
		v1.push_back(it->first);
		v2.push_back(it->second);
	}
}

// Useful for sorting one vector by a second vector of priorities
template <class T1, class T2>
void sortby(vector<T1> &v, vector<T2> &priority) {
	vector<pair<T2,T1> > pv = topair(priority, v);
	sort(pv.begin(), pv.end());
	frompair(pv, priority, v);
}


// RECTANGULAR ARRAY STUFF
vector<string> buffer(const vector<string> &m, char b) {
	if( m.size() == 0 || m[0].size() == 0 )
		return vector<string>(1, string()+b);
	string s;
	int i, j;
	for( i = 0; i < m[0].size()+2; i++ ) s += b;
	vector<string> buf(m.size()+2, s);
	for( j = 0; j < m.size(); j++ )
	for( i = 0; i < m[0].size(); i++ )
		buf[j+1][i+1] = m[j][i];
	return buf;
}

vector<string> rotatecl(const vector<string> &m) {
	vector<string> ret(m[0].size());
	for( int y = 0; y < m[0].size(); y++ )
	for( int x = 0; x < m.size(); x++ )
		ret[y] += m[m.size()-1-x][y];
	return ret;
}

vector<string> rotateccl(const vector<string> &m) {
	vector<string> ret(m[0].size());
	for( int y = 0; y < m[0].size(); y++ )
	for( int x = 0; x < m.size(); x++ )
		ret[y] += m[x][m[0].size()-1-y];
	return ret;
}

vector<string> rotate180(const vector<string> &m) {
	vector<string> ret(m.size());
	for( int y = 0; y < m.size(); y++ )
	for( int x = 0; x < m[0].size(); x++ )
		ret[y] += m[m.size()-1-y][m[0].size()-1-x];
	reverse(m.begin(), m.end();
}

vector<string> transpose(const vector<string> &m) {
	vector<string> ret(m[0].size());
	for( int y = 0; y < m[0].size(); y++ )
	for( int x = 0; x < m.size(); x++ )
		ret[y] += m[x][y];
	return ret;
}

int tobitmap(const string &s, const string &ones) {
	int ret = 0;
	for( int i = 0, x = 1; i < s.size(); i++, x <<= 1 )
		if( ones.find(s[i]) != -1 ) ret += x;
	return ret;
}

int tobitmap(const vector<string> &s, const string &ones) {
	int ret = 0;
	for( int i = 0, x = 1; i < s.size(); i++, x <<= 1 )
	for( int j = 0; j < s[0].size(); j++, x <<= 1 )
		if( ones.find(s[i][j]) != -1 ) ret += x;
	return ret;
}

string frombitmap(int b, int n, char one, char zero) {
	string ret;
	for( ; n > 0; n--, b >>= 1 ) ret += (b&1)?one:zero;
	return ret;
}

vector<string> frombitmap(int b, int xn, int yn, char one, char zero) {
	vector<string> ret(yn);
	for( int y = 0; y < yn; y++, b >>= 1 )
	for( int x = 0; x < xn; x++, b >>= 1 )
		ret[y] += (b&1)?one:zero;
	return ret;
}


// HEX COORDINATES
// NOTE: x is east, y is northeast
int hdx[6] = {1, 0, -1, -1, 0, 1};
int hdy[6] = {0, 1, 1, 0, -1, -1};

void hexrotateccl(int &x, int &y, int steps) {
	int x2 = hdx[steps]*x + hdx[(steps+1)%6]*y;
	y = hdy[steps]*x + hdy[(steps+1)%6]*y;
	x = x2;
}

int hexdist(int x, int y) {
	int ret = abs(x)+abs(y);
	ret = min(ret, abs(y)+abs(x+y));
	return min(ret, abs(x+y)+abs(x));
}


// CARDS
// NOTE: h must always be hand of 5 cards
string rankval = "A23456789TJQKA";
string suitval = "CDHS";

inline int cardval(string c) {
	return rankval.find(c[0]) + suitval.find(c[1])*13;
}

enum {HighCard, Pair, TwoPair, ThreeOfAKind, Straight, Flush,
	  FullHouse, FourOfAKind, StraightFlush};
int ratehand( VI h ) {
	VI r(13), n(5);
	bool flush = true, straight;
	int i, j, x, rs=12, re=0;
	for( i = 1; i < 5; i++ ) if( h[i]/13 != h[i-1]/13 ) flush = false;
	for( i = 0; i < 5; i++ ) {
		x = h[i]%13;
		n[++r[x]]++;
		rs = min(rs, x);
		re = max(re, x);
	}
	if( n[2] == 0 ) {
		if( re-rs == 4 ) straight = true;
		else if( r[0] && r[9] && r[10] && r[11] && r[12] ) straight = true;
	}
	if( flush && straight ) return StraightFlush;
	if( n[4] == 1 ) return FourOfAKind;
	if( n[3] == 1 && n[2] == 2 ) return FullHouse;
	if( flush ) return Flush;
	if( straight ) return Straight;
	if( n[3] == 1 ) return ThreeOfAKind;
	if( n[2] == 2 ) return TwoPair;
	if( n[2] ) return Pair;
	return HighCard;
}

inline bool isflush( VI h ) {
	int x = h[0]/13;
	return x==h[1]/13 && x==h[2]/13 && x==h[3]/13 && x==h[4]/13;
}

inline bool isstraight( VI h ) {
	VI r(13);
	int rs=12, re=0;
	for( int i = 0; i < h.size(); i++ ) {
		int x = h[i]%13;
		if( r[x]++ ) return false;
		rs = min(rs, x);
		re = max(re, x);
	}
	if( re-rs == 4 ) return true;
	return r[0] && r[9] && r[10] && r[11] && r[12];
}

bool cardcmp( int c1, int c2 ) {
	int x = c1%13, y = c2%13;
	x = (x==0)?13:x;
	y = (y==0)?13:y;
	if( x != y ) return x < y;
	return c1/13 < c2/13;
}

// NOTE: poker tie-breaker returns the highest card of the largest match,
// except in the case of straights with low ace
int tiebreaker( VI h ) {
	VI r(13), n(5, -1);
	int i;
	for( i = 0; i < h.size(); i++ ) {
		int x = h[i]%13;
		r[x]++;
		int &y = n[r[x]];
		if( y == -1 || cardcmp(y, h[i]) ) y = h[i];
	}
	if( r[0] && r[1] && r[2] && r[3] && r[4] )
		for( i = 0; i < h.size(); i++ )
			if( h[i]%13 == 4 ) return i;
	for( i = 4; ; i-- )
		if( n[i] != -1 ) return n[i];
}

bool handcmp( VI h1, VI h2 ) {
	int x = ratehand(h1), y = ratehand(h2);
	if( x != y ) return x < y;
	return cardcmp(tiebreaker(h1), tiebreaker(h2));
}

// HASH TOOLS
// String hash function
class hash<string> : hash<const char *> {
public:
size_t operator()(const string &s) const {
	return ((hash<const char*>)*this)(s.c_str());
}
};
