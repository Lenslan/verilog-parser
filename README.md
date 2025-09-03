
# Verilog Parser

## todos
- 为啥解析port 位宽总是带着后面的符号，透了！！
  <img width="2221" height="805" alt="image" src="https://github.com/user-attachments/assets/d5e10dd6-96bb-4c1c-bc9a-6c1f0e81c5ad" />


## Getting Started

This will get you going workspace wise.

```sh
$> make all
$> make test-all
```

This will download the test suite files, setup the build directory, and
compile the parser, library and test app.

To start using the parser in your own code, take a look at 
[main.c](./src/main.c) which is a simple demonstration app used for testing
and coverage. The basic code which you need is something like this:

```C
// Initialise the parser.
verilog_parser_init();

// Open A File handle to read data in.
FILE * fh = fopen("my_verilog_file.v", "r");

// Parse the file and store the result.
int result = verilog_parse_file(fh);

if(result == 0)
    printf("Parse successful\n");
else
    printf("Parse failed\n");

fclose(fh);
```

You can keep calling `verilog_parse_file(fh)` on as many different file
handles as you like to build up a multi-file project AST representation.
The parser will automatically follow any `include` directives it finds.

For an example of using the library in a real*ish* situation, the
[verilog-dot](https://github.com/ben-marshall/verilog-dot) project shows how
the library can be integrated into an existing project and used.

