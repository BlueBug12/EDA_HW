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
#include <utility>

struct Node;
class KL{
public:
  KL(char* benchmark);
  ~KL();
  void read_file(char* benchmark);
  int find_pair_to_swap(Node** n0 , Node** n1);
  void lock_pair(std::list<Node*>::iterator iter0,std::list<Node*>::iterator iter1);
  void update_Dvalue(Node*n0,Node*n1,bool temp);
  bool max_cost();
  void update_temp();
  void check();
  void result();

private:
  int numnodes;
  int cost;
  std::map<std::string,Node*>all_nodes;
  std::list<Node*>group0;
  std::list<Node*>group1;
  std::list<Node*>temp_group0;
  std::list<Node*>temp_group1;
};

#endif
