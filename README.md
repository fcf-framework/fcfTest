# fcfTest

**fcfTest** is a lightweight, header-only unit testing framework for C++. It provides a simple macro, FCF_TEST, which implements all possible checks and displays the current values ​​of variables. The framework also includes test registration, command-line execution, a built-in simple logger, and tools for measuring execution time.

The library is distributed as a single header file: `fcfTest/test.hpp`.

To use the library, you must define the `FCF_TEST_IMPLEMENTATION` macro before including the header file in your application's main cpp file to avoid errors due to multiple definitions.

### Liniks:
Usage article on dev.to: https://dev.to/vladimirm/fcftest-unit-test-library-3cgg

### Friendly:
If something doesn't work for you, please report it quickly in Issues and we'll fix it!

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
  fcf::NTest::cmdRun(a_argc, a_argv, fcf::NTest::CRM_RUN, &error);
  return error ? 1 : 0;
}
```

**Output:**
The program will execute the registered tests. If an assertion fails, it prints a descriptive error including file, line number and variable values. If successful, it prints a summary count of completed tests.

 ```stdout
Performing the test: "MyLibraryPartName" -> "ExamplesGroupName" -> "VectorSizeTestName" ...
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
- **Purpose**: Ensures consistent symbol visibility for functions and variables defined in `fcfTest` when compiled as a DLL.

### `FCF_TEST_IMPORT`

This macro is used to import symbols from the test library when using it as a client application linking against a shared library.
- **Usage**: Defined by the main FCF library macros (`FCF_IMPORT`) if applicable, otherwise empty.
- **Purpose**: Ensures consistent symbol visibility for functions and variables defined in external shared libraries when they are imported into the client process without multiple definition errors.

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
  - `am_part`: The name of the part (logical grouping level).
  - `am_group`: The name of the group (sub-grouping level).
  - `am_test`: The unique identifier of the test function.
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
- **Constructor**: `Duration(unsigned long long a_iterations)`
  - **Parameters**:
    - `a_iterations`: The number of times the enclosed functor will be executed.
- **Default Constructor**: `Duration()`
  - Sets the number of iterations to 1.
- **Methods**:
  - `unsigned long long iterations()`: Returns the number of iterations set for this duration.
  - `void begin()`: Records the start time for timing.
  - `void end()`: Records the end time for timing.
  - `std::chrono::nanoseconds totalDuration()`: Returns the total duration of all iterations in nanoseconds.
  - `std::chrono::nanoseconds duration()`: Returns the average duration of a single iteration in nanoseconds.
  - `void operator()(TFunctor&& a_functor)`: Executes a functor multiple times and measures the total duration.
    - **Parameters**:
      - `a_functor`: The callable object to execute.

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
  std::vector<std::string> parts;         ///< List of part names to run (empty means all).
  std::vector<std::string> groups;        ///< List of group names to run (empty means all).
  std::vector<std::string> tests;         ///< List of specific test names to run (empty means all).
  std::vector<std::string> ignoreParts;   ///< List of ignore part names.
  std::vector<std::string> ignoreGroups;  ///< List of ignore group names to run.
  std::vector<std::string> ignoreTests;   ///< List of ignore specific test names to run.
  std::string              logLevel;      ///< Desired logging level.
};
```

### The `cmdRun` Function

The central function for executing the test suite. It parses command-line arguments and determines the action.

**`ECmdMode cmdRun(Options& a_dstOptions, int a_argc, const char* const* a_argv, ECmdRunMode a_runMode, bool* a_errorPtr = 0)`**

Parses command line arguments and executes the appropriate action.

- **Parameters**:
  - `a_dstOptions`: Reference to the `Options` structure to populate with parsed arguments.
  - `a_argc`: Number of command line arguments.
  - `a_argv`: Array of argument strings.
  - `a_runMode`: Current mode of execution (`CRM_PARSE`, `CRM_EXECUTE`, or `CRM_RUN`).
  - `a_errorPtr`: A pointer to a variable that receives information about a test error. If an error occurs, the value is set to true. If a null pointer is passed, the function throws an exception.
- **Returns**: `ECmdMode` returns the selected mode based on the parameters of the command line a_argv.

#### `ECmdRunMode` Enum

This enum dictates how `cmdRun` behaves during parsing:

- **`CRM_PARSE`**: Parses arguments but **does not** execute tests or show help/list. Returns the determined mode (e.g., `CM_HELP`, `CM_LIST`) so the caller can decide what to do next.
- **`CRM_EXECUTE`**: Parses arguments. If the flag `--test-run` was provided, it executes tests. If `--test-help` or `--test-list` were provided, it displays that information immediately and returns.
- **`CRM_RUN`**: Parses arguments and **automatically executes** tests unless `--test-help` or `--test-list` was explicitly requested.

#### Usage Examples

```c++
int main(int a_argc, char* a_argv[]) {
    // Standard execution: Parse and run.
    bool error = false;
    fcf::NTest::cmdRun(a_argc, a_argv, fcf::NTest::CRM_RUN, &error);
    return error ? 1 : 0;
}
```

```c++
int main(int a_argc, char* a_argv[]) {
    // Standard execution: Parse and run by request.
    bool error = false;
    int mode = fcf::NTest::cmdRun(a_argc, a_argv, fcf::NTest::CRM_EXECUTE, &error);
    if (error) {
        // An error occurred while running a test.
        return 1;
    }
    if (mode != fcf::NTest::CM_NONE){
        // The --test-run | --test-help | ---test-list flags were passed at startup.
        // And the function performed all the actions
        return 0;
    }

    .... your application code ...

    return 0;
}
```

```c++
int main(int a_argc, char* a_argv[]) {
    // Or custom menu mode: Just parse to see what was asked
    fcf::NTest::Options options;
    int mode = fcf::NTest::cmdRun(options, a_argc, a_argv, fcf::NTest::CRM_PARSE);
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
- `--test-ignore-part PART_NAME`: Exclude tests in the specified part(s). Can be used multiple times.
- `--test-ignore-group GROUP_NAME`: Exclude tests in the specified group(s). Can be used multiple times.
- `--test-ignore-test TEST_NAME`: Exclude the specified test(s). Can be used multiple times.


**Example Command:**
```bash
./my_tests --test-run --test-part Network --test-group HTTP --test-log-level dbg
```

### Helper Functions

- **`cmdHelp()`**: Displays help information and available command-line flags.
- **`cmdList()`**: Displays a list of all registered tests with their hierarchy structure.
- **`run(const Options& a_options, bool* a_errorPtr = 0)`**: Executes the selected tests based on an `Options` object.
  - **Parameters**:
    - `a_options`: Configuration options specifying which tests to run and logging level.
    - `a_errorPtr`: A pointer to a variable receiving error information. If an error occurs, the value is set to true. If a null pointer is passed, the function throws an exception.
  - **Returns**: void.

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
    fcf::NTest::cmdRun(a_argc, a_argv, fcf::NTest::CRM_RUN, &error);
    return error ? 1 : 0;
}
```


