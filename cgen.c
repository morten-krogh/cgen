/*
 * The MIT License (MIT)
 * Copyright (c) 2016 Morten Krogh
 */

/*
 * The cgen program is called from the command line with one argument, the configuration file cgen
 * configuration-file-name
 *
 * The configuration file specifies the template file name, the output header file name, the output source file name, and the key-value pairs.
 * 
 * The cgen program works by first parsing the configuration file followed by parsing the template file and writing to the header and source files.
 * If successful, cgen termimnates silently and produces the header and spurce files. If unsuccessful, cgen outputs an error message to stderr and terminates.
 * In the case of failure the two output files will be partially written and can be ignored.
 *
 * cgen allocates and frees memory. Some memroy will only be released at program termination. 
 * The entire cgen source code is kept in this source file for simplicity.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

const char *std_header = "/* This file is generated by the cgen program, https://github.com/morten-krogh/cgen. The cgen program is released under the MIT license.*/\n";

/* trim removes leading and trailing whitespace and returns the trimmed string. The argument string is modified. str must have a null terminator */
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

struct conf_info parse_conf_file(const char *conf_file)
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
		
		if (strcmp(left, "template-file") == 0) {
			conf_info.template_file = strdup(right);
		} else if (strcmp(left, "header-file") == 0) {
			conf_info.header_file = strdup(right);
		} else if (strcmp(left, "source-file") == 0) {
			conf_info.source_file = strdup(right);
		} else {
			conf_info.key_values = realloc(conf_info.key_values, (conf_info.nkeys + 1) * sizeof(struct key_value));
			conf_info.key_values[conf_info.nkeys].key = strdup(left);
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

/* replace_key_with_value returns an allocated string. The caller owns the return string. */
char *replace_key_with_value(const char *str, const char *key, const char *value)
{
	size_t key_len = strlen(key);
	if (key_len == 0) return strdup(str);

	int count_of_key = 0;
	const char *occurence = str;
	while ((occurence = strstr(occurence, key)) != NULL) {
		count_of_key++;
		occurence += key_len;
	}

	size_t value_len = strlen(value);
	size_t result_len = strlen(str) + count_of_key * (value_len - key_len);
	char *result = malloc(result_len + 1);
	
	const char *src = str;
	char *dst = result;
	while ((occurence = strstr(src, key)) != NULL) {
		strncpy(dst, src, occurence - src);
		dst += occurence - src;
		strcpy(dst, value);
		dst += value_len;
		src = occurence + key_len;		
	}
	strcpy(dst, src);

	return result;
}

/* replace_keys_with_values returns an allocated string. The caller owns the return string. */
char *replace_all_keys_with_values(const char *str, const struct key_value *key_values, size_t nkeys)
{
	if (nkeys == 0) return strdup(str);
	char *current_str = replace_key_with_value(str, key_values[0].key, key_values[0].value);
	for (size_t i = 1; i < nkeys; i++) {
		char *next_str = replace_key_with_value(current_str, key_values[i].key, key_values[i].value);
		free(current_str);
		current_str = next_str;
	}
	return current_str;
}

/* function_declaration makes a simple check whether line is the first line in a function This
 * function does not parse c syntax. It uses very simple rules.  To be a valid function declaration,
 * line must start with a non-space character and end with ")" or ") {" besides white space.  This function
 * requires that the templates conform to this convention. This means in particular that the openng
 * brace { must be on its own line.  If the line is a declaration, this function returns the line
 * with an appended ";", otherwise it returns NULL.
 */ 
char *function_declaration(const char *line)
{
	size_t line_len = strlen(line);
	if (line_len < 5 || isspace(*line) || *line == '{') return NULL;
	const char *last = line + line_len - 1;
	while (isspace(*last)) last--;
	if (*last == '{') last--;
	while (isspace(*last)) last--;
	if (*last == ')') {
		char *result = malloc(last - line + 4);
		strncpy(result, line, last - line);
		strcpy(result + (last - line), ");\n");
		return result;
	}
	return NULL;
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

	fputs(std_header, header_file);
	fputs(std_header, source_file);
	fprintf(source_file, "\n#include \"%s\"\n", conf_info.header_file);
	
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
		} else if (state != state_none) {
			char *expanded_line = replace_all_keys_with_values(line, conf_info.key_values, conf_info.nkeys);
			if (state == state_header_file) {
				fputs(expanded_line, header_file);
			} else if (state == state_source_file) {
				fputs(expanded_line, source_file);
			} else {
				assert(state == state_both_files);
				fputs(expanded_line, source_file);

				char *decl = function_declaration(expanded_line);
				if (decl != NULL) {
					fputs(decl, header_file);
					free(decl);
				}
			}
			free(expanded_line);
		}
	}

	fclose(template_file);
	fclose(header_file);
	fclose(source_file);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		fprintf(stderr, "Usage: %s conf-file\n", argv[0]);
		return 1;
	}

	struct conf_info conf_info = parse_conf_file(argv[1]);
	specialize_template(conf_info);
	
	return 0;
}
