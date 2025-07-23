# Advanced C++ Projects

This directory contains 10 projects for advanced C++ developers. These projects challenge your understanding of C++ and showcase cutting-edge features, advanced programming techniques, and expert-level concepts that push the boundaries of what's possible with modern C++.

## Learning Path

These projects represent the pinnacle of C++ programming, covering the most sophisticated features:

### Expert-Level Concepts (1-10)
1. **Factorial Metaprogramming** - Compile-time computations and template recursion
2. **Coroutine Generator** - C++20 coroutines, co_yield, and asynchronous programming
3. **Template Metaprogramming** - Advanced compile-time programming and type manipulation
4. **Concepts Constraints** - C++20 concepts, type constraints, and template specialization
5. **Advanced Concurrency** - Lock-free programming, memory ordering, and parallel patterns
6. **Memory Pools** - Custom allocators, memory management optimization, and performance
7. **SFINAE Type Traits** - Substitution Failure Is Not An Error and advanced type introspection
8. **Async Programming** - Advanced futures, promises, and asynchronous task management
9. **Reflection System** - Runtime type information and dynamic programming capabilities
10. **Custom Containers** - STL-compatible containers with advanced iterator support

## Concepts Covered

### Template Metaprogramming
- **Compile-time Computation**: Constexpr functions, template recursion
- **Type Manipulation**: Type traits, SFINAE, enable_if
- **Template Specialization**: Partial specialization, tag dispatching
- **Variadic Templates**: Parameter packs, fold expressions
- **CRTP**: Curiously Recurring Template Pattern

### C++20 Modern Features
- **Concepts**: Type constraints, requires clauses, concept composition
- **Coroutines**: co_await, co_yield, co_return, generator patterns
- **Ranges**: Range adaptors, view compositions, pipeline operations
- **Modules**: Import/export, module partitions (where supported)

### Advanced Concurrency
- **Lock-free Programming**: Atomic operations, memory ordering
- **Thread Pools**: Task scheduling, work stealing algorithms
- **Parallel Patterns**: Producer-consumer, readers-writers, barriers
- **Memory Models**: Sequential consistency, acquire-release semantics
- **Coroutine-based Async**: Stackless coroutines, async generators

### Memory Management Mastery
- **Custom Allocators**: Pool allocators, stack allocators, linear allocators
- **Memory Optimization**: Small object optimization, memory alignment
- **RAII Advanced**: Resource management, exception safety guarantees
- **Performance Analysis**: Memory profiling, allocation patterns

### Type System Exploitation
- **SFINAE**: Enable/disable based on type properties
- **Type Traits**: Custom traits, detection idioms
- **Template Argument Deduction**: Perfect forwarding, universal references
- **Concept Design**: Semantic requirements, subsumption

### Reflection and Introspection
- **Runtime Type Info**: Dynamic type discovery, method invocation
- **Serialization**: Automatic serialization based on type information
- **Dynamic Dispatch**: Virtual method tables, type erasure
- **Meta-object Protocol**: Compile-time reflection emulation

## Prerequisites

These projects assume mastery of:
- All Beginner and Intermediate concepts
- Modern C++ standards (C++11 through C++20)
- Template programming and generic design
- Concurrency and threading
- STL internals and design patterns
- Performance optimization techniques
- Computer science fundamentals (algorithms, data structures)

## Compilation Requirements

These projects require cutting-edge compiler support:

### Recommended Compilers
- **GCC 10+** (for C++20 features)
- **Clang 11+** (for concepts and coroutines)
- **MSVC 19.29+** (Visual Studio 2019 16.11+)

### Compilation Commands

#### For Most Projects (C++17)
```bash
g++ -std=c++17 -O2 filename.cpp -o program
```

#### For C++20 Features
```bash
g++ -std=c++20 -O2 filename.cpp -o program
```

#### For Coroutines (GCC)
```bash
g++ -std=c++20 -fcoroutines -O2 filename.cpp -o program
```

#### For Threading Projects
```bash
g++ -std=c++20 -pthread -O2 filename.cpp -o program
```

#### Debug Build with Sanitizers
```bash
g++ -std=c++20 -g -fsanitize=address,thread filename.cpp -o program
```

## Project Deep Dive

### 🔥 Must-Master Projects

#### **Template Metaprogramming** (03)
- Compile-time factorial and Fibonacci
- Custom type traits and type manipulation
- Conditional compilation with if constexpr
- Performance comparison: compile-time vs runtime

#### **Concepts Constraints** (04)
- C++20 concepts for type safety
- Custom concept definitions
- Concept composition and subsumption
- Template constraint debugging

#### **Advanced Concurrency** (05)
- Producer-consumer with condition variables
- Readers-writers locks with shared_mutex
- Lock-free data structures with atomics
- Parallel algorithms and thread barriers

#### **Memory Pools** (06)
- Custom memory pool allocator
- Stack and linear allocators
- Performance benchmarking
- Memory usage optimization

### 🚀 Expert-Level Challenges

#### **SFINAE Type Traits** (07)
- Substitution Failure Is Not An Error
- Detection idioms and void_t
- Custom type traits for containers
- Template argument deduction guides

#### **Reflection System** (09)
- Runtime type information system
- Dynamic property access and method invocation
- Automatic serialization/deserialization
- Generic object manipulation

#### **Custom Containers** (10)
- STL-compatible dynamic array
- Circular buffer with iterator support
- Hash map with open addressing
- Binary search tree with custom iterators

## Learning Strategy

### Phase 1: Foundation (Projects 1-3)
- Master template metaprogramming basics
- Understand compile-time computation
- Learn advanced template techniques

### Phase 2: Modern C++ (Projects 4-5)
- Adopt C++20 concepts and constraints
- Master advanced concurrency patterns
- Understand lock-free programming

### Phase 3: Optimization (Projects 6-8)
- Custom memory management
- SFINAE and type manipulation
- Asynchronous programming patterns

### Phase 4: Architecture (Projects 9-10)
- Runtime reflection systems
- Custom container design
- Performance-critical programming

## Performance Considerations

- **Compile-time vs Runtime**: When to use constexpr and templates
- **Memory Allocation**: Custom allocators for specific use cases
- **Lock-free Design**: When and how to avoid synchronization
- **Template Instantiation**: Managing compile times and binary size
- **Cache Efficiency**: Memory layout and access patterns

## Debugging Advanced Code

### Tools and Techniques
- **Compiler Explorer**: Analyze generated assembly
- **Valgrind**: Memory error detection
- **Perf**: Performance profiling
- **GDB**: Advanced debugging with pretty printers
- **Template Instantiation**: -ftemplate-depth, -ftemplate-backtrace-limit

### Common Issues
- **Template Errors**: Cryptic error messages and debugging strategies
- **Linker Errors**: Template instantiation and ODR violations
- **Runtime Crashes**: Memory corruption in custom allocators
- **Performance Regression**: Unexpected allocations and copies

## Industry Applications

These techniques are used in:
- **Game Engines**: Memory pools, custom allocators
- **High-Frequency Trading**: Lock-free data structures
- **Database Systems**: Custom memory management
- **Graphics Programming**: Template metaprogramming for shaders
- **Embedded Systems**: Compile-time optimization
- **Scientific Computing**: Parallel algorithms, custom containers

## Beyond This Repository

### Next Level Learning
- **Books**: "C++ Templates" by Vandevoorde, "Concurrency in Action" by Williams
- **Conferences**: CppCon, Meeting C++, Pacific C++
- **Open Source**: Contribute to Boost, LLVM, or major C++ projects
- **Specialization**: Graphics programming, systems programming, embedded

### Career Paths
- **Core C++ Developer**: Language features, standard library
- **Performance Engineer**: Optimization, profiling, architecture
- **Systems Programmer**: Operating systems, embedded systems
- **Technical Lead**: Mentoring, architecture decisions

## Warning ⚠️

These projects are extremely advanced and can be dangerous if misused:
- **Memory Management**: Can cause crashes and security vulnerabilities
- **Concurrency**: Race conditions and deadlocks are possible
- **Template Metaprogramming**: Can cause exponential compile times
- **Undefined Behavior**: Easy to write incorrect code that appears to work

Always test thoroughly and understand the implications of advanced techniques.

## Final Words

Mastering these projects represents the pinnacle of C++ expertise. You'll join the ranks of library writers, language implementers, and performance engineers who push the boundaries of what's possible with C++.

These skills are rare, valuable, and in high demand in the industry. Use them wisely and responsibly.

Welcome to the cutting edge of C++ programming! 🚀