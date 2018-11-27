#ifndef CONFIG_H
#define CONFIG_H

#include <string>

using std::string;

class Config {
private:
public:
  Config() {}
  virtual ~Config() {}
  string base_dir() { return "/tmp/nvrdb/data"; }
};

#endif /* CONFIG_H */
