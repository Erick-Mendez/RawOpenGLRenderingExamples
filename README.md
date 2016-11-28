# "Raw" OpenGL Rendering Examples

This repository shows a set of minimal examples of how to do rendering with OpenGL. Most examples use a hard coded cube with a hard coded pose and projection. The target audience of this repo are developers who are familiar with OpenGL and just want to get a minimal version of what they already know how to write, but dont have the time or patience to type again. I call them "raw" because of the lack of dependencies or internally created libraries.

## Goals of this repo
- There are no external libraries needed except one for window management (currently SDL but trivially exchangeable to others, for example glfw)
- Every example is a *single .cpp file*
- Every example is self contained and does not depend on previous examples
- There are no classes that abstract away common functionalities (like loading shaders)
- There are no external header files that hide away nuisances
- There is a minimal set of custom functions, most is contained within the main function
- Hard coded geometry, no loading of OBJ files, textures or similar
- Comments only when necessary and highlighting larger logical blocks
- Verbose variable names
- It builds on *multiple platforms*, currently targeting Ubuntu Linux, macOS and Windows 10
- It builds with multiple compilers, currently targeting gcc, clang, msbuild
- Built with cmake
- Minimal usage of ifdefs
- No dependencies to the STL
- No command line parameters needed
- *Minimal error checking*. Yes, you read that right, minimal error checking. The code in here should be correct but will not be very resistance to things going severely wrong (like running out of memory). Users of this library know that a lot of more error checking is needed, for example, the result of shader compilation, but just want to see the bare minimum in a working state

## What you will *not* get from this repo:
- Interaction examples. There are no examples that will show how to rotate geometry or change the viewpoint
- Math examples. For example, how to generate a projection matrix or how to compute the model view matrix
- Libraries or a Framework. These are "Raw examples"
- Reusable code among the examples
- Mobile examples. Officially this code is not checked with Android or iOS, but it should be trivial to port it
- Exhaustive checking. I cant stress this enough, you need to take care of error checking yourself

## Building
The repo is built using cmake. Make sure you have SDL2 and Cmake installed. General steps for building on any platform are:
- Create a 'build' folder
- Go inside the folder
- Run `cmake ..`
- Your project files have been generated, you can now build as usual

## Examples
These are the current examples:

|    Example     | Shows                                    | How it should look like                |
|----------------|------------------------------------------|----------------------------------------|
|FlatColorsCube  | A cube with one flat color |![FlatColorsCube][FlatColorsCubeImg]    |
|VertexColorsCube| A cube with vertex coloring|![VertexColorsCube][VertexColorsCubeImg]|
|TexturedCube    | A cube with texturing      |![TexturedCube][TexturedCubeImg]|


[FlatColorsCubeImg]: ./images/FlatColorsCube.png "FlatColorsCube"
[VertexColorsCubeImg]: ./images/VertexColorsCube.png "VertexColorsCube"
[TexturedCubeImg]: ./images/TexturedCube.png "TexturedCube"
