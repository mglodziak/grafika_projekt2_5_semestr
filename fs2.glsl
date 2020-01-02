#version 330
//do tla
out vec4 FragColor;
in vec3 pos;
in vec3 outcol;

void main()
{
	
	if(pos.z==0.0f)
		gl_FragColor  = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	else if(pos.z>0.0 && pos.z<0.4)
		gl_FragColor  = vec4(0.0f, 0.0f+4*pos.z, 1.0f, 1.0f);
	
	else if(pos.z>0.4 && pos.z<0.6)
		gl_FragColor  = vec4(0.0f, 1.0f, 1.0f-(4*(pos.z-0.4)), 1.0f);

	else if(pos.z>0.6 && pos.z<1.1)
		gl_FragColor  = vec4(0.0f+(4*(pos.z-0.6)), 1.0f, 0.0f,1.0f);

	else if(pos.z>1.1 && pos.z<1.66988)
		gl_FragColor  = vec4(1.0f, 1.0f-(4*(pos.z-1.1)), 0.0f,1.0f);


	else 
		gl_FragColor  = vec4(1.0f, 1.0f, 1.0f, 1.0f);

} 