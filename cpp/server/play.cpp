#include "iostream"
#include "string"
#include "vector"

using std::cout;
using std::endl;
using std::string;
using std::vector;
class bq {};
class dq1 : public bq {};
class dq2 : public bq {};

template <class b> class exec {
public:
  exec(b _b) {}
};
template <typename baseT> struct base_traits {
  typedef typename baseT::base_type_tag base_type_tag;
};

struct base_type {};
struct der1_type : public base_type {};
struct der2_type : public base_type {};

template <typename bt> void handleT(const bt &_bt, bq *_qt);
class der1;
template <> void handleT(const der1 &_bt, bq *_qt);
class base {
  string name;

public:
  typedef base_type base_type_tag;
  base(string n) : name(n) {}
  base() : base("base") {}
  void print() const { cout << name << endl; }
  void q(bq _q) { cout << "q" << endl; }
  template <typename bt> friend void handleT(const bt &_bt, bq *q);
  virtual void handleTTr(bq *_q) { handleT(*this, _q); }
};

class der1 : public base {
  string secret;

public:
  typedef der1_type base_type_tag;
  der1() : base("der1"), secret("haha") {}
  void q(dq1 _q) { cout << "dq1" << endl; }
  template <typename bt> friend void handleT(const bt &_bt, bq *q);
  virtual void handleTTr(bq *_q) { handleT(*this, _q); }
};
class der2 : public base {
public:
  typedef der2_type base_type_tag;
  der2() : base("der2") {}
  void q(dq2 _q) { cout << "dq2" << endl; }
  virtual void handleTTr(bq *_q) { handleT(*this, _q); }
};

template <typename bt> void handleT(const bt &_bt, bq *_qt) {
  cout << "there" << endl;
  _bt.print();
}
template <> void handleT(const der1 &_bt, bq *_qt) {
  cout << "here" << _bt.secret << endl;

  _bt.print();
}

template <typename bt, typename qt>
void handleT(bt _bt, qt _qt, der2_type _base_type) {
  _bt.print();
}
void handle(base b, bq q) { cout << "b, bq" << endl; }
void handle(der1 b, dq1 q) { cout << "der1, bq1" << endl; }
void handle(der2 b, dq2 q) { cout << "der2, bq2" << endl; }
int main(int argc, char *argv[]) {
  vector<base *> v({new base, new der1, new der2});
  for (base *b : v) {
    b->print();
    b->handleTTr(new bq);
    b->handleTTr(new dq1);
    b->handleTTr(new dq2);
    // handleT(b, bq{},
  }
  return 0;
}
