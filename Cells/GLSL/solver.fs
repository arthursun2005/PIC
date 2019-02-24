layout (location = 0) out vec2 A;

uniform vec2 gravity;
uniform vec2 size;
uniform vec2 gridSize;
uniform float dt;
uniform int count;
uniform int root;

uniform sampler2D P;
uniform sampler2D V;
uniform sampler2D W;
uniform sampler2D G;
uniform sampler2D D;

void main() {
    if(flatten(gl_FragCoord.xy, root) >= count)
        discard;
    
    ivec2 k = ivec2(gl_FragCoord.xy);
    
    vec2 pos0 = texelFetch(P, k, 0).xy;
    vec2 vel0 = texelFetch(V, k, 0).xy;
    
    vec2 u = (pos0 + size) * 0.5;
    
    vec4 g = get(G, u);
    
    vec4 d = get(D, u);
    
    vec2 p2 = pos0 + g.xy * dt * 0.5;
    
    if(p2.x < -size.x) p2.x = -size.x;
    if(p2.x > size.x) p2.x = size.x;
    if(p2.y < -size.y) p2.y = -size.y;
    if(p2.y > size.y) p2.y = size.y;
    
    vec2 u2 = (p2 + size) * 0.5;
    
    vec4 g2 = get(G, u2);
    
    float a = 0.0;
    float b = 1.0;
    
    //vec2 vel1 = (vel0 * a + g.xy * b) / (a + b);
    //vec2 vel1 = ((vel0 + (g.xy - d.xy)) * a + g.xy * b) / (a + b);
    
    vec2 vel1 = g.xy;
    
    vel1 += gravity * dt;
    
    pos0 += vel1 * dt;
    
    if(pos0.x <= -size.x) {vel1.x = 0.0;}
    if(pos0.x >= size.x) {vel1.x = 0.0;}
    
    if(pos0.y <= -size.y) {vel1.y = 0.0;}
    if(pos0.y >= size.y) {vel1.y = 0.0;}

    A = vel1;
}
