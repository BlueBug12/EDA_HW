#include <iostream>
#include <string>
#include <ctime>
#include "KL_E24066242.h"
using namespace std;

int main(int argc,char **argv){
  clock_t start, stop;
  start = clock();
  KL kl(argv[1]);
  kl.find_pair_to_swap();
  stop = clock();
  cout << "runtime: "<<double(stop - start) / CLOCKS_PER_SEC <<" s"<<endl;
}
