#version 460 core

/*---- UNIFORMS - GLOBALS ----*/
uniform mat4 uModel;
uniform mat4 uViewProj;
uniform int uViewMode;
uniform vec3 uSunDir;



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
    vec3 n = normalize(vNormal);
    vec3 color;

    if(uViewMode == 1){
        color = n * 0.5 + 0.5;   // [-1,1] -> [0,1] for display
    }
    else if(uViewMode == 2){
        color = vec3(0.8);
    }
    else{
        float d = max(dot(n, normalize(uSunDir)), 0.0);
        color = vec3(0.8) * (0.15 + 0.85*d);
    }

    FragColor = vec4(color, 1.0);
}