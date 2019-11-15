//@Author: Conor M Golden
//@Email: Goldencm203@potsdam.edu

#ifndef FilVSSD
#define FilVSSD

#include "VVSSD.h"
#include "PersistentArray.h"
#include <string>


/*
*Virtualized disk implementation through the FileVSSD class,
*  the class stores the data in a PersistentArray and each element acts as the disk
*  sector. FileVSSD extends the VVSSD class inheriting calls requested of
*  the current status from the ds varaiable, the blocksize(block_s), or
*  the blockcount(block_c)
*/
class FileVSSD
: public VVSSD {
  public:
    PersistentArray * file;
    unsigned int block_s, block_c;


    //Constructor for initializion of the virtualized disk
    FileVSSD(unsigned int block_size, unsigned int block_count,
               const std::string & filename, bool initialize);

    //Destructor for memory
    virtual ~FileVSSD();
  public:

    //Returns the size of each block given on initializion
    //  if disk is NOT_READY it returns 0
    virtual std::size_t blockSize() const;

    //Returns the total block count on the disk given on initializion
    //  if disk is NOT_READY it returns 0
    virtual std::size_t blockCount() const;

    //Returns the ds(current DiskStatus) variable
    virtual DiskStatus status() const;

    //Given the sector it reads the data at the given block and sets the
    // buffer pointer to it.
    virtual DiskStatus read(blocknumber_t sector, void * buffer);

    //Given the sector it writes the data from the buffer into the block
    //  and passes the buffer to the read_k function in PersistentArray
    virtual DiskStatus write(blocknumber_t sector, void * buffer);

    //Flushes the buffer stream out to the file
    virtual DiskStatus sync();

    //A DiskStatus variable that is set anytime a FileVSSD function initializes,
    // writes or reads
    DiskStatus ds = DiskStatus::NOT_READY;

};

#endif
