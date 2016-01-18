# cgen

cgen is a small C program that can expand a template into a C header file and a C source file.
cgen could mean c generator, c generics, code generator or just cgen.

Besides the cgen program, this repository contain a directory of templates. At the moment there is just one template, a vector also known as a dynamic array. The plan is to include more templates.

cgen is an open source project licensed with the MIT license. I encourage others to join in order to improve the template format, and add templates for more data structures and algorithms.

# Motivation



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

> Hello
How are you

xdkdjkdsjk

*italic*
**bold**

* item 1
* item 2

dkldkdsl

- item a
- item b

fkjfkjf

1. item 1.
2. item 2.

dkdkjdf

```
struct foo {
  int a;  
  int b;
}
```
[amberbio](www.amberbio.com)
