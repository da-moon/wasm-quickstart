# WASM-Guide

## Development Tools

- [WASM Explorer](https://mbebenita.github.io/WasmExplorer) : Compile **W**eb **A**ssembly **T**ext format (WAT) to Webassembly native binary file in browser.
- [Wasm Fiddle](https://wasdk.github.io/WasmFiddle) : Convert `C` and `C++` code to WAT format straight in browser and test it with built in Javascript editor.It also supports compilation to Webassembly native binary file
- [Web Assembly Studio](https://webassembly.studio) : An online IDE that supports compilation of `Rust` , `C` , `C++` and `WAT` code to Webassembly native binary file.
- [Emscripten](https://kripken.github.io/emscripten-site) : Main tool used for compiling and porting `C` and `C++` code to Webassembly native binary file.
- [Rust Web assemly](https://github.com/rustwasm/team) : Main tool used for compiling and porting `Rust` code to Webassembly native binary file.

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
      bar = wasmInstance.bar;
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
  (export "sub" (func $subtract))
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
        > `(export "sub")`
    - Add `func` keyword with the function name the you are going to define in the WAT.Make sure to put `$` behind every name that you define in WAT.
        > `(export "sub"(func $subtract))`
  
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

### Types

### Built In Functions

### Syntax

## References

[1] [Understanding WebAssembly text format](https://developer.mozilla.org/en-US/docs/WebAssembly/Understanding_the_text_format)

- [Web Assembly Studio](https://webassembly.studio)
- [Wasm Fiddle - C to Wasm](https://wasdk.github.io/WasmFiddle/)
- [Rust Web assemly](https://github.com/rustwasm/team)
- [emscripten](https://github.com/kripken/emscripten/wiki)
- [Types](https://github.com/AssemblyScript/assemblyscript/wiki/Types)
- [List of WebAssembly Built In functions](https://github.com/AssemblyScript/assemblyscript/wiki/Built-in-functions)
- [Text Format](https://developer.mozilla.org/en-US/docs/WebAssembly/Understanding_the_text_format)
- [Quick Intro](https://rsms.me/wasm-intro)
