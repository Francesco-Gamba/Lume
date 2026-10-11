#version 460 core

/*---- UNIFORMS - GLOBALS ----*/
uniform mat4 uModel;
uniform mat4 uViewProj;



/*------ VERTEX SHADER ------*/
#stage vertex

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;

out vec3 vNormal;

void main() {
    vNormal = mat3(uModel) * aNormal;
    gl_Position = uViewProj * uModel * vec4(aPos, 1.0);
}

/*------ FRAGMENT SHADER ------*/
#stage fragment

in vec3 vNormal;
out vec4 FragColor;

void main() {
    float d = max(dot(normalize(vNormal), normalize(vec3(0.5, 1.0, 0.3))), 0.0);
    FragColor = vec4(vec3(0.8) * (0.15 + 0.85 * d), 1.0);
}