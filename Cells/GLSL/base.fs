layout (location = 0) out vec4 A;

uniform sampler2D T;
uniform ivec2 size;

void main() {
    A = texture(T, gl_FragCoord.xy * 1.0/vec2(size));
}

