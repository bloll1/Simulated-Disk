#include "RAMVSSD.h"

#include <stdio.h>
#include <cstring>
#include <iostream>


RAMVSSD::RAMVSSD(unsigned int block_size, unsigned int block_count,
                      const std::string & filename, bool initialize) {

  std::vector<std::string> tempram(block_count , "");
  ram = tempram;
  //for (unsigned int i = 0; i < block_count+2; i++)
    //ram.at(i) = "";

  ram.at(0) = std::to_string(block_size);
  ram.at(1) = std::to_string(block_count);

  block_s = block_size;
  block_c = block_count;

  ds = DiskStatus::OK;
}




RAMVSSD::~RAMVSSD() {
}





std::size_t RAMVSSD::blockSize() const {
  if (ds != DiskStatus::NOT_READY) {
    return block_s;
  }
  return 0;
}




std::size_t RAMVSSD::blockCount() const {
  if (ds != DiskStatus::NOT_READY) {
    return block_c;
  }
  return 0;
}





DiskStatus RAMVSSD::status() const {
  return ds;
}






DiskStatus RAMVSSD::read(blocknumber_t sector, void * buffer) {
  if (sector < blockCount() && sector >= 0) {
    free(buffer);
    char * entry = new char[stoi(ram.at(0))];
    strncpy(entry, (ram.at(sector + 2)).c_str(), stoi(ram.at(0)));
    buffer = entry;
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









DiskStatus RAMVSSD::write(blocknumber_t sector, void * buffer) {
  if (sector < blockCount() && sector >= 0) {
    if (buffer != nullptr) {
    std::string entry((char *) buffer);
    ram.at(sector + 2) = entry;
    for (size_t i = 0; i < ram.capacity(); i++) {
      std::cerr << "RAM [" << i << "] : " << ram.at(i) << '\n';
    }
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








DiskStatus RAMVSSD::sync() {
  return DiskStatus::OK;
}
