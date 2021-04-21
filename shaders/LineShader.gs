#version 330 core
layout(lines) in;
layout(line_strip, max_vertices = 4) out;

in V_DATA{
	vec3 VertexColor;
}gs_in[];

out vec3 fColor;

void main(){
	
	fColor = gs_in[0].VertexColor;
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();
	
	fColor = gs_in[1].VertexColor;
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	EndPrimitive();
}
