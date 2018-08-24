// compiled with https://wasdk.github.io/WasmFiddle/
int JSFactorial(int input);
void ConsoleLog(int input);

int factorialWASM(int n) { 
  int result = JSFactorial(n);
  return result;
}
void WriteOnJSCOnsole(int n){
  ConsoleLog(n);
}