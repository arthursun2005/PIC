layout (location = 0) in vec2 uv;

uniform sampler2D T;
uniform vec2 scl;
uniform int root;

void main() {
    gl_PointSize = 2.0;
    gl_Position = vec4(texelFetch(T, ivec2(uv * root), 0).xy * scl, 0.0, 1.0);
}

