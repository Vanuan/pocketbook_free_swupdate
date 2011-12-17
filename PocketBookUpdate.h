#ifndef POCKETBOOK_UPDATE_H
#define POCKETBOOK_UPDATE_H

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
  const static int fieldsNumber = 2;
  const unsigned char** fieldNames[fieldsNumber];

  static int sizes[fieldsNumber];
public:
  PocketBookUpdate(const char *filename);
  virtual ~PocketBookUpdate();
  void print();
  int printField(unsigned i);
  int printField(unsigned offset, unsigned size, const char* name);
  int printParts();

};

#endif
