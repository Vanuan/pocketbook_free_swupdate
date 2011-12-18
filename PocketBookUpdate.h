#ifndef POCKETBOOK_UPDATE_H
#define POCKETBOOK_UPDATE_H

#include <string>
#include <map>
#include <stdint.h>

enum PartitionType
{
  SWUPDATE_TAR_GZ = 0x73,
  ELF_IMG = 0x40,
  APP_IMG = 0x65,
  ROOTFS_IMG = 0x72,
  A_IMG = 0x61
};

struct FWPart
{
  uint32_t type;
  uint32_t reserved;
  uint32_t offset;
  uint32_t size;
};

struct PocketBookUpdateHeader
{
  char magic[0x10];
  char model[0x20];
  char unknownBuffer1[0x50];
  char padding1[0x40];
  uint32_t unknownUInt32;
  char padding2[0x3C];
  FWPart fwParts[0x30];
};

class PocketBookUpdate
{
  char *buffer;
  PocketBookUpdateHeader * header;

  std::string mFilename;
  std::map<PartitionType, const char*> partsNames;
  std::map<std::string, FWPart> parts;

public:
  PocketBookUpdate(const char *filename);
  virtual ~PocketBookUpdate();
  void extract();
  void print();

private:
  int printField(unsigned i);
  int printField(unsigned offset, unsigned size, const char* name);
  int printParts();

};

#endif
