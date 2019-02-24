layout (location = 0) out float A;

uniform sampler2D W;
uniform sampler2D M;
uniform sampler2D D;
uniform ivec2 size;

float bw(in ivec2 c) {
    return texelFetch(W, clamp(c, ivec2(0, 0), size - 1), 0).x;
}

float bm(in ivec2 c) {
    return texelFetch(M, clamp(c, ivec2(0, 0), size - 1), 0).x;
}

void main() {
    ivec2 u = ivec2(gl_FragCoord.xy);
    float r = 0.0;
    float w = r + clamp(bm(u + ivec2(-1, 0)), 0.0, 1.0 - r);
    float e = r + clamp(bm(u + ivec2(1, 0)), 0.0, 1.0 - r);
    float n = r + clamp(bm(u + ivec2(0, 1)), 0.0, 1.0 - r);
    float s = r + clamp(bm(u + ivec2(0, -1)), 0.0, 1.0 - r);
    float wW = bw(u + ivec2(-1, 0)) * w;
    float wE = bw(u + ivec2(1, 0)) * e;
    float wN = bw(u + ivec2(0, 1)) * n;
    float wS = bw(u + ivec2(0, -1)) * s;
    float d = texelFetch(D, u, 0).x;
    A = (wW + wE + wN + wS - d) * 0.25;
    //A = (wW + wE + wN + wS) / (w + e + n + s) - d * 0.25;
}


