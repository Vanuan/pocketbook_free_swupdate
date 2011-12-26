#include "PocketBookUpdate.h"

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include <sys/stat.h>

string convertInt(int number)
{
   if( number == 0) return "";
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

PocketBookUpdate::PocketBookUpdate(const char*filename) : mFilename(filename)
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

  partsNames[SWUPDATE_TAR_GZ] = "swupdate.tar.gz";
  partsNames[ELF_IMG] = "elf.img";
  partsNames[APP_IMG] = "app.img";
  partsNames[ROOTFS_IMG] = "rootfs.img";
  partsNames[A_IMG] = "a.img";

  for(int i = 0; i < sizeof(header->fwParts)/sizeof(header->fwParts[0]); ++i)
  {
    if(0 == header->fwParts[i].size ) continue;
    string name;
    // printName
    map<PartitionType, const char*>::const_iterator partNameIter =
       partsNames.find(static_cast<PartitionType>(header->fwParts[i].type));
    if(partNameIter != partsNames.end())
    {
      name = partNameIter->second;
    }
    else
    {
      name = "unknownImg";
    }
    int nameCounter = 0;
    string tempName;
    map<string, FWPart>::iterator it = parts.end();
    do {
      tempName = name + convertInt(nameCounter);
      it = parts.find(string(tempName));
      nameCounter++;
    }
    while(it != parts.end());
    name = tempName;
    pair<map<string, FWPart>::iterator,bool> index = 
        parts.insert(pair<string, FWPart>(name, header->fwParts[i]));
    index.first->second.offset += sizeof(*header);
  }
}

PocketBookUpdate::~PocketBookUpdate()
{
  delete[] buffer;
}

void PocketBookUpdate::extract(const char * outputDir)
{
  struct stat st;
  if(stat(outputDir,&st) != 0)
  {
    cout << "Directory " << outputDir << " doesn't exist" << endl;
    return;
  }
  ifstream infile(mFilename.c_str(), ifstream::binary);
  map<string, FWPart>::const_iterator it;
  for ( it=parts.begin(); it != parts.end(); it++ )
  {
    infile.seekg(it->second.offset);
    string outputFilename = string(outputDir) + string("/") + it->first;
    ofstream outfile(outputFilename.c_str(), ofstream::binary);
    long unsigned int size = it->second.size;
    char*buf = new char[size];
    infile.read(buf, size);
    outfile.write(buf, size);
    delete[] buf;
    outfile.close();
  }
  infile.close();
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
  map<string, FWPart>::const_iterator it;
  for ( it=parts.begin() ; it != parts.end(); it++ )
  {
    printField(it->second.offset, it->second.size, it->first.c_str());
  }
}

