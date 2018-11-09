#ifndef SNOWBALL_ANALYZER_H
#define SNOWBALL_ANALYZER_H
#include "analyzer.h"
#include <iostream>
#include <sstream>

#include "cpp/analyzer/snowball/api.h"
#include "cpp/analyzer/snowball/snowball_english.h"

using std::istringstream;
using std::string;
using std::vector;

class SnowballAnalyzer : public Analyzer {

private:
public:
  SnowballAnalyzer() {}
  virtual vector<string> tokens(string text) override {

    std::transform(text.begin(), text.end(), text.begin(), ::tolower);
    struct SN_env *z = snowball_english_create_env();
    symbol *cstr = new symbol[text.length() + 1];
    std::strcpy((char *)cstr, text.c_str());
    SN_set_current(z, (int)text.length(), cstr);
    snowball_english_stem(z);
    string stemmed((char *)z->p, z->l);
    snowball_english_close_env(z);
    istringstream iss(stemmed);
    vector<string> results(std::istream_iterator<std::string>{iss},
                           std::istream_iterator<std::string>());
    return results;
  }
  virtual ~SnowballAnalyzer() {}
};

#endif /* SNOWBALL_ANALYZER_H */
