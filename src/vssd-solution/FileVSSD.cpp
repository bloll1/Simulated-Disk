#include "FileVSSD.h"

#include <stdio.h>
#include <cstring>
#include <iostream>


FileVSSD::FileVSSD(unsigned int block_size, unsigned int block_count,
                      const std::string & filename, bool initialize) {

    file = new PersistentArray(filename, initialize);
    block_s = block_size;
    block_c = block_size;

    char k[block_size];
    memcpy(k, (char*)&block_size, sizeof(block_size));
    file->write_k(0, k, block_size);

    memcpy(k, (char*)&block_count, sizeof(block_count));
    file->write_k(1, k, block_size);
    
    ds = DiskStatus::OK;
}

FileVSSD::~FileVSSD() {
  file->~PersistentArray();
}


std::size_t FileVSSD::blockSize() const {
  if (ds != DiskStatus::NOT_READY) {
    return block_s;
  }
  return 0;
}

std::size_t FileVSSD::blockCount() const {
  if (ds != DiskStatus::NOT_READY) {
    return block_c;
  }
  return 0;
}

DiskStatus FileVSSD::status() const {
  return ds;
}

DiskStatus FileVSSD::read(blocknumber_t sector, void * buffer) {
  if (sector < blockCount() && sector >= 0) {
    free(buffer);
    buffer = file->read_k(sector + 2, (char *)buffer, block_s);
    if (buffer == nullptr) {
      std::cout << "Error: Block #" << sector << " is a null pointer" << '\n';
      ds = DiskStatus::ERROR;
    } else
      ds = DiskStatus::OK;
  } else {
    std::cout << "Error: Block #" << sector << " is out of range" << '\n';
    ds = DiskStatus::BLOCK_OUT_OF_RANGE;
  }
  return ds;
}





DiskStatus FileVSSD::write(blocknumber_t sector, void * buffer) {
  if (sector < blockCount() && sector >= 0) {
    if (buffer != nullptr) {
      file->write_k(sector + 2, (char *)buffer, block_s);
      free(buffer);
      ds = DiskStatus::OK;
    } else {
    std::cout << "Error: Block #" << sector << " is a null pointer" << '\n';
    ds = DiskStatus::ERROR;
    }
  } else {
    std::cout << "Error: Block #" << sector << " is out of range" << '\n';
    ds = DiskStatus::BLOCK_OUT_OF_RANGE;
  }
  return ds;
}

DiskStatus FileVSSD::sync() {
  file->flush_stream();
  return DiskStatus::OK;
}
