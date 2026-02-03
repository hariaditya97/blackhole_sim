#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <chrono>
#include <fstream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace glm;
using namespace std;
using Clock = std::chrono::high_resolution_clock;

// VARS
double lastPrintTime = 0.0;
int framesCount = 0;
double c = 299792458.0; // Speed of light in m/s
double G = 6.67430e-11; // Gravitational constant in m^3 kg^-1 s^-2
struct Ray;
bool gravity = false; 


// ---------------- Engine Struct -------------------
struct Engine {
    GLFWwindow* window;
    int WIDTH = 800;
    int HEIGHT = 600;

    bool init() {
        if (!glfwInit()) {
            cerr << "Failed to initialize GLFW\n";
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

        // Create window
        window = glfwCreateWindow(WIDTH, HEIGHT, "Black Hole Sim", nullptr, nullptr);
        if (!window) {
            cerr << "Failed to create GLFW window\n";
            glfwTerminate();
            return false;
        }
        glfwMakeContextCurrent(window);

        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            cerr << "Failed to initialize GLAD\n";
            return false;
        }

        float aspect = (float)WIDTH / HEIGHT;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-aspect, aspect, -1.0, 1.0, -1.0, 1.0);
        glMatrixMode(GL_MODELVIEW);


        return true;
    }

    void run() {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();  // reset modelview
    }

    void cleanup() {
        if (window) glfwDestroyWindow(window);
        glfwTerminate();
    }
};


// ---------------- BlackHole Struct ----------------
struct BlackHole {
    vec3 position;
    double mass;
    float radius = 0.2f; // Schwarzschild radius approximation

    BlackHole (vec3 pos, float m) : position(pos), mass(m) {} //* G * mass / (c * c);}
    
    void draw() {
        glColor3f(1.0f, 0.0f, 0.0f); // red for visibility

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(position.x, position.y); // center of circle

        for (int i = 0; i <= 360; i++) {
            float angle = i * M_PI / 180.0f;
            float x = cos(angle) * radius + position.x;
            float y = sin(angle) * radius + position.y;
            glVertex2f(x, y);
        }

        glEnd();
    }
};

// ---------------- Ray Struct ----------------
struct Ray {
    double x; double y; 
    vec2 dir;
    
    Ray(vec2 pos, vec2 d) : x(pos.x), y(pos.y), dir(d) {}

    void draw() {
        glPointSize(10.0f); 
        glColor3f(1.0f, 1.0f, 1.0f); // white color
        glBegin(GL_POINTS);
        glVertex2f(x, y);
        glEnd();
    }

    void step () {
        x += dir.x * 0.05;
        y += dir.y * 0.05;
    }

};

vector<Ray> rays;

int main() {
    Engine engine;     
    if (!engine.init()) return -1;

    for (int i = -5; i <= 5; ++i) {
        rays.push_back(Ray(vec2(-1.2f, i*0.05f), vec2(0.05f, 0.0f)));
    }
       
    // Create a black hole
    BlackHole black_hole(vec3(0.0f, 0.0f, 0.0f), 1.0e30f); // small mass for now

    // Main Loop
    while (!glfwWindowShouldClose(engine.window)) {
        engine.run();

        // Draw black hole
        black_hole.draw();

        // Draw light rays
        for (auto& ray: rays) {
            ray.draw();
            ray.step(); 
        }

        glfwSwapBuffers(engine.window);
        glfwPollEvents();
    }

    engine.cleanup();
    return 0;
}