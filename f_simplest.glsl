#version 330

uniform sampler2D textureMap0; 

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec2 iTexCoord0;
in vec4 n;
in vec4 l_red;
in vec4 l_green;
in vec4 l_blue;
in vec4 v;

void main(void) {
	//Znormalizowane interpolowane wektory
	vec4 ml_red = normalize(l_red);
	vec4 ml_green = normalize(l_green);
	vec4 ml_blue = normalize(l_blue);

	vec4 mn = normalize(n);
	vec4 mv = normalize(v);

	//otoczenie
	vec4 ambient = vec4(0.2f, 0.2f, 0.2f, 1.0f) * vec4(texture(textureMap0, iTexCoord0));

	//diffuse
	float diff = max(dot(mn, ml_red), 0.0f);
	vec4 diffuse = mn * vec4(0.5f, 0.5f, 0.5f, 1.0f) * vec4(texture(textureMap0, iTexCoord0));

	//specular viewdir = mv

	//spotlight
	float epsilon = ( 0.9781f - 0.9511f);
	float theta = dot(ml_red, normalize(-vec4(0.0f, -1.0f, 0.0f, 1.0f)));
    float intensity = clamp((theta - 0.9511) / epsilon, 0.0, 1.0);
	
	//Wektor odbity (reflectDir)
	vec4 mr_red = reflect(-ml_red, mn);
	vec4 mr_green = reflect(-ml_green, mn);
	vec4 mr_blue = reflect(-ml_blue, mn);

	//Parametry powierzchni
	vec4 kd = texture(textureMap0, iTexCoord0);

	vec4 ks_red = vec4(1, 0, 0, 1);
	vec4 ks_green = vec4(0, 1, 0, 1);
	vec4 ks_blue = vec4(0, 0, 1, 1);

	//Obliczenie modelu oświetlenia
	float nl_red = clamp(dot(mn, ml_red), 0, 1);
	float nl_green = clamp(dot(mn, ml_green), 0, 1);
	float nl_blue = clamp(dot(mn, ml_blue), 0, 1);

	//spec
	float rv_red = pow(clamp(dot(mr_red, mv), 0, 1), 1) * intensity;
	float rv_green = pow(clamp(dot(mr_green, mv), 0, 1), 2);
	float rv_blue = pow(clamp(dot(mr_blue, mv), 0, 1), 2);

	pixelColor = vec4 ( intensity * kd.r * nl_red + ks_red.r * rv_red, kd.g * nl_green + ks_green.g*rv_green, kd.b * nl_blue + ks_blue.b*rv_blue, 1 );
}
