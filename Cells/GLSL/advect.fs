layout (location = 0) out vec4 A;

uniform sampler2D T;
uniform sampler2D V;

uniform float dt;
uniform vec2 invSize;

void main() {
    vec2 coord = gl_FragCoord.xy * invSize;
    A = texture(T, (-texture(V, coord).xy * dt + gl_FragCoord.xy) * invSize);
}
