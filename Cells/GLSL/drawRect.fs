layout (location = 0) out vec2 A;

uniform vec2 pos;
uniform float sp;
uniform int hx;
uniform int hy;
uniform int root;
uniform int count;

void main() {
    int on = flatten(gl_FragCoord.xy, root) - count;
    int dx = (on%hx) - (hx >> 1);
    int dy = (on/hx) - (hy >> 1);
    A = pos + vec2(dx * sp, dy * sp);
}
