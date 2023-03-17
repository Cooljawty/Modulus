#version 330
out vec4 FragColor;

in vec2 TexCoordinates;

struct Material{
	sampler2D framebuffer;
};
uniform Material material;

const float offset = 1.0/1366 * 0.8;

const vec2 offsets[9] = vec2[](
  vec2(-offset,  offset), // top-left
  vec2( 0.0f,    offset), // top-center
  vec2( offset,  offset), // top-right
  vec2(-offset,  0.0f),   // center-left
  vec2( 0.0f,    0.0f),   // center-center
  vec2( offset,  0.0f),   // center-right
  vec2(-offset, -offset), // bottom-left
  vec2( 0.0f,   -offset), // bottom-center
  vec2( offset, -offset));

const vec3 aberration = vec3(1.0, 1.8, 2.5);
const vec3 aberration_kernal[9] = vec3[](
   aberration,  aberration, -aberration,
   aberration,  vec3(1),    -aberration,
   aberration, -aberration, -aberration);

const float blur_kernal[9] = float[](
  1/16.0,  2/16.0, 1/16.0,
  2/16.0,  4/16.0, 2/16.0,
  1/16.0,  2/16.0, 1/16.0 );

const float identity_kernal[9] = float[](
  0,  0, 0,
  0,  1, 0,
  0,  0, 0  );

// A re-weighted Gaussian kernel by: u/Fourdrinier
const vec3 chromaticAberrationKernel[9] = vec3[9](
vec3(0.0000000000000000000, 0.04416589065853191, 0.0922903086524308425), vec3(0.10497808951021347), vec3(0.0922903086524308425, 0.04416589065853191, 0.0000000000000000000),
vec3(0.0112445223775533675, 0.10497808951021347, 0.1987116566428735725), vec3(0.40342407932501833), vec3(0.1987116566428735725, 0.10497808951021347, 0.0112445223775533675),
vec3(0.0000000000000000000, 0.04416589065853191, 0.0922903086524308425), vec3(0.10497808951021347), vec3(0.0922903086524308425, 0.04416589065853191, 0.0000000000000000000)
);

void main(){
  vec3 sampleTex[9];
  for(int i = 0; i < 9; i++){
    sampleTex[i] = vec3(texture(material.framebuffer, TexCoordinates.st + offsets[i]));
  }
 
  vec3 col = vec3(0.0);
  for(int i = 0; i < 9; i++)
    col += sampleTex[i] * identity_kernal[i];


  FragColor = vec4(col, 1.0);
}
