#include "FileVSSD.h"

#include <stdio.h>
#include <cstring>


FileVSSD::FileVSSD(unsigned int block_size, unsigned int block_count,
                      const std::string & filename, bool initialize) {
file = new PersistentArray(filename);
char k[block_size];
memcpy(k, (char*)&block_size, sizeof(block_size));
file->write_k(0, k, block_size);
block_s = block_size;
memcpy(k, (char*)&block_count, sizeof(block_count));
file->write_k(1, k, block_size);
block_c = block_count;
}

FileVSSD::~FileVSSD() {
  file->~PersistentArray();
}


std::size_t FileVSSD::blockSize() const {
  return block_s;
}

std::size_t FileVSSD::blockCount() const {
  return block_c;
}

DiskStatus FileVSSD::status() const {
return DiskStatus::OK;
}

DiskStatus FileVSSD::read(blocknumber_t sector, void * buffer) {
  FileVSSD disk;
  if (sizeof(buffer) < blockSize() && disk.blockCount() < ((file->length() / disk.blockSize()) + 2)) {
    file->read_k(sector + 2, (char *)buffer, disk.blockSize());
    return DiskStatus::OK;
    }
  return DiskStatus::BLOCK_OUT_OF_RANGE;
  }





DiskStatus FileVSSD::write(blocknumber_t sector, void * buffer) {
  FileVSSD disk;
  if (sizeof(buffer) < disk.blockSize() && disk.blockCount() < ((file->length() / disk.blockSize()) + 2)) {
    file->write_k(sector + 2, (char *)buffer, disk.blockSize());
    return DiskStatus::OK;
    }
  return DiskStatus::BLOCK_OUT_OF_RANGE;
  }













DiskStatus FileVSSD::sync() {
  return DiskStatus::NOT_YET_IMPLEMENTED;
}