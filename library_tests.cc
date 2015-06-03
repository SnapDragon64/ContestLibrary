void fail_test(const string& test, const string& err) {
  cerr << test << " FAILED: " << err << endl;
  exit(1);
}

void TEST_PokerHand() {
  cerr << "  Running TEST_PokerHand()..." << endl;
  string hands[] = {
    "2C 3D 4H 5S 7H 0",
    "2S 3D 4H 5C 7S (equal)",
    "2S 3C 4S 6S 7S",
    "3C 4S 5D 7D 8S",
    "3S 4C 5S 8S 9H",
    "3S 4C 5S 8S TH",
    "2S 3C 4S 8S JH",
    "3S 4C 5S 8S JH",
    "2S 3C 5S 8S QH",
    "3C 4C 5D 8D QD",
    "3S 4C 5S 8S QH (equal)",
    "4D 6H 8C TS QC",
    "3D 7H 8C TS QC",
    "4D 5H 9C TS QC",
    "4D 6H 7C JS QC",
    "3S 4C 5S 6S KH",
    "3S 4C 5S 8S KH",
    "4D 6H 7C 9S KC",
    "3S 4C 5D TS KD",
    "3D 4C 5S JS KC",
    "6D 7C 8S QS KC",
    "2S 3C 4D 6D AH",
    "2S 3C 4S KS AS",
    "2H 3C QS KS AH",
    "9H JC QD KD AS",
    "2D 2C 3D 4D 5S 1",
    "2S 2C 4D 6D 8H",
    "2H 2C 3D 7D 8H",
    "2D 2C 4D 5D 9S",
    "3S 3C 2D 4D 5S",
    "3H 3C JH QC KH",
    "3D 3C QC KD AH",
    "4S 4C 2D 3D 5S",
    "KS KC 3D 8H QS",
    "AS AC 3C 7C QS",
    "AS AC JD QD KH",
    "2S 2C 3H 3D 4S 2",
    "2S 2H 3H 3D 6S",
    "2H 2C 4H 4D 5S",
    "5S 5H 7D 7C JS",
    "4S 4H 8D 8C TS",
    "KS KC AS AC QS",
    "2S 2H 2D 3C 4S 3",
    "2S 2H 2C 7C 8S",
    "2S 2H 2D 7D 8C (equal)",
    "2S 2H 2D 6C 9S",
    "2D 2H 2C 7C 9S",
    "3D 3H 3C 2C 5S",
    "TD TS TC 3C 4S",
    "AD AC AS QC KS",
    "AD 2D 3D 4D 5C 4",
    "AH 2H 3C 4S 5S (equal)",
    "AS 2H 3C 4S 5D (equal)",
    "2H 3C 4S 5C 6C",
    "2H 3C 4S 5D 6D (equal)",
    "3S 4C 5H 6C 7S",
    "9H TH JH QD KS",
    "TS JC QS KC AD",
    "2C 3C 4C 5C 7C 5",
    "2D 3D 4D 5D 7D (equal)",
    "2H 3H 4H 5H 7H (equal)",
    "2S 3S 4S 5S 7S (equal)",
    "2C 3C 4C 5C 8C",
    "3C 5C 7C 9C JC",
    "2C 6C 7C 9C JC",
    "3S 4S 8S 9S JS",
    "3D 5D 6D TD JD",
    "3C 5C 7C 8C QC",
    "9H TH QH KH AH",
    "9H JH QH KH AH",
    "2S 2D 2C 3H 3S 6",
    "2S 2H 2C 4C 4D",
    "2S 2H 2D AD AS",
    "3S 3H 3D 2D 2S",
    "3C 3H 3D 4D 4C",
    "7S 7H 7D 3H 3S",
    "7S 7H 7C 5D 5C",
    "8S 8D 8C 4D 4C",
    "QS QH QC JH JC",
    "KH KD KS 2D 2S",
    "KS KH KD AH AD",
    "AS AC AD KH KC",
    "2C 2D 2H 2S 3C 7",
    "2C 2D 2H 2S 3D (equal)",
    "2C 2D 2H 2S 4D",
    "2C 2D 2H 2S 7H",
    "2C 2D 2H 2S AC",
    "3C 3D 3H 3S 2S",
    "3C 3D 3H 3S AD",
    "7C 7D 7H 7S 5C",
    "KC KD KH KS AD",
    "AC AD AH AS KS",
    "AC 2C 3C 4C 5C 8",
    "AD 2D 3D 4D 5D (equal)",
    "AH 2H 3H 4H 5H (equal)",
    "AS 2S 3S 4S 5S (equal)",
    "2C 3C 4C 5C 6C",
    "4C 5C 6C 7C 8C",
    "8D 9D TD JD QD",
    "9D TD JD QD KD",
    "TC JC QC KC AC 9",
    "TD JD QD KD AD (equal)",
    "TH JH QH KH AH (equal)",
    "TS JS QS KS AS (equal)",
  };
  int nhands = sizeof(hands)/sizeof(string);
  vector<pair<int, string> > v;
  for (int i = 0; i < nhands; i++) {
    vector<int> hand;
    for (int j = 0; j < 5; j++) {
      hand.push_back(CardToInt(hands[i][j*3], hands[i][j*3+1]));
      assert(hand.back() != -1);
    }
    random_shuffle(hand.begin(), hand.end());
    v.push_back(make_pair(PokerHand(hand), hands[i]));
    //printf("%x %s\n", v.back().first, hands[i].c_str());
  }

  sort(v.begin(), v.end());
  int value = 0;
  for (int i = 0; i < nhands; i++) {
    if (v[i].second != hands[i]) {
      fail_test("PokerHands", "Saw \"" + v[i].second +
                              "\", expected \"" + hands[i] + "\".");
    }
    if (hands[i].size() == 16) {
      value = hands[i][15]-'0';
    }
    if ((v[i].first>>24) != value) {
      fail_test("PokerHands", "Incorrect hand at \"" + hands[i] + "\".");
    }
    if ((i && v[i].first == v[i-1].first) ^ (hands[i].size() == 22)) {
      fail_test("PokerHands", "Equality mismatch at \"" + hands[i] + "\".");
    }
  }
}

bool CallIsPrime(int func, uint64 n) {
  if (func == 0) return IsPrime(n);
  if (func == 1) return IsPrimeFast(n);
  if (func == 2) return IsPrimeFast48(n);
  if (func == 3) return IsPrimeFast64(n);
  return false;
}
void TEST_IsPrimeFast() {
  cerr << "  Running TEST_IsPrimeFast()..." << endl;
  static const uint64 A014233[] =  // From OEIS.
    {2047LL, 1373653LL, 25326001LL, 3215031751LL, 2152302898747LL,
     3474749660383LL, 341550071728321LL, 341550071728321LL,
     3825123056546413051LL, 3825123056546413051LL, 3825123056546413051LL};
  uint64 limits[4] = {(1LL<<34)-1, (1LL<<32)-1, (1LL<<42)-1, ~0ULL};
  int p2prime[65][10] = {{},{},{},{},{},{},{},{},
      {5, 15, 17, 23, 27, 29, 33, 45, 57, 59},            /* 8  */
      {3, 9, 13, 21, 25, 33, 45, 49, 51, 55},              /* 9  */
      {3, 5, 11, 15, 27, 33, 41, 47, 53, 57},              /* 10 */
      {9, 19, 21, 31, 37, 45, 49, 51, 55, 61},            /* 11 */
      {3, 5, 17, 23, 39, 45, 47, 69, 75, 77},              /* 12 */
      {1, 13, 21, 25, 31, 45, 69, 75, 81, 91},            /* 13 */
      {3, 15, 21, 23, 35, 45, 51, 65, 83, 111},           /* 14 */
      {19, 49, 51, 55, 61, 75, 81, 115, 121, 135},        /* 15 */
      {15, 17, 39, 57, 87, 89, 99, 113, 117, 123},        /* 16 */
      {1, 9, 13, 31, 49, 61, 63, 85, 91, 99},              /* 17 */
      {5, 11, 17, 23, 33, 35, 41, 65, 75, 93},            /* 18 */
      {1, 19, 27, 31, 45, 57, 67, 69, 85, 87},            /* 19 */
      {3, 5, 17, 27, 59, 69, 129, 143, 153, 185},          /* 20 */
      {9, 19, 21, 55, 61, 69, 105, 111, 121, 129},        /* 21 */
      {3, 17, 27, 33, 57, 87, 105, 113, 117, 123},        /* 22 */
      {15, 21, 27, 37, 61, 69, 135, 147, 157, 159},       /* 23 */
      {3, 17, 33, 63, 75, 77, 89, 95, 117, 167},          /* 24 */
      {39, 49, 61, 85, 91, 115, 141, 159, 165, 183},      /* 25 */
      {5, 27, 45, 87, 101, 107, 111, 117, 125, 135},      /* 26 */
      {39, 79, 111, 115, 135, 187, 199, 219, 231, 235},   /* 27 */
      {57, 89, 95, 119, 125, 143, 165, 183, 213, 273},    /* 28 */
      {3, 33, 43, 63, 73, 75, 93, 99, 121, 133},          /* 29 */
      {35, 41, 83, 101, 105, 107, 135, 153, 161, 173},    /* 30 */
      {1, 19, 61, 69, 85, 99, 105, 151, 159, 171},        /* 31 */
      {5, 17, 65, 99, 107, 135, 153, 185, 209, 267},      /* 32 */
      {9, 25, 49, 79, 105, 285, 301, 303, 321, 355},      /* 33 */
      {41, 77, 113, 131, 143, 165, 185, 207, 227, 281},   /* 34 */
      {31, 49, 61, 69, 79, 121, 141, 247, 309, 325},      /* 35 */
      {5, 17, 23, 65, 117, 137, 159, 173, 189, 233},      /* 36 */
      {25, 31, 45, 69, 123, 141, 199, 201, 351, 375},     /* 37 */
      {45, 87, 107, 131, 153, 185, 191, 227, 231, 257},   /* 38 */
      {7, 19, 67, 91, 135, 165, 219, 231, 241, 301},      /* 39 */
      {87, 167, 195, 203, 213, 285, 293, 299, 389, 437},  /* 40 */
      {21, 31, 55, 63, 73, 75, 91, 111, 133, 139},        /* 41 */
      {11, 17, 33, 53, 65, 143, 161, 165, 215, 227},      /* 42 */
      {57, 67, 117, 175, 255, 267, 291, 309, 319, 369},   /* 43 */
      {17, 117, 119, 129, 143, 149, 287, 327, 359, 377},  /* 44 */
      {55, 69, 81, 93, 121, 133, 139, 159, 193, 229},     /* 45 */
      {21, 57, 63, 77, 167, 197, 237, 287, 305, 311},     /* 46 */
      {115, 127, 147, 279, 297, 339, 435, 541, 619, 649}, /* 47 */
      {59, 65, 89, 93, 147, 165, 189, 233, 243, 257},     /* 48 */
      {81, 111, 123, 139, 181, 201, 213, 265, 283, 339},  /* 49 */
      {27, 35, 51, 71, 113, 117, 131, 161, 195, 233},     /* 50 */
      {129, 139, 165, 231, 237, 247, 355, 391, 397, 439}, /* 51 */
      {47, 143, 173, 183, 197, 209, 269, 285, 335, 395},  /* 52 */
      {111, 145, 231, 265, 315, 339, 343, 369, 379, 421}, /* 53 */
      {33, 53, 131, 165, 195, 245, 255, 257, 315, 327},   /* 54 */
      {55, 67, 99, 127, 147, 169, 171, 199, 207, 267},    /* 55 */
      {5, 27, 47, 57, 89, 93, 147, 177, 189, 195},        /* 56 */
      {13, 25, 49, 61, 69, 111, 195, 273, 363, 423},      /* 57 */
      {27, 57, 63, 137, 141, 147, 161, 203, 213, 251},    /* 58 */
      {55, 99, 225, 427, 517, 607, 649, 687, 861, 871},   /* 59 */
      {93, 107, 173, 179, 257, 279, 369, 395, 399, 453},  /* 60 */
      {1, 31, 45, 229, 259, 283, 339, 391, 403, 465},     /* 61 */
      {57, 87, 117, 143, 153, 167, 171, 195, 203, 273},   /* 62 */
      {25, 165, 259, 301, 375, 387, 391, 409, 457, 471},  /* 63 */
      {59, 83, 95, 179, 189, 257, 279, 323, 353, 363}};    /* 64 */

  PrimeSieve(1000000);
  for (int func = 0; func < 4; func++) {
    // Try on first 1000000 numbers, compared to prime sieve.
    for (int n = 0; n <= 1000000; n++) {
      if (CallIsPrime(func, n) != prime[n]) {
        fail_test("IsPrimeFast", "Small prime test mismatch.");
      }
    }

    // Try on random slices of 100000 numbers, compared to Lehmer Pi count.
    for (int ntest = 0; ntest < 10; ntest++) {
      uint64 x = rand()%1000000000;
      int cum = 0;
      for (int i = 0; i <= 100000; i++) cum += CallIsPrime(func, x+i);
      if (cum != CountPrimes(x+100000) - CountPrimes(x-1)) {
        fail_test("IsPrimeFast", "Incorrect prime count");
      }
    }

    // Try on some "hard" pseudoprimes from OEIS.
    for (int i = 0; i < sizeof(A014233)/sizeof(uint64); i++) {
      if (A014233[i] <= limits[func] && CallIsPrime(func, A014233[i])) {
        fail_test("IsPrimeFast", "Fooled by pseudoprime");
      }
    }

    // Match up with known list of primes just under each power of 2.
    for (int i = 8; i <= 64; i++) {
      uint64 first = (i==64) ? -1 : (1LL<<i)-1;
      if (limits[func] < first) continue;
      int nfound = 0;
      for (uint64 x = first; nfound < 10; x--) {
        if (CallIsPrime(func, x)) {
          if (first+1-x != p2prime[i][nfound]) {
            fail_test("IsPrimeFast", "Extra prime near power of 2");
          }
          nfound++;
        } else {
          if (first+1-x == p2prime[i][nfound]) {
            fail_test("IsPrimeFast", "Missed prime near power of 2");
          }
        }
      }
    }
  }
}

void TEST_BigInt() {
  cerr << "  Running TEST_BigInt()..." << endl;
  bigint a1, b1;
  binarybigint a2, b2;
  hexbigint a3, b3;
  BigInt<10, 10, 1, int> a4, b4;
  for (int i = 0; i < 1000; i++) {
    long long x = rand()-RAND_MAX/2;
    long long y = rand()-RAND_MAX/2;
    a1 = x; b1 = y;
    a2 = x; b2 = y;
    a3 = x; b3 = y;
    a4 = x; b4 = y;
    if ((a1*b1).toint() != x*y) fail_test("BigInt", "bigint mult");
    if ((a2*b2).toint() != x*y) fail_test("BigInt", "binarybigint mult");
    if ((a3*b3).toint() != x*y) fail_test("BigInt", "hexbigint mult");
    if ((a4*b4).toint() != x*y) fail_test("BigInt", "single-digit bigint mult");
  }
}

int main() {
  srand(time(0));
  TEST_PokerHand();
  TEST_IsPrimeFast();
  TEST_BigInt();
}
