/*
 * The MIT License (MIT)
 * Copyright (c) 2016 Morten Krogh
 */

/*
 * The cgen program reads the configurstion file and the template file and produces a header and
 * source file for the specialized types and functions.  The program produces the two files and
 * exits or prints an error message to stderr and exits.  The program allocates memory. Freeing of
 * memory is performed by the OS at program termination.  The entire cgen program is kept in this
 * source file for simplicity.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* trim removes leading and trailing whitespace and returns the trimmed string. The argument is
 * modified. str must have a null terminator */
char *trim(char *str)
{
	char *start = str;
	while (isspace(*start)) start++;

	if (*start == '\0') return start;

	char *end = start + strlen(start) - 1;
	while (isspace(*end)) end--;

	*(end + 1) = '\0';
	
	return start;
}

struct conf_info {
	char *template_file;
	char *header_file;
	char *source_file;
	char **keys;
	char **values;
	int nkeys;
};

struct conf_info parse_conf_file(char *conf_file)
{
	struct conf_info conf_info = {NULL, NULL, NULL, NULL, NULL, 0};

	FILE *file;
	if ((file = fopen(conf_file, "r")) == NULL) {
		fprintf(stderr, "The conf file %s could not be opened\n", conf_file);
		exit(1);
	}

	char line[1024];
	for (;;) {
		if (fgets(line, sizeof line, file) == NULL) break;
		if (strlen(line) >= sizeof line - 1) {
			fprintf(stderr, "The conf file has too long lines\n");
			exit(1);
		}

		char *right = line;
		char *left = strsep(&right, "=");
		if (right == NULL) continue;

		left = trim(left);
		right = trim(right);
		
//		printf("left = %s\n", left);
//		printf("right = %s\n", right);

		if (strcmp(left, "template-file") == 0) {
			conf_info.template_file = strdup(right);
		} else if (strcmp(left, "header-file") == 0) {
			conf_info.header_file = strdup(right);
		} else if (strcmp(left, "source-file") == 0) {
			conf_info.source_file = strdup(right);
		} else {
			conf_info.keys = realloc(conf_info.keys, conf_info.nkeys + 1);
			conf_info.keys[conf_info.nkeys] = strdup(left);
			conf_info.values = realloc(conf_info.values, conf_info.nkeys + 1);
			conf_info.values[conf_info.nkeys] = strdup(right);
			conf_info.nkeys++;
		}
	}

	if (!feof(file)) {
		fprintf(stderr, "There was a problem reading the conf file\n");
		exit(1);
	}

	fclose(file);
	
	return conf_info;
}










int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s conf-file\n", argv[0]);
		return 1;
	}

	struct conf_info conf_info = parse_conf_file(argv[1]);
	
	printf("Result: %s\n", conf_info.source_file);








	return 0;
}
