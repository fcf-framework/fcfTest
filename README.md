
# fcfTest

Provides the FCF_TEST macro to perform the check in unit tests

The library is distributed as a header file `fcfTest/test.hpp`

### Example

```c++
#define FCF_TEST_IMPLEMENTATION
#include <fcfTest/test.hpp>
#include <vector>

FCF_TEST_DECLARE("MyLibraryPartName", "ExamplesGroupName", "VectorSizeTestName"){
  std::vector<std::string> vec;
  vec.push_back("test");
  fcf::NTest::att() << "Comparison started" << std::endl;
  FCF_TEST(vec.size() == 2, vec.size());
}

int main(int a_argc, char* a_argv[]){
  fcf::NTest::cmdRun(a_argc, (const char**)a_argv, fcf::NTest::CRM_RUN); 
  return 0;
}
```

__Output:__

```stdout
Performing the test: "MyLibraryPartName" -> "ExamplesGroupName" -> "VectorSizeTestName" ...
Comparison started
terminate called after throwing an instance of 'std::runtime_error'
  what():  Test error: vec.size() == 2  [FILE: DIR_PATH/main.cpp:9]
  Values:
    vec.size(): 1
```

## Description of macros

### FCF_TEST(a_expression [, a_observedVariables ...])

The macro performs the "`(a_expression) ==true`" test, and if the result of the calculation does not satisfy the equality, the exception is called std::runtime_error.

The text of the message contains the file name and the line number where the macro FCF_TEST was called, as well as the values of the additional variables `a_observedVariables`.

