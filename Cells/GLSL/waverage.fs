layout (location = 0) out vec4 velocity;

uniform sampler2D V;

void main()
{
    float m = 1.0;
    vec4 x = texelFetch(V, ivec2(gl_FragCoord.xy), 0).xyzw;
    vec2 zw = x.zw + 1e-16;
    velocity = vec4(x.xy / zw, sign(x.z) * m, sign(x.w) * m);
}
