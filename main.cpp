#include "PocketBookUpdate.h"

#include <iostream>
using namespace std;

int main(int argc, char ** argv)
{
  if(argc != 2) {
    cout << "Usage:" << argv[0] << " SWUPDATE.BIN_filename" << endl;
    return -1;
  }
  PocketBookUpdate update(argv[1]);
  update.print();
  update.extract();
}

