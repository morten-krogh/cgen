# cgen

cgen is a small C program that can expand a template into a C header file and a C source file.  cgen
could mean c generator, c generics, code generator or just cgen.

Besides the cgen program, this repository contain a directory of templates. At the moment there is
just one template, a vector also known as a dynamic array. The plan is to include more templates.

cgen is an open source project licensed with the MIT license. I encourage others to join in order to
improve the template format, and add templates for more data structures and algorithms.

# Installation

The cgen program is a single C program, cgen.c. 

make (produces the executable cgen)
make install (copies cgen to /usr/local. requires root access)

cgen can be installed anywhere by a simple copy.

# Directories

The templates are located in the templates directory. See the vector directory for an example.


# Usage

Go to a template directory such as templates/vector.

Type

cgen conf-file-name

or 

../../cgen conf-file-name

The conf-file-name is the file name of the configuration file described below.
The cgen program will produce two output files or exit with an error message to std error.
The two output files are a .h and .c file. Their names are specified in the configuration file.

#### Example

cd templates/vector
../../cgen vector_int.conf

These two files will be produced 

vector_int.h
vector_int.c

# Configuration file format

A typical configuration file looks like this

```
template = vector.template.c
header = vector_int.h
source = vector_int.c

NAME = int                               
TYPE = int
```

A configuration file consists of lines of the form

```
key = value
```

Lines without `=` are ignored. There are three special keys: 

1. `template` which is the file path of the input template file
2.  `header` which is the file path of the output `.h` file.
3.  `source` which is the file path of the output `.c` file.


All other key, value pairs are used for substitutions in the template file. In the example above the words
`NAME` and `TYPE` will be replaced by `int` in the template file. 


# Template file format

A template file	is divided into	three sections delimited by special lines of the form `// cgen header` and `// cgen source`.

The upper part of the template file is interpreted as cgen comments by the cgen program.
Text after `// cgen header` is interpreted as header file code, and text after `// cgen source` is interpreted as source file code.

See the file `templates/vector/vector.template.c` for an example.

C comments have no special meaning to cgen. Template files can hence include C comments. The only exception are lines starting with `// cgen header` and `// cgen source`.

The reason the vector template file,`templates/vector/vector.template.c`, has the extension `.c` is that template files can be made syntactically correct C files. This makes it easy to write template files using a C editor;
indentation and syntax coloring will be correct. Also, template files can be syntactically verified by a C compiler

```cc -fsyntax-only template-file```

In order to make the template file valid C two things must be done. Firstly the cgen comment section
should be written as C comments as well. Secondly, unknown types in the header and source sections
should be typedefed in the cgen comment section outside c comments. See the
`templates/vector/vector.template.c` as an example and note the line

```
typedef int TYPE;
```

without a C comment. This line never makes it to the output header and source files because it is inside the cgen comment section. 

The special cgen delimiters `// cgen header` and `// cgen source` are designed to be C comments exactly in order to make it possible for a template file to be valid C.  
 
 
# Functionality

```
cgen configuration-file
```

executes the cgen program. The following steps are taken by cgen.

 	1. cgen parses the configuration file. The three special keys are found and all other key, value pairs are stored. 
 	2. cgen parses the template file whose path was given in the configuration file.
 	3. The cgen comment section is ignored. 
 	4. The cgen header section is parsed, substitutions are made using the key, value pairs from the configuration file, and the output is written to the header file specified in the configuration file.
 	5. The cgen source section is parsed, substitutions are made using the key, value pairs from the configuration file, and the output is written to the source file specified in the configuration file.
	6. The header file is included in the source file as `#include header-file`.
	7. In case of errors, cgen exits and writes an error message to stderr. The output files could be incomplete in cases of errors and should be discarded.

The resulting header and source files are ready to be used in another program after possibly some project specific `#include`. The header file is already included in the source file. 


# Motivation

The C language is a great language. However, it has some drawbacks. One drawback is the lack of
statically typed data structures and algorithms.

Suppose for example a program has a type

```
struct customer {
  unsigned long id;
  char name[32];
  int age;
}
```

The program might need a container of customers.

Later, the program might get a product type

```
struct product {
  unsigned long id;
  char name[128];
  double price;
}
```

and need a container of products.

Several approaches are available to C developers.

### Static allocation of a large array

The first approach in C is to allocate a large array to start with

```
struct customer customers[10000];
struct product products[50000];
```
The problem is that there is no natural choice of the size of the array.
The program either wastes memory or crashes at some point.

### Malloc/realloc in user code

A second approach is to litter the code with malloc/realloc logic

```
if (i >= capacity) {
  customers = realloc(customers, 2 * capacity);
}
customers[i] = customer;
```

All the container logic must be repeated for the products and other items types.

### void* data structure

A third approach is to define a general data structure based on void**

```
struct vector {
	void* data;
	size_t size;
	size_t capacity;
};

insert_item(struct vector vector, void* item);
```

This approach will avoid the code duplication. However, lots of casts must be performed and static
typing by the compiler will not be able to catch many errors. Also casts are ugly in our
opinion. Furthermore, the arrays do not contain the objects themselves but just pointers. This leads
to larger memory consumption, and worse cache utilization.

### Preprocessor macros

A fourth approach is to code all the container logic in preprocessor macros. Code to insert a customer

```
INSERT_CUSTOMER(customer, customers)
```

would be a macro defined in some header.

In our opinion, macros reduce code readability. But more importantly, the code is expanded and compiled multiple times. At every code use, and at every code change, the macros must be expanded and compiled. The resulting binary executable becomes larger than necessary, and development time becomes slower. Debugging might also become harder.

### Manual code duplication

A fifth approach is to close the eyes and write more or less identical code for a vector of
customers, a vector of products, etc. This approach works quite well if done properly. However, the
error rate is high due to sloppy copy/pasting. Also, changes in one part of the container code must
be updated manually everywhere else. It is also boring to write and maintain such code which
increases the error rate.

### Automatic code generation

A sixth approach is to use an automatic code generator to achieve the same output as the fifth
approach. This approach is the one taken by cgen.

The biggest disadvantage is that another layer in the development process is needed. However, cgen is so simple that this should not be a major problem.

In basically every other way, code generation approach is superior.

Compile times are shorter. In most projects, the container logic is updated rarely whereas the user
code, such as the vectors of customers and products, are updated regularly. There is no reason to
expand and recompile the container logic again and again.

Changes in the container and algorithm code just needs to be done once.

The code is easier to read. There are no macros or special template syntax in the actual program.

There is no memory hit by indirection.

The compiler can statically check the code and casts are avoided.


# Comparison with other programming languages

High level languages, like Javascript and Python, use an approach similar to the void* data
structure described above.  The arrays and other containers use indirection and type tags. This
approach is the easiest for development purposes. The only downside is loss of performance and
higher memory consumption.

Languages with generics or templates, like C++, use an approach similar to preprocessor macros. The
compiled code is very efficient. The downsides are enhanced compilation times, and a more
complicated language. In our opinion the preprocessor or template handler should not be tightly
coupled to the main language.
