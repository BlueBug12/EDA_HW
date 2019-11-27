#include <iostream>
#include <string>
#include <ctime>
#include "KL_E24066242.h"
using namespace std;

int main(int argc,char **argv){
  clock_t start, stop;
  start = clock();
  string benchmark = argv[1];
  KL kl(benchmark);

  while(kl.max_cost()){
    kl.update_new_group();
  }

  stop = clock();
  kl.result();
  cout << "runtime: "<<double(stop - start) / CLOCKS_PER_SEC <<" s"<<endl;
  kl.write_file(benchmark,double(stop - start) / CLOCKS_PER_SEC);
}
