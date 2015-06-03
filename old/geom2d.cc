double INF = (1.0/0.0);
double EPS = 0.0000001;

struct Vector {
	double x, y;
	inline Vector operator-(const Vector &v) const {
		return Vector(x-v.x, y-v.y);
	}
	inline Vector operator+(const Vector &v) const {
		return Vector(x+v.x, y+v.y);
	}
	inline bool operator==(const Vector &v) const {
		return fabs(x-v.x) < EPS && fabs(y-v.y) < EPS;
	}
	inline bool operator<(const Vector &v) const {
		return x-v.x < -EPS || x-v.x < EPS && y-v.y < -EPS;
	}
	inline Vector::Vector() {};
	inline Vector::Vector(double x, double y) : x(x), y(y) {};
};
typedef Vector Point;

// NOTE: a Line is "open" (does not contain its vertices)
struct Line {
	Point c1, c2;
	inline bool operator==(const Line &l) const {return c1==l.c1 && c2==l.c2;}
	inline Line::Line() {};
	inline Line::Line(const Point &c1, const Point &c2) : c1(c1), c2(c2) {};
};

// NOTE: a Polygon is "open" (does not contain its border)
typedef vector<Point> Polygon;

inline double lensqr( const Vector &v ) {
	return v.x*v.x + v.y*v.y;
}

inline double len( const Vector &v ) {
	return sqrt(lensqr(v));
}

inline double dotp( const Vector &v1, const Vector &v2 ) {
	return v1.x*v2.x + v1.y*v2.y;
}

inline double crossp( const Vector &v1, const Vector &v2 ) {
	return v1.x*v2.y - v1.y*v2.x;
}

inline double dotp( const Line &l, const Point &p ) {
	return dotp(l.c2-l.c1, p-l.c1);
}

inline double crossp( const Line &l, const Point &p ) {
	return crossp(l.c2-l.c1, p-l.c1);
}

inline bool parallel( const Vector &v1, const Vector &v2 ) {
	return fabs(crossp(v1, v2)) < EPS;
}

inline bool perpendicular( const Vector &v1, const Vector &v2 ) {
	return fabs(dotp(v1, v2)) < EPS;
}

inline double angle( const Vector &v1, const Vector &v2 ) {
	return atan2( crossp(v1, v2), dotp(v1, v2) );
}

// NOTE: if lines overlap, returns false
bool intersect( const Line &l1, const Line &l2 ) {
	double x = crossp(l1, l2.c1), y = crossp(l1, l2.c2);
	if( !(x < -EPS && y > EPS) && !(x > EPS && y < -EPS) ) return false;
	x = crossp(l2, l1.c1), y = crossp(l2, l1.c2);
	return (x < -EPS && y > EPS) || (x > EPS && y < -EPS);
}

bool intersect( Line l, const Point &p ) {
	if( fabs(crossp(l, p)) > EPS ) return false;
	if( dotp(l, p) < EPS ) return false;
	swap(l.c1, l.c2);
	return dotp(l, p) > EPS;
}

bool overlap( Line l1, const Line &l2 ) {
	if( l1.c1 == l1.c2 || l2.c1 == l2.c2 ) return false;
	if( fabs(crossp(l1, l2.c1)) > EPS || fabs(crossp(l1, l2.c2)) > EPS )
		return false;
	if( dotp(l1, l2.c1) < EPS && dotp(l1, l2.c2) < EPS ) return false;
	swap(l1.c1, l1.c2);
	if( dotp(l1, l2.c1) < EPS && dotp(l1, l2.c2) < EPS ) return false;
	return true;
}

Point intersectpoint( const Line &l1, const Line &l2 ) {
	Point ret(INF, INF);
	double d1 = crossp(l1, l2.c1);
	double d2 = crossp(l1, l2.c2);
	if( fabs(d1-d2) < EPS ) return ret;
	ret.x = l2.c1.x + (l2.c2.x-l2.c1.x)*d1/(d1-d2);
	ret.y = l2.c1.y + (l2.c2.y-l2.c1.y)*d1/(d1-d2);
	return ret;
}

inline Line edge( const Polygon &p, int e ) {
	return Line(p[e], p[(e+1)%p.size()]);
}

bool degenerate( const Polygon &p ) {
	if( p.size() < 3 ) return true;
	int i, j;
	for( i = 0; i < p.size(); i++ )
	for( j = i+1; j < p.size(); j++ )
		if( p[i] == p[j] ) return true;
	for( i = 0; i < p.size(); i++ )
	for( j = 0; j < p.size(); j++ )
		if( intersect( edge(p, i), p[j] ) ) return true;
	for( i = 0; i < p.size(); i++ )
	for( j = i+1; j < p.size(); j++ )
		if( intersect( edge(p, i), edge(p, j) ) ) return true;
	return false;
}

// NOTE: assumes non-degeneracy
bool inside( const Polygon &poly, const Point &p ) {
	int i;
	for( i = 0; i < poly.size(); i++ )
		if( poly[i] == p || intersect(edge(poly, i), p) ) return false;
	Line l(p, Point(p.x+10000, p.y+1));
	int n = 0;
	for( i = 0; i < poly.size(); i++ )
		l.c2.x = max(l.c2.x, poly[i].x+10000);
	for( i = 0; i < poly.size(); i++ )
		if( intersect(edge(poly, i), l) ) n++;
	return n%2;
}

// NOTE: assumes non-degeneracy
// NOTE: negative result if p's vertices are clockwise
double area( const Polygon &p ) {
	double x = 0;
	for( int i = 1; i+1 < p.size(); i++ )
		x += crossp(p[i]-p[0], p[i+1]-p[0]);
	return x/2.0;
}

// NOTE: returns point indices, starting uppermost right, clockwise
vector<int> convexhull( const vector<Point> &p ) {
	vector<int> ret;
	if( p.size() == 0 ) return ret;
	vector<bool> u(p.size());
	Point cp = p[0];
	int i, n = 0;
	double x;
	for( i = 1; i < p.size(); i++ )
		if( p[i] < cp ) {
			cp = p[i];
			n = i;
		}
	for(;;) {
		if( u[n] ) return ret;
		u[n] = true;
		ret.push_back(n);
		n = 0;
		for( i = 1; i < p.size(); i++ ) {
			x = crossp(p[n]-cp, p[i]-cp);
			if( x > EPS ||
					(fabs(x) < EPS && lensqr(p[i]-cp) > lensqr(p[n]-cp)+EPS) )
				n = i;
		}
		cp = p[n];
	}
}

// clips off everything to the right of l
vector<Polygon> clippolys( const vector<Polygon> &p, const Line &l ) {
	int i, j, k, pv, pv2, wascut;
	vector<Polygon> ret;
	for( i = 0; i < p.size(); i++ ) {
		pv = (crossp(l, p[i].back()) > EPS);
		wascut = 0;
		for( j = 0; j < p[i].size(); j++ ) {
			pv2 = (crossp(l, p[i][j]) > EPS);
			if( !pv && pv2 ) {
				wascut = 1;
				ret.push_back(Polygon());
				ret.back().push_back(intersectpoint(l,
						Line(p[i][(j+p[i].size()-1)%p[i].size()], p[i][j])));
				ret.back().push_back(p[i][k=j]);
				while( k=(k+1)%p[i].size(), (pv2 = (crossp(l, p[i][k]) > EPS)) )
					ret.back().push_back(p[i][k]);
				ret.back().push_back(intersectpoint(l,
						Line(p[i][k], p[i][(k+1)%p[i].size()])));
				if( ret.back().back() == ret.back()[0] )
					ret.back().pop_back();
				j = (k <= j) ? p[i].size() : k;
			}
			pv = pv2;
		}
		if( !wascut && pv ) ret.push_back(p[i]);
	}
	return ret;
}
