

uniform sampler2D tex1;
uniform sampler2D konstants;
uniform float type; // SFML won't let us pass in ints, so doing funky stuff
uniform vec4 color;
uniform float spriteWidth;
uniform float spriteHeight;
uniform bool selected;

void main()
{
	// Calculate glow falloff from center of sprite
	float falloff = .6;
	vec2 pos = gl_TexCoord[0].xy;
	float dist = distance(vec2(.5, .5), pos); // Distance from center of sprite
	float inv_dist = min( 1.0, 2.0*dist);	// clamp to [0,1]
	inv_dist = 1.0 - inv_dist;		// Invert
	
	// fraction proportional to (1-d)
	falloff *= inv_dist;	

	// Lighting change on x and y axis of buttons
	float lightx = 0.05;
	float lighty = 0.10;

	// Constants to choose effect. Either 0 or 1
	float type_ = (type + .5) / 5.0;
	float type1or3 = texture2D(konstants, vec2(type_,0.0)).x;
	float type2	   = texture2D(konstants, vec2(type_,0.0)).y;
	float type4    = texture2D(konstants, vec2(type_,0.0)).z;
	float type5    = texture2D(konstants, vec2(type_,0.0)).w;

	vec4 texture = texture2D(tex1, gl_TexCoord[0].xy);

	// If it is an image or box this will be used
	float type1or3color = type1or3*1.5;

	// If it is a button this will be used
	float x = pos.x;
	float y = pos.y;
	float leftx = x * spriteWidth;
	float boty = y * spriteHeight;
	float rightx = (spriteWidth - leftx);
	float topy = (spriteHeight - boty);
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
	float type4color = type4*( xshading + yshading + 1.0);

	// If it is a hotspot this will be used
	vec4 type5color = type5 * vec4(color.rgb, falloff);

	// If it is text this will be used
	vec4 type2color = type2 * vec4(1.0, 1.0, 1.0, falloff);
	if( type2 > .01 && texture.a > 0.0 )  
	{
		type2color = mix(type2color, color, texture.a);
	}

	gl_FragColor = vec4((type1or3color + type4color)*texture.rgb, texture.a)
				 + type2color + type5color;
	
    // Add box around image if selected
	if( selected && (leftx < 5.0 || rightx < 5.0 || boty < 5.0 || topy < 5.0) )
		gl_FragColor = vec4(color.rgb, 1.0);
}