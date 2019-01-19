parseropt
====

Simple cmd options parser (C++11)

## Features 
- [x] One function per option.
- [x] Tiny, self-contained, header-only.
- [x] Compatibility is good.
- [x] Use free.

## Example
```c++
#include <iostream>
#include <string>
#include "parseropt.hpp"
int main() {
    bool help = parseropt(false, "-h", "--help", "-?");
    int version = parseropt(0, "-v", "--version", "--show-version");
    std::string name = parseropt("", "-u", "--user", "--user-name");
    std::string pass = parseropt("", "-p", "--pass", "--passwprd");
    // [...]
    std::cout << help << ',' << version << std::endl;
    std::cout << name << ',' << pass << std::endl;
}
```

```shell
#> g++ demo.cpp -o demo -std=c++11
#> ./demo -u nine -p dreams
#> ./demo -h
#> ./demo -v
```

## Changelog
- v1.0 : Initial version
