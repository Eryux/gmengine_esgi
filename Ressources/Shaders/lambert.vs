
attribute vec4 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_TexCoords;

uniform mat4 u_WorldMatrix;
uniform mat4 u_ProjectionMatrix;

uniform float u_Time;

varying vec3 v_Normal;
varying vec3 v_Position;
varying vec2 v_TexCoords;

void main(void)
{
	v_TexCoords = a_TexCoords;
	v_Normal = vec3(u_WorldMatrix * vec4(a_Normal.xyz, 0.0));
	v_Position = vec3(u_WorldMatrix * a_Position);

	gl_Position = u_ProjectionMatrix * u_WorldMatrix * a_Position;
}
