#version 150

//uniform float r;
//uniform float b;
out vec4 outputColor;

void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    // we use gl_FragCoord.x position to control the red color value.
    // we use gl_FragCoord.y position to control the green color value.
    // please note that all r, g, b, a values are between 0 and 1.
    
    float windowWidth = 600;
    float windowHeight = 900;
    
    float r = 1;
	float g = gl_FragCoord.y / windowHeight;
    //float g = 0;
	float b = gl_FragCoord.x / windowWidth;
    
	float a = 1;
	outputColor = vec4(r, g, b, a);
}
