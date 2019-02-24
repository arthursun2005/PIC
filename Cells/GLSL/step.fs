layout (location = 0) out vec2 A;

uniform sampler2D P;
uniform sampler2D V;

uniform float dt;

uniform vec2 size;

uniform int count;
uniform int root;

void main() {
    if(flatten(gl_FragCoord.xy, root) >= count)
        discard;
    
    ivec2 k = ivec2(gl_FragCoord.xy);
    
    vec2 pos0 = texelFetch(P, k, 0).xy;
    vec2 vel0 = texelFetch(V, k, 0).xy;
    
    pos0 += vel0 * dt;
    
    if(pos0.x <= -size.x) pos0.x = -size.x;
    if(pos0.x >= size.x) pos0.x = size.x;
    if(pos0.y <= -size.y) pos0.y = -size.y;
    if(pos0.y >= size.y) pos0.y = size.y;
    
    A = pos0;
}



