/*
 * The MIT License (MIT)
 * Copyright (c) 2016 Morten Krogh
 */

/*
 * The cgen program reads the configurstion file and the template file and produces a header and
 * source file for the specialized types and functions.  The program produces the two files and
 * exits or prints an error message to stderr and exits.  The program allocates memory and opens
 * files. Clean up is performed by the OS at program termination.  The entire cgen program is kept
 * in this source file for simplicity.
 */

#include <stdio.h>












int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s conf-file\n", argv[0]);
		return 1;
	}

	








	return 0;
}
