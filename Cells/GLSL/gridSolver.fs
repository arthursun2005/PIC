layout (location = 0) out vec4 velocity;

uniform sampler2D V;
uniform sampler2D U;
uniform sampler2D M;
uniform ivec2 size;

float bu(in ivec2 k) {
    return texelFetch(U, clamp(k, ivec2(0, 0), size - 1), 0).x;
}

vec4 bv(in ivec2 k) {
    return texelFetch(V, clamp(k, ivec2(0, 0), size - 1), 0);
}

float bm(in ivec2 k) {
    return texelFetch(M, clamp(k, ivec2(0, 0), size - 1), 0).x;
}

void main()
{
    ivec2 coord = ivec2(gl_FragCoord.xy);
    vec4 vel0 = texelFetch(V, coord, 0);
    float mn = bm(coord) + 1.0;
    float mx = bm(coord + ivec2(-1, 0)) + 1.0;
    float my = bm(coord + ivec2(0, -1)) + 1.0;
    float n = bu(coord);
    vel0.x -= (n - bu(coord + ivec2(-1, 0))) / (mn + mx) * 2.0;
    vel0.y -= (n - bu(coord + ivec2(0, -1))) / (mn + my) * 2.0;
    velocity = vel0;
}
