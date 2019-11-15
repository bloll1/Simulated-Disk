@course CIS310 Operating Systems
@assignment p006
@due 11/15/2019
@Author: Conor M Golden
@Email: Goldencm203@potsdam.edu



Commands:

	q - quit
	c <size> <count> ... - connect
  	c <size> <count> n <fname> - connect to new file
  	c <size> <count> o <fname> - connect to existing file
  	 <size> <count> r - connect to new RAM disk
  	d - disconnect
	r <block> [x <expected data>] [s <expected status>]
	w <block> <data> [s <expected status>]

	data formats: 0xXXXXXX....XX - hex byte data w/even number of digits
              "escaped\tstring" - C/C++ escaped string in quotes


Test Data:

	c 5 5 n newDisk

	w 0 "HELLO"

	w 1 "THINK"

	r 0

	r 1

	d

	c 5 5 r

	w 0 "HELLO"

	w 1 "THINK"

	r 1

	r 0

	d

	c 5 5 o newDisk

	r 0

	r 1


Expected Output:

	(creates a new disk called newDisk

	"HELLO"

	"THINK"

	(disconnected to newDisk disk)

	(creates a new disk on virtual ram)

	"THINK"

	"HELLO"

	(disconnects from the disk)

	(reconnects to the newDisk disk created earlier)


	"HELLO"

	"THINK"

Execution:

	Execute the vssd-test.exe in the build folder (no command line arguments are required

Compilation:

	A makefile is provided for the compilation of this program.
