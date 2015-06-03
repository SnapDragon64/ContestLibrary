#ifndef __PARSING_H
#define __PARSING_H

//// Parsing functions.  Formatted for process.exe.

//// *** Tokenizing strings ***

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

//// *** Transformation to/from string representation ***

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

//// *** Hashing ***

struct HashString {int operator()(const string &s) const {return hash<const char*>()(s.c_str());}};

#endif // __PARSING_H
