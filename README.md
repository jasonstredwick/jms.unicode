# jms.unicode
A C++ library for a subset of unicode functionality. (current c++23)

# How to build
Downloading data files - this required to run testing and code generation
```
mkdir build; cd build
cmake ..
cmake --build data
```


Testing
```
mkdir build; cd build
cmake ..
cmake --build tests
ctest
```


Code Generation
```
mkdir build; cd build
cmake ..
cmake --build code_gen
./bin/code_gen.exe --help
# run all; make sure your data files are in {repo_root}/data
./bin/code_gen.exe --root ..

# example of generating two specific files
./bin/code_gen.exe --root .. DerivedBidiClass BidiBrackets
```


Overall example for running all options
```
mkdir build; cd build
cmake ..
cmake --build data
cmake --build code_gen
./bin/code_gen.exe --root ..
cmake --build tests
ctest
```
