
attribute vec4 a_Position;
attribute vec3 a_Normal;
attribute vec2 a_TexCoords;
attribute vec4 a_JointWeights;
attribute vec4 a_JointIndexes;

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

	float sum_weight = a_JointWeights.x + a_JointWeights.y + a_JointWeights.z + a_JointWeights.w;

	vec4 skin_position;
		skin_position = (a_JointWeights.x * u_jointMatrix[int(a_JointIndexes.x)] * u_bindposeMatrix[int(a_JointIndexes.x)] * a_Position)
			+ (a_JointWeights.y * u_jointMatrix[int(a_JointIndexes.y)] * u_bindposeMatrix[int(a_JointIndexes.y)] * a_Position)
			+ (a_JointWeights.z * u_jointMatrix[int(a_JointIndexes.z)] * u_bindposeMatrix[int(a_JointIndexes.z)] * a_Position)
			+ (a_JointWeights.w * u_jointMatrix[int(a_JointIndexes.w)] * u_bindposeMatrix[int(a_JointIndexes.w)] * a_Position);

	v_Position = vec3(u_WorldMatrix * skin_position);

	gl_Position = u_ProjectionMatrix * u_WorldMatrix * skin_position;
}
