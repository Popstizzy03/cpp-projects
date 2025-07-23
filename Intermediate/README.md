# Intermediate C++ Projects

This directory contains 30 projects aimed at intermediate C++ programmers. These projects introduce more complex concepts, object-oriented programming, STL usage, and advanced programming patterns.

## Learning Path

The projects are organized to build upon each other, introducing increasingly sophisticated concepts:

### Object-Oriented Programming (1-6)
1. **Classes Objects** - Basic class definition, objects, and member functions
2. **Pointers** - Pointer arithmetic, memory management, and pointer operations
3. **Dynamic Memory** - new/delete, memory allocation, and memory leaks
4. **Vectors** - STL vector usage, dynamic arrays, and iterators
5. **File IO** - Reading from and writing to files, stream operations
6. **Inheritance** - Base classes, derived classes, and polymorphism

### Templates & Advanced Features (7-15)
7. **Templates** - Function templates, class templates, and generic programming
8. **Exception Handling** - try/catch blocks, custom exceptions, and error management
9. **Linked List** - Custom data structure implementation and pointer manipulation
10. **Maps** - STL map container, key-value pairs, and associative containers
11. **Sets** - STL set container, unique elements, and sorted containers
12. **Smart Pointers** - unique_ptr, shared_ptr, weak_ptr, and RAII
13. **Operator Overloading** - Custom operators, comparison operators, and stream operators
14. **Multithreading Basic** - std::thread, synchronization, and parallel execution
15. **Lambda Expressions** - Anonymous functions, closures, and functional programming

### Algorithms & Data Structures (16-20)
16. **Binary Search** - Efficient searching algorithms and complexity analysis
17. **Sorting Vectors** - Various sorting algorithms and STL sort functions
18. **String Streams** - stringstream, parsing, and string manipulation
19. **Enums** - Enumerated types, scoped enums, and type safety
20. **Function Pointers** - Function pointers, callbacks, and higher-order functions

### Advanced Concepts (21-30)
21. **Stack Implementation** - Custom stack data structure with templates
22. **Queue Implementation** - Custom queue data structure with templates
23. **Binary Tree** - Tree data structure, traversals, and recursive algorithms
24. **Regex Patterns** - Regular expressions, pattern matching, and text processing
25. **Custom Iterators** - Iterator design patterns and STL-compatible iterators
26. **Variadic Templates** - Variable argument templates and parameter packs
27. **Move Semantics** - Rvalue references, move constructors, and perfect forwarding
28. **Thread Pool** - Advanced threading concepts and task management
29. **Design Patterns** - Singleton, Observer, Factory, and Command patterns
30. **Memory Management** - Custom allocators, memory pools, and performance optimization

## Concepts Covered

### Object-Oriented Programming
- **Classes & Objects**: Encapsulation, data members, member functions
- **Inheritance**: Single inheritance, multiple inheritance, virtual functions
- **Polymorphism**: Virtual functions, abstract classes, dynamic binding
- **Encapsulation**: Access specifiers, getters/setters, data hiding

### STL (Standard Template Library)
- **Containers**: vector, map, set, queue, stack, deque
- **Iterators**: Forward, bidirectional, random access iterators
- **Algorithms**: sort, find, transform, for_each, binary_search
- **Function Objects**: Functors, lambda expressions, std::function

### Memory Management
- **Dynamic Allocation**: new/delete, malloc/free
- **Smart Pointers**: unique_ptr, shared_ptr, weak_ptr
- **RAII**: Resource Acquisition Is Initialization
- **Memory Leaks**: Detection and prevention

### Concurrency
- **Threading**: std::thread, thread synchronization
- **Synchronization**: mutex, lock_guard, condition_variable
- **Atomic Operations**: std::atomic, memory ordering
- **Parallel Algorithms**: std::async, std::future

### Modern C++ Features
- **C++11**: auto, range-based for, lambda expressions
- **C++14**: generic lambdas, variable templates
- **C++17**: if constexpr, structured bindings
- **C++20**: concepts, ranges, coroutines (where applicable)

## Prerequisites

Before starting these projects, you should be comfortable with:
- Basic C++ syntax and control structures
- Functions and basic arrays
- Compilation and debugging
- All concepts from the Beginner directory

## Compilation Instructions

Most projects require C++11 or later. Use these compilation commands:

### Windows (MinGW/Visual Studio)
```bash
g++ -std=c++17 filename.cpp -o program.exe
program.exe
```

### Linux/macOS
```bash
g++ -std=c++17 filename.cpp -o program
./program
```

### For Threading Projects
```bash
g++ -std=c++17 -pthread filename.cpp -o program
./program
```

### For Projects Using Advanced Features
```bash
g++ -std=c++20 filename.cpp -o program
./program
```

## Project Highlights

### Must-Try Projects
- **Smart Pointers** (12) - Essential for modern C++
- **Lambda Expressions** (15) - Functional programming concepts
- **Move Semantics** (27) - Performance optimization
- **Thread Pool** (28) - Advanced concurrency patterns
- **Design Patterns** (29) - Software engineering best practices

### Challenging Projects
- **Custom Iterators** (25) - Deep STL understanding
- **Variadic Templates** (26) - Advanced template programming
- **Memory Management** (30) - Performance-critical programming

## Learning Tips

1. **Read the Code**: Each project includes detailed comments
2. **Experiment**: Modify parameters and see what happens
3. **Debug**: Use debuggers and print statements
4. **Research**: Look up unfamiliar concepts online
5. **Practice**: Try implementing variations of each concept
6. **Test**: Write test cases for your modifications

## Common Pitfalls

- **Memory Leaks**: Always pair new with delete
- **Iterator Invalidation**: Be careful when modifying containers during iteration
- **Thread Safety**: Understand when synchronization is needed
- **Template Errors**: Template error messages can be cryptic
- **Performance**: Profile before optimizing

## Next Steps

After mastering these concepts, you'll be ready for the **Advanced** directory, which covers:
- Template Metaprogramming
- C++20 Concepts and Constraints
- Advanced Concurrency Patterns
- SFINAE and Type Traits
- Custom Memory Allocators
- Reflection Systems

## Additional Resources

- **Books**: "Effective Modern C++" by Scott Meyers
- **Online**: cppreference.com, learncpp.com
- **Practice**: LeetCode, HackerRank for algorithm practice
- **Communities**: Stack Overflow, Reddit r/cpp

Dive into these projects to strengthen your C++ skills and explore advanced programming concepts!