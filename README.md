# cgen

cgen is a small C program that can expand a template into a C header file and a C source file.
cgen could mean c generator, c generics, code generator or just cgen.

Besides the cgen program, this repository contain a directory of templates. At the moment there is just one template, a vector also known as a dynamic array. The plan is to include more templates.

cgen is an open source project licensed with the MIT license. I encourage others to join in order to improve the template format, and add templates for more data structures and algorithms.

# Motivation

The C language is a great language. However, it has some drawbacks. One drawback is the lack of statically typed data structures and algorithms.

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

Several options are available to C developers.

### First approach

The first approach in C is to allocate a large array to start with

```
struct customer customers[10000];
struct product products[50000];
```
This approach is bad because the program either wastes memory or must exit at some point.

### Second approach

A second approach is to litter the code with malloc/realloc logic

```
if (i >= capacity) {
  customers = realloc(customers, 2 * capacity);
}
customers[i] = customer;
```

All the allocation code must be repeated for the products and all other future items types.

### Third approach

A third approach is to define a general data structure based on void**
```
void **customers;
void **products;
```
This approach will avoid the code duplication. However, lots of casts must be performed and static typing by the compiler will not be able to catch errors as easily. Also casts are ugly in my opinion. Furthermore, the arrays do not contain the objects themselves but just pointers. This leads to larger memory consumption, and worse cache utilization.

### Fourth approach

A fourth approach is to code all the container logic in preprocessor macros. Code to insert a customer

```
INSERT_CUSTOMER(customer, customers)
```

would be a macro defined in some header.

In my opinion, macros reduce code readability. But more importantly, the code is expanded and compiled multiple times. At every code use, and at every code change, the macros must be expanded and compiled. The resulting binary executable becomes larger than necessary, and development time becomes slower.

### Fifth approach

A fifth approach is to close the eyes and write more or less identical code for a vector of custmoers, a vector of products, etc. This approach works quite well if done properly. However, the error rate is high due to sloppy copy/paste and changes in one version of the code must be updated manually everywhere else. It is also boring to write and maintain such code which increases the error rate.

### Sixth approach















For instance,


To be filled out

A template file	is divided into	four sections delimited by special lines of the form "<cgen some text>".
A delimiter must be the only	text on	      a line besides white space.

## Section 1

# Section 2


Section 1 starts at the top of the template file and goes to. It is used for
free text comments. This section will be ignored by the cgen program.



Section 2 starts after a line containing exactly "<cgen template names>"  . This section specifies
the names that will be subsituted by the cgen program. Each name must ber specified on its own line. Blank lines are ignored.

Section 3 starts after a line

The first section, which is the current section, can be used for commenting
thew template file.
