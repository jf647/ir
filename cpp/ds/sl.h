#ifndef SL_H
#define SL_H

#include <iostream>
#include <memory>
#include <random>

template <typename NodeType>
class Node : public std::enable_shared_from_this<Node<NodeType>> {
private:
  typedef std::shared_ptr<Node<NodeType>> node_ptr;
  NodeType node;
  std::vector<node_ptr> forward;

public:
  const static node_ptr zero_p;
  Node(const NodeType &rhs, int level);
  virtual ~Node();
  void make(std::vector<node_ptr> trace);
  node_ptr right(int level);
  typename NodeType::key_type key() const;
  typename NodeType::value_type value() const;
  NodeType &ref();
  bool operator==(const Node<NodeType> &rhs);
};

template <typename NodeType>
const std::shared_ptr<Node<NodeType>> Node<NodeType>::zero_p =
    std::make_shared<Node<NodeType>>(NodeType::Zero(), 1024);
template <typename NodeType>
Node<NodeType>::Node(const NodeType &rhs, int level)
    : node(rhs), forward(level) {}
template <typename NodeType> Node<NodeType>::~Node() {}

template <typename NodeType>
void Node<NodeType>::make(std::vector<Node<NodeType>::node_ptr> trace) {
  auto max = std::min(trace.size(), forward.size());
  for (auto i = 0; i < max; i++) {
    forward[i] = trace[i]->right(i);
    trace[i]->forward[i] = this->shared_from_this();
  }
}
template <typename NodeType>
typename Node<NodeType>::node_ptr Node<NodeType>::right(int level) {
  if (level >= forward.size()) {
    return zero_p;
  }
  return forward[level];
}
template <typename NodeType>
typename NodeType::key_type Node<NodeType>::key() const {
  return node.key();
}
template <typename NodeType>
typename NodeType::value_type Node<NodeType>::value() const {
  return node.value();
}
template <typename NodeType> NodeType &Node<NodeType>::ref() { return node; }
template <typename NodeType>
bool Node<NodeType>::operator==(const Node<NodeType> &rhs) {
  return node.key() == rhs.key();
}

template <typename NodeType> class SkipList {
public:
  typedef std::shared_ptr<Node<NodeType>> node_ptr;
  typedef typename NodeType::key_type key_type;
  typedef typename NodeType::value_type value_type;

  SkipList(int max_level, float density);
  virtual ~SkipList();
  virtual void operator+=(const NodeType &rhs);
  value_type operator[](const key_type &rhs);

private:
  const node_ptr root;
  int max_level;
  std::bernoulli_distribution bernD;
  std::default_random_engine generator;
  int next_level();
};

template <typename NodeType>
SkipList<NodeType>::SkipList(int max_level, float density)
    : root(Node<NodeType>::zero_p), max_level(max_level), bernD(density) {
  root->make(std::vector<node_ptr>(max_level, root->zero_p));
}

template <typename NodeType>
void SkipList<NodeType>::operator+=(const NodeType &rhs) {
  node_ptr current = std::const_pointer_cast<Node<NodeType>>(root);
  // Find the fastest way to point of insert.
  std::vector<node_ptr> trace(max_level);
  for (int i = max_level - 1; i >= 0; i--) {
    while (current->right(i) != Node<NodeType>::zero_p &&
           current->right(i)->key() < rhs.key()) {
      current = current->right(i);
    }
    trace[i] = current;
  }
  if (current->right(0) != Node<NodeType>::zero_p &&
      current->right(0)->key() == rhs.key()) {
    current->right(0)->ref() += rhs;
    return;
  }
  int nodeLevel = next_level();
  // std::cout << "insert key:" << rhs.key() << " level: " << nodeLevel << " --
  // "; for (auto t : trace)
  //  std::cout << ":" << t->key();
  // std::cout << std::endl;

  auto node = std::make_shared<Node<NodeType>>(rhs, nodeLevel + 1);
  node->make(trace);
}
template <typename NodeType> int SkipList<NodeType>::next_level() {
  int l = 0;
  while (l < max_level - 1 && bernD(generator)) {
    l++;
  }
  return l;
}
template <typename NodeType>
typename SkipList<NodeType>::value_type SkipList<NodeType>::
operator[](const typename SkipList<NodeType>::key_type &rhs) {
  const static typename NodeType::value_type zero_value =
      NodeType::Zero().value();
  node_ptr current = std::const_pointer_cast<Node<NodeType>>(root);
  for (int i = max_level; i >= 0; i--) {

    while (current->right(i) && current->right(i) != Node<NodeType>::zero_p &&
           current->right(i)->key() < rhs) {
      current = current->right(i);
    }
  }
  if (current->right(0) != Node<NodeType>::zero_p &&
      current->right(0)->key() == rhs)
    return current->right(0)->value();
  return zero_value;
}

template <typename NodeType> SkipList<NodeType>::~SkipList() {}

#endif /* SL_H */
