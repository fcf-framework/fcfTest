# fcfTest

Provides the FCF_TEST macro to perform the check in unit tests

The library is distributed as a header file `fcfTest/test.hpp`

## Description of macros

### FCF_TEST(a_expression [, a_observedVariables ...])

The macro performs the "`(a_expression) ==true`" test, and if the result of the calculation does not satisfy the equality, the exception is called std:::runtime_error.

The text of the message contains the file name and the line number where the macro FCF_TEST was called, as well as the values of the additional variables `a_observedVariables`.

### Example

```c++
#include <fcfTest/test.hpp>
#include <vector>

int main(int a_argc, char* a_argv[]){
  std::vector<std::string> vec;
  vec.push_back("test");
  FCF_TEST(vec.size() == 2, vec.size());
  return 0;
}
```

__Output:__

```stdout
terminate called after throwing an instance of 'std::runtime_error'
  what():  Test error: vec.size() == 2  [FILE: /home/phoenix/Development/projects/fcfcpp/fcfMath/libraries/fcfBasis/test/tests/main.cpp:7]
  Values:
    vec.size(): 1

```