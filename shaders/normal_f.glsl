

uniform sampler2D tex1;
uniform sampler2D konstants; // SFML won't let us pass in Mat4s, funky workaround
uniform float type; 		 // SFML won't let us pass in ints, either
uniform vec4 color;
uniform float spriteWidth;
uniform float spriteHeight;
uniform bool selected;

void main()
{
	vec2 pos = gl_TexCoord[0].xy;
	float dist = distance(vec2(.5, .5), pos); // Distance from center of sprite

	// Lighting change on x and y axis of buttons
	float lightx = -0.05;
	float lighty = -0.10;

	// Constants to choose effect. Either 0 or 1
	float type_ = (type + 1.0) / 5.0;
	float btn1_3 = texture2D(konstants, vec2(type_,0.0)).x;
	float btn2   = texture2D(konstants, vec2(type_,0.0)).y;
	float btn4   = texture2D(konstants, vec2(type_,0.0)).z;
	float btn5   = texture2D(konstants, vec2(type_,0.0)).w;

	vec4 texture = texture2D(tex1, gl_TexCoord[0].xy);

	// If it is a button this will be used
	float x = pos.x; // x value as percent of sprite width
	float y = pos.y; // y value as percent of sprite width
	float leftx = x * spriteWidth; // x value as number of pixels from left
	float boty = y * spriteHeight; // y value as number of pixels from bottom
	float rightx = (spriteWidth - leftx); // number of pixels from right
	float topy = (spriteHeight - boty);   // number of pixels from top
	float lower_left  = min(1.0, floor(leftx/boty));
	float lower_right = min(1.0, floor(rightx/boty));
	float upper_left  = min(1.0, floor(leftx/topy));
	float upper_right = min(1.0, floor(rightx/topy)); 
	float upperx = 4.0 / spriteWidth;
	float lowerx = -1.0 - upperx;  
	float uppery = 4.0 / spriteHeight;
	float lowery = -1.0 - uppery;
	float xshading =(float(int(x+lowerx))*(1.0-lower_left)*(1.0-upper_left)
				   + float(int(x+upperx))*(1.0-lower_right)*(1.0-upper_right))*lightx;
	float yshading =(float(int(y+lowery))*lower_left*lower_right
				   + float(int(y+uppery))*upper_left*upper_right)*lighty;
	float btn4color = btn4*( xshading + yshading + 1.0);

	gl_FragColor = (btn1_3 + btn2 + btn4color + btn5) * texture;

    // Add box around image if selected
	if( selected && (leftx < 5.0 || rightx < 5.0 || boty < 5.0 || topy < 5.0) )
		gl_FragColor = vec4(color.rgb, 1.0);
}