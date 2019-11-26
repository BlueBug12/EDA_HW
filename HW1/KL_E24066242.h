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
typedef std::list<Node*>::iterator it;
class KL{
public:
  KL(char* benchmark);
  ~KL();
  void read_file(char* benchmark);
  int find_pair_to_swap(Node** n0 , Node** n1);
  //void lock_pair(std::list<Node*>::iterator iter0,std::list<Node*>::iterator iter1);
  void update_temp_Dvalue(Node* n0,Node* n1);
  void update_Dvalue(Node*n0,Node*n1);
  bool max_cost();
  void update_temp();
  void check(bool temp=1);
  void result();
  std::pair<it,it> priority(it new0,it new1, it old0, it old1);
  void f();

private:
  int numnodes;
  int cost;
  std::map<std::string,Node*>all_nodes;
  std::list<Node*>group0;
  std::list<Node*>group1;
  std::list<Node*>temp_group0;
  std::list<Node*>temp_group1;
  it header0;
  it header1;
};

#endif
