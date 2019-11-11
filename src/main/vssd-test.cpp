#include <iostream>
#include <fstream>

#include <memory>

#include <string>
#include <cstring>
#include <sstream>
#include <vector>
#include "VVSSD.h"
#include "string_util.h"
#include "UnImplVSSD.h"
#include "FileVSSD.h"

// commented out until they are actually implemented; remove comments
// as each type of disk is implemented
//#include "FileVSSD.h"
//#include "RAMVSSD.h"

using namespace std;

// A type for pointers at blocks in memory
typedef char * BlockPtr;

// line number in the input; used for reporting errors
unsigned int line_number = 0;
bool saw_an_error = false;
/* Question: is this code designed to permit some sort of #include type
   command so that one stream of input could open another, finish it, and
   then resume the first? Why do you answer as you do? This might make an
   interesting coding question on an exam.
*/

/**
 * Create a disk with the given parameters. Note that the call to the
 * templated function make_unique<> takes the same parameters as one of the
 * constructors for the class in the angle brackets.
 *
 * Further, for this to compile, the type in the brackets must be a class
 * that extends VVSSD. The commented out lines should be included as the
 * different types of disk become operational.
 *
 * @param dType string encoding of the disk type
 * @param block_size required block size for the disk being opened; must be > 0
 * @param block_count required number of blocks in the disk; must be > 0
 * @param filename name of file for disk types that need it; defaults to ""
 * @param truncate should the disk be reinitialized? defaults to false
 * @return a pointer to the newly created disk or nullptr.
 */
unique_ptr<VVSSD> make_disk(string dType, size_t block_size, size_t block_count,
                            const string & filename = "", bool truncate = false) {
  if (dType == "UnImplVSSD") {
    return make_unique<UnImplVSSD>(block_size, block_count, filename, truncate);
  } else if (dType == "FileVSSD") {
    return make_unique<FileVSSD>(block_size, block_count, filename, truncate);
  } else if (dType == "RAMVSSD") {
    //return make_unique<RAMVSSD>(block_size, block_count, filename, truncate);
  }
  return nullptr;
}

/**
 * Get character over the whitespace
 *
 * Skip whitespace pending on the input stream.
 * @param is reference to input stream for whitespace skipping
 * @return the istream after reading past whitespace
 */
istream & overws(istream & is) {
  char ch;
  if (is >> skipws >> ch)
    is.unget();
  return is;
}

/**
 * Read a pattern of bytes from the given input stream. The pattern is
 * either a quoted string or a sequence of hex-encoded
 * characters. Which it is is determined by the first non-whitespace
 * character encountered.
 *
 * A quoted string can include C++ string literal escaped characters
 * (except for character entities in octal, hex, or Unicode) and they
 * will be internalized in the returned string. A hex-encoded string
 * is formatted like a C++ hex literal except that there must be an
 * even number of characters since it encodes a sequence of 8-bit
 * characters; the hex codes are internalized to the equivalent characters
 * and that string is returned.
 *
 * @param fin the input stream to read from
 * @return a string with the internalized value of the hex or quoted
 * string if all goes well; an empty string if there were any
 * problems.
 */
string read_operand(istream & fin) {
  string str;
  char ch;
  if (fin >> skipws >> ch) {
    if (ch == '\"') {
      fin.unget();
      if (get_quoted_string(fin, str)) {
        overws(fin); // skip trailing whitespace
        str = intern_escaped_string(str);
      }
    } else if (ch == '0') {
      if ((fin >> noskipws >> ch) && ((ch == 'X') || (ch == 'x'))) {
        fin.unget();
        fin.unget();
        if (get_hex_string(fin, str)) {
          overws(fin);
          str = intern_hex_encoded_string(str);
        }
      } else {
        fin.unget();
        fin.unget();
      }
    } else {
      fin.unget();
    }
  }
  cout << "str.length() = " << str.length() << endl;
  return str;
}

/**
 * Read a DiskStatus value from the input stream
 *
 * @param fin the input stream to read
 * @return the parsed disk status of the next token or NO_SUCH_STATUS on error
 */
DiskStatus read_disk_status(istream & fin) {
  string status;

  if (fin >> status) {
    return fromString(status);
  } else {
    return NO_SUCH_STATUS;
  }
}

/**
 * Stupid dump of binary data in the buffer into an output stream. No
 * character conversion at all. Bytes print according to the installed
 * code page or other text parameters of the shell.
 *
 * @param fout the output stream to write to
 * @param buffer the buffer contents to display
 * @param buffer_size size, in bytes of buffer to print
 */
ostream & dump_block(ostream & fout, BlockPtr buffer, size_t buffer_size) {
  for (size_t i = 0; i < buffer_size; ++i)
    fout << buffer[i];
  return fout;
}

/**
 * Check if two blocks differ
 *
 * @param first pointer to first block to compare
 * @param second pointer to second block to compare
 * @param buffer_size size of blocks in bytes
 * @return false if the blocks do NOT differ; true if they do differ.
 */
bool diff_blocks(BlockPtr first, BlockPtr second, size_t buffer_size) {
  bool diff = false;
  for (size_t i = 0; i < buffer_size; ++i)
    if (first[i] != second[i])
      diff = true;
  return diff;
}

/**
 * Fill the block-sized buffer with copies of the pattern. The last
 * copy may be incomplete as the buffer_size may run out.
 *
 * @param buffer a naked pointer into memory that has room for buffer_size bytes
 * @param pattern string containing the byte values to copy across the block
 * @param buffer_size the size in bytes of the buffer to be filled
 */
void fill_block(BlockPtr buffer, const string & pattern, size_t buffer_size) {
  size_t offset = 0;
  cout << string_in_hex(pattern) << endl;
  while ((buffer_size - offset) > pattern.length()) {
    memcpy(buffer + offset, pattern.data(), pattern.length());
    offset += pattern.length();
  }
  if (buffer_size > offset)
    memcpy(buffer + offset, pattern.data(), buffer_size - offset);
}

/**
 * Disconnect the disk/block pair.
 *
 * @param disk the disk pointer to make sure is not connected
 * @param block the block pointer to clean up and disconnect
 * @note the passing (and setting of these) as a pair all of the time
 * implies they should be put into a class/struct of some sort.
 */
void disconnect(unique_ptr<VVSSD> & disk, BlockPtr & block) {
  disk = nullptr; // unique_ptr calls the delete automatically
  if (block != nullptr)
    delete[] block;
  block = nullptr;
}

/**
 * Print out a short menu with all of the commands.
 */
void menu(ostream & out) {
  out << "q - quit" << endl
      << "c <size> <count> ... - connect" << endl
      << "  c <size> <count> n <fname> - connect to new file" << endl
      << "  c <size> <count> o <fname> - connect to existing file" << endl
      << "  c <size> <count> r - connect to new RAM disk" << endl
      << "  d - disconnect" << endl
      << "r <block> [x <expected data>] [s <expected status>]" << endl
      << "w <block> <data> [s <expected status>]" << endl
      << endl
      << "data formats: 0xXXXXXX....XX - hex byte data w/even number of digits" << endl
      << "              \"escaped\\tstring\" - C/C++ escaped string in quotes" << endl
      << endl;
}

/**
 * Display an error on the standard error stream.
 *
 * @param message required message about the error
 * @param blocknumber optional block number to include
 * @param ds optional disk status
 */
void error(const string & message,
           VVSSD::blocknumber_t block = VVSSD::NoBlockNumber,
           DiskStatus ds = NO_SUCH_STATUS) {
  cerr << line_number << ": " << message;
  if (block != VVSSD::NoBlockNumber)
    cerr << " block[" << block << "]";
  if (ds != NO_SUCH_STATUS)
    cerr << " DiskStatus = " << toString(ds);
  cerr << endl;
  saw_an_error = true;
}

string set_prompt(const string fname = "DISCONNECTED", size_t block_size=0, size_t block_count=0) {
  ostringstream ps;
  ps << fname;
  if (block_size || block_count)
    ps << "[" << block_size << ", " << block_count << "]";

  ps << " cmd: ";
  return ps.str();
}

bool display_prompt(const string & prompt, bool quiet = false) {
  if (quiet || cout << prompt)
    return true;
  else
    return false;
}

/**
 * main program
 */
int main(int argc, char *argv[]) {
    vector<string> arguments; // command-line arguments translated to strings
  for (int aNdx = 0; aNdx < argc; ++aNdx)
    arguments.push_back(argv[aNdx]);

  bool quiet = false;
  for (auto a : arguments) {
    if (a == "-q")
      quiet = !quiet;
  }

  unique_ptr<VVSSD> disk = nullptr;
  BlockPtr block = nullptr;

  // string with all the one-letter commands for quick validation
  const string valid_commands = "?cdqrsw";

  string prompt = set_prompt();
  string line;

  // keep disk status across commands (the s command)
  DiskStatus ds = NO_SUCH_STATUS;

  while (display_prompt(prompt, quiet) && getline(cin, line)) {
    line_number++;

    // strip leading and trailing whitespace (it has no impact)
    line = trim_left(trim_right(line));

    // skip the line if it is empty or a comment
    if (is_skippable(line)) continue;

    stringstream cmdin(line, ios::in);
    string cmd;

    if (!(cmdin >> cmd)) {
      error("unable to read command from \"" + line + "\"");
      break;
    }

    if (valid_commands.find_first_of(cmd) == string::npos) {
      error("unknown command in \"" + line + "\"");
      continue;
    }

    if (cmd == "q") {
      break;
    } else if (cmd == "?") {
      menu(cout);
    } else if (cmd == "c") {
      disconnect(disk, block);
      size_t block_size;
      size_t block_count;
      if ((cmdin >> block_size) && (cmdin >> block_count)) {
        string d_type;
        if (cmdin >> d_type) {
          if ((d_type == "n") || (d_type == "o")) {
            // file-based
            string fname;
            if (cmdin >> fname) {
              bool init = (d_type == "n");
              disk = make_disk("FileVSSD", block_size, block_count, fname, init);
              if (disk && disk->status() == OK) {
                block = new char[disk->blockSize()];
                prompt = set_prompt(fname, block_size, block_count);
              } else {
                error("connecting error", VVSSD::NoBlockNumber, disk->status());
                disconnect(disk, block);
              }
            } else {
              error("missing file name for disk image device");
            }
          } else if (d_type == "r") {
            disk = make_disk("RAMVSSD", block_size, block_count);
            if (disk && disk->status() == OK) {
              block = new char[disk->blockSize()];
              prompt = set_prompt("RAM", block_size, block_count);
            }
          } else if (d_type == "u") {
            disk = make_disk("UnImplVSSD", block_size, block_count);
            prompt = set_prompt("UnImplemented");
          } else {
            prompt = set_prompt();
            error("no disk connected: \""+d_type+"\" unknown disk type");
          }
        } else {
          prompt = set_prompt();
          error("no valid disk type for connection");
        }
      } else {
        prompt = set_prompt();
        error("missing <block_size> or <block_count> for connection");
      }
    } else if (disk == nullptr) { // -------------------------------------------
      // no need to process other commands; there is no connected device
      error("no disk currently connected");
      continue;
    } else if (cmd == "d") {
      disk->sync();
      disconnect(disk, block);
      prompt = set_prompt();
    } else if (cmd == "w") {
      size_t blockNumber;
      string operand;
      DiskStatus expected_status = NO_SUCH_STATUS;
      if (cmdin >> blockNumber) { // read the block number
        operand = read_operand(cmdin);
        if (operand != "") {
          string extra_cmd;
          while (cmdin >> extra_cmd) { // are there extra parameters?
            if (extra_cmd == "s") {
              expected_status = read_disk_status(cmdin);
            } else {
              error("unknown extra write parameter \"" + extra_cmd + "\"");
            }
          }
          fill_block(block, operand, disk->blockSize());

          disk->sync();

          ds = disk->write(blockNumber, block);

          if (expected_status == NO_SUCH_STATUS) { // expect OK
            if (ds != OK) {
              error("write generates an error", blockNumber, ds);
            }
          } else {
            if (ds != expected_status) {
              error("write error: expected DiskStatus = " + toString(expected_status) + "; got " + toString(ds) + " block[" + to_string(blockNumber) + "]");
            }
          }
        } else {
          error("missing <pattern> for write");
        }
      } else {
        error("missing <block_number> for write");
      }
    } else if (cmd == "r") {
      size_t blockNumber;
      string operand;
      BlockPtr expected_block = nullptr;
      DiskStatus expected_status = NO_SUCH_STATUS;

      if (cmdin >> blockNumber) { // there is a block number
        string extra_cmd;
        while (cmdin >> extra_cmd) { // are there more parameters?
          if (extra_cmd == "x") { // expected block value
            operand = read_operand(cmdin);
            expected_block = new char[disk->blockSize()];
            fill_block(expected_block, operand, disk->blockSize());
          } else if (extra_cmd == "s") {
            expected_status = read_disk_status(cmdin);
          } else {
            error("unknown extra read parameter \"" + extra_cmd + "\"");
          }
        }

        disk->sync();
        ds = disk->read(blockNumber, block);

        if (expected_status == NO_SUCH_STATUS) { // expect OK
          if (ds != OK)
            error("read error", blockNumber, ds);
        } else {
          if (ds != expected_status) {
            error("read error: expected DiskStatus = " + toString(expected_status) + "; got " + toString(ds) + " block[" + to_string(blockNumber) + "]");
          }
        }

        if (ds == OK) {
          if (operand.empty()) {
            dump_block(cout, block, disk->blockSize());
          } else if (diff_blocks(block,expected_block, disk->blockSize())) {
            error("block read failed to match block expected", blockNumber, ds);
          } else {
            // do nothing
          }
        }

        if (expected_block)
          delete[] expected_block;
      } else {
        error("missing <block_number> for read");
      }
    } else if (cmd == "s") {
      DiskStatus expected_status = NO_SUCH_STATUS;
      string extra_cmd;
      while (cmdin >> extra_cmd && !extra_cmd.empty()) { // are there more parameters?
        if (extra_cmd == "s") {
          expected_status = read_disk_status(cmdin);
        } else {
          error("unknown extra status parameter \"" + extra_cmd + "\"");
        }
      }

      ds = disk->status();
      if (expected_status == NO_SUCH_STATUS) {
        cout << "Last Command DiskStatus = " << toString(ds) << endl;
      } else if (ds != expected_status) {
        error("status error: expected DiskStatus = " + toString(expected_status) + "; got " + toString(ds));
      }
    } else {
    }
  }

  return saw_an_error?1:0;
}
