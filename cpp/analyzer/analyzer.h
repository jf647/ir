#ifndef ANALYZER_H
#define ANALYZER_H
#include <string>
#include <vector>

using std::string;
using std::vector;

class Analyzer {
private:
public:
  virtual vector<string> tokens(string text) { return {text}; }
  Analyzer() {}
  virtual ~Analyzer() {}
};

#endif /* ANALYZER_H */
