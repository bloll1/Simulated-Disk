#ifndef rammVSSD
#define rammVSSD

#include "VVSSD.h"
#include "PersistentArray.h"
#include <string>
#include <vector>
class RAMVSSD
: public VVSSD {
  public:
    std::vector<std::string> ram;
    unsigned int block_s, block_c;

    RAMVSSD(unsigned int block_size, unsigned int block_count,
               const std::string & filename, bool initialize);

    virtual ~RAMVSSD();
public:

virtual std::size_t blockSize() const;

virtual std::size_t blockCount() const;

virtual DiskStatus status() const;

virtual DiskStatus read(blocknumber_t sector, void * buffer);

virtual DiskStatus write(blocknumber_t sector, void * buffer);

virtual DiskStatus sync();

};

#endif
