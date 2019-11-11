#include "RAMVSSD.h"



RAMVSSD::RAMVSSD(unsigned int block_size, unsigned int block_count,
                       const std::string & filename, bool initialize) {

}

RAMVSSD::~RAMVSSD() {

}


std::size_t RAMVSSD::blockSize() const {
  return 0;
}

std::size_t RAMVSSD::blockCount() const {
  return 0;
}

DiskStatus RAMVSSD::status() const {

}

DiskStatus RAMVSSD::read(blocknumber_t sector, void * buffer) {

}

DiskStatus RAMVSSD::write(blocknumber_t sector, void * buffer) {

}

DiskStatus RAMVSSD::sync() {

}
