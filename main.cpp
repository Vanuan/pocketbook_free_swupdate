#include "PocketBookUpdate.h"

#include <iostream>
using namespace std;

int main(int argc, char ** argv)
{
  if(argc != 3) {
    cout << "Usage:" << argv[0] << " SWUPDATE.BIN_filename output_dir" << endl;
    return -1;
  }
  PocketBookUpdate update(argv[1]);
  update.print();
  update.extract(argv[2]);
}

