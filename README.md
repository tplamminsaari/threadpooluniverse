# threadpooluniverse
A simple library that provides a threadpool for executing tasks in parallel. Tasks can be either inherited classes from the `threadpooluniverse::TaskBase` or lambda functions. This library has been impleted in standard C++ and does not directly use any platform specific APIs.

## License

This project has been licensed under the MIT License.

## How to build

You need CMake 3.15 or newer to generate the build files or Visual Studio solution.

### Building in Windows

To create the Visual Studio solution, run following CMake command in Windows command prompt:

```
cmake -A x64 -DCMAKE_GENERATOR_PLATFORM="x64" -S . -B ./build
```

This will generate ./build folder where the Visual Studio solution gets created. Open `.\build\threadpooluniverselib.sln` in Visual Studio and build the entire solution. It will generate statically linked library `threadpooluniverselib.lib` you can use to link your project with. The necessary headers can be found from `.\include` folder.

### Building in Linux

## Using in your own project

You can use this threadpooluniverse library in your project with following mechanisms
- Build this library locally and link you project against `threadpooluniverse.lib` library and the headers.
- Use e.g. CMake's "FetchContent" feature to include this library to your build chain and build it as part of your project.
- Take the source codes and include them directly to be part of your own project and build them together with your project.