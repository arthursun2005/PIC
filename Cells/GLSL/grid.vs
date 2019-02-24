layout (location = 0) in vec2 uv;

uniform sampler2D P;
uniform sampler2D V;
uniform vec2 size;
uniform vec2 gridSize;
uniform int root;

uniform int x;
uniform int y;

out vec4 a;

void main() {
    gl_PointSize = 1.0;
    vec2 p = texelFetch(P, ivec2(uv * root), 0).xy;
    vec2 v = texelFetch(V, ivec2(uv * root), 0).xy;
    p += size;
    float kx[2] = float[](p.x, p.x + 0.5);
    float ky[2] = float[](p.y, p.y + 0.5);
    float k[2] = float[](0.0, 1.0);
    vec2 kv = vec2(k[x], k[y]);
    a = vec4(v * kv, kv);
    gl_Position = vec4(vec2(kx[x], ky[y]) / gridSize - 1.0, 0.0, 1.0);
}
