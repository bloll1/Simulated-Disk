#ifndef FilVSSD
#define FilVSSD

#include "VVSSD.h"
#include "PersistentArray.h"
#include "WorkingBitMap.h"
#include <string>

class FileVSSD
: public VVSSD {
  public:
    PersistentArray * file;
    unsigned int block_s, block_c;

    FileVSSD(unsigned int block_size, unsigned int block_count,
               const std::string & filename, bool initialize);

    virtual ~FileVSSD();
public:

virtual std::size_t blockSize() const;

virtual std::size_t blockCount() const;

virtual DiskStatus status() const;

virtual DiskStatus read(blocknumber_t sector, void * buffer);

virtual DiskStatus write(blocknumber_t sector, void * buffer);

virtual DiskStatus sync();

};

#endif
