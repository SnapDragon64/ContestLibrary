#ifndef __MATH_H
#define __MATH_H

//// Advanced math functions.  Formatted for process.exe.

//// Modular functions

inline unsigned ModMult(unsigned a, unsigned b, unsigned m) {
  return (uint64)a*b%m;
}

unsigned ModPow(unsigned a, uint64 b, unsigned m) {
  unsigned ret = (m>1);
  for(;;) {
    if (b&1) ret = ModMult(ret, a, m);
    if (!(b>>=1)) return ret;
    a = ModMult(a, a, m);
  }
}

inline unsigned ModInv(unsigned a, unsigned p) {  // Works for prime p only.
  return ModPow(a, p-2, p);
}

inline uint64 ModMult48(uint64 a, uint64 b, uint64 m) {  // Requires m < 2^42.
  uint64 ahi = (a>>21);
  uint64 alo = (a&0x1fffff);
  uint64 rethi = (ahi*b)%m;
  return ((rethi<<21) + alo*b)%m;
}

uint64 ModPow48(uint64 a, uint64 b, uint64 m) {  // Requires m < 2^42.
  uint64 ret = (m>1);
  for(;;) {
    if (b&1) ret = ModMult48(ret, a, m);
    if (!(b>>=1)) return ret;
    a = ModMult48(a, a, m);
  }
}

inline uint64 ModInv48(uint64 a, uint64 p) {  // Requires prime p < 2^42.
  return ModPow48(a, p-2, p);
}

inline uint64 ModMult64(uint64 a, uint64 b, uint64 m) {
  uint64 hi, lo;
  Mult128(a, b, hi, lo);
  for (int i = 0; i < 64; i++) {
    bool car = (hi>>63);
    hi = (hi<<1) | (lo >> 63);
    lo <<= 1;
    if (car || hi >= m) hi -= m;
  }
  return hi;
}

uint64 ModPow64(uint64 a, uint64 b, uint64 m) {
  uint64 ret = (m>1);
  for(;;) {
    if (b&1) ret = ModMult64(ret, a, m);
    if (!(b>>=1)) return ret;
    a = ModMult64(a, a, m);
  }
}

inline uint64 ModInv64(uint64 a, uint64 p) {  // Requires prime p.
  return ModPow64(a, p-2, p);
}

//// *** Primality and Factoring ***

template<typename T> T Gcd(const T& a, const T& b) {
  return b != T() ? Gcd(b, a%b) : abs(a);
}

vector<bool> prime;
void PrimeSieve(int limit) {
  prime = vector<bool>(limit+1, true);
  prime[0] = prime[1] = false;
  for (int x = 2; x*x <= limit; x++) if (prime[x]) {
    int inc = (x==2) ? x : 2*x;
    for (int y = x*x; y <= limit; y += inc) {
      prime[y] = false;
    }
  }
}

bool IsPrime(uint64 n) {
  if (n <= 3) return n > 1;
  if ((n&1) == 0 || n%3 == 0) return false;
  for (uint64 x = 5, xsq = 5*5, xsqd = 11*11 - 5*5; xsq > 0 && xsq <= n;
       x += 6, xsq += xsqd, xsqd += (17*17-11*11) - (11*11-5*5))
    if (n%x == 0 || n%(x+2) == 0) return false;
  return true;
}

// Works for all primes p < 2^32.
bool IsPrimeFast(unsigned n) {
  if (n <= 3) return (n >= 2);
  static const unsigned small[] =
    {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67};
  for (int i = 0; i < sizeof(small)/sizeof(unsigned); i++) {
    if (n%small[i] == 0) return n == small[i];
  }

  // Jaeschke93 showed that 2,7,61 suffice for n < 4,759,123,141.
  static const unsigned millerrabin[] = {2, 7, 61};
  unsigned s = n-1, r = 0;
  while (s%2 == 0) {s /= 2; r++;}
  for (int i = 0, j; i < sizeof(millerrabin)/sizeof(unsigned); i++) {
    unsigned md = ModPow(millerrabin[i], s, n);
    if (md == 1) continue;
    for (j = 1; j < r; j++) {
      if (md == n-1) break;
      md = ModMult(md, md, n);
    }
    if (md != n-1) return false;
  }

  return true;
}

// Works for all primes p < 2^42.
bool IsPrimeFast48(uint64 n) {
  if (n <= 3) return (n >= 2);
  static const uint64 small[] =
      {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67,
       71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139};
  for (int i = 0; i < sizeof(small)/sizeof(uint64); i++) {
    if (n%small[i] == 0) return n == small[i];
  }

  if (n == 17431) return true;
  if (n == 3281359) return true;
  // These values, valid for n < 21,652,684,502,221, were found by Steve Worley.
  static const uint64 millerrabin[] = {2, 1215, 34862, 574237825};
  uint64 s = n-1, r = 0;
  while (s%2 == 0) {s /= 2; r++;}
  for (int i = 0, j; i < sizeof(millerrabin)/sizeof(uint64); i++) {
    uint64 md = ModPow48(millerrabin[i], s, n);
    if (md == 1) continue;
    for (j = 1; j < r; j++) {
      if (md == n-1) break;
      md = ModMult48(md, md, n);
    }
    if (md != n-1) return false;
  }

  return true;
}

// Works for all primes p < 2^64.
bool IsPrimeFast64(uint64 n) {
  if (n <= 3) return (n >= 2);
  static const uint64 small[] =
      {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67,
       71, 73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139,
       149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199};
  for (int i = 0; i < sizeof(small)/sizeof(uint64); i++) {
    if (n%small[i] == 0) return n == small[i];
  }

  // Makes use of the known bounds for Miller-Rabin pseudoprimes.
  static const uint64 millerrabin[] =
      {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
  static const uint64 A014233[] =  // From OEIS.
      {2047LL, 1373653LL, 25326001LL, 3215031751LL, 2152302898747LL,
       3474749660383LL, 341550071728321LL, 341550071728321LL,
       3825123056546413051LL, 3825123056546413051LL, 3825123056546413051LL, 0};
  uint64 s = n-1, r = 0;
  while (s%2 == 0) {s /= 2; r++;}
  for (int i = 0, j; i < sizeof(millerrabin)/sizeof(uint64); i++) {
    uint64 md = ModPow64(millerrabin[i], s, n);
    if (md != 1) {
      for (j = 1; j < r; j++) {
        if (md == n-1) break;
        md = ModMult64(md, md, n);
      }
      if (md != n-1) return false;
    }
    if (n < A014233[i]) return true;
  }

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

// Count of x, 1 <= x <= n, such that Gcd(x, n) == 1.
// Requires factoring to compute.
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

// Lehmer's method for counting primes p, 2 <= p <= x.
vector<int> cumprime, primelist(1);
uint64 CountPrimes(uint64 x);
uint64 CountPrimes_Phi(uint64 m, int pi) {
  if (primelist[pi]*primelist[pi] > m) return CountPrimes(m)-(pi-1);
  uint64 ret = m;
  for (; pi > 0; pi--) ret -= CountPrimes_Phi(m/primelist[pi], pi-1);
  return ret;
}
uint64 CountPrimes(uint64 x) {
  #define COUNTPRIME__MAX 1e13
  if (cumprime.size() == 0) {
    if (prime.size() < sqrt(COUNTPRIME__MAX)) PrimeSieve(sqrt(COUNTPRIME__MAX));
    cumprime.resize(prime.size());
    for (int i = 1; i < prime.size(); i++) {
      cumprime[i] = cumprime[i-1] + prime[i];
      if (prime[i]) primelist.push_back(i);
    }
  }
  if (x < cumprime.size()) return cumprime[x];
  int a = cumprime[pow(x, 1/4.0)];
  int b = cumprime[pow(x, 1/2.0)];
  int c = cumprime[pow(x, 1/3.0)]+1;
  uint64 ret = CountPrimes_Phi(x, a) + (uint64)(b+a-2)*(b-a+1)/2;
  for (int i = a+1; i <= b; i++) {
    uint64 d = x / primelist[i];
    ret -= CountPrimes(d);
    if (i <= c) {
      int bi = cumprime[sqrt(d)];
      for (int j = i; j <= bi; j++) ret -= CountPrimes(d/primelist[j]) - (j-1);
    }
  }
  return ret;
}

//// *** Combinations/Permutations ***

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

//// *** Large numbers ***

void Mult128(uint64 a, uint64 b, uint64& hi, uint64& lo) {
  uint64 ahi = (a>>32), alo = (a&0xffffffffLL);
  uint64 bhi = (b>>32), blo = (b&0xffffffffLL);
  hi = alo*blo;
  lo = (hi&0xffffffffLL);
  hi >>= 32;
  uint64 x = alo*bhi, y = ahi*blo;
  hi += x+y; 
  uint64 car = (hi < y);
  lo |= (hi&0xffffffffLL)<<32;
  hi >>= 32;
  hi += ahi*bhi + (car<<32);
}

#endif // __MATH_H
