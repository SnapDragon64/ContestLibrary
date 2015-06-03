//////////////// Contains all of my library declarations and functions for easy digestion by process.cc ////////////////

//// *** Typedefs ***

typedef signed long long int64;
typedef unsigned long long uint64;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<char> VC;
typedef vector<VC> VVC;
typedef vector<string> VS;
typedef vector<VS> VVS;
typedef vector<double> VD;
typedef vector<VD> VVD;
typedef vector<long long> VLL;
typedef vector<VLL> VVLL;
typedef vector<pair<int, int> > VPII;
typedef pair<int, int> PII;
typedef pair<int, pair<int, int> > PIPII;

//// *** Macros ***

#define ITER(s) typeof(s.begin())
#define FORALL(s,i) for (auto i=s.begin(); i != s.end(); ++i)
#define FORALLBITS(x,i) for (int xTmP=(x),i; (i=__builtin_ffs(xTmP)-1)>-1; xTmP&=xTmP-1)
#define ALL(s) s.begin(), s.end()

//// *** Constants ***

#define PI 3.14159265358979323846
#define EPS 1e-9
#define INF __builtin_inf()

//// *** Parsing ***

//// Tokenizing strings

vector<string> Tokenize(string s, string ch) {
  vector<string> ret;
  for (int p = 0, p2; p < s.size(); p = p2+1) {
    p2 = s.find_first_of(ch, p);
    if (p2 == -1) p2 = s.size();
    if (p2-p > 0) ret.push_back(s.substr(p, p2-p));
  }
  return ret;
}

vector<int> TokenizeInt(string s, string ch) {
  vector<int> ret;
  vector<string> p = Tokenize(s, ch);
  for( int i = 0; i < p.size(); i++ )
    ret.push_back(atoi(p[i].c_str()));
  return ret;
}

vector<long long> TokenizeLL(string s, string ch) {
  vector<long long> ret;
  vector<string> p = Tokenize(s, ch);
  for( int i = 0; i < p.size(); i++ )
    ret.push_back(StringToLL(p[i]));
  return ret;
}

vector<vector<int> > TokenizeMatrix(vector<string> s, string ch) {
  vector<vector<int> > ret;
  for( int i = 0; i < s.size(); i++ )
    ret.push_back( TokenizeInt(s[i], ch) );
  return ret;
}

//// Transformation to/from string representation

long long StringToLL(string s) {
  istringstream sin(s);
  long long ret;
  sin >> ret;
  return ret;
}

template<typename T> string ToString(T a) {
  ostringstream sout;
  sout << a;
  return sout.str();
}

string VCToString(const VC &a) {return string(a.begin(), a.end());}

VC StringToVC(const string &s) {return VC(s.begin(), s.end());}

template<typename T> string VectorToString(vector<T> v, char delim = ' ') {
  ostringstream sout;
  for (int i = 0; i < v.size(); i++) {
    if (i) sout << delim;
    sout << v[i];
  }
  return sout.str();
}

template<typename T> string ArrayToString(const T* ar, int n, char delim=' ') {
  ostringstream sout;
  for (int i = 0; i < n; i++) {
    if (i) sout << delim;
    sout << ar[i];
  }
  return sout.str();
}

//// *** Misc Functions ***

int IsLeapYear(int y) {return y%4 == 0 && (y%100 || y%400 == 0);}

int DaysSince1600(int d, int m, int y) {
  // Returns # of days since Jan. 1, 1600 (ie, 1 1 1600 returns 0).
  static int md[12] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
  return d-1 + md[m-1] + (y-1600)*365 + (y-(m<3)-1596)/4 - (y-(m<3)-1500)/100*3/4;
}

int DayOfWeek(int d, int m, int y) {
  // 0 = Monday, 6 = Sunday
  return (DaysSince1600(d, m, y)+5)%7;
}

string RomanNumeral(int x) {
  static string s[]={"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
  static int val[] ={1000,900, 500,400, 100,90,  50, 40,  10, 9,   5,  4,   1 };
  string ret;
  for (int i = 0; x; i++)
    while (x >= val[i]) {x -= val[i]; ret += s[i];}
  return ret;
}

struct HashString {int operator()(const string &s) const {return hash<const char*>()(s.c_str());}};

long long Rand64() {
  // Note that the upper bits of rand() are more reliably random.
  return ((long long)rand()<<33) ^
         ((long long)rand()<<11) ^
         ((long long)rand()>>9);
}

//// *** Bitwise Functions ***

inline int BitCount(unsigned x) {return __builtin_popcount(x);}

inline int BitCountLL(unsigned long long x) {return __builtin_popcountll(x);}

inline unsigned LowestBit(unsigned x) {return x & ~(x-1);}

inline unsigned long long LowestBitLL(unsigned long long x) {return x & ~(x-1);}

inline unsigned ClearLowestBit(unsigned x) {return x & (x-1);}

inline unsigned long long ClearLowestBitLL(unsigned long long x) {return x & (x-1);}

inline int LowestBitIndex(unsigned x) {return __builtin_ffs(x)-1;}

inline int LowestBitIndexLL(unsigned long long x) {return __builtin_ffsll(x)-1;}

//// *** Math Functions ***

//// Greatest Common Denominator

template<typename T> T Gcd(const T& a, const T& b) {return b != T() ? Gcd(b, a%b) : abs(a);}

//// Modular functions

inline int ModMult(int a, int b, int m) {return (long long)a*b%m;}

int ModPow(int a, unsigned long long b, int m) {
  int ret = (m!=1);
  for(;;) {
    if (b&1) ret = ModMult(ret, a, m);
    if (!(b>>=1)) return ret;
    a = ModMult(a, a, m);
  }
  return ret;
}

inline int ModInv(int a, int p) {  // Works for prime p only.
  return ModPow(a, p-2, p);
}

//// Primality and Factoring

bool IsPrime(int n) {
  if (n <= 3) return n > 1;
  if ((n&1) == 0 || n%3 == 0) return false;
  for (int x = 5, xsq = 5*5, xsqd = 11*11 - 5*5; xsq > 0 && xsq <= n;
       x += 6, xsq += xsqd, xsqd += (17*17-11*11) - (11*11-5*5))
    if (n%x == 0 || n%(x+2) == 0) return false;
  return true;
}

vector<int> PrimeFactors(int n) {
  vector<int> ret;
  while ((n&1) == 0) {ret.push_back(2); n /= 2;}
  while (n%3 == 0) {ret.push_back(3); n /= 3;}
  for (int x = 5, xsq = 5*5, xsqd = 11*11 - 5*5; xsq > 0 && xsq <= n;
       x += 6, xsq += xsqd, xsqd += (17*17-11*11) - (11*11-5*5)) {
    while (n%x == 0) {ret.push_back(x); n /= x;}
    while (n%(x+2)==0) {ret.push_back(x+2); n /= x+2;}
  }
  if (n > 1) ret.push_back(n);
  return ret;
}

vector<int> UniquePrimeFactors(int n) {
  vector<int> ret;
  if ((n&1) == 0) {ret.push_back(2); do {n /= 2;} while ((n&1) == 0);}
  if (n%3 == 0) {ret.push_back(3); do {n /= 3;} while (n%3 == 0);}
  for (int x = 5, xsq = 5*5, xsqd = 11*11 - 5*5; xsq > 0 && xsq <= n;
       x += 6, xsq += xsqd, xsqd += (17*17-11*11) - (11*11-5*5)) {
    if (n%x == 0) {ret.push_back(x); do {n /= x;} while (n%x == 0);}
    if (n%(x+2)==0) {ret.push_back(x+2); do {n /= x+2;} while (n%(x+2) == 0);}
  }
  if (n > 1) ret.push_back(n);
  return ret;
}

// As slow as factoring a number.
int EulerPhi(int n) {
  int ret = n;
  if ((n&1) == 0) {ret >>= 1; do {n /= 2;} while ((n&1) == 0);}
  if (n%3 == 0) {ret /= 3; ret *= 2; do {n /= 3;} while (n%3 == 0);}
  for (int x = 5, xsq = 5*5, xsqd = 11*11 - 5*5; xsq > 0 && xsq <= n;
       x += 6, xsq += xsqd, xsqd += (17*17-11*11) - (11*11-5*5)) {
    if (n%x == 0) {ret /= x; ret *= x-1; do {n /= x;} while (n%x == 0);}
    if (n%(x+2)==0) {ret /= x+2; ret *= x+1; do {n /= x+2;} while (n%(x+2) == 0);}
  }
  if (n > 1) {ret /= n; ret *= n-1;}
  return ret;
}

//// Combinations/Permutations

int Comb(int a, int b) {
  #define COMB__MAXA 1000
  #define COMB__MAXB 1000
  if (b > a || b < 0) return 0;
  if (!a) return 1;
  static int combmemo[COMB__MAXA+1][COMB__MAXB+1];
  int &ret = combmemo[a][b];
  if (!ret) ret = Comb(a-1, b-1)+Comb(a-1,b);
  return ret;
}

//// *** Matrix Functions and Linear Algebra ***

template<typename T> vector<vector<T> > MatrixAdd(const vector<vector<T> > &a, const vector<vector<T> > &b) {
  if (!a.size() || a.size() != b.size() || a[0].size() != b[0].size()) {
    return vector<vector<T> >();  // ERROR
  }
  vector<vector<T> > ret(a.size(), vector<T>(a[0].size()));
  for (int i = 0; i < ret.size(); i++)
  for (int j = 0; j < ret[0].size(); j++)
    ret[i][j] = a[i][j] + b[i][j];
  return ret;
}

template<typename T> vector<vector<T> > MatrixMult(const vector<vector<T> > &a, const vector<vector<T> > &b) {
  if (!a.size() || !b.size() || a[0].size() != b.size()) {
    return vector<vector<T> >();  // ERROR
  }
  vector<vector<T> > ret(a.size(), vector<T>(b[0].size()));
  for (int i = 0; i < ret.size(); i++)
  for (int j = 0; j < ret[0].size(); j++)
  for (int k = 0; k < a[0].size(); k++)
    ret[i][j] += a[i][k] * b[k][j];
  return ret;
}

template<typename T> vector<vector<T> > MatrixPow(vector<vector<T> > a, int pow) {
  if (!a.size() || a.size() != a[0].size()) {
    return vector<vector<T> >();  // ERROR
  }
  vector<vector<T> > ret(a.size(), vector<T>(a[0].size()));
  for (int i = 0; i < ret.size(); i++) ret[i][i] = 1;
  for(;;) {
    if (pow&1) ret = MatrixMult(ret, a);
    if (!(pow>>=1)) return ret;
    a = MatrixMult(a, a);
  }
  return ret;
}

template<typename T> vector<vector<T> > MatrixRotateCL(const vector<vector<T> > &a) {
  if (!a.size()) return a;
  vector<vector<T> > ret(a[0].size(), vector<T>(a.size()));
  for (int i = 0; i < a.size(); i++)
  for (int j = 0; j < a[0].size(); j++)
    ret[j][a.size()-1-i] = a[i][j];
  return ret;
}

template<typename T> vector<vector<T> > MatrixRotateCCL(const vector<vector<T> > &a) {
  if (!a.size()) return a;
  vector<vector<T> > ret(a[0].size(), vector<T>(a.size()));
  for (int i = 0; i < a.size(); i++)
  for (int j = 0; j < a[0].size(); j++)
    ret[i][j] = a[j][a.size()-1-i];
  return ret;
}

template<typename T> vector<vector<T> > MatrixTranspose(const vector<vector<T> > &a) {
  if (!a.size()) return a;
  vector<vector<T> > ret(a[0].size(), vector<T>(a.size()));
  for (int i = 0; i < a.size(); i++)
  for (int j = 0; j < a[0].size(); j++)
    ret[j][i] = a[i][j];
  return ret;
}

template<typename T> vector<vector<T> > MatrixRotate180(vector<vector<T> > a) {
  reverse(a.begin(), a.end());
  for (int i = 0; i < a.size(); i++) reverse(a[i].begin(), a[i].end());
  return a;
}

template<typename T> vector<vector<T> > MatrixFlipHorizontal(vector<vector<T> > a) {
  for (int i = 0; i < a.size(); i++) reverse(a[i].begin(), a[i].end());
  return a;
}

template<typename T> vector<vector<T> > MatrixFlipVertical(vector<vector<T> > a) {
  reverse(a.begin(), a.end());
  return a;
}

template<typename T> vector<vector<T> > MatrixPad(const vector<vector<T> >& a, T pad) {
  vector<vector<T> > ret(a.size()+2, vector<T>(a[0].size()+2, pad));
  for (int i = 0; i < a.size(); i++)
  for (int j = 0; j < a[0].size(); j++)
    ret[i+1][j+1] = a[i][j];
  return ret;
}

//// *** Functional Optimizing ***

double Func(double x) {
  // Your function here.
}

//// *** Integer with constant modulus ***

// Note: MOD should be < 2^30.  If division is used, MOD must be prime.
template<int MOD> struct ModInt {
  int value;
  ModInt() : value(0) {}
  ModInt(int v) : value(v % MOD) {}
  ModInt(long long v) : value(v % MOD) {}
  ModInt Pow(int b) const {
    ModInt ret, a; ret.value = (MOD!=1); a.value = value;
    for(;;) {if (b&1) ret *= a; if (!(b>>=1)) return ret; a *= a;}
  }
  ModInt Inverse() const { return Pow(MOD-2); }
  ModInt operator+(const ModInt& m) const {return value+m.value;}
  ModInt& operator+=(const ModInt& m) {value = (value+m.value) % MOD; return *this;}
  ModInt operator-(const ModInt& m) const {return value-m.value;}
  ModInt& operator-=(const ModInt& m) {value = (value-m.value) % MOD; return *this;}
  ModInt operator-() const {return -value;}
  ModInt operator*(const ModInt& m) const {return (long long)value*m.value;}
  ModInt& operator*=(const ModInt& m) {value = ((long long)value*m.value) % MOD; return *this;}
  ModInt operator/(const ModInt& m) const {return *this * m.Inverse();}
  ModInt& operator/=(const ModInt& m) {return *this *= m.Inverse();}
  bool operator==(const ModInt& m) const {return toint() == m.toint();}
  bool operator!=(const ModInt& m) const {return toint() != m.toint();}
  int toint() const {return (value < 0) ? value + MOD : value;}
  friend ostream& operator<<(ostream& out, const ModInt& m) {out << m.toint(); return out;}
};

//// *** Fraction class ***

template<typename T = long long> struct Fract {
  T n, d;
  Fract(const T& n=0) : n(n), d(1) {}
  Fract(const T& n, const T& d, bool reduce=true) : n(n), d(d) {if (reduce) Reduce();}
  void Reduce() {if (d < 0) {n = -n; d = -d;} T g = Gcd(n, d); if (g != 1) {n /= g; d /= g;}}
  Fract operator+(const Fract& f) const {return Fract(n*f.d+f.n*d, d*f.d);}
  Fract& operator+=(const Fract& f) {n = n*f.d+f.n*d; d *= f.d; Reduce(); return *this;}
  Fract operator-(const Fract& f) const {return Fract(n*f.d-f.n*d, d*f.d);}
  Fract& operator-=(const Fract& f) {n = n*f.d-f.n*d; d *= f.d; Reduce(); return *this;}
  Fract operator-() const {return Fract(-n, d, false);}
  Fract operator*(const Fract& f) const {return Fract(n*f.n, d*f.d);}
  Fract& operator*=(const Fract& f) {n *= f.n; d *= f.d; Reduce(); return *this;}
  Fract operator/(const Fract& f) const {return Fract(n*f.d, d*f.n);}
  Fract& operator/=(const Fract& f) {n *= f.d; d *= f.n; Reduce(); return *this;}
  bool operator<(const Fract& f) const {return n*f.d < d*f.n;}
  bool operator<=(const Fract& f) const {return n*f.d <= d*f.n;}
  bool operator>(const Fract& f) const {return n*f.d > d*f.n;}
  bool operator>=(const Fract& f) const {return n*f.d >= d*f.n;}
  bool operator==(const Fract& f) const {return n == f.n && d == f.d;}
  bool operator!=(const Fract& f) const {return n != f.n || d != f.d;}
  double todouble() const {return (double)n/(double)d;}
  friend Fract abs(const Fract& f) {return Fract(abs(f.n), f.d, false);}
  friend ostream& operator<<(ostream& out, const Fract& f) {out << f.n; if (f.d != 1) out << '/' << f.d; return out;}
};

//// *** Simple Bigint (string-based) class ***

struct SimpleBigInt {
  bool neg;
  string dig;
  SimpleBigInt() : dig("0"), neg(false) {}
  SimpleBigInt(int x)
    {if ((neg=x<0)) x=-x; do {dig.push_back(x%10+'0'); x /= 10;} while (x); reverse(dig.begin(), dig.end());}
  SimpleBigInt(long long x)
    {if ((neg=x<0)) x=-x; do {dig.push_back(x%10+'0'); x /= 10;} while (x); reverse(dig.begin(), dig.end());}
  SimpleBigInt(const string& s) {if ((neg=(s[0]=='-'))) dig = s.substr(1); else dig = s;}
  SimpleBigInt(const string& dig, bool neg) : dig(dig), neg(neg) {}
  static bool LessThan(const string& a, bool an, const string& b, bool bn, bool count_eq) {
    if (an != bn) return an;
    if (a.size() != b.size()) return (a.size() < b.size()) ^ an;
    for (int i = 0; i < a.size(); i++) if (a[i] != b[i]) return (a[i] < b[i]) ^ an;
    return count_eq;
  }
  static SimpleBigInt AddOrSubtract(const string& a, bool an, const string& b, bool bn) {
    SimpleBigInt ret("", an);
    if (an == bn) {
      for (int i = 0, c = 0; i < a.size() || i < b.size() || c; i++) {
        if (i < a.size()) c += a[a.size()-1-i]-'0';
        if (i < b.size()) c += b[b.size()-1-i]-'0';
        ret.dig += c%10 + '0'; c /= 10;
      }
    } else {
      bool swp = LessThan(a, false, b, false, an);
      if (swp) ret.neg = bn;
      const string& s1 = (swp ? b : a), s2 = (swp ? a : b);
      for (int i = 0, c = 0; i < s1.size(); i++) {
        c += 10 + s1[s1.size()-1-i]-'0';
        if (i < s2.size()) c -= s2[s2.size()-1-i]-'0';
        ret.dig += c%10 + '0'; c = c/10-1;
      }
      for (int i = ret.dig.size()-1; i >= 0; i--)
        if (i==0 || ret.dig[i]!='0') {ret.dig = ret.dig.substr(0, i+1); break;}
    }
    reverse(ret.dig.begin(), ret.dig.end());
    return ret;
  }
  SimpleBigInt operator+(const SimpleBigInt& b) const {return AddOrSubtract(dig, neg, b.dig, b.neg);}
  SimpleBigInt& operator+=(const SimpleBigInt& b) {*this = AddOrSubtract(dig, neg, b.dig, b.neg); return *this;}
  SimpleBigInt operator-(const SimpleBigInt& b) const {return AddOrSubtract(dig, neg, b.dig, !b.neg);}
  SimpleBigInt& operator-=(const SimpleBigInt& b) {*this = AddOrSubtract(dig, neg, b.dig, !b.neg); return *this;}
  SimpleBigInt operator-() const {return SimpleBigInt(dig, !(neg || dig=="0"));}
  SimpleBigInt operator*(int x) const {  // NOTE: x*10 should not overflow int.
    if (x == 0) return SimpleBigInt();
    SimpleBigInt ret("", neg);
    if (x < 0) {x = -x; ret.neg = !neg && dig != "0";}
    for (int i = 0, c = 0; i < dig.size() || c; i++) {
      if (i < dig.size()) c += x * (dig[dig.size()-1-i]-'0');
      ret.dig += c%10 + '0'; c /= 10;
    }
    reverse(ret.dig.begin(), ret.dig.end());
    return ret;
  }
  SimpleBigInt& operator*=(int x) {return *this = *this * x;}
  SimpleBigInt operator*(const SimpleBigInt& b) const {
    if (b == 0 || *this == 0) return SimpleBigInt();
    SimpleBigInt ret;
    for (int i = 0; i < b.dig.size(); i++) {
      char ch = b.dig[b.dig.size()-1-i];
      if (ch == '0') continue;
      SimpleBigInt v = *this * (int)(ch-'0');
      for (int j = 0; j < i; j++) v.dig += '0';
      ret += v;
    }
    if (b.neg) ret.neg = !ret.neg;
    return ret;
  }
  SimpleBigInt& operator*=(const SimpleBigInt& b) {return *this = *this * b;}
  SimpleBigInt operator/(SimpleBigInt b) const {
    if (b == 0 || *this == 0) return SimpleBigInt();
    SimpleBigInt ret("", neg ^ b.neg), rem(dig, false);
    b.neg = false;
    int d = 0;
    while (b <= rem) {b.dig += '0'; d++;}
    if (d == 0) return SimpleBigInt();
    ret.dig = string(d, '0');
    for (d--; d >= 0; d--) {
      b.dig.resize(b.dig.size()-1);
      while (b <= rem) {rem -= b; ret.dig[ret.dig.size()-1-d]++;}
    }
    return ret;
  }
  SimpleBigInt& operator/=(const SimpleBigInt& b) {return *this = *this / b;}
  SimpleBigInt operator%(const SimpleBigInt& b) const {return *this - b * (*this / b);}
  SimpleBigInt& operator%=(const SimpleBigInt& b) {return *this -= b * (*this / b);}
  bool operator<(const SimpleBigInt& b) const {return LessThan(dig, neg, b.dig, b.neg, false);}
  bool operator<=(const SimpleBigInt& b) const {return LessThan(dig, neg, b.dig, b.neg, true);}
  bool operator>(const SimpleBigInt& b) const {return !LessThan(dig, neg, b.dig, b.neg, true);}
  bool operator>=(const SimpleBigInt& b) const {return !LessThan(dig, neg, b.dig, b.neg, false);}
  bool operator==(const SimpleBigInt& b) const {return neg == b.neg && dig == b.dig;}
  bool operator!=(const SimpleBigInt& b) const {return neg != b.neg || dig != b.dig;}
  string tostring() const {return (neg ? "-" : "") + dig;}
  friend SimpleBigInt abs(const SimpleBigInt& b) {return SimpleBigInt(b.dig, false);}
  friend istream& operator>>(istream& in, SimpleBigInt& b) {string s; in >> s; b = SimpleBigInt(s); return in;}
  friend ostream& operator<<(ostream& out, const SimpleBigInt& b) {if (b.neg) out << '-'; out << b.dig; return out;}
};

//// *** Polynomial class ***

template<typename T> struct Polynomial {
  vector<T> co;
  Polynomial() : co() {}
  Polynomial(const T& x) : co(1, x) {Simplify();}
  Polynomial(const T& x, const T& y) : co(2) {co[1] = x; co[0] = y; Simplify();}
  Polynomial(const vector<T>& v) : co(v) {Simplify();}
  T Eval(const T& x) const {T ret = T(); for (int i = co.size()-1; i >= 0; i--) {ret *= x; ret += co[i];} return ret;}
  void Simplify() {while (co.size() && co.back() == T()) co.pop_back();}
  Polynomial operator+(const Polynomial& p) const {Polynomial ret = *this; return ret += p;}
  Polynomial& operator+=(const Polynomial& p) {
    if (p.co.size() > co.size()) co.resize(p.co.size());
    for (int i = 0; i < p.co.size(); i++) co[i] += p.co[i];
    Simplify();
    return *this;
  }
  Polynomial operator-(const Polynomial& p) const {Polynomial ret = *this; return ret -= p;}
  Polynomial& operator-=(const Polynomial& p) {
    if (p.co.size() > co.size()) co.resize(p.co.size());
    for (int i = 0; i < p.co.size(); i++) co[i] -= p.co[i];
    Simplify();
    return *this;
  }
  Polynomial operator-() const
    {Polynomial ret = *this; for (int i = 0; i < ret.co.size(); i++) ret.co[i] = -ret.co[i]; return ret;}
  Polynomial operator*(const T& x) const {Polynomial ret = *this; return ret *= x;}
  Polynomial& operator*=(const T& x) {for (int i = 0; i < co.size(); i++) co[i] *= x; Simplify(); return *this;}
  Polynomial operator/(const T& x) const {Polynomial ret = *this; return ret /= x;}
  Polynomial& operator/=(const T& x) {for (int i = 0; i < co.size(); i++) co[i] /= x; Simplify(); return *this;}
  Polynomial operator*(const Polynomial& p) const {
    if (!co.size() || !p.co.size()) return Polynomial();
    Polynomial ret;
    ret.co.resize(co.size() + p.co.size() - 1);
    for (int i = 0; i < co.size(); i++)
    for (int j = 0; j < p.co.size(); j++)
      ret.co[i+j] += co[i] * p.co[j];
    ret.Simplify();
    return ret;
  }
  Polynomial& operator*=(const Polynomial& p) {*this = *this * p; return *this;}
  friend ostream& operator<<(ostream& out, const Polynomial& p) {
    if (!p.co.size()) {out << 0; return out;}
    for (int i = p.co.size()-1; i >= 0; i--) if (p.co[i] != T()) {
      if (i < p.co.size()-1 || p.co[i] < 0) out << ((p.co[i] < 0) ? '-' : '+');
      if (abs(p.co[i]) != T(1) || i == 0) out << abs(p.co[i]);
      if (i > 0) out << 'x';
      if (i > 1) out << '^' << i;
    }
    return out;
  }
};

template<typename T> Polynomial<T> PolynomialDerivative(const Polynomial<T>& p) {
  Polynomial<T> ret;
  for (int i = 1; i < p.co.size(); i++)
    ret.co.push_back(p.co[i] * i);
  ret.Simplify();
  return ret;
}

// Note: Returns the integral with constant coefficient 0.
template<typename T> Polynomial<T> PolynomialIntegral(const Polynomial<T>& p) {
  Polynomial<T> ret;
  ret.co.push_back(T());
  for (int i = 0; i < p.co.size(); i++)
    ret.co.push_back(p.co[i] / (i+1));
  ret.Simplify();
  return ret;
}

template<typename T> Polynomial<T> PolynomialSubstitute(const Polynomial<T>& p, const Polynomial<T>& x) {
  Polynomial<T> xpow(1), ret;
  for (int i = 0; i < p.co.size(); i++) {
    if (i) xpow *= x;
    ret += xpow * p.co[i];
  }
  ret.Simplify();
  return ret;
}

// Note: T must support division.
template<typename T> Polynomial<T> PolynomialSum0ToX(const Polynomial<T>& p) {
  static vector<Polynomial<T> > powers_of_x_plus_one;
  static vector<Polynomial<T> > power_sums;
  if (powers_of_x_plus_one.size() == 0)
    powers_of_x_plus_one.push_back(Polynomial<T>(1));
  if (powers_of_x_plus_one.size() == 1)
    powers_of_x_plus_one.push_back(Polynomial<T>(vector<T>(2, 1)));
  for (int i = powers_of_x_plus_one.size(); i <= p.co.size(); i++)
    powers_of_x_plus_one.push_back(powers_of_x_plus_one[i-1] *
                                   powers_of_x_plus_one[1]);
  for (int n = power_sums.size(); n < p.co.size(); n++) {
    power_sums.push_back(powers_of_x_plus_one[n+1]);
    for (int i = 0; i < n; i++)
      power_sums.back() -= power_sums[i] * powers_of_x_plus_one[n+1].co[i];
    for (int i = 0; i <= n+1; i++)
      power_sums.back().co[i] /= n+1;
  }
  Polynomial<T> ret;
  for (int i = 0; i < p.co.size(); i++) ret += power_sums[i] * p.co[i];
  return ret;
}

template<typename T> string PolynomialToString(const Polynomial<T>& p) {
  ostringstream out;
  out << p;
  return out.str();
}

//// *** Algorithms ***

//// Bipartite Matching

// Returns -1 for unmatched items.
// Complexity: O(V*E)
VI BipartiteMatch(const VVI &mat, VI *back_match = NULL) {
  int max_item = -1;
  VI fmat(mat.size(), -1), seen(mat.size(), -1), prev(mat.size());
  for (int i = 0; i < mat.size(); i++) if (mat[i].size())
    max_item >?= *max_element(mat[i].begin(), mat[i].end());
  VI bmat(max_item+1, -1);

  for (int i = 0; i < mat.size(); i++) {
    VI q(1, i);
    seen[i] = i; prev[i] = -1;
    int x, y;
    while (!q.empty()) {
      x = q.back(); q.pop_back();
      for (VI::const_iterator it = mat[x].begin(); it != mat[x].end(); ++it) {
        int bm = bmat[*it];
        if (bm == -1) {y = *it; goto found_match;}
        if (seen[bm] < i) {
          seen[bm] = i; prev[bm] = x;
          q.push_back(bm);
        }
      }
    }
    continue;
found_match:
    while (x != -1) {
      bmat[y] = x;
      swap(y, fmat[x]);
      x = prev[x];
    }
  }

  if (back_match) *back_match = bmat;
  return fmat;
}

