# CChess

## Two in One
### Chess game and engine:
My objective for this project is to develop a chess engine capable of 
competing online against real players.

During the development, I will be bundling this engine together with
a chess GUI app to aid in testing and visualising its capabilities. 

Therefore, I will be developing both the gui and engine in parallel in this repository.

After I am pleased with its capabilities, an additional GitHub repository containing only the chess 
engine as a library will be created.

## Dependencies:
* SDL 2
* CMake 3.26
* gcc \* && C17
* Doxygen (optional)

\* other compiles can be supported if the compiler-specific flags in the main CMakeLists.txt 
are changed. Support for MSVC will be added later on.

## Cross-platform in mind
One of my objectives for this app is for it to be cross-platform across both Linux and Windows.

I am developing on Linux and I cannot yet guarantee Windows compatibility at every stage of development. 
As soon as I achieve a minimally working example, I will make sure the Windows side works as it should.

I will not promise functionality on macOS, but it might just work by itself with a bit of luck.



## Motivation:

I am doing this in order to learn and improve my skills. This project gives
me the opportunity to improve my C writing capabilities, as I have been mostly
writing C++ code until now. 

Moreover, thanks to the GUI app developed in parallel, I can also get familiar with 
SDL2.
