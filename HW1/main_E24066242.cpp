#include <iostream>
#include <string>
#include <ctime>
#include "KL_E24066242.h"
using namespace std;

int main(int argc,char **argv){
  clock_t start, stop;
  start = clock();
  KL kl(argv[1]);
  //vector<pair<Node*,Node*>>swapped_candidates;
  //Node* n0,n1;
  /*
  while(1){
    if(!kl.max_cost()){break;}
  }*/
  kl.check();
  while(kl.max_cost()){
    kl.update_temp();
    kl.check();
    cout<<endl;
  }
  kl.result();
  stop = clock();
  cout << "runtime: "<<double(stop - start) / CLOCKS_PER_SEC <<" s"<<endl;
}
