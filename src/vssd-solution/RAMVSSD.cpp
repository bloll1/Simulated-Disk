#include "RAMVSSD.h"

#include <stdio.h>
#include <cstring>
#include <iostream>


RAMVSSD::RAMVSSD(unsigned int block_size, unsigned int block_count,
                      const std::string & filename, bool initialize) {
    std::cout << "MADE IT 10" << '\n';
ram = new char[block_count*block_size +2];
block_c = block_count;
block_s = block_s;
char k[block_size];
std::cout << "MADE IT 15" << '\n';
memcpy(k, (char*)&block_size, sizeof(block_size));
std::cerr << "MADE IT 17" << '\n';
write(0, k);
memcpy(k, (char*)&block_count, sizeof(block_count));
std::cout << k << '\n';
std::cout << "MADE IT 20" << '\n';
write(1, k);
}

RAMVSSD::~RAMVSSD() {
delete ram;
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
  free(buffer);
  char tempbuff[block_c*block_s +2];
  char * newbuff = new char[block_s];

  strncpy(tempbuff, ram, block_c * block_s +2);
  for (size_t i = 0; i < block_s; i++) {
    newbuff[i] = tempbuff[i + (sector * block_s)];
  }
  buffer = newbuff;
  return DiskStatus::OK;
  }





DiskStatus RAMVSSD::write(blocknumber_t sector, void * buffer) {
  char tempbuff[block_c*block_s +2];
  char * newbuff = new char[block_s];
std::cout << "MADE IT 61" << '\n';
std::cout << (char *)buffer << '\n';
  strncpy(newbuff, (char *) buffer,  block_s);

  strncpy(tempbuff, ram, block_c * block_s +2);

  for (size_t i = 0; i < block_s; i++) {
    tempbuff[i + (sector * block_s)] = newbuff[i];
  }
  std::cout << "MADE IT 68" << '\n';
  tempbuff[block_c * block_s +2] = '\0';
  strncpy(ram, tempbuff, block_c * block_s +2);
  std::cout << "MADE IT 70" << '\n';
  //free(buffer);
  for (size_t i = 0; i < block_c * block_s +2; i++) {
    std::cout << ram[i];
  }
  std::cout << std::endl;
  return DiskStatus::OK;
  }

DiskStatus RAMVSSD::sync() {
  return DiskStatus::OK;
}
