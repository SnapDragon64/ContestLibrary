#ifndef __MISC_H
#define __MISC_H

//// Miscellaneous functions that sometimes come in handy in contests.
//// Formatted for process.exe.

//// *** Calendar ***

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

//// *** Playing cards ***

#define CARD_RANKS "23456789TJQKA"
#define CARD_SUITS "CDHS"

inline int CardToInt(char rank, char suit) {
  const char* r = strchr(CARD_RANKS, rank);
  const char* s = strchr(CARD_SUITS, suit);
  if (!r || !s) return -1;
  return (r-CARD_RANKS)*4 + (s-CARD_SUITS);
}

// Takes in a set of 5 cards and turns them into an integer ranking the poker
// hand.  Takes high cards, etc., into account.  Take the top byte if you just
// want the hand's description.
enum {HIGH_CARD, ONE_PAIR, TWO_PAIRS, THREE_OF_A_KIND, STRAIGHT, FLUSH,
  FULL_HOUSE, FOUR_OF_A_KIND, STRAIGHT_FLUSH, ROYAL_FLUSH};
int PokerHand(const vector<int>& hand) {
  bool flush = true;
  for (int i = 0; i+1 < 5; i++) {
    if ((hand[i]&3) != (hand[i+1]&3)) flush = false;
  }

  vector<pair<int, int> > rank;
  for (int i = 0, j; i < 5; i++) {
    int r = hand[i]>>2;
    for (j = 0; j < rank.size(); j++) if (rank[j].second == r) break;
    if (j == rank.size()) {
      rank.push_back(make_pair(1, r));
    } else {
      rank[j].first++;
    }
  }
  sort(rank.begin(), rank.end());

  int all_ranks = 0;
  for (int i = rank.size()-1; i >= 0; i--) {
    all_ranks = (all_ranks<<4) + rank[i].second;
  }

  int straight = -1;
  if (rank.back().first == 1 && rank[4].second-rank[0].second == 4) {
    straight = rank[4].second;
  }
  // Special case for low straights.
  if (rank.back().first == 1 && rank[4].second == 12 && rank[3].second == 3) {
    straight = rank[3].second;
  }

  if (flush && straight == 12) {
    return (ROYAL_FLUSH<<24);
  }
  if (flush && straight != -1) {
    return (STRAIGHT_FLUSH<<24) + straight;
  }
  if (rank.back().first == 4) {
    return (FOUR_OF_A_KIND<<24) + all_ranks;
  }
  if (rank.back().first == 3 && rank[0].first == 2) {
    return (FULL_HOUSE<<24) + all_ranks;
  }
  if (flush) {
    return (FLUSH<<24) + all_ranks;
  }
  if (straight != -1) {
    return (STRAIGHT<<24) + straight;
  }
  if (rank.back().first == 3) {
    return (THREE_OF_A_KIND<<24) + all_ranks;
  }
  if (rank.back().first == 2 && rank[1].first == 2) {
    return (TWO_PAIRS<<24) + all_ranks;
  }
  if (rank.back().first == 2) {
    return (ONE_PAIR<<24) + all_ranks;
  }
  return (HIGH_CARD<<24) + all_ranks;
}

//// *** Potpourri ***

string RomanNumeral(int x) {
  static string s[]={"M","CM","D","CD","C","XC","L","XL","X","IX","V","IV","I"};
  static int val[] ={1000,900, 500,400, 100,90,  50, 40,  10, 9,   5,  4,   1 };
  string ret;
  for (int i = 0; x; i++)
    while (x >= val[i]) {x -= val[i]; ret += s[i];}
  return ret;
}


int64 Rand64() {
  // Note that the upper bits of rand() are more reliably random.
  return ((int64)rand()<<33) ^ ((int64)rand()<<11) ^ ((int64)rand()>>9);
}

#endif // __MISC_H
