# CChess [DEVELOPMENT ON PAUSE]

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

### A project with a backstory:
It is a bit funny, but I actually am not a big fan of Chess. I am doing this because a few years back
I promised a friend from highschool that I will make a chess program that will finally be capable of beating him.

I tried to create one at that time in C++ but as I was not experienced with app development or bigger projects I
gave up when I realised that my whole design was actually heavily depending on Undefined Behavior. 

As most experiences with UB go, the app was apparently working as expected for quite some time until abruptly it
wasn't anymore. 

So I could say that this project is my revenge against UB.
