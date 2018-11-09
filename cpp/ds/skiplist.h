#ifndef SKIPLIST_H
#define SKIPLIST_H
#include <iostream>
#include <memory>
#include <random>
#include <vector>

using std::cout;
using std::endl;

using std::make_shared;
using std::ostream;
using std::shared_ptr;
using std::vector;

using std::bernoulli_distribution;
using std::default_random_engine;

template <class NodeType>
class Node : public std::enable_shared_from_this<Node<NodeType>> {
public:
  Node(NodeType val, int level);
  shared_ptr<Node> Right(int level);
  NodeType Value() const;
  NodeType &ValueRef();
  void Make(vector<shared_ptr<Node<NodeType>>> trace);

private:
  NodeType value;
  std::vector<shared_ptr<Node<NodeType>>> forward;
};
template <class NodeType> class NodeRange {
private:
  shared_ptr<Node<NodeType>> _node;

public:
  NodeRange(shared_ptr<Node<NodeType>> node) : _node(node) {}
  NodeRange begin() { return *this; }
  NodeRange end() { return NodeRange(shared_ptr<Node<NodeType>>()); }
  NodeRange operator++() {
    _node = _node->Right(0);
    return *this;
  }
  shared_ptr<Node<NodeType>> operator*() { return _node; }
  bool operator!=(NodeRange other) {
    if (_node) {
      if (other._node) {
        return _node->Value() != other._node->Value();
      }
      return true;
    }
    return false;
  }
};
template <class NodeType> class Skiplist {
private:
  shared_ptr<Node<NodeType>> root;
  int max_level;
  bernoulli_distribution bernD;
  default_random_engine generator;
  double sample_bern() { return bernD(generator); }
  int next_level();

public:
  Skiplist(int level, float density);
  virtual ~Skiplist();
  void Insert(NodeType k);
  NodeRange<NodeType> Find(NodeType k) const;
  template <class NT>
  friend ostream &operator<<(ostream &os, const Skiplist<NT> &sl);
};

template <class NodeType>
Node<NodeType>::Node(NodeType val, int level)
    : value(val), forward(level + 1) {}
template <class NodeType>
shared_ptr<Node<NodeType>> Node<NodeType>::Right(int level) {
  if (forward.size() <= level)
    return shared_ptr<Node<NodeType>>();
  return forward[level];
}
template <class NodeType>
void Node<NodeType>::Make(vector<shared_ptr<Node<NodeType>>> trace) {
  for (int i = 0; i < forward.size(); i++) {
    forward[i] = trace[i]->Right(i);
    trace[i]->forward[i] = this->shared_from_this();
  }
}
template <class NodeType> NodeType Node<NodeType>::Value() const {
  return value;
}
template <class NodeType> NodeType &Node<NodeType>::ValueRef() { return value; }
template <class NodeType>
Skiplist<NodeType>::Skiplist(int level, float density)
    : root(make_shared<Node<NodeType>>(-1, level)), max_level(level),
      bernD(density) {}

template <class NodeType> void Skiplist<NodeType>::Insert(NodeType k) {
  auto current = root;
  vector<shared_ptr<Node<NodeType>>> track(max_level + 1);
  for (int i = max_level; i >= 0; i--) {
    while (current->Right(i) && current->Right(i)->Value() < k) {
      current = current->Right(i);
    }
    track[i] = current;
  }
  if (current->Right(0) && current->Right(0)->Value() == k) {
    current->Right(0)->ValueRef() += k;
  } else {
    auto nodeLevel = next_level();
    auto node = make_shared<Node<NodeType>>(k, nodeLevel);
    node->Make(track);
  }
}
template <class NodeType> int Skiplist<NodeType>::next_level() {
  int l = 0;
  while (l < max_level && sample_bern()) {
    l++;
  }
  return l;
}
template <class NodeType>
NodeRange<NodeType> Skiplist<NodeType>::Find(NodeType k) const {
  auto current = root;
  for (int i = max_level; i >= 0; i--) {
    while (current->Right(i) && current->Right(i)->Value() < k) {
      current = current->Right(i);
    }
  }
  return NodeRange<NodeType>(current->Right(0));
}
template <class NodeType> Skiplist<NodeType>::~Skiplist() {}
template <class NodeType>
ostream &operator<<(ostream &os, const Skiplist<NodeType> &sl) {
  for (int l = sl.max_level; l >= 0; l--) {
    auto current = sl.root;
    os << "Level " << l << ":";
    while (current) {
      os << current->Value() << " -> ";
      current = current->Right(l);
    }
    os << '\n';
  }
  return os;
}

// template <> class IntSkipList : SkipList<int> {};

#endif /* SKIPLIST_H */
