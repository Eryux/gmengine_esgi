
attribute vec4 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_TexCoords;
attribute vec4 a_JointWeights;
attribute int a_JointIndexes[4];

uniform mat4 u_WorldMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_bindposeMatrix[68];
uniform mat4 u_jointMatrix[68];

uniform float u_Time;

varying vec3 v_Normal;
varying vec3 v_Position;
varying vec2 v_TexCoords;

void main(void)
{
	v_TexCoords = a_TexCoords;
	v_Normal = vec3(u_WorldMatrix * vec4(a_Normal.xyz, 0.0));
	v_Position = vec3(u_WorldMatrix * a_Position);

	gl_Position = u_ProjectionMatrix * u_WorldMatrix * 
		((a_JointWeights.x * u_jointMatrix[a_JointIndexes[0]] * u_bindposeMatrix[a_JointIndexes[0]] * a_Position)
		+ (a_JointWeights.y * u_jointMatrix[a_JointIndexes[1]] * u_bindposeMatrix[a_JointIndexes[1]] * a_Position)
		+ (a_JointWeights.z * u_jointMatrix[a_JointIndexes[2]] * u_bindposeMatrix[a_JointIndexes[2]] * a_Position)
		+ (a_JointWeights.w * u_jointMatrix[a_JointIndexes[3]] * u_bindposeMatrix[a_JointIndexes[3]] * a_Position));
}
