#ifndef __ALGORITHMS_H
#define __ALGORITHMS_H

//// Common programming contest algorithms.
//// Formatted for process.exe.

//// *** Graph theory ***

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

#endif // __ALGORITHMS_H
