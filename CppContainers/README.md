## About The Project

Implementation of the s21_containers.h. library.
Read `src/Task.md` to get closer with the project.

## Installation

1. Clone repository and cd to project
```sh
    git clone git@github.com:umed-hotamov/school21.git
    cd Cpp_containers
```

2. Include the necessary container header
```cpp
#include list/s21_list.h
```

## Running The Tests

```sh
make
```

## Usage

Example of using `list/s21_list.h`

```cpp
#include <iostream>
#include "list/s21_list.h"

int main() {
    s21::List<int> list;

    list.push_back(10);
    list.push_back(20);
    list.push_back(30);

    for (const auto& element : list) {
        std::cout << element << " ";
    }
    std::cout << std::endl;

    return 0;
}
```
