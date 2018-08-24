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
