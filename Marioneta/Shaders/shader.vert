#version 330

layout (location = 0) in vec3 pos;

out vec4 vCol;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main(){
    gl_Position = projection * view * model * vec4(pos, 1.0);

    vec4 baseColor = vec4(0.0f, 0.1f, 0.3f, 1.0f); // Deep blue
    vec4 topColor = vec4(1.0f, 0.4f, 0.0f, 1.0f);  // Vibrant orange-red

    // Blend between the two colors based on the Y-position.
    // Assuming the puppet's height ranges between -1.0 and 1.0, adjust if different.
    float blendFactor = clamp(pos.y * 0.5 + 0.5, 0.0f, 1.0f);
    vCol = mix(baseColor, topColor, blendFactor);
}
