// compiled with https://wasdk.github.io/WasmFiddle/
char result[] = "Loaded From Webassembly Memory";
char lowercase[30];

char* getStringStartingOffset() {
  return &result[0];
}
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