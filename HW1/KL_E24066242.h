#ifndef KL_E24066242_H
#define KL_E24066242_H
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <utility>
#include <climits>
#include <algorithm>
struct Node;
typedef std::list<Node*>::iterator it;
class KL{
public:
  KL(const std::string benchmark);
  ~KL();
  void read_file(const std::string filename);
  void write_file(const std::string filename,const double runtime);
  int find_pair_to_swap(Node** n0 , Node** n1);
  void pre_swap(Node* n0,Node* n1);
  void swap_nodes(Node*n0,Node*n1);
  bool max_cost();
  void update_new_group();
  void result();
  std::pair<it,it> priority(it new0,it new1, it old0, it old1);

private:
  int numnodes;
  int cost;
  int first_cost;
  std::map<std::string,Node*>all_nodes;
  std::list<Node*>group0;
  std::list<Node*>group1;
  std::list<Node*>temp_group0;
  std::list<Node*>temp_group1;
};

#endif
