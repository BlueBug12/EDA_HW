#ifndef KL_E24066242_H
#define KL_E24066242_H
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <list>
#include <utility>

struct Node;
class KL{
public:
  KL(char* benchmark);
  ~KL();
  void read_file(char* benchmark);
  void find_pair_to_swap();
  void swap_pair(map<string, Node*>::iterator iter1,map<string, Node*>::iterator iter2);
  void move_out(Node*n);

private:
  int numnodes;
  int cost;
  std::map<std::string,Node*>all_nodes;
  std::map<std::string,Node*>group0;
  std::map<std::string,Node*>group1;
};

#endif
