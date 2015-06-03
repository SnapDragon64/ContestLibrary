#define TOP 0
#define BOTTOM 1
#define LEFT 2
#define RIGHT 3
#define FRONT 4
#define BACK 5
class Die {
public:
	int face[6];

	// turn one side to another in 1 or 2 rotations
	void roll(int startface, int endface) {
		if( startface == endface ) return;
		if( startface == (endface^1) ) {
			swap(face[startface], face[endface]);
			swap(face[(startface+2)%6], face[(endface+2)%6]);
		} else {
			int x = face[endface];
			face[endface] = face[startface];
			face[startface] = face[endface^1];
			face[endface^1] = face[startface^1];
			face[startface^1] = x;
		}
	}
	void mirror() {swap(face[LEFT], face[RIGHT]);}
	void orient(int topnum, int frontnum) {
		int x;
		for( x = 0; x < 6; x++ ) if( face[x] == topnum ) break;
		roll(x, TOP);
		for( x = 0; x < 6; x++ ) if( face[x] == frontnum ) break;
		if( x == BACK ) {roll(LEFT, FRONT); roll(LEFT, FRONT);}
		else roll(x, FRONT);
	}
	void orient(int topnum, int frontnum, int leftnum) {
		orient(topnum, frontnum);
		if( face[LEFT] != leftnum ) mirror();
	}
	Die() {face[0] = 6; face[1] = 1; face[2] = 3;
		   face[3] = 4; face[4] = 2; face[5] = 5;}
};
