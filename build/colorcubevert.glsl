#version 430 core

//in-out variables
layout (location = 0) in vec4 vpos;
layout (location = 1) in vec4 color;
                      out vec4 vcolor;

//uniform ones

//object
layout (location = 3) uniform mat4 translate;
layout (location = 4) uniform mat4 objrotate;

//camera
layout (location = 5) uniform mat4 camtran;
layout (location = 6) uniform mat4 camlookat;

//proj
layout (location = 7) uniform mat4 proj;

void main() {
    mat4 objsumtran = translate*objrotate;
    mat4 camsumtran = camtran*camlookat;
    mat4 projection = proj;

    gl_Position = projection*camsumtran*vpos;
    vcolor = color;
}
