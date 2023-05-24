/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/

#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE  

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include "myTeapot.h"

#pragma comment(lib, "winmm.lib")

// ZAŁĄCZ SZKIELETA W KAWAŁKACH
#include "head.h"
#include "torso.h"
#include "forearmRight.h"
#include "forearmLeft.h"
#include "legRight.h"
#include "legLeft.h"
#include "armRight.h"
#include "armLeft.h"
#include "thighLeft.h"
#include "thighRight.h"

float speed_x=0;
float speed_y=0;
float aspectRatio=1;

ShaderProgram *sp;

GLuint tex0;

/* SZKIELET W CAŁOŚCI
float* vertices = SkeletonPositions;
float* normals = SkeletonNormals;
float* texCoords = SkeletonTexels;
float* colors = SkeletonColors;
int vertexCount = SkeletonVertices;
*/

//OTOCZENIE
float* verticesCube = myCubeVertices;
float* normalsCube = myCubeNormals;
float* texCoordsCube = myCubeTexCoords;
float* colorsCube = myCubeColors;
int vertexCountCube = myCubeVertexCount;

// HEAD
float* verticesHead = headPositions;
float* normalsHead = headNormals;
float* texCoordsHead = headTexels;
float* colorsHead = headColors;
int vertexCountHead = headVertices;

// TUŁÓW
float* verticesTorso = torsoPositions;
float* normalsTorso = torsoNormals;
float* texCoordsTorso = torsoTexels;
float* colorsTorso = torsoColors;
int vertexCountTorso = torsoVertices;

// PRAWE UDO
float* verticesLeftThigh = thighLPositions;
float* normalsLeftThigh = thighLNormals;
float* texCoordsLeftThigh = thighLTexels;
float* colorsLeftThigh = thighLColors;
int vertexCountLeftThigh = thighLVertices;

//LEWE UDO
float* verticesRightThigh = thighRPositions;
float* normalsRightThigh = thighRNormals;
float* texCoordsRightThigh = thighRTexels;
float* colorsRightThigh = thighRColors;
int vertexCountRightThigh = thighRVertices;

// PRAWA NOGA
float* verticesRightLeg = legRPositions;
float* normalsRightLeg = legRNormals;
float* texCoordsRightLeg = legRTexels;
float* colorsRightLeg = legRColors;
int vertexCountRightLeg = legRVertices;

// LEWA NOGA
float* verticesLeftLeg = legLPositions;
float* normalsLeftLeg = legLNormals;
float* texCoordsLeftLeg = legLTexels;
float* colorsLeftLeg = legLColors;
int vertexCountLeftLeg = legLVertices;

// PRAWE PRZEDRAMIE
float* verticesRightForearm = forearmRPositions;
float* normalsRightForearm = forearmRNormals;
float* texCoordsRightForearm = forearmRTexels;
float* colorsRightForearm = forearmRColors;
int vertexCountRightForearm = forearmRVertices;

// LEWE PRZEDRAMIE
float* verticesLeftForearm = forearmLPositions;
float* normalsLeftForearm = forearmLNormals;
float* texCoordsLeftForearm = forearmLTexels;
float* colorsLeftForearm = forearmLColors;
int vertexCountLeftForearm = forearmLVertices;

// PRAWA RĘKA
float* verticesRightArm = armRPositions;
float* normalsRightArm = armRNormals;
float* texCoordsRightArm = armRTexels;
float* colorsRightArm = armRColors;
int vertexCountRightArm = armRVertices;

// LEWA RĘKA
float* verticesLeftArm = armLPositions;
float* normalsLeftArm = armLNormals;
float* texCoordsLeftArm = armLTexels;
float* colorsLeftArm = armLColors;
int vertexCountLeftArm = armLVertices;


//Procedura obsługi błędów
void error_callback(int error, const char* description) {
	fputs(description, stderr);
}

void keyCallback(GLFWwindow* window,int key,int scancode,int action,int mods) {
    if (action==GLFW_PRESS) {
        if (key==GLFW_KEY_LEFT) speed_x=-PI/2;
        if (key==GLFW_KEY_RIGHT) speed_x=PI/2;
        if (key==GLFW_KEY_UP) speed_y=PI/2;
        if (key==GLFW_KEY_DOWN) speed_y=-PI/2;
    }
    if (action==GLFW_RELEASE) {
        if (key==GLFW_KEY_LEFT) speed_x=0;
        if (key==GLFW_KEY_RIGHT) speed_x=0;
        if (key==GLFW_KEY_UP) speed_y=0;
        if (key==GLFW_KEY_DOWN) speed_y=0;
    }
}

void windowResizeCallback(GLFWwindow* window,int width,int height) {
    if (height==0) return;
    aspectRatio=(float)width/(float)height;
    glViewport(0,0,width,height);
}

GLuint readTexture(const char* filename) {
	GLuint tex;
	glActiveTexture(GL_TEXTURE0);

	//Wczytanie do pamięci komputera
	std::vector<unsigned char> image;   //Alokuj wektor do wczytania obrazka
	unsigned width, height;   //Zmienne do których wczytamy wymiary obrazka
	//Wczytaj obrazek
	unsigned error = lodepng::decode(image, width, height, filename);

	//Import do pamięci karty graficznej
	glGenTextures(1, &tex); //Zainicjuj jeden uchwyt
	glBindTexture(GL_TEXTURE_2D, tex); //Uaktywnij uchwyt
	//Wczytaj obrazek do pamięci KG skojarzonej z uchwytem
	glTexImage2D(GL_TEXTURE_2D, 0, 4, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (unsigned char*)image.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return tex;
}

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);

	PlaySound(TEXT("Spooky_scary_skeletons.wav"), NULL, SND_FILENAME | SND_LOOP | SND_ASYNC);

	sp = new ShaderProgram("v_simplest.glsl", NULL, "f_simplest.glsl");

	tex0 = readTexture("bones_texture.jpg");
}

//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

    delete sp;

	glDeleteTextures(1, &tex0);
}

void draw(float* vertices, float* colors, float* normals, float* texCoords, float verticesCount) {
	glVertexAttribPointer(sp->a("vertex"), 3, GL_FLOAT, false, 0, vertices); //Wskaż tablicę z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors); //Wskaż tablicę z danymi dla atrybutu color
	glVertexAttribPointer(sp->a("normal"), 3, GL_FLOAT, false, 0, normals); //Wskaż tablicę z danymi dla atrybutu normal
	glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0, texCoords); //Wskaż tablicę z danymi dla atrybutu normal

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);

	glDrawArrays(GL_TRIANGLES, 0, verticesCount); //Narysuj obiekt
}

//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle_x,float angle_y) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V=glm::lookAt(
         glm::vec3(0, 36, 75),
         glm::vec3(0, 20, 0),
         glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz widoku

    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.1f, 100.0f); //Wylicz macierz rzutowania

    glm::mat4 M=glm::mat4(1.0f);

    sp->use();//Aktywacja programu cieniującego

    //Przeslij parametry programu cieniującego do karty graficznej
	glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
	glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	//Włącz przesyłanie danych do atrybutu vertex, color i normal
	glEnableVertexAttribArray(sp->a("vertex"));
	glEnableVertexAttribArray(sp->a("color")); 
	glEnableVertexAttribArray(sp->a("normal"));
	glEnableVertexAttribArray(sp->a("texCoord0"));

	glUniform1i(sp->u("textureMap0"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex0);

	//************************			PĘTLA RYSUJĄCA SZKIELETY		*********************************************
	M = glm::scale(M, glm::vec3(300.0f, 1.0f, 300.0f));

	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
	draw(verticesCube, colorsCube, normalsCube, texCoordsCube, vertexCountCube);
	for (int i = 0; i < 4; i++) {
		M = glm::mat4(1.0f);
		M = glm::rotate(M, angle_x + sin(2.0f * 3.15f * angle_x + 3.1415f) / (4.0f * 3.15f), glm::vec3(0.0f, 1.0f, 0.0f));
		M = glm::translate(M, glm::vec3(0.0f, 5*abs(sin(3.15f * angle_x)), 0.0f));
		M = glm::rotate(M, i * 3.1415f /2.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		M = glm::translate(M, glm::vec3(0.0f, 0.0f, -14.0f));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

		draw(verticesTorso, colorsTorso, normalsTorso, texCoordsTorso, vertexCountTorso);

		//zlokalizowanie głowy
		glm::mat4 MHead = glm::translate(M, glm::vec3(0.012f, 36.03f, -0.69f));
			//tutaj mogą być rotaty
		MHead = glm::rotate(MHead, cos(angle_x * 2 * 3.1415f)*0.2f, glm::vec3(1.0f, 0.0f, 0.0f));
		MHead = glm::translate(MHead, glm::vec3(-0.012f, -36.03f, 0.69f));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(MHead));
		draw(verticesHead, colorsHead, normalsHead, texCoordsHead, vertexCountHead);

		//zlokalizowanie uda
		glm::mat4 MRightLeg = glm::translate(M, glm::vec3(-2.12f, 21.51f, 0.52f));
			//tutaj mogą być rotaty
		MRightLeg = glm::rotate(MRightLeg, - 1.5f + 1.5f * abs(sin(1.5708f * angle_x)) - 0.1f, glm::vec3(1.0f, 0.0f, 1.0f));
		MRightLeg = glm::rotate(MRightLeg, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		MRightLeg = glm::translate(MRightLeg, glm::vec3(2.12f, -21.51f, -0.52f));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(MRightLeg));
		draw(verticesRightThigh, colorsRightThigh, normalsRightThigh, texCoordsRightThigh, vertexCountRightThigh);

		//zlokalizowanie nogi
		MRightLeg = glm::translate(MRightLeg, glm::vec3(1.3f, 12.7f, -0.38f));
			//tutaj mogą być rotaty
		MRightLeg = glm::rotate(MRightLeg, 1.5f - 1.5f * abs(sin(1.5708f * angle_x)) + 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
		MRightLeg = glm::translate(MRightLeg, glm::vec3(-1.3f, -12.7f, 0.38f));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(MRightLeg));
		draw(verticesRightLeg, colorsRightLeg, normalsRightLeg, texCoordsRightLeg, vertexCountRightLeg);

		//zlokalizowanie uda
		glm::mat4 MLeftLeg = glm::translate(M, glm::vec3(2.12f, 21.51f, 0.52f));
			//tutaj mogą być rotaty
		MLeftLeg = glm::rotate(MLeftLeg, 1.5f - 1.5f * abs(cos(1.5708f * angle_x)) + 0.1f, glm::vec3(-1.0f, 0.0f, 1.0f));
		MLeftLeg = glm::rotate(MLeftLeg, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		MLeftLeg = glm::translate(MLeftLeg, glm::vec3(-2.12f, -21.51f, -0.52f));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(MLeftLeg));
		draw(verticesLeftThigh, colorsLeftThigh, normalsLeftThigh, texCoordsLeftThigh, vertexCountLeftThigh);

		//zlokalizowanie nogi
		MLeftLeg = glm::translate(MLeftLeg, glm::vec3(1.3f, 12.7f, -0.38f));
			//tutaj mogą być rotaty
		MLeftLeg = glm::rotate(MLeftLeg, 1.5f - 1.5f * abs(cos(1.5708f * angle_x)) + 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
		MLeftLeg = glm::translate(MLeftLeg, glm::vec3(-1.3f, -12.7f, 0.38f));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(MLeftLeg));
		draw(verticesLeftLeg, colorsLeftLeg, normalsLeftLeg, texCoordsLeftLeg, vertexCountLeftLeg);

		//zlokalizowanie ramienia
		glm::mat4 MRightArm = glm::translate(M, glm::vec3(-3.7f, 32.0f, -0.9f));
			//tutaj mogą być rotaty
		MRightArm = glm::rotate(MRightArm, -abs(sin(3.1415f * angle_x) / 3.0f) - glm::radians(25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		MRightArm = glm::translate(MRightArm, glm::vec3(3.7f, -32.0f, 0.9f));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(MRightArm));
		draw(verticesRightArm, colorsRightArm, normalsRightArm, texCoordsRightArm, vertexCountRightArm);

		//zlokalizowanie łokcia
		MRightArm = glm::translate(MRightArm, glm::vec3(-4.4f, 25.9f, -0.9f));
			//tutaj mogą być rotaty
		MRightArm = glm::rotate(MRightArm, abs(sin(3.1415f * angle_x) / 3.5f), glm::vec3(0.0f, 0.0f, 1.0f));
		MRightArm = glm::translate(MRightArm, glm::vec3(4.4f, -25.9f, 0.9f));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(MRightArm));
		draw(verticesRightForearm, colorsRightForearm, normalsRightForearm, texCoordsRightForearm, vertexCountRightForearm);

		//zlokalizowanie ramienia
		glm::mat4 MLeftArm = glm::translate(M, glm::vec3(3.7f, 32.0f, -0.9f));
			//tutaj mogą być rotaty
		MLeftArm = glm::rotate(MLeftArm, abs(sin(3.1415f * angle_x) / 3.0f) + glm::radians(25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		MLeftArm = glm::translate(MLeftArm, glm::vec3(-3.7f, -32.0f, 0.9f));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(MLeftArm));
		draw(verticesLeftArm, colorsLeftArm, normalsLeftArm, texCoordsLeftArm, vertexCountLeftArm);

		//zlokalizowanie łokcia
		MLeftArm = glm::translate(MLeftArm, glm::vec3(4.4f, 25.9f, -0.9f));
			//tutaj mogą być rotaty
		MLeftArm = glm::rotate(MLeftArm, -abs(sin(3.1415f * angle_x) / 3.5f), glm::vec3(0.0f, 0.0f, 1.0f));
		MLeftArm = glm::translate(MLeftArm, glm::vec3(-4.4f, -25.9f, 0.9f));

		glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(MLeftArm));
		draw(verticesLeftForearm, colorsLeftForearm, normalsLeftForearm, texCoordsLeftForearm, vertexCountLeftForearm);
	}
	
	//************************		KONIEC PĘTLI RYSUJĄCEJ SZKIELETY	*********************************************

	//Wyłącz przesyłanie danych do atrybutu vertex, color i normal
    glDisableVertexAttribArray(sp->a("vertex"));
	glDisableVertexAttribArray(sp->a("color"));
	glDisableVertexAttribArray(sp->a("normal"));  
	glDisableVertexAttribArray(sp->a("texCoord0"));  

    glfwSwapBuffers(window); //Przerzuć tylny bufor na przedni
}


int main(void)
{
	GLFWwindow* window; //Wskaźnik na obiekt reprezentujący okno

	glfwSetErrorCallback(error_callback);//Zarejestruj procedurę obsługi błędów

	if (!glfwInit()) { //Zainicjuj bibliotekę GLFW
		fprintf(stderr, "Nie można zainicjować GLFW.\n");
		exit(EXIT_FAILURE);
	}

	window = glfwCreateWindow(1000, 1000, "Danse macabre", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

	if (!window) //Jeżeli okna nie udało się utworzyć, to zamknij program
	{
		fprintf(stderr, "Nie można utworzyć okna.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window); //Od tego momentu kontekst okna staje się aktywny i polecenia OpenGL będą dotyczyć właśnie jego.
	glfwSwapInterval(1); //Czekaj na 1 powrót plamki przed pokazaniem ukrytego bufora

	if (glewInit() != GLEW_OK) { //Zainicjuj bibliotekę GLEW
		fprintf(stderr, "Nie można zainicjować GLEW.\n");
		exit(EXIT_FAILURE);
	}

	initOpenGLProgram(window); //Operacje inicjujące

	//Główna pętla

	

	float angle_x=0; //Aktualny kąt obrotu obiektu
	float angle_y=0; //Aktualny kąt obrotu obiektu
	glfwSetTime(0); //Zeruj timer
	while (!glfwWindowShouldClose(window)) //Tak długo jak okno nie powinno zostać zamknięte
	{
        angle_x+=speed_x*glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
        angle_y+=speed_y*glfwGetTime(); //Zwiększ/zmniejsz kąt obrotu na podstawie prędkości i czasu jaki upłynał od poprzedniej klatki
        glfwSetTime(0); //Zeruj timer
		drawScene(window,angle_x,angle_y); //Wykonaj procedurę rysującą
		glfwPollEvents(); //Wykonaj procedury callback w zalezności od zdarzeń jakie zaszły.
	}

	freeOpenGLProgram(window);

	glfwDestroyWindow(window); //Usuń kontekst OpenGL i okno
	glfwTerminate(); //Zwolnij zasoby zajęte przez GLFW
	exit(EXIT_SUCCESS);
}
