# DSA01 Assignment 5 – CO5
Register number: 192524372
Student name: [ENTER YOUR NAME]
Compiler: g++ (record your installed version)

This archive is organized by assignment task:
- TaskA1–TaskA5: small C++ demonstrations supporting the Part A design concepts and output-prediction questions.
- TaskB1–TaskB4: implementation programs for Part B.

Important: Part A also requires written design artefacts (UML/vtable/exception diagrams, SIMATS pseudocode, explanations and complexity analysis). C++ files do not replace those report requirements. Part B additionally requires your own test tables, measured timings, screenshots, sanitizer reports and interpretation. Do not invent measured results.

## Compile
From the extracted project root, for example:
```powershell
g++ -std=c++17 -Wall -Wextra TaskB1/main.cpp -o TaskB1/main.exe
.\TaskB1/main.exe
```
Use the same pattern for each `main.cpp`.

For sanitizer builds (where supported):
```powershell
g++ -std=c++17 -Wall -Wextra -fsanitize=address -g TaskB2/main.cpp -o TaskB2/asan.exe
.\TaskB2/asan.exe
```

For B3 performance measurements, use optimization:
```powershell
g++ -std=c++17 -O2 -Wall -Wextra TaskB3/main.cpp -o TaskB3/benchmark.exe
.\TaskB3/benchmark.exe 1000
.\TaskB3/benchmark.exe 1000000
.\TaskB3/benchmark.exe 10000000
```

The exact compiler version and all timings must be filled in from your own machine. Some task formulas and examples are illustrative where the PDF does not prescribe a specific business rule.
