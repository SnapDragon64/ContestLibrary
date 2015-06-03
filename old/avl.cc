template<class T=int, bool multi=false>
class AVLNode {
	inline void check() {
		height = max(lc->height, rc->height)+1;
		lc->par = rc->par = this;
	}
	void rotateLeft() {
		AVLNode *u = rc;
		rc = u->rc; u->rc = u->lc; u->lc = lc; lc = u;
		swap(key, u->key); u->check(); check();
	}
	void rotateRight() {
		AVLNode *u = lc;
		lc = u->lc; u->lc = u->rc; u->rc = rc; rc = u;
		swap(key, u->key); u->check(); check();
	}
	void balance() {
		check();
		if( lc->height == rc->height+2 ) {
			if( lc->lc->height >= lc->rc->height )
				rotateRight();
			else
				{lc->rotateLeft(); rotateRight();}
		} else if( rc->height == lc->height+2 ) {
			if( rc->rc->height >= rc->lc->height )
				rotateLeft();
			else
				{rc->rotateRight(); rotateLeft();}
		}
	}

public:
	T key;
	signed char height;
	AVLNode *lc, *rc, *par;

	struct iterator {
		AVLNode *p;
		inline iterator(AVLNode *pt = 0) : p(pt) {}
		inline operator AVLNode *() {return p;}
		iterator &operator++() {
			if( p->rc->height != -1 ) return *this = p->rc->begin();
			while( p->par && p->par->rc == p ) p = p->par;
			return *this = p->par;
		}
		iterator operator++(int) {iterator tmp = *this; ++*this; return tmp;}
	};
	iterator begin() {
		if( height == -1 ) return 0;
		return (lc->height == -1) ? iterator(this) : lc->begin();
	}
	iterator end() {return 0;}

	AVLNode *find(const T &k) const {
		if( height == -1 ) return 0;
		if( key == k ) return this;
		return ((key>k)?lc:rc)->find(k);
	}
	void insert(const T &k) {
		if( height == -1 ) {
			key = k; lc = new AVLNode(); rc = new AVLNode(); check();
			for( AVLNode *u = par; u; u = u->par ) u->balance();
		} else if( multi || key != k )
			((key>k)?lc:rc)->insert(k);
	}
	void remove(const T &k) {
		if( height == -1 ) return;
		if( key == k ) {
			if( height == 0 ) {
				delete lc; delete rc; lc = rc = 0; height = -1;
				for( AVLNode *u = par; u; u = u->par ) u->balance();
			} else {
				AVLNode *u;
				if( lc->height > rc->height )
					for( u = lc; u->rc->height != -1; u = u->rc );
				else
					for( u = rc; u->lc->height != -1; u = u->lc );
				swap(key, u->key); u->remove(u->key);
			}
		} else
			((key>k)?lc:rc)->remove(k);
	}
	~AVLNode() {if( lc ) delete lc; if( rc ) delete rc;}
	AVLNode() {height = -1; lc = rc = par = 0;}
};
