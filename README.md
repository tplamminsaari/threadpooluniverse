# threadpooluniverse
A simple library that provides a threadpool for executing tasks in parallel. Tasks can be either inherited classes from the `threadpooluniverse::TaskBase` or lambda functions. This library has been impleted in standard C++ and does not directly use any platform specific APIs.

## License

This project has been licensed under the MIT License.

## How to build

This project uses default pretty modern C++ standard so you need build environment that supports C++17.

You also need CMake 3.15 or newer to generate the build files.

### Building in Windows

In Windows, you need Visual Studio with Desktop C++ enabled. To create the Visual Studio solution, run following CMake command in Windows command prompt:

```
cmake -A x64 -DCMAKE_GENERATOR_PLATFORM="x64" -S . -B ./build
```

This will create the  `.\build` folder from where you can find the Visual Studio solution. When you build this solution, it will create a statically linked library `threadpooluniverselib.lib` you can use to link your project with.

To run the tests, select `threadpooluniverselib_test` as your startup project and run solution by pressing F5 (assuming you have default Visual Studio keybinding). Or run following in your terminal. Change the "Debug" to "Release" if you built the project in release mode.

```
cd build\Debug
threadpooluniverselib_test.exe
```

### Building in Linux

To build this project in linux, you need the C++ tools installed. To generate the build files, run:
```
cmake -S . -B ./build
```

This will generate the Makefiles to ./build directory. To build the project, run
```
cd ./build
make
```

To run the tests:
```
cd ./build
./threadpooluniverselib_test 
```

## Using in your own project

You can use this threadpooluniverse library in your project with following mechanisms
- Build this library locally and link you project against `threadpooluniverse.lib` library and the headers.
- Use e.g. CMake's "FetchContent" feature to include this library to your build chain and build it as part of your project.
- Take the source codes and include them directly to be part of your own project and build them together with your project.

When including the sources to your own project, please notice that the source code are in `src` and in `include` directories so you must include both to your project.