layout (location = 0) out vec4 A;

uniform sampler2D V;
uniform ivec2 size;

vec4 bv(in ivec2 n) {
    return texelFetch(V, clamp(n, ivec2(0, 0), size - 1), 0);
}

void main() {
    ivec2 c = ivec2(gl_FragCoord.xy);
    vec4 T = bv(c);
    vec4 N = bv(c + ivec2(0, 1));
    vec4 S = bv(c - ivec2(0, 1));
    vec4 W = bv(c - ivec2(1, 0));
    vec4 E = bv(c + ivec2(1, 0));
    A.yw = (T.yw + N.yw + S.yw) / 3.0;
    A.xz = (T.xz + W.xz + E.xz) / 3.0;
}

