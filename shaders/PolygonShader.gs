#version 330 core
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in VS_DATA{
	vec3 FragPosition;
	vec3 VertexNormal;
	vec2 TextureCoords;
} vData[];

out GS_DATA{
	vec3 FragPosition;
	vec3 VertexNormal;
	vec2 TextureCoords;
} fData;

vec3 GetNormal(){
  vec3 a = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);
  vec3 b = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
  return normalize(cross(a,b));
}

void main(){

  //fData.VertexNormal = GetNormal();
  for(int i = 0; i < gl_in.length(); i++){
    fData.FragPosition = vData[i].FragPosition;
    fData.VertexNormal = vData[i].VertexNormal;
	fData.TextureCoords = vData[i].TextureCoords;
    gl_Position = gl_in[i].gl_Position;
    EmitVertex();
  }
  EndPrimitive();
}
