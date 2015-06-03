#ifndef __MATRIX_H
#define __MATRIX_H

//// Matrix manipulation and linear algebra.  Formatted for process.exe.

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

#endif // __MATRIX_H
