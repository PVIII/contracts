# contracts
[![Build Status](https://travis-ci.com/PVIII/contracts.svg?branch=master)](https://travis-ci.com/PVIII/contracts)

This is a library for contract-based programming. It tries to mimic the capabilities of the upcoming C++20-feature for [contracts](https://en.cppreference.com/w/cpp/language/attributes/contract).

## Getting Started

This is a header-only library. It should compile with any C++14-capable compiler.

### Installing

You can just download or clone the library and set your include directory to `include`.

If you use [Conan](https://conan.io/) you can add the Conan remote
```
conan remote add <REMOTE> https://api.bintray.com/conan/pviii/conan
```
and reference the package `contracts/x.x.x@pviii/stable` as a dependency.

## Built With

* [CMake](https://cmake.org/) - Build System (Generator)
* [Conan](https://conan.io/) - Dependency Management
* [Bintray](https://bintray.com) - Package Hosting

## Versioning

This package is versioned using [SemVer](http://semver.org/).

## License

This project is licensed under the MIT License - see the [LICENSE.txt](LICENSE.txt) file for details.

