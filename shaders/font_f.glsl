

uniform sampler2D tex1;
uniform vec3 color;

void main()
{
		gl_FragColor = vec4(color, texture2D(tex1, gl_TexCoord[0].xy).a);
}