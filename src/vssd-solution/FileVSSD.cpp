#include "FileVSSD.h"

#include <stdio.h>
#include <cstring>
#include <iostream>


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
    memcpy(k, (char*)"", block_size);
    for (size_t i = 0; i < block_count; i++) {
      file->write_k(i + 2, k, block_size);
    }
    map = new WorkingBitMap(block_count);
}

FileVSSD::~FileVSSD() {
  map->~WorkingBitMap();
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
  if (sizeof(buffer) < blockSize() && sector < blockCount() && map->get((unsigned int) sector + 2) == 1) {
    delete (char *)buffer;
    buffer = (void *)file->read_k(sector + 2, blockSize());
    return DiskStatus::OK;
    }
  return DiskStatus::BLOCK_OUT_OF_RANGE;
  }





DiskStatus FileVSSD::write(blocknumber_t sector, void * buffer) {
  if (sizeof(buffer) < blockSize() && sector < blockCount()) {
    file->write_k(sector + 2, (char *)buffer, blockSize());
    map->set((unsigned int) sector + 2);
    return DiskStatus::OK;
   }
  delete (char *)buffer;
  return DiskStatus::BLOCK_OUT_OF_RANGE;
  }

DiskStatus FileVSSD::sync() {
  file->flush_stream();
  return DiskStatus::OK;
}
