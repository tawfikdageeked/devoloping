#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>

// ==================== STRUCTURES ====================

struct Body {
    double x, y, vx, vy, mass;
    float r, g, b, a;
    float radius;
};

struct Vertex {
    float x, y;    // NDC/normalized device coordinate pixels
    float r, g, b, a;
    float size;
};

// ==================== PHYSICS SETUP ====================

void setup_system(std::vector<Body> &system, int num_asteroids) {
    // Planets:                 x        y     vx      vy     mass         color                             rad
    system = {
        {0,        0,       0,     0,     1.989e30,  1,1,0,1,      10.f},  // Sun
        {5.79e10,  0,       0, 47360,     3.301e23,  0.66,0.66,0.66,1, 2.f},   // Mercury
        {1.082e11, 0,       0, 35020,     4.867e24,  1,0.8,0.6,1,     4.f},   // Venus
        {1.496e11, 0,       0, 29780,     5.972e24,  0,1,1,1,         4.f},   // Earth
        {2.279e11, 0,       0, 24070,     6.39e23,   1,0,0,1,         3.f},   // Mars
        {7.785e11, 0,       0, 13070,     1.898e27,  0.78,0.59,0.39,1, 8.f}   // Jupiter
    };
    srand(42);

    const double G = 6.6743e-11;
    for (int i = 0; i < num_asteroids; i++) {
        double angle = (rand() % 360) * 3.14159 / 180.0;
        double distance = 3.5e11 + (rand() % 250) * 1e9;
        double x = distance * cos(angle);
        double y = distance * sin(angle);
        double v_circ = sqrt((G * 1.989e30) / distance);
        double vx = -v_circ * sin(angle);
        double vy = v_circ * cos(angle);
        system.push_back({x, y, vx, vy, 1e15, 0.6f,0.6f,0.6f,0.3f, 1.0f});
    }
}

void step_system(std::vector<Body> &sys, double G, double dt, double softening, int steps_per_frame) {
    int N = sys.size();
    for (int step = 0; step < steps_per_frame; ++step) {
        std::vector<double> ax(N, 0.0), ay(N, 0.0);
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (i == j) continue;
                double dx = sys[j].x - sys[i].x;
                double dy = sys[j].y - sys[i].y;
                double r2 = dx*dx + dy*dy + softening*softening;
                double r = sqrt(r2);
                double accel = (G * sys[j].mass) / (r2 * r);
                ax[i] += accel * dx;
                ay[i] += accel * dy;
            }
        }
        for (int i = 0; i < N; ++i) {
            sys[i].vx += ax[i] * dt;
            sys[i].vy += ay[i] * dt;
            sys[i].x += sys[i].vx * dt;
            sys[i].y += sys[i].vy * dt;
        }
    }
}

// ==================== DATA => VERTEX BUFFER ====================

void fill_vertices(const std::vector<Body> &sys, std::vector<Vertex> &out, double scale, float x0, float y0) {
    out.resize(sys.size());
    for (size_t i = 0; i < sys.size(); ++i) {
        out[i].x = float(sys[i].x / scale + x0);  // to pixel coordinates
        out[i].y = float(sys[i].y / scale + y0);  //
        out[i].r = sys[i].r;
        out[i].g = sys[i].g;
        out[i].b = sys[i].b;
        out[i].a = sys[i].a;
        out[i].size = sys[i].radius;
    }
}

// ==================== SHADERS ====================

// VERTEX SHADER: pass through position/color/size, map to OpenGL coord
const char* vs = R"(
#version 330 core
layout (location=0) in vec2 pos;
layout (location=1) in vec4 color;
layout (location=2) in float size;
out vec4 vColor;
out float vSize;
uniform float winw, winh;
void main() {
    // Map from [0,winw], [0,winh] to OpenGL [-1,1] xy
    float x = 2.0 * pos.x / winw - 1.0;
    float y = 2.0 * pos.y / winh - 1.0;
    gl_Position = vec4(x,y,0,1);
    gl_PointSize = size * 2.5; // scale: bigger for Sun/Jupiter
    vColor = color;
    vSize = size;
}
)";

// FRAGMENT SHADER: draw circles with smooth edge ("glow")
const char* fs = R"(
#version 330 core
in vec4 vColor;
in float vSize;
out vec4 FragColor;
void main() {
    // gl_PointCoord: [0,1] across point (fragment location in point sprite)
    float dx = gl_PointCoord.x - 0.5;
    float dy = gl_PointCoord.y - 0.5;
    float dist = sqrt(dx*dx + dy*dy);
    float edge = smoothstep(0.48, 0.5, dist);
    FragColor = vec4(vColor.rgb, vColor.a * (1.0 - edge));
}
)";

// ======= GLFW error callback =======
void glfw_errcb(int code, const char* msg) {
    std::cerr << "GLFW error: " << msg << std::endl;
}
// ==================== MAIN ====================

int main() {
    glfwSetErrorCallback(glfw_errcb);

    if (!glfwInit()) {
        std::cerr << "GLFW failed.\n";
        return 1;
    }

    int winw = 720, winh = 480;
    GLFWwindow* w = glfwCreateWindow(winw, winh, "CPU Melter N-Body (OpenGL)", nullptr, nullptr);
    if (!w) { std::cerr << "glfwCreateWindow failed\n"; glfwTerminate(); return 1; }
    glfwMakeContextCurrent(w);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) { std::cerr << "glew failed\n"; return 1; }
    glViewport(0,0,winw,winh);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ======= Compile Shaders =======
    auto assert_shader = [](GLuint sh) {
        int ok=0; glGetShaderiv(sh,GL_COMPILE_STATUS,&ok);
        if(!ok) {
            char log[512]; glGetShaderInfoLog(sh, 512, 0, log);
            std::cerr<<"Shader error:\n"<<log<<std::endl; exit(1);
        }
    };
    GLuint vsh = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vsh,1, &vs, nullptr); glCompileShader(vsh); assert_shader(vsh);
    GLuint fsh = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fsh,1,&fs, nullptr); glCompileShader(fsh); assert_shader(fsh);
    GLuint prog = glCreateProgram();
    glAttachShader(prog, vsh); glAttachShader(prog, fsh); glLinkProgram(prog);
    glDeleteShader(vsh); glDeleteShader(fsh);
    glUseProgram(prog);

    GLuint winw_uni = glGetUniformLocation(prog, "winw");
    GLuint winh_uni = glGetUniformLocation(prog, "winh");
    glUniform1f(winw_uni, float(winw));
    glUniform1f(winh_uni, float(winh));

    // ======= Set up VAO/VBO for points =======
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1,&vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // layout: x,y, r,g,b,a, size

    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,x));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,r));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,1,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,size));
    glEnableVertexAttribArray(2);

    std::vector<Body> system;
    int num_asteroids = 100;
    setup_system(system, num_asteroids);

    double G = 6.6743e-11, dt = 500;
    int steps_per_frame = 20;
    double softening = 5e8;
    double scale = 2.5e9;

    float x0 = winw/2.0f, y0 = winh/2.0f;

    std::vector<Vertex> vbuf(system.size());

    int frame = 0;

    while (!glfwWindowShouldClose(w)) {
        glfwPollEvents();

        // --- Run CPU sim ---
        step_system(system, G, dt, softening, steps_per_frame);

        // --- Fill vertex buffer (screen position/color/radius) ---
        fill_vertices(system, vbuf, scale, x0, y0);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vbuf.size()*sizeof(Vertex), vbuf.data(), GL_STREAM_DRAW);

        glClearColor(0,0,0,1);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog);
        glUniform1f(winw_uni, float(winw));
        glUniform1f(winh_uni, float(winh));
        glBindVertexArray(vao);
        glDrawArrays(GL_POINTS, 0, vbuf.size());

        glfwSwapBuffers(w);

        if (frame++ % 60 == 0) {
            std::cout<<"Crunching "<<system.size()<<" bodies..."<<std::endl;
        }
    }

    glDeleteVertexArrays(1,&vao); glDeleteBuffers(1,&vbo);
    glDeleteProgram(prog);
    glfwDestroyWindow(w); glfwTerminate();
    return 0;
}