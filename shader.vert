attribute vec3 aPos;
attribute vec3 aColor;
attribute vec2 aTexCoords;
void main() {  
	gl_TexCoord[0] = gl_MultiTexCoord0;        
	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;  
}  