#define MAX_GAMES 1000

class ConwayGame {
private:
  ConwayGame() {}  // Private; only MakeConwayGame should construct games.
  struct ConwayGameHash {
    int operator()(const ConwayGame* G) const {
      return G->hash;
    }
  };
  struct ConwayGameEq {
    bool operator()(const ConwayGame* G, const ConwayGame* H) const {
      return *G == *H;
    }
  };

public:
  int index;  // Index of this game into list of canonical games.
  int hash;  // G == H implies G.hash == H.hash.
  vector<const ConwayGame*> L, R;

  static const ConwayGame& MakeConwayGame(const vector<const ConwayGame*>& l,
                                          const vector<const ConwayGame*>& r) {
    static hash_set<const ConwayGame*, ConwayGameHash, ConwayGameEq>
        canonical_games;

    // Canonicalize the game.
    ConwayGame* G = new ConwayGame();
    G->L = l; G->R = r; G->index = canonical_games.size();
    G->RemoveDominatedOptions();
    while (G->ReplaceReversibleMoves()) G->RemoveDominatedOptions();

    int lhash = 392384382, rhash = -1495686783;
    for (int i = 0; i < G->L.size(); i++) lhash += G->L[i]->hash;
    for (int i = 0; i < G->R.size(); i++) rhash += G->R[i]->hash;
    G->hash = (lhash ^ rhash);

    pair<typeof(canonical_games.begin()), bool> response =
        canonical_games.insert(G);
    if (response.second) {
      // This is a new game.
      if (canonical_games.size() > MAX_GAMES) {
        cerr << "Too many games in memory!";
        exit(0);
      }
      return *G;
    } else {
      // This was a known game.  Clear leq_memo in a fast way.
      if ((leq_cookie[G->index] += 2) < 0) {
        leq_cookie[G->index] = 0;
        for (int i = 0; i <= G->index; i++)
          leq_memo[G->index][i] = leq_memo[i][G->index] = 0;
      }
      delete G;
      return **response.first;
    }
  }

  bool RemoveDominatedOptions() {
    bool changed = false;
    for (int i = 0; i < L.size(); i++)
    for (int j = 0; j < L.size(); j++)
      if (j != i && *L[j] <= *L[i]) {
        L.erase(L.begin() + j);
        if (i > j--) i--;
        changed = true;
      }
    for (int i = 0; i < R.size(); i++)
    for (int j = 0; j < R.size(); j++)
      if (j != i && *R[i] <= *R[j]) {
        R.erase(R.begin() + j);
        if (i > j--) i--;
        changed = true;
      }
    return changed;
  }

  bool ReplaceReversibleMoves() {
    bool changed = false;
    for (int i = 0; i < L.size(); i++)
    for (int j = 0; j < L[i]->R.size(); j++) {
      const ConwayGame& G = *L[i]->R[j];
      if (G <= *this) {
        L.erase(L.begin() + i--);
        L.insert(L.end(), G.L.begin(), G.L.end());
        changed = true;
        break;
      }
    }
    for (int i = 0; i < R.size(); i++)
    for (int j = 0; j < R[i]->L.size(); j++) {
      const ConwayGame& G = *R[i]->L[j];
      if (*this <= G) {
        R.erase(R.begin() + i--);
        R.insert(R.end(), G.R.begin(), G.R.end());
        changed = true;
        break;
      }
    }
    return changed;
  }

  static const ConwayGame& ZERO;
  static const ConwayGame& ONE;
  static const ConwayGame& STAR;
  static const ConwayGame& ConwayInteger(int v) {
    if (v == 0) {
      return ZERO;
    } else if (v > 0) {
      return ConwayInteger(v-1) + ONE;
    } else {
      return ConwayInteger(v+1) - ONE;
    }
  }

  static int leq_memo[MAX_GAMES][MAX_GAMES];
  static int leq_cookie[MAX_GAMES];
  bool operator<=(const ConwayGame& G) const {
    int& ret = leq_memo[index][G.index];
    int cookie = leq_cookie[max(index, G.index)];
    if (ret > cookie) return ret & 1;
    for (int i = 0; i < L.size(); i++)
      if (G <= *L[i]) {ret = cookie+2; return false;}
    for (int i = 0; i < G.R.size(); i++)
      if (*G.R[i] <= *this) {ret = cookie+2; return false;}
    ret = cookie+1; return true;
  }
  bool operator<(const ConwayGame& G) const {return *this<=G && !(G<=*this);}
  bool operator>=(const ConwayGame& G) const {return G <= *this;}
  bool operator>(const ConwayGame& G) const {return G<=*this && !(*this<=G);}
  bool operator==(const ConwayGame& G) const {return G<=*this && *this<=G;}
  bool operator!=(const ConwayGame& G) const {return !(G == *this);}
  bool Fuzzy(const ConwayGame &G) {return !(*this <= G) && !(G <= *this);}

  const ConwayGame& operator-() const {
    static const ConwayGame* neg_memo[MAX_GAMES];
    const ConwayGame*& ret = neg_memo[index];
    if (ret) return *ret;
    vector<const ConwayGame*> l, r;
    for (int i = 0; i < L.size(); i++) r.push_back(&(-*L[i]));
    for (int i = 0; i < R.size(); i++) l.push_back(&(-*R[i]));
    return *(ret = &MakeConwayGame(l, r));
  }

  const ConwayGame& operator+(const ConwayGame& G) const {
    static const ConwayGame* add_memo[MAX_GAMES][MAX_GAMES];
    const ConwayGame*& ret = add_memo[index][G.index];
    if (ret) return *ret;
    vector<const ConwayGame*> l, r;
    for (int i = 0; i <   L.size(); i++) l.push_back(&(*L[i] + G));
    for (int i = 0; i < G.L.size(); i++) l.push_back(&(*this + *G.L[i]));
    for (int i = 0; i <   R.size(); i++) r.push_back(&(*R[i] + G));
    for (int i = 0; i < G.R.size(); i++) r.push_back(&(*this + *G.R[i]));
    add_memo[G.index][index] = ret = &MakeConwayGame(l, r);
    return *ret;
  }
  ConwayGame& operator+=(const ConwayGame& G) {return *this = *this + G;}
  const ConwayGame& operator-(const ConwayGame& G) const {return *this + -G;}
  ConwayGame& operator-=(const ConwayGame& G) {return *this = *this + -G;}

  string Representation() const {
    string ret = "{";
    for (int i = 0; i < L.size(); i++) {
      if (i) ret += ',';
      ret += L[i]->Representation();
    }
    ret += '|';
    for (int i = 0; i < R.size(); i++) {
      if (i) ret += ',';
      ret += R[i]->Representation();
    }
    ret += '}';
    if (ret == "{|}") return "0";
    if (ret == "{0|0}") return "*";
    int v, n;
    if (sscanf(ret.c_str(), "{%d|}%n", &v, &n) && n == ret.size())
      return ToString(v+1);
    if (sscanf(ret.c_str(), "{|%d}%n", &v, &n) && n == ret.size())
      return ToString(v-1);
    return ret;
  }
  friend ostream& operator<<(ostream& out, ConwayGame G) {
    out << G.Representation();
    return out;
  }
};
int ConwayGame::leq_memo[MAX_GAMES][MAX_GAMES];
int ConwayGame::leq_cookie[MAX_GAMES];
const ConwayGame& ConwayGame::ZERO =
    MakeConwayGame(vector<const ConwayGame*>(),
                   vector<const ConwayGame*>());
const ConwayGame& ConwayGame::ONE =
    MakeConwayGame(vector<const ConwayGame*>(1, &ConwayGame::ZERO),
                   vector<const ConwayGame*>());
const ConwayGame& ConwayGame::STAR =
    MakeConwayGame(vector<const ConwayGame*>(1, &ConwayGame::ZERO),
                   vector<const ConwayGame*>(1, &ConwayGame::ZERO));
