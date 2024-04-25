# s21_containers

Implementation of the s21_containers.h. library.

### Part 1. Implementation of the s21_containers.h library

You need to implement the `s21_containers.h` library classes (specifications are given in the relevant material sections, see **"Main containers"**). \
List of classes: `list`, `map`, `queue`, `set`, `stack`, `vector`.
- Make it as a header file `s21_containers.h` which includes different header files with implementations of the specified containers (`s21_list.h`, `s21_map.h` and etc.); 
- Provide a Makefile for testing the library (with targets clean, test);
- The classical implementation of containers should be considered as a basis, but the final choice of implementations remains free. Except for the list - it should be implemented via the list structure rather than the array.

*Tip*: You can move the same implementation of container methods to base classes. For example, for a queue and a stack, or for a list and a vector. There is a UML diagram of the STL library in materials as *one possible example* of hierarchical construction. However, your implementation does not have to be strictly tied to this UML diagram.

### Part 2. Bonus. Implementation of the s21_containersplus.h library.

You need to implement the `s21_containersplus.h` library functions (see **"Additional containers"** for specifications). \
List of classes to be implemented additionally: `array`, `multiset`.
- Make it as a header file `s21_containersplus.h` which includes different header files with implementations of the specified containers (`s21_array.h`, `s21_multiset.h`); 
- Provide a Makefile for testing the library (with targets clean, test);
- The classical implementation of containers should be considered as a basis, but the final choice of the algorithm remains free.

### Part 3. Bonus. Implementation of the `insert_many` method.

You need to complete the classes with the appropriate methods, according to the table:

| Modifiers      | Definition                                      | Containers |
|----------------|-------------------------------------------------| -------------------------------------------|
| `iterator insert_many(const_iterator pos, Args&&... args)`          | Inserts new elements into the container directly before `pos`.  | List, Vector. |
| `void insert_many_back(Args&&... args)`          | Appends new elements to the end of the container.  | List, Vector, Queue. |
| `void insert_many_front(Args&&... args)`          | Appends new elements to the top of the container.  | List, Stack. |
| `vector<std::pair<iterator,bool>> insert_many(Args&&... args)`          | Inserts new elements into the container.  | Map, Set, Multiset. |
