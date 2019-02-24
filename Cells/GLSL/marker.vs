layout (location = 0) in vec2 uv;

uniform sampler2D P;
uniform int root;
uniform vec2 invSize;

void main() {
    gl_PointSize = 1.0;
    vec2 p = texelFetch(P, ivec2(uv * root), 0).xy * invSize;
    gl_Position = vec4(p, 0.0, 1.0);
}
