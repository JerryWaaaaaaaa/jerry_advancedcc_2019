#version 150

uniform mat4 modelViewProjectionMatrix;

in vec4 position;

uniform float time;

void main(){
    float displacementHeight = 100;
    float displacementY = sin(time + (position.x)/100.0) * displacementHeight;
    
    vec4 modifidedPosition = modelViewProjectionMatrix * position;
    modifidedPosition.y += displacementY;
    
	gl_Position = modifidedPosition;
}
