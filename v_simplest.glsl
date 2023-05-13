#version 330
#define GLM_FORCE_SWIZZLE

float d;

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 color;
in vec4 normal;

//zmienne interpolowane
out vec4 iC;

void main(void) {
	 d = distance(V*M*vertex, vec4(0,0,0,1));
	 d -= 50;
	 d /= 15;
	 d = 1 - d;
	 iC = vec4(color.rgb * d, color.a);
	 
    
	
	//wspolrzedne swiatla w przestrzeni swiata
	vec4 lp=vec4(0,0,1,1);
	vec4 La=vec4(0,0,0,1);
	vec4 Ld=vec4(1,1,1,1);
	
	//glm::vec4 normal, vertex, lp; //wsp. Wierzcholkow
	vec4 a = vertex - normal;
	vec4 b = lp - normal;

	//Wektor normalny (wynik cross)
	//skracamy do dlugosci jednostkowej (normalize)
	a = vec4 (normalize(a));
	b = vec4 (normalize(b));

	float n = dot(a,b);
	n = clamp(n, 0, 10);

	iC = La + Ld * color * n;


	gl_Position=P*V*M*vertex;
}
