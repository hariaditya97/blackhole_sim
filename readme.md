# Code to run the application 
clang++ -std=c++17 main.cpp glad/src/glad.c \
-Iglad/include \
-I/opt/homebrew/Cellar/glfw/3.4/include \
-I/opt/homebrew/include \
-L/opt/homebrew/Cellar/glfw/3.4/lib -lglfw \
-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo \
-o BlackHoleSim

