// A program that reads in a C++ file intended for a TopCoder problem, and
// substitutes in any required #includes and referenced library code.
//
// Makes the assumption that the code looks like this:
//   (header comments and stuff)
//   (#include)
//   (#include)
//   ...
//   (#include)
//   using namespace std;
//
//   (function/struct/class/typedef/#define block or blank line)
//   (declaration)
//   ...
//   (declaration)
//
// Declaration blocks consist of:
//   [possible non-function lines, comments, globals, etc.]
//   foo func(...) {
//   }
//   [possible blank line, except after typedefs/#defines]
// This helps preserve blank lines as intended in the original file.
// Other miscellaneous blocks are possible, demarcated by blank lines.  They
// will be preserved.

#include <algorithm>
#include <cassert>
#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

string USINGSTR = "using namespace std;";

char buf[10000];
vector<string> inlines, outlines;

void throw_error(int errnum, const string& err) {
  cerr << err << endl;
  for (int i = 0; i < inlines.size(); i++) cout << inlines[i] << endl;
  exit(errnum);
}


// Strips trailing spaces from a string.
string strip_trailing_spaces(const string &s) {
  int i;
  for (i = s.size()-1; i >= 0 && isspace(s[i]); i--)
    ;
  return s.substr(0, i+1);
}

// Recognizes a C++ identifier character.
inline bool is_ident_char(char ch) {
  return (ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') ||
         (ch >= '0' && ch <= '9') || ch == '_';
}


// Increments to the next character on the same line.  Properly follows
// backslashes to the next line.
inline void next_char_on_line(const vector<string>& lines, int& line, int& i) {
  i++;
  if (i+1 == lines[line].size() && lines[line][i] == '\\' &&
      line+1 < lines.size()) {
    line++;
    i = 0;
  }
}

// Increments to the next valid character, possibly on a subsequent line.
// Skips blank lines.
// Postcondition: line == lines.size() || lines[line][i] is valid.
inline void next_char(const vector<string>& lines, int& line, int& i) {
  if (++i >= lines[line].size()) {
    i = 0;
    do {line++;} while (line < lines.size() && !lines[line].size());
  }
}

// If we're on a comment or #PRAGMA, skip over it.
inline bool skip_comment(const vector<string>& lines, int& line, int& i) {
  if (line == lines.size()) return false;
  if (i+1 < lines[line].size() && lines[line][i] == '/') {
    if (lines[line][i+1] == '/') {
      while (i < lines[line].size()) next_char_on_line(lines, line, i);
      next_char(lines, line, i);
      return true;
    } else if (lines[line][i+1] == '*') {
      i += 2;
      do {
        next_char(lines, line, i);
        if (line == lines.size()) {
          throw_error(1, "// PROCESS ERROR: Unclosed comment.");
        }
      } while (!(i && lines[line][i-1] == '*' && lines[line][i] == '/'));
      next_char(lines, line, i);
      return true;
    }
  }
  return false;
}

// If we're on a string, skip over it.
inline bool skip_string(const vector<string>& lines, int& line, int& i) {
  if (line < lines.size() && i < lines[line].size() &&
      (lines[line][i] == '"' || lines[line][i] == '\'')) {
    char ch = lines[line][i];
    do {
      next_char_on_line(lines, line, i);
      if (i && i < lines[line].size() && lines[line][i-1] == '\\') {
        next_char_on_line(lines, line, i);
      }
      if (i == lines[line].size()) {
        throw_error(1, "// PROCESS ERROR: Unclosed string.");
      }
    } while (lines[line][i] != ch);
    next_char(lines, line, i);
    return true;
  }
  return false;
}

// Increments until it finds the matching bracket, parenthesis, etc.  Skips
// comments/strings and handles nesting properly.
void match_bracket(const vector<string>& lines, char open, char close,
    int& line, int& i) {
  assert(lines[line][i] == open);
  int v = 0;
  for (;;) {
    v += (lines[line][i] == open) - (lines[line][i] == close);
    if (!v) return;
    next_char(lines, line, i);
    while (skip_comment(lines, line, i) || skip_string(lines, line, i))
      ;
    if (line == lines.size()) {
      throw_error(1, "// PROCESS ERROR: Unclosed bracket.");
    }
  }
}

// Increments "line" and "i" until it finds a semicolon that is in the current
// scope (ie, outside any {} or () brackets).
void find_semicolon(const vector<string>& lines, int& line, int& i) {
  for (;;) {
    while (skip_comment(lines, line, i) || skip_string(lines, line, i))
      ;
    if (line < lines.size() && lines[line][i] == '(') {
      match_bracket(lines, '(', ')', line, i);
    }
    if (line < lines.size() && lines[line][i] == '{') {
      match_bracket(lines, '{', '}', line, i);
    }
    if (line == lines.size()) {
      throw_error(1, "// PROCESS ERROR: Missing semicolon.");
    }
    if (lines[line][i] == ';') return;
    next_char(lines, line, i);
  }
}

// Returns (and skips over) the keyword at the current character.
// A keyword is a C++ identifier, possibly followed by a '(' or '<'.
string keyword(const vector<string>& lines, int& line, int& i) {
  string ret = "";
  if (line == lines.size() || i == lines[line].size()) return ret;
  if (!is_ident_char(lines[line][i])) return ret;

  int startline = line;
  for (;;) {
    ret += lines[line][i];
    next_char(lines, line, i);
    if (line > startline) {
      return ret;
    }
    if (lines[line][i] == '(' || lines[line][i] == '<') {
      return ret + lines[line][i];
    }
    if (!is_ident_char(lines[line][i])) {
      return ret;
    }
  }
}


// A map of keywords that force various #includes to be added.
map<string, string> include_map;
void initialize_include_map(const char *include_filename) {
  ifstream fin(include_filename);
  if (!fin.good()) {
    throw_error(3, string("// PROCESS ERROR: Could not read file \"") +
        include_filename + "\".");
  }
  string s, include;
  while (fin >> s) {
    if (s == "#include") {
      fin >> include;
      include = "#include " + include;
    } else {
      include_map[s] = include;
    }
  }
}

// Reads a complete declaration and associates a keyword with it.  (The keyword
// may be empty if the declaration is an unrecognized type.)
pair<string, vector<string> > parse_declaration(
    const vector<string> &lines, int &line) {
  int startline = line, i = 0;
  pair<string, vector<string> > ret;
  bool saw_template = false;

  if (lines[line].size() == 0) {
    line++;
    return make_pair("", vector<string>(1, ""));
  }

  for (;;) {
    while (skip_comment(lines, line, i) || skip_string(lines, line, i))
      ;

    if (line == lines.size()) {
      break;
    }
    if (line > startline && lines[line-1].size() == 0) {
      int j;
      for (j = i-1; j >= 0 && isspace(lines[line][j]); j--)
        ;
      if (j < 0) {
        // We just passed a blank line, ending a miscellaneous block.
        while (line > startline && lines[line-1].size() == 0) {
          line--;
        }
        break;
      }
    }

    if (line < lines.size() && lines[line][i] == '{') {
      match_bracket(lines, '{', '}', line, i);
    }
    if (line < lines.size() && lines[line][i] == '(') {
      match_bracket(lines, '(', ')', line, i);
    }

    if (i == 0 && (lines[line].substr(0, 9) == "#include " ||
                   lines[line] == USINGSTR)) {
      // #includes are a block to themselves, and do not include previous lines.
      if (line > startline) break;
      ret.first = lines[line];
      if (lines[line] == USINGSTR && line+1 < lines.size() &&
          lines[line+1].size() == 0) {
        // Include blank line after USINGSTR.
        line++;
      }
      line++; i = 0;
      break;
    }

    if (i == 0 && lines[line].substr(0, 8) == "#define ") {
      // #define statement; keyword is next on line.
      for (i = 8; i < lines[line].size() && !is_ident_char(lines[line][i]); i++)
        ;
      for (; i < lines[line].size() && is_ident_char(lines[line][i]); i++)
        ret.first += lines[line][i];
      if (ret.first == "") {
        throw_error(1, "// PROCESS ERROR: Could not parse #define.");
      }
      if (i == lines[line].size()) {
        // Empty defines are probably for include files and can be ignored.
        ret.first = "";
      }
      if (i < lines[line].size() && lines[line][i] == '(') {
        ret.first += lines[line][i];
      }
      while (i < lines[line].size()) next_char_on_line(lines, line, i);
      line++; i = 0;
      break;
    }

    string kw = keyword(lines, line, i);
    if (kw == "typedef") {
      // typedef statement; keyword is last on line.
      find_semicolon(lines, line, i);
      for (i--; i >= 0 && is_ident_char(lines[line][i]); i--) {
        ret.first += lines[line][i];
      }
      if (ret.first == "") {
        throw_error(1, "// PROCESS ERROR: Could not parse typedef.");
      }
      reverse(ret.first.begin(), ret.first.end());
      line++; i = 0;
      break;
    }
    
    if (kw == "template<") {
      saw_template = true;
      match_bracket(lines, '<', '>', line, i);
    }
    
    if (kw == "struct" || kw == "class" ||
        (kw.size() >= 2 && kw[kw.size()-1] == '(')) {
      bool func = (kw[kw.size()-1] == '(');
      if (!func) {
        // struct/class declaration; actual keyword is next.
        for (;;) {
          while (skip_comment(lines, line, i) || skip_string(lines, line, i))
            ;
          if (line == lines.size() ||
              (lines[line][i] != ' ' && !is_ident_char(lines[line][i]))) {
            throw_error(1, "// PROCESS ERROR: Could not parse struct def.");
          }
          kw = keyword(lines, line, i);
          if (kw != "") break;
          next_char(lines, line, i);
        }
      }

      // Find a '{' to make sure this isn't just a forward declaration.
      for (;;) {
        while (skip_comment(lines, line, i) || skip_string(lines, line, i))
          ;
        if (line == lines.size()) {
          throw_error(1, "// PROCESS ERROR: Could not parse func/struct def.");
        }
        if (lines[line][i] == ';' || lines[line][i] == '{') break;
        next_char(lines, line, i);
      }

      if (lines[line][i] == '{') {
        ret.first = kw;
        // Templated structs are invoked using a '<', so be sure to include
        // that in the keyword.
        if (!func && saw_template) ret.first += '<';

        match_bracket(lines, '{', '}', line, i);

        // Include a possible trailing blank line.
        line++; i = 0;
        if (line < lines.size() && lines[line].size() == 0) {
          line++;
        }
        break;
      }
    }

    if (kw == "") next_char(lines, line, i);
  }

  ret.second = vector<string>(lines.begin()+startline, lines.begin()+line);
  return ret;
}

// Parse all declarations from a library file.
map<string, vector<string> > declaration_map;
void read_library_file(const char *library_filename) {
  ifstream fin(library_filename);
  if (!fin.good()) {
    throw_error(3, string("// PROCESS ERROR: Could not read file \"") +
        library_filename + "\".");
  }
  vector<string> lines;
  while (fin.getline(buf, 10000))
    if (buf[0] != '/' || buf[1] != '/' || buf[2] != '/' || buf[3] != '/')
      lines.push_back(strip_trailing_spaces(buf));
  for (int line = 0; line < lines.size();) {
    // Precede declarations with a blank line, EXCEPT between #define and
    // typedefs.
    pair<string, vector<string> > dec = parse_declaration(lines, line);
    if (dec.first.size()) declaration_map[dec.first] = dec.second;
  }
}

bool is_simple_typedef(const pair<string, vector<string> >& decl) {
  return decl.second.size() == 1 && decl.second[0].substr(0, 8) == "typedef " &&
         decl.second[0].find('<') == -1;
}
bool is_simple_define(const pair<string, vector<string> >& decl) {
  return decl.second[0].substr(0, 8) == "#define ";
}

vector<string> process(const vector<string> &lines) {
  // First parse the #include block at the top of the file.
  vector<pair<string, vector<string> > > includes;
  int line = 0;
  int first_include = -1;
  while (line < lines.size()) {
    includes.push_back(parse_declaration(lines, line));
    if (includes.back().first.substr(0, 9) == "#include ") {
      if (first_include == -1) first_include = includes.size()-1;
    } else if (includes.back().first == USINGSTR) {
      if (first_include == -1) first_include = includes.size()-1;
      break;
    } else if (first_include != -1) {
      throw_error(2, string("// PROCESS ERROR: Missing \"") + USINGSTR + "\".");
    }
  }
  if (first_include != -1 && includes.back().first != USINGSTR) {
    throw_error(2, string("// PROCESS ERROR: Missing \"") + USINGSTR + "\".");
  }

  if (first_include == -1) {
    // There was no #include block at all.  Make one.
    includes.clear();
    includes.push_back(make_pair(USINGSTR, vector<string>(1, USINGSTR)));
    includes.back().second.push_back("");
    first_include = 0;
    line = 0;
  }

  // Run through the declarations in order, parsing for keywords and adding any
  // new includes or declarations as needed.
  vector<pair<string, vector<string> > > declarations;
  while (line < lines.size()) {
    declarations.push_back(parse_declaration(lines, line));
  }

restart_scan:
  int first_typedef = -1, last_typedef = -1;
  int first_define = -1, last_define = -1;
  for (int i = 0; i < declarations.size(); i++) {
    // Try to keep simple typedefs and defines together.
    if (is_simple_typedef(declarations[i])) {
      if (first_typedef == -1) first_typedef = i;
    } else {
      if (first_typedef != -1 && last_typedef == -1) last_typedef = i-1;
    }
    if (is_simple_define(declarations[i])) {
      if (first_define == -1) first_define = i;
    } else {
      if (first_define != -1 && last_define == -1) last_define = i-1;
    }

    // Scan for keywords.
    const vector<string>& ds = declarations[i].second;
    int dline = 0, di = 0;
    for (;;) {
      while (skip_comment(ds, dline, di) || skip_string(ds, dline, di))
        ;
      if (dline == ds.size()) break;
      string kw = keyword(ds, dline, di);
      if (kw == "") {
        next_char(ds, dline, di);
      } else {
        // We have a keyword; check for includes or declarations.
        if (include_map.count(kw)) {
          pair<string, vector<string> > decl(include_map[kw],
              vector<string>(1, include_map[kw]));
          int d;
          for (d = 0; d < includes.size(); d++) {
            if (includes[d] == decl) break;
          }
          if (d == includes.size()) {
            includes.push_back(decl);
          }
        }
        if (declaration_map.count(kw)) {
          // Make sure there's not already a declaration by this name.
          int d;
          for (d = 0; d < declarations.size(); d++) {
            if (declarations[d].first == kw) break;
          }
          if (d == declarations.size()) {
            pair<string, vector<string> > decl(kw,
                declaration_map[kw]);

            // Normally we add the library declaration just before the current
            // block.  However, try to keep simple typedefs/defines together.
            int insert_index = i;
            if (is_simple_typedef(decl)) {
              if (first_typedef == -1) {
                insert_index = 0;
                declarations.insert(declarations.begin() + insert_index,
                    make_pair("", vector<string>(1, "")));
              } else if (last_typedef != -1) {
                insert_index = last_typedef + 1;
              }
            }
            if (is_simple_define(decl)) {
              if (first_define == -1) {
                insert_index = 0;
                if (last_typedef != -1) {
                  insert_index = last_typedef + 1;
                  if (insert_index < declarations.size() &&
                      declarations[insert_index].second ==
                          vector<string>(1, "")) {
                    insert_index++;
                  }
                }
                declarations.insert(declarations.begin() + insert_index,
                    make_pair("", vector<string>(1, "")));
              } else if (last_define != -1) {
                insert_index = last_define + 1;
              }
            }

            declarations.insert(declarations.begin() + insert_index, decl);
            goto restart_scan;
          }
        }
      }
    }
  }

  // Keep includes in sorted order.
  sort(includes.begin()+first_include, includes.end());
  includes.erase(unique(includes.begin()+first_include, includes.end()),
      includes.end());

  // Put together the final source file.
  vector<string> ret;
  for (int i = 0; i < includes.size(); i++) {
    ret.insert(ret.end(), includes[i].second.begin(),
                          includes[i].second.end());
  }
  for (int i = 0; i < declarations.size(); i++) {
    ret.insert(ret.end(), declarations[i].second.begin(),
                          declarations[i].second.end());
  }
  return ret;
}

int main(int argc, char* argv[]) {
  if (argc != 3) {
    cerr << "Usage: " << argv[0]
         << " (includes file) (library file)" << endl;
    return 0;
  }

  // Read input source file.
  while (cin.getline(buf, 10000))
    inlines.push_back(strip_trailing_spaces(buf));

  initialize_include_map(argv[1]);
  ifstream fin(argv[2]);
  if (!fin.good()) {
    throw_error(3, string("// PROCESS ERROR: Could not read file \"") +
        argv[2] + "\".");
  }
  string fname;
  while (getline(fin, fname)) read_library_file(fname.c_str());

  outlines = process(inlines);
  for (int i = 0; i < outlines.size(); i++) cout << outlines[i] << '\n';

  return 0;
}
