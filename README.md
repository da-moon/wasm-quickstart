# WASM-Guide
- [WASM-Guide](#wasm-guide)
    - [Development Tools](#development-tools)
    - [Simple Environment Setup](#simple-environment-setup)
        - [Lesson 1 - **W**eb **A**ssembly **T**ext format (WAT)](#lesson-1---web-assembly-text-format-wat)
        - [Lesson 2 - `C` to `WASM`](#lesson-2---c-to-wasm)
    - [Lesson 3 - Passing Functions from Javascript to Webassembly](#lesson-3---passing-functions-from-javascript-to-webassembly)
    - [Lesson 4 - Reading and Writing Webassembly Memory from Javascript](#lesson-4---reading-and-writing-webassembly-memory-from-javascript)
    - [References](#references)
## Development Tools

- [WASM Explorer](https://mbebenita.github.io/WasmExplorer) : Compile **W**eb **A**ssembly **T**ext format (WAT) to Webassembly native binary file in browser.
- [Wasm Fiddle](https://wasdk.github.io/WasmFiddle) : Convert `C` and `C++` code to WAT format straight in browser and test it with built in Javascript editor.It also supports compilation to Webassembly native binary file
- [Web Assembly Studio](https://webassembly.studio) : An online IDE that supports compilation of `Rust` , `C` , `C++` and `WAT` code to Webassembly native binary file.

## Simple Environment Setup

There are multiple ways to test and run Webassembly code. For the purpose of this instruction, we will use an online Webassembly compiler to get Webassembly binary file and and import it to a HTML file.

We need a webserver to see the result and interact with the compiled binary in web browser based on `Chrome`. We use `Python simple web server` by invoking the following command in `HTML` files directory.

```shell
python -m http.server 8000
npm install -g http-server
```

Keep in mind that the HTML file that imports the Webassembly binary has to be named `index.html`

The following is the standard HTML template for loading a `WASM` file from a URL:

```html
<!doctype html>
  <title>WASM Testing Environment</title>
  <script>
    function FetchAndInstantiateWasmFromAddress (url, imports) {
      return fetch(url)
      .then(result => {
        if (result.ok)
          return result.arrayBuffer();
        throw new Error(`Unable to fetch Web Assembly file ${url}.`);
      })
      .then(wasmByteArray => WebAssembly.compile(wasmByteArray))
      .then(wasmModule => WebAssembly.instantiate(wasmModule, imports || {}))
      .then(wasmInstance => wasmInstance.exports);
    }
    // load <foo> function with a Wasm Binary stored in the webserver directory ...
    FetchAndInstantiateWasmFromAddress('./foo.wasm')
    .then(wasmInstance => {
      window.foo = wasmInstance.foo;
    });
    // load <bar> function with a Wasm Binary stored in a remote server ...
    FetchAndInstantiateWasmFromAddress('https://someaddress.com/bar.wasm')
    .then(wasmInstance => {
      window.bar = wasmInstance.bar;
    });
  </script>
```

### Lesson 1 - **W**eb **A**ssembly **T**ext format (WAT)

the fundamental unit of code in WebAssembly is a module.  In the text format, a module is represented as one big S-expression.

S-expressions are textual format for representing trees. We can think of a module as a tree of nodes that describe the module’s structure and its code.  

Unlike the Abstract Syntax Tree of a programming language, though, WebAssembly’s tree is pretty flat, mostly consisting of lists of instructions.[1](#references)

The following is a simple module that uses a built in Webassembly function:

```wasm
(module
  (export "subWASM" (func $subtract))
  (
    func $subtract
        (param $arg1 i32) (param $arg2 i32)
        (result i32)
            get_local $arg1
            get_local $arg2
            i32.sub
    )
)
```

to break down the module:

1. start by writing `(module)`
2. use `export` keyword to make it accessible for getting called in Javascript.I prefer to export functions before declaring them so that In do not forget exporting at the end!

    - Start by writing `(export)`.
    - Add the function name that gets used in Javascript.
        > `(export "subWASM")`
    - Add `func` keyword with the function name the you are going to define in the WAT.Make sure to put `$` behind every name that you define in WAT.
        > `(export "subWASM"(func $subtract))`
  
3. Define function signature

    - Start by writing function's name after `func` keyword.
        > `func $subtract`
    - Define the function's input parameters. use keyword `param` then append parametersname `$<some name>` and then append parameters type `i32`.
        > `(param $arg1 i32) (param $arg2 i32)`
    - write the return type after `result` keyword.
        > `(result i32)`

    the total function signature will look like :

    ```wasm
    (
        func $subtract
        (param $arg1 i32) (param $arg2 i32)
        (result i32)
    )
    ```
4. Add the function's body to the function signature

For more information on WAT Syntax, visit the following :

1. [Mozilla Guide](https://developer.mozilla.org/en-US/docs/WebAssembly/Understanding_the_text_format)
2. [https://github.com/WebAssembly/design/blob/master/Semantics.md](https://github.com/WebAssembly/design/blob/master/Semantics.md)

as you can see, WAT syntax is not developer friendly. There are two major projects that tries to address this:

- [Emscripten](https://kripken.github.io/emscripten-site) : Main tool used for compiling and porting `C` and `C++` code to Webassembly native binary file.
- [Rust Web assembly](https://github.com/rustwasm/team) : Main tool used for compiling and porting `Rust` code to Webassembly native binary file.

>Use chocolatey package manager to install Rust compiler if you are behind Huawei's corporate proxy. Input the following in a Powershell window with admin privilledges : `choco install -y rust`

### Lesson 2 - `C` to `WASM`

Thanks to [emscripten](https://github.com/kripken/emscripten/wiki), `C` and `C++` code can get compiled to native WASM binary.

As an example, we use [Webassembly Studio](https://wasdk.github.io/WasmFiddle)to write an compile a simple function that calculates factorial.

```c
int factorialWASM(int n) {
  if (n==0)
    return 1;
  else if (n==1)
    return 1;
  else
    return n*factorialWASM(n-1);
}
```

it would be interensting to see how the above operation would look in WAT. We will use [Wasm Fiddle](https://wasdk.github.io/WasmFiddle/) to generate WAT file.

```wasm
(module
 (table 0 anyfunc)
 (memory $0 1)
 (export "memory" (memory $0))
 (export "factorialWASM" (func $factorialWASM))
 (func $factorialWASM (; 0 ;) (param $0 i32) (result i32)
  (local $1 i32)
  (set_local $1
   (i32.const 1)
  )
  (block $label$0
   (br_if $label$0
    (i32.lt_u
     (get_local $0)
     (i32.const 2)
    )
   )
   (set_local $1
    (i32.const 1)
   )
   (loop $label$1
    (set_local $1
     (i32.mul
      (get_local $0)
      (get_local $1)
     )
    )
    (br_if $label$1
     (i32.gt_u
      (tee_local $0
       (i32.add
        (get_local $0)
        (i32.const -1)
       )
      )
      (i32.const 1)
     )
    )
   )
  )
  (get_local $1)
 )
)
```

quite complex , eh ?! ...

## Lesson 3 - Passing Functions from Javascript to Webassembly

Passing functions defined/available in javascript file to Webassembly is easy. All one has to do is to define a function prototype in `C` in which it's signature matches the Javascript function. For instance, we want to pass a Factorial function that was defined in the javascript file and also call console.log through webassembly to put the result on console. First, we define function prototypes:

```c
int JSFactorial(int input);
void ConsoleLog(int input);
```

Function prototypes are automatically treated as external function in the compiler. By default, These external function can receive value from module `env`.

At this point, we just pass the functions defined in Javascript by instanticating a new `env` object passing it to
`FetchAndInstantiateWasmFromAddress()` :

```javascript
FetchAndInstantiateWasmFromAddress('./main.wasm', {
      env: {
        JSFactorial: input => console.log(input),
        ConsoleLog:input => console.log(input),
      }
    })
```

## Lesson 4 - Reading and Writing Webassembly Memory from Javascript

As you might have notice, up until now, we have only worked with integers. We know that webassembly has very limited support for different data type (eg `integer` and `float`). We cannot return more complex types, for instance `String`, from our `C` code that is getting compiled to WASM binary. What can we do if we want to access string in Webassembly memory and show it on our web page?

To address this issue, we have to access memory directly and access the values through pointers.Lets work on a simple program that prints `Loaded From Webassembly Memory` string which was defined in C file.

```c
char result[] = "Loaded From Webassembly Memory";
```

we define a function that returns the memory pointer to the start of `result` array.

```c
char* getStringStartingOffset() {
  return &result[0];
}
```

the compiler automatically exports a `memory` object that we can access and read from.
after calling `FetchAndInstantiateWasmFromAddress()` function, we store the memory in a constant variable.  

```javascript
  FetchAndInstantiateWasmFromAddress('./main.wasm')
    .then(wasmInstance => {
      const memory = wasmInstance.memory;
      });
```

the way we access memory in javascript is through typed arrays. We need to create a typed array on top of the loaded Webassembly memory that represent our string. Since each character of an string is `8 bits` , we initialise a `Uint8Array` in our code by passing the following argument to it's constructor :

- `memory.buffer` so that it can access Webassembly memory.
- String offset starting point through calling `getStringStartingOffset()` that we defined in our webassembly code.
- String length, in our case `30`

```javascript
  FetchAndInstantiateWasmFromAddress('./main.wasm')
    .then(wasmInstance => {
        const memory = wasmInstance.memory;
        const stringBuffer = new Uint8Array(memory.buffer,          wasmInstance.getStringStartingOffset(), 30);
      });
```

Now, to convert this `raw buffer` to a `string` , we use `TextDecoder()`

```javascript
  FetchAndInstantiateWasmFromAddress('./main.wasm')
    .then(wasmInstance => {
        const memory = wasmInstance.memory;
        const stringBuffer = new Uint8Array(memory.buffer,          wasmInstance.getStringStartingOffset(), 30);
        const result = new TextDecoder().decode(stringBuffer);
      });
```

Now, lets address writing to web assembly memory.
for example, lets turn a string into it's lower case form.

First things first, to write to Webassembly memory, we create another buffer that holds the same values as the following string : `LOADED FROM WEBASSEMBLY MEMORY`.

now , lets make some changes in the `C` file.
First, lets add another string that is going to get used to hold the lowercased string.

```c
char lowercase[30];
```

lets define a function that lowercases and stores the changes in the memory.

```c
char* toLowerCase () {
  for (int i = 0; i < 30; i++) {
    char temp = result[i];
    if (temp > 64 && temp < 91) {
      temp = temp + 32;
    }
    lowercase[i] = temp;
  }
  return &lowercase[0];
}
```

> Caution : There is a lot of overhead costs when writing to Webassembly memory so it is recommended to first think about were each piece of data should belong.

## References

[1] [Understanding WebAssembly text format](https://developer.mozilla.org/en-US/docs/WebAssembly/Understanding_the_text_format)