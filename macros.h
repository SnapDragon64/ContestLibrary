#ifndef __MACROS_H
#define __MACROS_H

//// Standard typedefs and Macros.  Formatted for process.exe.

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

#define ALL(s) s.begin(), s.end()
#define FORALL(s,i) for (auto i=s.begin(); i != s.end(); ++i)
#define FORALLBITS(x,i) for (int xTmP=(x),i; (i=__builtin_ffs(xTmP)-1)>-1; xTmP&=xTmP-1)

//// *** Constants ***

#define PI 3.14159265358979323846
#define EPS 1e-9
#define INF __builtin_inf()

//// *** Bitwise functions ***

inline int BitCount(unsigned x) {return __builtin_popcount(x);}

inline int BitCountLL(unsigned long long x) {return __builtin_popcountll(x);}

inline unsigned LowestBit(unsigned x) {return x & ~(x-1);}

inline unsigned long long LowestBitLL(unsigned long long x) {return x & ~(x-1);}

inline unsigned ClearLowestBit(unsigned x) {return x & (x-1);}

inline unsigned long long ClearLowestBitLL(unsigned long long x) {return x & (x-1);}

inline int LowestBitIndex(unsigned x) {return __builtin_ffs(x)-1;}

inline int LowestBitIndexLL(unsigned long long x) {return __builtin_ffsll(x)-1;}

#endif // __MACROS_H
