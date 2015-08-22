#define GLEW_STATIC
// #define GLM_FORCE_RADIANS
#include <iostream>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <sharfun.h>
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

GLuint plane_vbo;
GLuint cube_vbo;

point4 points[36];

point4 plane[32];

void colorcube();
void quad(int, int, int, int);
void keys(GLFWwindow *window);

vec4 eye = vec4(0.0, 0.0,  0.0, 1.0);
vec4 at =  vec4(0.0, 0.0, -1.0, 1.0);
vec4 up =  vec4(0.0, 1.0,  0.0, 1.0);


const GLchar *vrt ="\
#version 450 core \n \
layout(location = 0) in vec4 vpos; \
out vec4 test;\
layout(location = 1) uniform mat4 proj; \
layout(location = 2) uniform mat4 tran; \
layout(location = 3) uniform mat4 to; \
void main() { gl_Position = proj*tran*to*vpos; test = proj*tran*to*vpos;}\
";

const GLchar *frg = "\
#version 450 core \n \
out vec4 color; \
in vec4 test;\
void main() { \
    color = vec4(1.0, 0, 0, 1.0); }\
";

//
mat4 camera = shalookat(eye, at, up);

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


double anglex = 0;
double angley = 0;

void mouseshit(GLFWwindow*, double, double);
void data();
GLFWwindow* shainitwindow();
void planes();

int main(int argc, char ** argv) {
    
    GLFWwindow *window = shainitwindow();
    
    data();

    // glm::mat4 t = glm::transpose(glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f));
    glEnableVertexAttribArray(0);

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    while(glfwWindowShouldClose(window) == 0) {
        if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        keys(window);

        // glClearColor(1, 1, 1, 1);
        // glUniformMatrix4fv(1, 1, GL_TRUE, glm::value_ptr(t));
        glUniformMatrix4fv(1, 1, GL_TRUE, shaperspective(45.0f, 4.0f / 3.0f, 0.1f, 100.f));
        glUniformMatrix4fv(2, 1, GL_TRUE, sharotatex(anglex)*sharotatey(angley)*camera);

        
        glUniformMatrix4fv(3, 1, GL_TRUE, shatranslate(0, 0.0, 0.0));

        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
        
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUniformMatrix4fv(3, 1, GL_TRUE, shascale(100.0, 100.0, 100.0));
        glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
        
        glDrawArrays(GL_LINE_STRIP, 0, 32);
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    planes();    

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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return window;
}//end shainitwindow


void data() {
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);



    // glBufferData(GL_ARRAY_BUFFER, sizeof(datum), datum, GL_STATIC_DRAW);
    colorcube();
    planes();


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

}



void colorcube() {
    quad(0, 3, 2, 1);
    quad(2, 3, 7, 6);
    quad(3, 0, 4, 7);
    quad(1, 2, 6, 5);
    quad(4, 5, 6, 7);
    quad(5, 4, 0, 1);

    glGenBuffers(1, &cube_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);
}

void planes() {
    vec3 start(-1, 0, -1);
    double d = 2.0/7.0;

    for(int i = 1; i <= 16; i++) {
        cout << start << endl;
        
        plane[i-1] = point4(start.x, start.y, start.z, 1.0);
        
        if(i%2 == 0) {
            start.x += d;
        } else {
            start.z *= -1;
        }
    }

    start.x -= d;

    for(int i = 17; i <= 32; i++) {
        plane[i-1] = point4(start.x, start.y, start.z, 1.0);
        
        if(i%2 == 0) {
            start.z += d;
        } else {
            start.x *= -1;
        }   
    }

    cout << start << endl;

    glGenBuffers(1, &plane_vbo);

    glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(plane), plane, GL_STATIC_DRAW);
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
    static double prevx = 0.0f;
    static double prevy = 0.0f;

    double new_angle = 0.0;

    double cury = (-y/250.0)+1;
    double curx = (x/250.0)-1;

    // vec4 new_at = rots.z;
    // new_at.x += curx;
    if((curx - prevx)) {
    //     // cout << getangle(at-eye, new_at-eye) << endl;
    //     // rots = sharotatex(getangle(rots.z, new_at));
    //     angle -= getangle(rots.z, new_at);
        angley++;
    } else {
        angley--;
    //     // cout << -getangle(at-eye, new_at-eye) << endl;
    //     // rots = sharotatex(-getangle(rots.z, new_at));
    //     angle  -= -getangle(rots.z, new_at);
    }

    cout << "x: " << (curx - prevx)  << endl;
    cout << "y: " << (cury - prevy)  << endl;


    // if((cury - prevy) > 0.01) {
        if((cury - prevy)){
            anglex--;
        } else {
            anglex++;
        }    
    // }

    

    // rots = ;

    // cout << getangle(rots.z, new_at) << endl;
    // new_at = rots.z;
    // new_at.x += curx;
    // if(curx > 0) {
    //     cout << getangle(rots.z, new_at) << endl;
    //     rots = rots*sharotatey(getangle(rots.z, new_at));
    // }
    // else {
    //     cout << -getangle(rots.z, new_at) << endl;
    //     rots = rots*sharotatey(-getangle(rots.z, new_at));
    // }

    //i have previous angle here, calculate new angle
    // cout << "old " << angle << endl;
    // double cury = (-y/250.0)+1;

    // double new_angle = 0;

    // vec4 new_at = rots.z;
    // new_at.y += cury;
    
    // if(cury < 0) {
    //     angle += getangle(rots.z, new_at);
    // } else {
    //     angle -= getangle(rots.z, new_at);
    // }

    prevx = curx;
    prevy = cury;


}

void keys(GLFWwindow *window) {
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);



    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
    //     eye.z -= 0.1;
        camera = camera * shatranslate((rots.z)*0.1);
    //      at.z -= 0.1;
    }

    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        // eye.z += 0.1;
        // at.z += 0.1;
        camera = camera * shatranslate((-rots.z)*0.1);
    }

    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
    //     eye.x -= 0.1;
    //     at.x -= 0.1;
        camera = camera * shatranslate((rots.x)*0.1);
    }

    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
    //     eye.x += 0.1;
    //      at.x += 0.1;
        camera = camera * shatranslate(-rots.x*0.1);
    }


}



/*    static double prevx = 0.0f;
    static double prevy = 0.0f;

    double new_angle = 0.0;

    double cury = (-y/250.0)+1;
    double curx = (x/250.0)-1;

    // vec4 new_at = rots.z;
    // new_at.x += curx;
    if((curx - prevx) > 0) {
    //     // cout << getangle(at-eye, new_at-eye) << endl;
    //     // rots = sharotatex(getangle(rots.z, new_at));
    //     angle -= getangle(rots.z, new_at);
        angley++;
    } else {
        angley--;
    //     // cout << -getangle(at-eye, new_at-eye) << endl;
    //     // rots = sharotatex(-getangle(rots.z, new_at));
    //     angle  -= -getangle(rots.z, new_at);
    }

    cout << "x: " << (curx - prevx)  << endl;
    cout << "y: " << (cury - prevy)  << endl;


    if((cury - prevy) > 0.01) {
        if((cury - prevy) > 0){
            anglex--;
        } else {
            anglex++;
        }    
    }

    

    // rots = ;

    // cout << getangle(rots.z, new_at) << endl;
    // new_at = rots.z;
    // new_at.x += curx;
    // if(curx > 0) {
    //     cout << getangle(rots.z, new_at) << endl;
    //     rots = rots*sharotatey(getangle(rots.z, new_at));
    // }
    // else {
    //     cout << -getangle(rots.z, new_at) << endl;
    //     rots = rots*sharotatey(-getangle(rots.z, new_at));
    // }

    //i have previous angle here, calculate new angle
    // cout << "old " << angle << endl;
    // double cury = (-y/250.0)+1;

    // double new_angle = 0;

    // vec4 new_at = rots.z;
    // new_at.y += cury;
    
    // if(cury < 0) {
    //     angle += getangle(rots.z, new_at);
    // } else {
    //     angle -= getangle(rots.z, new_at);
    // }

    prevx = curx;
    prevy = cury;
*/
