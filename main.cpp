#include "PocketBookUpdate.h"

#include <iostream>
#include <fstream>
using namespace std;

int main(int argc, char ** argv)
{
  if(argc != 2) {
    cout << "Usage:" << argv[0] << " SWUPDATE.BIN_filename" << endl;
    return -1;
  }
  PocketBookUpdate update(argv[1]);
  update.print();
}

PocketBookUpdate::PocketBookUpdate(const char*filename)
{
  // allocate buffer
  int size_of_header = sizeof(PocketBookUpdateHeader);
  buffer = new char[size_of_header];

  // read buffer
  fstream file;
  file.open(filename);
  file.read(buffer, size_of_header);
  file.close();

  // interpret buffer as header
  header = reinterpret_cast<PocketBookUpdateHeader*>(buffer);

  

}

PocketBookUpdate::~PocketBookUpdate()
{
  delete[] buffer;
}

int PocketBookUpdate::printField(unsigned offset, unsigned size, const char* name)
{
  cout.width(10);
  cout << showbase << hex;
  cout << offset << " ";
  cout.width(10);
  cout << offset+size << "\t\t" << name << ":\t\t";
  cout << endl;
}

int PocketBookUpdate::printField(unsigned i)
{
  static const int fieldsNumber = 7;
  if( i >= fieldsNumber ) return 0;
  static const int sizes[] = {
    sizeof(header->magic),
    sizeof(header->model),
    sizeof(header->unknownBuffer1),
    sizeof(header->padding1),
    sizeof(header->unknownUInt32),
    sizeof(header->padding2),
    sizeof(header->fwParts)
  };
  const char* fieldNames[] = {
    "Magic",
    "Model",
    "Unknown buffer 1",
    "Padding1",
    "UnknownUInt32",
    "Padding2",
    "Partition table"
  };
  static int offset = 0;
  printField(offset, sizes[i], fieldNames[i]);
  offset += sizes[i];
  return sizes[i];
}

void PocketBookUpdate::print()
{
  cout << "Header part: " << endl;

  unsigned int offset = 0;
  unsigned int size = 0;

  size = printField(0);
  const char * tab = "\t\t\t\t\t\t\t";
  cout << tab;
  cout.write(header->magic, size);
  cout << endl;

  size = printField(1);
  cout << tab;
  cout.write(header->model, size);
  cout << endl;

  size = printField(2);
  cout << tab;
  cout << endl;
  size = printField(3);
  cout << tab;
  cout << endl;
  size = printField(4);
  cout << tab;
  cout << header->unknownUInt32;
  cout << endl;
  size = printField(5);
  cout << tab;
  cout << endl;
  size = printField(6);
  cout << tab;
  cout << endl;

  cout << "Firmware partitions: " << endl;
  printParts();
}

int PocketBookUpdate::printParts()
{
  int baseOffset = sizeof(*header);
  for(int i = 0; i < sizeof(header->fwParts)/sizeof(header->fwParts[0]); ++i)
  {
    int offset = header->fwParts[i].offset;
    int size = header->fwParts[i].size;
    if(0 == size ) continue;
    offset += baseOffset;
    printField(offset, size, "part");
    switch(header->fwParts[i].type)
    {
      case SWUPDATE_TAR_GZ:
        cout << "swupdate.tar.gz";
        break;
      case ELF_IMG:
        cout << "elf.img";
        break;
      case APP_IMG:
        cout << "app.img";
        break;
      case ROOTFS_IMG:
        cout << "rootfs.img";
        break;
      case A_IMG:
        cout << "a.img";
        break;
      default:
        cout << "unknown type" << header->fwParts[i].type;
    }
    cout << endl;
  }
}
