vector<string> tokenize(string s, string ch) {
	vector<string> ret;
	for( int p = 0, p2; p < s.size(); p = p2+1 ) {
		p2 = s.find_first_of(ch, p);
		if( p2 == -1 ) p2 = s.size();
		if( p2-p > 0 ) ret.push_back( s.substr(p, p2-p) );
	}
	return ret;
}

vector<int> tokint(string s, string ch) {
	vector<int> ret;
	vector<string> p = tokenize(s, ch);
	for( int i = 0; i < p.size(); i++ )
		ret.push_back( atoi(p[i].c_str()) );
	return ret;
}

vector<vector<int> > tokmat(vector<string> s, string ch) {
	vector<vector<int> > ret;
	for( int i = 0; i < s.size(); i++ )
		ret.push_back( tokint(s[i], ch) );
	return ret;
}
