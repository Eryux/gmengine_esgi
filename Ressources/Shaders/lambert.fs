#version 150

uniform sampler2D u_Texture;
varying vec2 v_TexCoords;

varying vec3 v_Normal;		// normal dans le monde calculee dans le VS
varying vec3 v_Position;	// position dans le monde calculee dans le VS

//const vec3 directionalLight = normalize(-vec3(1.0, 1.0, -1.0));
// si W = 0 (X,Y,Z) est interprete comme un vecteur directeur
// si W = 1 (X,Y,Z) est interprete comme une position
uniform vec4 u_PositionOrDirection;

// composantes ambiantes
uniform vec3 u_lightAmbientColor;
uniform vec3 u_materialAmbientColor;

// composantes diffuses
uniform vec3 u_lightDiffuseColor;
uniform vec3 u_materialDiffuseColor;

uniform float u_textured;

struct Lights
{
	vec3 position;
	vec3 diffuse; // Colour
};

float lambert(vec3 N, vec3 L)
{
	float result = dot(normalize(N), normalize(L));
	return max(result, 0.0);
}

void main(void)
{
	Lights light;
	light.diffuse = u_lightDiffuseColor;

	vec2 uv = vec2(v_TexCoords.x, 1.0 - v_TexCoords.y);
	light.position = vec3(-u_PositionOrDirection);

	if (u_PositionOrDirection.w > 0.0) {
		// Positionnel (point light ou spot light)
		light.position = normalize(u_PositionOrDirection.xyz - v_Position);
	}

	// facteur d'intensite qui suis la loi du cosinus de Lambert
	float diffuseFactor = lambert(v_Normal, light.position);

	vec3 ambientColor = u_lightAmbientColor * u_materialAmbientColor;
	vec3 diffuseColor = u_lightDiffuseColor * u_materialDiffuseColor;

	vec4 diffuse = vec4(diffuseFactor * diffuseColor, 1.0);
	vec4 specular = vec4(0.0);

	if (u_textured > 0.0) {
		gl_FragColor = vec4(ambientColor, 1.0) * texture2D(u_Texture, uv) + (diffuse * texture2D(u_Texture, uv) + specular);
	}
	else {
		gl_FragColor = vec4(ambientColor, 1.0) + (diffuse + specular);
	}

}

