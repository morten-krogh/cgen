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

/* A key is a string that will be replaced with value in the template */
struct key_value {
	char *key;
	char *value;
};

struct conf_info {
	char *template_file;
	char *header_file;
	char *source_file;
	struct key_value *key_values;
	size_t nkeys;
};

struct conf_info parse_conf_file(char *conf_file)
{
	struct conf_info conf_info = {NULL, NULL, NULL, NULL, 0};

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
		
		printf("left = %s\n", left);
		printf("right = %s\n", right);

		if (strcmp(left, "template-file") == 0) {
			conf_info.template_file = strdup(right);
		} else if (strcmp(left, "header-file") == 0) {
			conf_info.header_file = strdup(right);
		} else if (strcmp(left, "source-file") == 0) {
			conf_info.source_file = strdup(right);
		} else {
			conf_info.key_values = realloc(conf_info.key_values, (conf_info.nkeys + 1) * sizeof(struct key_value));
			conf_info.key_values[conf_info.nkeys].key = strdup(left);
			printf("KEY = %s\n", conf_info.key_values[conf_info.nkeys].key);
			conf_info.key_values[conf_info.nkeys].value = strdup(right);
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

char *replace_key_with_value(const char *str, const char *key, const char *value)
{


	char *result = malloc(10);



	return result;
}











enum specialization_state {
	state_none,
	state_header_file,
	state_source_file,
	state_both_files
};

void specialize_template(struct conf_info conf_info)
{
	FILE *template_file;
	if ((template_file = fopen(conf_info.template_file, "r")) == NULL) {
		fprintf(stderr, "The template file %s could not be opened\n", conf_info.template_file);
		exit(1);
	}

	FILE *header_file;
	if ((header_file = fopen(conf_info.header_file, "w")) == NULL) {
		fprintf(stderr, "The header file %s could not be opened for writing\n", conf_info.header_file);
		exit(1);
	}

	FILE *source_file;
	if ((source_file = fopen(conf_info.source_file, "w")) == NULL) {
		fprintf(stderr, "The source file %s could not be opened for writing\n", conf_info.source_file);
		exit(1);
	}

	char *header_file_line = "// cgen header file include";
	size_t header_file_line_len = strlen(header_file_line);

	char *source_file_line = "// cgen source file include";
	size_t source_file_line_len = strlen(source_file_line);

	char *both_files_line = "// cgen header file declarations and source file definitions";
	size_t both_files_line_len = strlen(both_files_line);
	
	enum specialization_state state = state_none;
	char line[8192];
	for (;;) {
		if (fgets(line, sizeof line, template_file) == NULL) break;
		if (strlen(line) >= sizeof line - 1) {
			fprintf(stderr, "The template file has too long lines\n");
			exit(1);
		}

		if (strncmp(line, header_file_line, header_file_line_len) == 0) {
			state = state_header_file;
		} else if (strncmp(line, source_file_line, source_file_line_len) == 0) {
			state = state_source_file;
		} else if (strncmp(line, both_files_line, both_files_line_len) == 0) {
			state = state_both_files;
		} else if (state == state_header_file) {
			fputs(line, header_file);
				

		} else {
			printf("state = %d\n", state);

		}

	}

	fclose(template_file);
	fclose(header_file);
	fclose(source_file);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s conf-file\n", argv[0]);
		return 1;
	}

	struct conf_info conf_info = parse_conf_file(argv[1]);
	specialize_template(conf_info);
	

	
	printf("Result: %zu\n", conf_info.nkeys);
	for (size_t i = 0; i < conf_info.nkeys; i++) {
		printf("Result: %zu, %s\n", i, conf_info.key_values[i].value);
	}







	return 0;
}
