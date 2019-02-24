layout (location = 0) out float Div;

uniform sampler2D V;
uniform sampler2D M;
uniform ivec2 size;

vec2 bvel(in ivec2 c) {
    vec2 k = texelFetch(V, clamp(c, ivec2(0, 0), size - 1), 0).xy;
    if(c.x < 0 || c.y < 0 || c.x >= size.x || c.y >= size.y) {
        return -k;
    }else{
        return k;
    }
}

float bm(in ivec2 k) {
    return texelFetch(M, clamp(k, ivec2(0, 0), size - 1), 0).x;
}

void main() {
    ivec2 u = ivec2(gl_FragCoord.xy);
    float mn = bm(u) + 1.0;
    vec2 N = bvel(u + ivec2(0, 0));
    vec2 X = bvel(u + ivec2(1, 0));
    vec2 Y = bvel(u + ivec2(0, 1));
    Div = ((X.x - N.x) + (Y.y - N.y)) * mn * 2.0;
}
