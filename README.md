# C++ Projects Repository

Welcome to the C++ Projects repository! This collection showcases a comprehensive variety of projects developed using C++. Each project is designed to demonstrate different programming concepts, algorithms, and techniques, ranging from beginner-friendly exercises to advanced applications showcasing cutting-edge C++ features.

This repository is organized by skill level to provide a clear progression path:

* **Beginner:** 30 projects for those new to C++ (basic syntax, control structures, functions)
* **Intermediate:** 30 projects for those with some experience in C++ (OOP, STL, data structures, concurrency)
* **Advanced:** 10 projects for advanced C++ programmers (metaprogramming, concepts, advanced patterns)

## Tech Stack

Here are some of the technologies used in this repository:

<img src="images/c++_logo.png" alt="c++ Logo" width="200"/> <img src="images/gcc_logo.png" alt="GCC Logo" width="200"/> <img src="images/codeblocks.png" alt="CodeBlocks Logo" width="230"/>

## Directory Structure

* **Beginner/**: Contains 30 projects suitable for beginners learning C++. Covers basic syntax, variables, control structures, functions, arrays, and simple problem-solving.
* **Intermediate/**: Contains 30 projects that require a solid understanding of C++ concepts. Covers OOP, STL containers, algorithms, file I/O, templates, concurrency, and design patterns.
* **Advanced/**: Contains 10 projects that showcase cutting-edge C++ techniques. Covers template metaprogramming, concepts, SFINAE, advanced concurrency, memory management, and modern C++ features.
* **Projects/**: Real-world application examples and larger projects that combine multiple concepts.

## Getting Started

To get started with these projects, you'll need a C++ compiler. Here's how to set up your environment on different operating systems:

### Windows

* **Code::Blocks:**
    * Code::Blocks is a free, open-source, cross-platform IDE that supports multiple compilers, including GCC. It provides a user-friendly interface for writing, compiling, and debugging C++ code.
    * Download and install Code::Blocks from [http://www.codeblocks.org/](http://www.codeblocks.org/).
    * Ensure that a compatible compiler (like MinGW) is included or configured within Code::Blocks.
    * Open the project files (.cpp) within Code::Blocks and use the build and run options.
* **Visual Studio:**
    * Visual Studio Community edition is a free and powerful IDE from Microsoft.
    * Download and install Visual Studio from [https://visualstudio.microsoft.com/](https://visualstudio.microsoft.com/).
    * Create a new C++ project or open existing .cpp files.
    * Use the built-in compiler and debugger.
* **Command Line (MinGW or similar):**
    * Install MinGW (Minimalist GNU for Windows) or another GCC-based compiler.
    * Add the compiler's `bin` directory to your system's PATH environment variable.
    * Open a command prompt or PowerShell window.
    * Navigate to the directory containing your .cpp files.
    * Compile using `g++ filename.cpp -o executable_name`.
    * Run the executable with `executable_name.exe`.

### Linux

* **g++ (GCC):**
    * Most Linux distributions come with GCC (GNU Compiler Collection) pre-installed. If not, install it using your distribution's package manager (e.g., `sudo apt-get install g++` on Debian/Ubuntu, `sudo dnf install gcc-c++` on Fedora/CentOS).
    * Open a terminal.
    * Navigate to the directory containing your .cpp files.
    * Compile using `g++ filename.cpp -o executable_name`.
    * Run the executable with `./executable_name`.
* **Other IDEs:**
    * Many Linux IDE's are available, such as Code::Blocks, Eclipse, and others.

### macOS

* **Xcode:**
    * Xcode is Apple's integrated development environment (IDE).
    * Install Xcode from the Mac App Store.
    * Open Xcode, create a new C++ project, or open existing .cpp files.
    * Use the built-in compiler and debugger.
* **g++ (via Homebrew):**
    * Install Homebrew ([https://brew.sh/](https://brew.sh/)).
    * Install GCC using `brew install gcc`.
    * Follow the Linux g++ instructions for compiling and running from the command line.

### Cellular (Termux on Android)

* **Termux:**
    * Termux is an Android terminal emulator and Linux environment.
    * Install Termux from the F-Droid app store or the Google Play Store.
    * Open Termux and install the necessary packages: `pkg update && pkg upgrade && pkg install clang`.
    * Navigate to the directory of your cpp file.
    * Compile using `clang++ filename.cpp -o executable_name`.
    * Run the executable using `./executable_name`.

## Contributing

Contributions are welcome! If you have a C++ project you'd like to add, please follow these guidelines:

1.  Fork the repository.
2.  Create a new branch for your project.
3.  Add your project to the appropriate directory (Beginner, Intermediate, or Advanced).
4.  Include a README file in your project directory with detailed instructions on how to compile and run the code.
5.  Submit a pull request.

## License

This repository is licensed under the [MIT License](LICENSE).
