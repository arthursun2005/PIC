layout (location = 0) out vec4 A;

uniform vec4 a;
uniform vec4 u;
uniform sampler2D T;
uniform vec2 invSize;

void main() {
    A = texture(T, gl_FragCoord.xy * invSize) * u + a;
}



