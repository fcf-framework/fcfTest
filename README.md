
# fcfTest

**fcfTest** is a lightweight, header-only unit testing framework for C++. It provides a simple macro, FCF_TEST, which implements all possible checks and displays the current values ​​of variables. The framework also includes test registration, command-line execution, a built-in simple logger, and tools for measuring execution time.

The library is distributed as a single header file: `fcfTest/test.hpp`.

To use the library, you must define the `FCF_TEST_IMPLEMENTATION` macro before including the header file in your application's main cpp file to avoid errors due to multiple definitions.

## Quick Start Example

```c++
#define FCF_TEST_IMPLEMENTATION
#include <fcfTest/test.hpp>
#include <vector>

// Declare a test case
FCF_TEST_DECLARE("MyLibraryPartName", "ExamplesGroupName", "VectorSizeTestName"){
  std::vector<std::string> vec;
  vec.push_back("test");

  // Perform an assertion. If false, throws std::runtime_error with details.
  FCF_TEST(vec.size() == 2, vec.size());
}

int main(int a_argc, char* a_argv[]){
  // Run the test suite via command line interface
  bool error;
  fcf::NTest::cmdRun(a_argc, (const char**)a_argv, fcf::NTest::CRM_RUN, &error);
  return error ? 1 : 0;
}
```

**Output:**
The program will execute the registered tests. If an assertion fails, it prints a descriptive error including file, line number, and variable values. If successful, it prints a summary count of completed tests.

 ```stdout
Performing the test: "MyLibraryPartName" -> "ExamplesGroupName" -> "VectorSizeTestName" ...
Comparison started
Test error: vec.size() == 2  [FILE: DIR_PATH/main.cpp:9]
  Values:
    vec.size(): 1
```

## Core Macros

### `FCF_TEST_IMPLEMENTATION`

This macro is used to enable the implementation section within the header file `fcfTest/test.hpp`.
- **Usage**: Must be defined (`#define FCF_TEST_IMPLEMENTATION`) before including the header if you want to use the library's functionality (e.g., running tests, using the logger).
- **Purpose**: Prevents multiple definition errors by ensuring that global variables and function definitions are only generated once per compilation unit. When omitted, the header provides only declarations.

### `FCF_TEST_EXPORT`

This macro is used to export symbols from the test library when building it as a DLL or shared library.
- **Usage**: Defined by the main FCF library macros (`FCF_EXPORT`) if applicable, otherwise empty.
- **Purpose**: Ensures consistent symbol visibility for functions and variables defined in `fcfTest` when compiled with `/FD` (DLL) settings on Windows or similar export flags on Linux/macOS.

### `FCF_TEST_IMPORT`

This macro is used to import symbols from the test library when using it as a client application linking against a shared library.
- **Usage**: Defined by the main FCF library macros (`FCF_IMPORT`) if applicable, otherwise empty.
- **Purpose**: Ensures that functions and variables defined in external shared libraries are correctly imported into the client process without multiple definition errors.

### `FCF_TEST`

The primary macro for performing checks in unit tests.
- **Behavior**: Evaluates `(a_expression)`. If the result is false (non-zero), it throws a `std::runtime_error`.
- **Error Message**: The exception message includes:
  - The failing expression (`#exp`).
  - The file name and line number where the macro was called.
  - Values of all additional arguments provided in `a_observedVariables`.

**Example:**
```c++
int x = 5;
FCF_TEST(x == 4, x);
// Throws error: Test error: x == 4 [FILE: main.cpp:10]
// Values:
//    x: 5
```

## Testing Organization Macros

Tests are organized hierarchically into Parts, Groups, and Tests. This allows for filtering execution based on these levels.

### `FCF_TEST_DECLARE(am_part, am_group, am_test)`

Declares a new test case.
- **Parameters**:
  - `am_part`: The name of the logical part (highest level).
  - `am_group`: The name of the sub-group within the part.
  - `am_test`: The unique identifier for the test function.
- **Usage**: This macro generates a static anonymous class that registers the test with the global storage upon instantiation.

**Example:**
```c++
FCF_TEST_DECLARE("Network", "HTTP", "GetRequestTest") {
    // Test implementation
}
FCF_TEST_DECLARE("Network", "HTTP", "PostRequestTest") {
    // Test implementation
}
```

### `FCF_TEST_PART_ORDER(am_part, am_order)`

Registers the execution order for a specific part.
- **Parameters**:
  - `am_part`: Name of the part.
  - `am_order`: Integer priority (lower values run first).

**Example:**
```c++
FCF_TEST_PART_ORDER("CriticalTests", 1); // Run critical tests first
FCF_TEST_PART_ORDER("GeneralTests", 2);
```

### `FCF_TEST_GROUP_ORDER(am_group, am_order)`

Registers the execution order for a specific group within a part.
- **Parameters**:
  - `am_group`: Name of the group.
  - `am_order`: Integer priority.

**Example:**
```c++
FCF_TEST_GROUP_ORDER("Integration", 1);
FCF_TEST_GROUP_ORDER("Unit", 2);
```

### `FCF_TEST_TEST_ORDER(am_test, am_order)`

Registers the execution order for a specific test within a group.
- **Parameters**:
  - `am_test`: Name of the test.
  - `am_order`: Integer priority.

**Example:**
```c++
FCF_TEST_TEST_ORDER("SmokeTest", 1);
FCF_TEST_TEST_ORDER("LoadTest", 2);
```

## Global Logging Functions

The library provides global shortcuts to access streams based on log levels. These are defined in the `fcf::NTest` namespace and also available globally if included directly or via specific exports.

### `ftl()`, `err()`, `wrn()`, `att()`, `log()`, `inf()`, `dbg()`, `trc()`

These functions return references to output streams (`std::ostream&`) corresponding to specific log levels:
- `ftl()`: Fatal level.
- `err()`: Error level.
- `wrn()`: Warning level.
- `att()`: Attention level.
- `log()`: Log level (default).
- `inf()`: Information level.
- `dbg()`: Debug level.
- `trc()`: Trace level.

If the global logger's level is set lower than the requested level, these return a reference to an empty stream buffer (no output).

**Example:**
```c++
fcf::NTest::ftl() << "This will only appear if log level is FTL or higher" << std::endl;
fcf::NTest::dbg() << "This appears only in debug mode" << std::endl;
```

## Benchmarking: `Duration` Class

The `fcf::NTest::Duration` class provides a simple interface for measuring the execution time of code blocks. It uses `std::chrono::high_resolution_clock`.

### Members and Methods
- **Constructor**: `Duration(unsigned long long a_iterations)` sets the number of iterations. Default is 1.
- **Methods**:
  - `void begin()`: Records start time.
  - `void end()`: Records end time.
  - `std::chrono::nanoseconds totalDuration()`: Returns elapsed time for all iterations.
  - `std::chrono::nanoseconds duration()`: Returns average time per iteration.
- **Operator Call**: `operator()(TFunctor&& a_functor)` executes the functor `_iterations` times, timing the block automatically via `begin()` and `end()`.

**Example:**
```c++
// Measure sorting 1000 times
fcf::NTest::Duration bench(1000);
bench([](){
    std::vector<int> v = {5, 2, 9};
    std::sort(v.begin(), v.end());
});

std::cout << "Total: " << bench.totalDuration().count() << " ns\n";
std::cout << "Avg: " << bench.duration().count() << " ns\n";
```

## Command Line Interface

The test runner supports filtering tests by Part, Group, or specific Test name, and setting the global log level. It uses a command-line argument parser implemented in `cmdRun`.

### The `Options` Structure

This structure holds the configuration for running tests. It is populated automatically by the command line parser but can be used manually.

```c++
struct Options {
  std::vector<std::string> parts;     ///< List of part names to run (empty = all).
  std::vector<std::string> groups;    ///< List of group names to run (empty = all).
  std::vector<std::string> tests;     ///< List of specific test names to run (empty = all).
  std::string              logLevel;  ///< Desired logging level ("off", "err", "log", etc.).
};
```

### The `cmdRun` Function

The central function for executing the test suite. It parses command-line arguments and determines the action.

#### `CmdRunMode` Enum

This enum dictates how `cmdRun` behaves when parsing arguments:

- **`CRM_PARSE`**: Parses arguments but **does not** execute tests or show help/list. Returns the determined mode (e.g., `CM_HELP`, `CM_LIST`) so the caller can decide what to do next. Useful for building a custom menu system.
- **`CRM_EXECUTE`**: Parses arguments. If the flag `--test-run` was provided, it executes tests. If `--test-help` or `--test-list` were provided, it displays that information immediately and returns.
- **`CRM_RUN`**: Parses arguments and **automatically executes** tests unless `--test-help` or `--test-list` is explicitly requested. This is the standard mode for running tests directly from the command line (equivalent: `./my_tests --test-run`).

#### Usage Examples

```c++
int main(int a_argc, char* a_argv[]) {
    // Standard execution: Parse and run if requested via flags
    bool error = false;
    fcf::NTest::cmdRun(a_argc, (const char**)a_argv, fcf::NTest::CRM_RUN, &error);
    return error ? 1 : 0;
}
```

```c++
int main(int a_argc, char* a_argv[]) {
    // Or custom menu mode: Just parse arguments to see what was asked
    fcf::NTest::Options options;
    int mode = fcf::NTest::cmdRun(options, a_argc, (const char**)a_argv, fcf::NTest::CRM_PARSE);
    if (mode == fcf::NTest::CM_HELP) {
        fcf::NTest::cmdHelp();
        return 0;
    } else if (mode == fcf::NTest::CM_LIST) {
        fcf::NTest::cmdList();
        return 0;
    } else if (mode == fcf::NTest::CM_RUN) {
        bool error = false;
        fcf::NTest::run(options, &error);
        return error ? 1 : 0;
    }
    // ... default execution
}
```

### Command Line Flags

- `--test-run`: Forces execution of selected tests.
- `--test-list`: Prints a list of all available tests in the hierarchy (Part -> Group -> Test).
- `--test-help`: Displays usage information and available flags.
- `--test-log-level LEVEL`: Sets the global logging verbosity (e.g., `dbg`, `log`, `err`).
- `--test-part PART_NAME`: Filters execution to only tests belonging to the specified part. Can be used multiple times.
- `--test-group GROUP_NAME`: Filters execution to only tests belonging to the specified group. Can be used multiple times.
- `--test-test TEST_NAME`: Filters execution to run only the specific test named. Can be used multiple times.

**Example Command:**
```bash
./my_tests --test-run --test-part Network --test-group HTTP --test-log-level dbg
```

### Helper Functions

- **`cmdHelp()`**: Displays help information and available command-line flags.
- **`cmdList()`**: Displays a list of all registered tests with their hierarchy structure.
- **`run(const Options& a_options, bool* a_errorPtr = 0)`**: Executes the selected tests based on an `Options` object. Usually called internally by `cmdRun`. If a null pointer is passed, the function throws an exception when an error occurs.

## Full Usage Example

```c++
#define FCF_TEST_IMPLEMENTATION
#include <fcfTest/test.hpp>
#include <vector>
#include <cmath>

// --- Test Declarations ---

FCF_TEST_DECLARE("Math", "BasicArithmetic", "Addition") {
    int a = 2;
    int b = 3;
    FCF_TEST(a + b == 5, a, b);
}

FCF_TEST_DECLARE("Math", "BasicArithmetic", "Subtraction") {
    int a = 10;
    int b = 4;
    FCF_TEST(a - b == 6, a, b);
}

FCF_TEST_DECLARE("Vector", "SizeCheck", "EmptyVector") {
    std::vector<int> v;
    FCF_TEST(v.size() == 0, v.size());
}

// --- Order Registration ---
// Run Math tests before Vector tests
FCF_TEST_PART_ORDER("Math", 1);
FCF_TEST_PART_ORDER("Vector", 2);

// Run "BasicArithmetic" group first within Math part
FCF_TEST_GROUP_ORDER("BasicArithmetic", 1);

// Run Addition test first
FCF_TEST_TEST_ORDER("Addition", 1);

int main(int a_argc, char* a_argv[]) {
    // Use CRM_RUN for standard execution
    bool error;
    fcf::NTest::cmdRun(a_argc, (const char**)a_argv, fcf::NTest::CRM_RUN, &error);
    return error ? 1 : 0;
}
```


