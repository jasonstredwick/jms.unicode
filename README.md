# jms.unicode
A C++ library for a subset of unicode functionality. (current c++23)

# How to build
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
./bin/code_gen.exec --root ..
```
