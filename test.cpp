#define GLEW_STATIC
// #define GLM_FORCE_RADIANS
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <Shar\sharfunc.h>

// #include <glm/vec3.hpp> // glm::vec3
// #include <glm/vec4.hpp> // glm::vec4, glm::ivec4
// #include <glm/mat4x4.hpp> // glm::mat4
// #include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
// #include <glm/gtc/type_ptr.hpp> // glm::value_ptr


using namespace std;

int index = 0;
point4 vertecis[8] = {
    point4(-0.5, -0.5, -0.5, 1.0), point4( 0.5, -0.5, -0.5, 1.0),
    point4( 0.5,  0.5, -0.5, 1.0), point4(-0.5,  0.5, -0.5, 1.0),
    point4(-0.5, -0.5,  0.5, 1.0), point4( 0.5, -0.5,  0.5, 1.0),
    point4( 0.5,  0.5,  0.5, 1.0), point4(-0.5,  0.5,  0.5, 1.0)
}; //da cube faces/vertex
point4 points[36];

void colorcube();
void quad(int, int, int, int);
void keys(GLFWwindow *window);

vec4 eye = vec4(0.0, 0.0,  0.0, 1.0);
vec4 at =  vec4(0.0, 0.0, -1.0, 1.0);
vec4 up =  vec4(0.0, 1.0,  0.0, 1.0);


const GLchar *vrt ="\
#version 450 core \n \
layout(location = 0) in vec4 vpos; \
layout(location = 1) uniform mat4 proj; \
layout(location = 2) uniform mat4 tran; \
layout(location = 3) uniform mat4 to; \
void main() { gl_Position = proj*to*tran*vpos; }\
";

const GLchar *frg = "\
#version 450 core \n \
out vec4 color; \
void main() { \
    color = vec4(1.0, 0, 0, 1); }\
";

//data
GLfloat datum[36];
GLfloat proj[16] = {
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, -1.0, 0.0
};


GLfloat tran[16] = {
    1, 0.0, 0.0, 0.0,
    0.0, 1, 0.0, 0.0,
    0.0, 0.0, 1, -1,
    0.0, 0.0, 0.0, 1.0
};

mat4 rots = mat4(1);

void mouseshit(GLFWwindow*, double, double);

void data();

GLFWwindow* shainitwindow();

int main(int argc, char ** argv) {
    
    GLFWwindow *window = shainitwindow();
    
    data();


    glUniformMatrix4fv(3, 1, GL_TRUE, shatranslate(0, 0, -2));

    // glm::mat4 t = glm::transpose(glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(glfwWindowShouldClose(window) == 0) {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        keys(window);
        // glClearColor(1, 1, 1, 1);
        // glUniformMatrix4fv(1, 1, GL_TRUE, glm::value_ptr(t));
        glUniformMatrix4fv(1, 1, GL_TRUE, shaperspective(90.0f, 1.0, 1.0f, 1000.0));
        glUniformMatrix4fv(2, 1, GL_TRUE, shalookat(eye, at, up));

        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glm::mat4 t = glm::transpose(glm::lookAt(eye1, at1, up1));

    // glm::mat4 t = glm::transpose(glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f));
    
    // for(int i = 0; i < 4; i++) {
    //     for(int j = 0; j < 4; j++) {
    //         cout << t[i][j] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << endl;
    // cout << shalookat(eye, at, up) << endl;
    // cout << shaperspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f) << endl;

    return 0;
}

//initiate the basic window for open GL 4.3
GLFWwindow* shainitwindow() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow *window = glfwCreateWindow(500, 500, "Yes", nullptr, nullptr);
    
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glfwSetCursorPosCallback(window, mouseshit);
    glfwSetCursorPos(window, 250, 250);
    return window;
}//end shainitwindow


void data() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // glBufferData(GL_ARRAY_BUFFER, sizeof(datum), datum, GL_STATIC_DRAW);
    colorcube();


    GLuint vert = glCreateShader(GL_VERTEX_SHADER);
    GLuint frag = glCreateShader(GL_FRAGMENT_SHADER);


    glShaderSource(vert, 1, &vrt, NULL);
    glShaderSource(frag, 1, &frg, NULL);

    glCompileShader(vert);
    glCompileShader(frag);

    GLuint prog = glCreateProgram();
    glAttachShader(prog, vert);
    glAttachShader(prog, frag);

    glLinkProgram(prog);

    glUseProgram(prog);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

}



void colorcube() {
    quad(0, 3, 2, 1);
    quad(2, 3, 7, 6);
    quad(3, 0, 4, 7);
    quad(1, 2, 6, 5);
    quad(4, 5, 6, 7);
    quad(5, 4, 0, 1);

    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
}

void quad(int a, int b, int c, int d) {
    points[index] = vertecis[a];
    index++;

    points[index] = vertecis[b];
    index++;

    points[index] = vertecis[c];
    index++;

    points[index] = vertecis[a];
    index++;

    points[index] = vertecis[c];
    index++;

    points[index] = vertecis[d];
    index++;
}

void mouseshit(GLFWwindow *window, double x, double y) {
    // static double prevx = 0.0f;
    // static double prevy = 0.0f;

    
    // cout << "[" << (x/250)-1 << "|" << (-y/250)+1 << "]" << endl;

    double cury = (-y/250)+1;
    double curx = (x/250)-1;

    at.y = cury;
    at.x = curx;

    // vec4 new_at = at;
    // new_at.y += cury;
    // if(cury < 0)
    //     rots = sharotatex(+getangle(at-eye, new_at-eye));
    // else 
    //     rots = sharotatex(-getangle(at-eye, new_at-eye));




}



void keys(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);


    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        eye.z -= 0.1;
         at.z -= 0.1;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        eye.z += 0.1;
         at.z += 0.1;
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        eye.x -= 0.1;
         // at.x -= 0.1;
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        eye.x += 0.1;
         // at.x += 0.1;
    }


}
