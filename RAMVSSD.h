#ifndef RAMVSSD
#define RAMVSSD

#include "VVSSD.h"


class RAMVSSD : public VVSSD {
  public:
    RAMVSSD(unsigned int block_size, unsigned int block_count,
               const std::string & filename, bool initialize);

    virtual ~RAMVSSD();
}

std::size_t RAMVSSD::blockSize() const;

std::size_t RAMVSSD::blockCount() const;

DiskStatus RAMVSSD::status() const;

DiskStatus RAMVSSD::read(blocknumber_t sector, void * buffer);

DiskStatus RAMVSSD::write(blocknumber_t sector, void * buffer);

DiskStatus RAMVSSD::sync()



#endif
