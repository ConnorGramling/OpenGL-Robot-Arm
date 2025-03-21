#version 120

varying vec4 color;
//varying vec2 texCoord;
varying vec4 N, V, L;

uniform int ambient_on;
uniform int diffuse_on;
uniform int specular_on;

vec4 ambient, diffuse, specular;

void main()
{
    // Normalize vectors
    vec4 NN = normalize(N);
    vec4 LL = normalize(L);
    vec4 VV = normalize(V);
    vec4 H = normalize(LL + VV);

    // Ambient lighting
    ambient = 0.3 * color;

    // Diffuse lighting
    diffuse = max(dot(LL, NN), 0.0) * color;

    // Specular lighting
    specular = pow(max(dot(NN, H), 0.0), 100) * vec4(1.0);

    // Combine components
    gl_FragColor = ambient + diffuse + specular;
}
