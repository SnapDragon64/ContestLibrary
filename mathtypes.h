#ifndef __MATHTYPES_H
#define __MATHTYPES_H

//// Special structures that can be manipulated like numbers.
//// Formatted for process.exe.

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

//// *** BigInts ***

// A string-based BigInt.  Straightforward but rather slow.
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

// Copyright (c) Derek Kisman (SnapDragon)  dkisman@gmail.com
// You must ask permission to use my BigInt code in your contests
const string DIGITS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
template<int RADIX, int CHUNKRADIX=RADIX, int CHUNKSIZE=1, class CT=char>
struct BigInt {
  vector<CT> ch;
  bool neg;

  BigInt& operator+=(const BigInt& b) {
    int car = 0; unsigned x;
    for (int i = 0; i < ch.size() || i < b.ch.size(); i++) {
      x = CHUNKRADIX + car + ((i<ch.size()) ? ch[i] : 0);
      if (neg == b.neg) x += (i<b.ch.size()) ? b.ch[i] : 0;
      else x -= (i<b.ch.size()) ? b.ch[i] : 0;
      if (i == ch.size()) ch.push_back(x%CHUNKRADIX);
      else ch[i] = x%CHUNKRADIX;
      car = (x/CHUNKRADIX)-1;
    }
    if (car == -1) {
      neg = !neg;
      auto it = ch.begin();
      for (car = 0; it != ch.end(); it++) {
        x = car + CHUNKRADIX - *it;
        *it = x%CHUNKRADIX;
        car = (x/CHUNKRADIX)-1;
      }
    } else if (car) ch.push_back(car);
    trim();
    return *this;
  }
  inline BigInt operator+(const BigInt& b) const {return BigInt(*this)+=b;}
  inline BigInt& operator-=(const BigInt& b) {return *this += -b;}
  inline BigInt operator-(const BigInt& b) const {return BigInt(*this)-=b;}
  BigInt& operator*=(const BigInt& b) {
    BigInt b2(*this); *this = BigInt();
    vector<CT> zeros;
    for (auto it = b.ch.begin(); it != b.ch.end(); it++) {
      BigInt b3 = b2 * *it;
      b3.ch.insert(b3.ch.begin(), zeros.begin(), zeros.end());
      *this += b3;
      zeros.push_back(0);
    }
    if (b.neg && !zero()) neg = !neg;
    return *this;
  }
  inline BigInt operator*(const BigInt& b) const {return BigInt(*this)*=b;}
  BigInt& operator/=(BigInt b) {
    bool retneg = (b.neg) ? !neg : neg;
    b.neg = neg = false;
    if (b > *this || b.zero()) return (*this=BigInt());
    if (b.ch.size() == 1) return (*this /= retneg ? -b.ch[0] : b.ch[0]);
    if (CHUNKRADIX >= (1<<21)) {
      int bm = CHUNKRADIX/b.ch.back() + 1; b *= bm; *this *= bm;
    }
    int64 p, q = (int64)CHUNKRADIX*b.ch.back() + b.ch[b.ch.size()-2];
    vector<CT> ret(ch.size()-b.ch.size()+1);
    b.ch.insert(b.ch.begin(), ret.begin(), ret.end());
    for (int i = ret.size()-1, j = ch.size()-1; i >= 0; i--, j--) {
      b.ch.erase(b.ch.begin());
      if (j >= ch.size()) {ret[i] = 0; continue;}
      p = (int64)CHUNKRADIX*ch[j] + ch[j-1] + 1;
      if (j+1 < ch.size()) p += (int64)CHUNKRADIX*CHUNKRADIX*ch[j+1];
      *this -= b*(ret[i] = p/q);
      while (neg) {*this += b; ret[i]--;}
    }
    ch = ret; neg = retneg;
    trim();
    return *this;
  }
  inline BigInt operator/(const BigInt& b) const {return BigInt(*this)/=b;}
  inline BigInt& operator%=(const BigInt& b) {return *this -= (*this/b)*b;}
  inline BigInt operator%(const BigInt& b) const {return BigInt(*this)%=b;}
  BigInt& operator*=(int x) {
    if (!x || zero()) return (*this = BigInt());
    neg ^= (x<0);
    uint64 car = 0, xl = abs(x); if (xl == 1) return *this;
    for (auto it = ch.begin(); it != ch.end(); it++) {
      car += *it * xl;
      *it = car%CHUNKRADIX;
      car /= CHUNKRADIX;
    }
    while (car) {ch.push_back(car%CHUNKRADIX); car /= CHUNKRADIX;}
    return *this;
  }
  inline BigInt operator*(int x) const {return BigInt(*this)*=x;}
  BigInt& operator/=(int x) {
    if (!x) {return *this = BigInt();}
    neg ^= (x<0);
    uint64 car = 0; x = abs(x); if (x == 1) return *this;
    for (auto it = ch.rbegin(); it != ch.rend(); it++) {
      car = CHUNKRADIX*car + *it;
      *it = car / x;
      car %= x;
    }
    trim();
    return *this;
  }
  inline BigInt operator/(int x) const {return BigInt(*this)/=x;}
  int operator%(int x) const {
    int ret = 0; x = abs(x);
    auto it = ch.rbegin();
    if (CHUNKRADIX%x == 0 && ch.size()) {it = --ch.rend();}
    for (; it != ch.rend(); it++) ret = ((uint64)CHUNKRADIX*ret + *it)%x;
    return neg ? -ret : ret;
  }

  bool operator<(const BigInt& b) const {
    if (neg != b.neg) return neg;
    if (ch.size() != b.ch.size()) return neg ^ (ch.size() < b.ch.size());
    for (int i = ch.size()-1; i >= 0; i--)
      if (ch[i] != b.ch[i]) return neg ^ (ch[i] < b.ch[i]);
    return false;
  }
  bool operator<=(const BigInt& b) const {
    if (neg != b.neg) return neg;
    if (ch.size() != b.ch.size()) return neg ^ (ch.size() < b.ch.size());
    for (int i = ch.size()-1; i >= 0; i--)
      if (ch[i] != b.ch[i]) return neg ^ (ch[i] < b.ch[i]);
    return true;
  }
  inline bool operator==(const BigInt& b) const
    {return neg==b.neg && ch==b.ch;}
  inline bool operator>(const BigInt& b) const {return !(*this <= b);}
  inline bool operator>=(const BigInt& b) const {return !(*this < b);}
  inline bool operator!=(const BigInt& b) const {return !(*this == b);}
  friend inline BigInt operator-(const BigInt& b)
    {BigInt ret(b); ret.neg = !ret.neg; return ret;}
  friend inline BigInt abs(const BigInt& b) {return b.neg ? -b : b;}

  size_t digits() const {
    if (ch.size() == 0) return 1;
    size_t ret = (ch.size()-1)*CHUNKSIZE;
    for (int x = ch.back(); x; x /= RADIX) ret++;
    return ret;
  }
  inline bool zero() const {return ch.size() == 0;}
  int64 toint() const {
    int64 ret = 0;
    for (auto it = ch.rbegin(); it != ch.rend(); it++)
      ret = ret*CHUNKRADIX + *it;
    return neg ? -ret : ret;
  }
  string tostring() const {
    if (ch.size() == 0) return "0";
    auto it = ch.begin();
    string ret;
    int i, x;
    while (it != ch.end()) {
      x = *it;
      if (++it == ch.end()) break;
      for (i = 0; i < CHUNKSIZE; i++, x /= RADIX)
        ret += DIGITS[x%RADIX];
    }
    for (; x; x /= RADIX) ret += DIGITS[x%RADIX];
    if (neg) ret += '-';
    reverse(ret.begin(), ret.end());
    return ret;
  }
  friend inline ostream& operator<<(ostream& o, const BigInt& b)
    {return o << b.tostring();}
  friend inline istream& operator>>(istream& i, BigInt& b)
    {string s; i >> s; b = BigInt(s); return i;}
  inline void trim() {
    while (ch.size() && !ch.back()) ch.pop_back();
    if (zero()) neg = false;
  }

  inline BigInt() {neg = false;}
  inline BigInt(const BigInt& b) {ch = b.ch; neg = b.neg;}
  template<int R, int CR, int CS, class T>
  BigInt(const BigInt<R,CR,CS,T>& b) {
    if (b.zero()) {*this = BigInt(); return;}
    neg = false;
    BigInt p(1);
    for (int i = 0; i < b.ch.size(); i++) {
      *this += p*b.ch[i];
      p *= CR;
    }
    neg = b.neg;
  }
  BigInt(int64 n) {
    if ((neg = (n < 0))) n = -n;
    for (; n; n /= CHUNKRADIX) ch.push_back(n%CHUNKRADIX);
  }
  inline BigInt(int n) {*this = BigInt((int64)n);}
  BigInt(const char *s) {
    const char *it, *it2 = s-1;
    if ((neg = (*s=='-'))) it2++;
    int x = 0, y = 1, z;
    for (it = s; *it; it++);
    for (--it; it != it2; --it) {
      z = DIGITS.find(toupper(*it));
      if (z < 0 || z >= RADIX) {ch.clear(); x = 0; y = 1; continue;}
      x += y*z; y *= RADIX;
      if (y == CHUNKRADIX) {ch.push_back(x); x = 0; y = 1;}
    }
    if (y > 1) ch.push_back(x);
    trim();
  }
  inline BigInt(const string& s) {*this = BigInt(s.c_str());}
};

typedef BigInt<10, 1000000000, 9, int> bigint;
typedef BigInt<2, (1LL<<30), 30, unsigned> binarybigint;
typedef BigInt<16, (1<<28), 7, int> hexbigint;

#endif // __MATHTYPES_H
