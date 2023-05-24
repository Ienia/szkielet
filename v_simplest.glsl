#version 330

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

//Atrybuty
in vec3 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
in vec4 color; //kolor związany z wierzchołkiem
in vec3 normal; //wektor normalny w przestrzeni modelu
in vec2 texCoord0;

//Zmienne interpolowane
out vec4 ic_red;
out vec4 ic_green;
out vec4 ic_blue;
out vec4 l_red;
out vec4 l_green;
out vec4 l_blue;
out vec4 n;
out vec4 v;
out vec2 iTexCoord0; 

void main(void) {
    
    vec4 vertexx = vec4(vertex, 1);
    vec4 normall = vec4(normal, 1);

    vec4 lp_red = vec4(0, 50, 0, 1); //pozcyja światła, przestrzeń świata
    vec4 lp_green = vec4(-20, 50, 0, 1); //pozcyja światła, przestrzeń świata
    vec4 lp_blue = vec4(20, 50, 0, 1); //pozcyja światła, przestrzeń świata

    l_red = normalize(V * lp_red - V*M*vertexx); //wektor do światła w przestrzeni oka
    l_green = normalize(V * lp_green - V*M*vertexx); //wektor do światła w przestrzeni oka
    l_blue = normalize(V * lp_blue - V*M*vertexx); //wektor do światła w przestrzeni oka

    v = normalize(vec4(0, 0, 0, 1) - V * M * vertexx); //wektor do obserwatora w przestrzeni oka
    n = normalize(V * M * normall); //wektor normalny w przestrzeni oka
    
    iTexCoord0 = texCoord0;
    
    gl_Position=P*V*M*vertexx;
}
