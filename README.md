Volant Engine
=============

The engine is still in development and not currently usable.
To build use: gcc -Wall -o volant main.c -lm -lGL -lGLEW -lglfw shade.c vmath.c mesh.c log.c
Should work on any linux with opengl3+, you will probably need to install GLFW3
The makefile is not yet in use.

You can contribute using pull requests or email me, please follow the guidelines below.

Use this_kind_of_variable for everything in the engine, this will allow code built on top of the engine to freely use camelCase.
Use _leading_underscores for private variables and functions.
Use plain C and GLSL only!
Do not use any libraries besides GLFW3 and GLEW.
Put { on new lines.
Don't typedef structs unless there is a good practical reason.
Don't define macros for speed at this stage, this sort of optimization can always be done later.
Try not to use deprecated OpenGL and GLSL functionality, there is a lot of outdated information out there so be wary.

Have Fun!
