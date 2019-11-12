#include "RAMVSSD.h"

#include <stdio.h>
#include <cstring>
#include <iostream>


RAMVSSD::RAMVSSD(unsigned int block_size, unsigned int block_count,
                      const std::string & filename, bool initialize) {

ram.resize(block_count + 3);
ram.at(0) = std::to_string(block_size);
block_s = block_size;
block_c = block_count;
ram.at(1) = std::to_string(block_count);
}

RAMVSSD::~RAMVSSD() {
delete &ram;
}


std::size_t RAMVSSD::blockSize() const {
  return block_s;
}

std::size_t RAMVSSD::blockCount() const {
  return block_c;
}

DiskStatus RAMVSSD::status() const {
return DiskStatus::OK;
}

DiskStatus RAMVSSD::read(blocknumber_t sector, void * buffer) {
char * entry = new char[stoi(ram.at(0))];
free(buffer);
strncpy(entry, (ram.at(sector + 2)).c_str(), stoi(ram.at(0)));
buffer = entry;
return DiskStatus::OK;
}





DiskStatus RAMVSSD::write(blocknumber_t sector, void * buffer) {
  std::cout << "BUFFER on Write: "<< (char *) buffer << '\n';
  std::string entry((char *) buffer);
  ram.at(sector + 2) = entry;
  free(buffer);
  return DiskStatus::OK;
}


DiskStatus RAMVSSD::sync() {
  return DiskStatus::OK;
}
