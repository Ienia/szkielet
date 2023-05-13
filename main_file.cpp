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
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "myCube.h"
#include "myTeapot.h"

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

/* SZKIELET W CAŁOŚCI
float* vertices = SkeletonPositions;
float* normals = SkeletonNormals;
float* texCoords = SkeletonTexels;
float* colors = SkeletonColors;
int vertexCount = SkeletonVertices;
*/

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

//Procedura inicjująca
void initOpenGLProgram(GLFWwindow* window) {
	//************Tutaj umieszczaj kod, który należy wykonać raz, na początku programu************
	glClearColor(0,0,0,1);
	glEnable(GL_DEPTH_TEST);
	glfwSetWindowSizeCallback(window,windowResizeCallback);
	glfwSetKeyCallback(window,keyCallback);

	sp=new ShaderProgram("v_simplest.glsl",NULL,"f_simplest.glsl");
}


//Zwolnienie zasobów zajętych przez program
void freeOpenGLProgram(GLFWwindow* window) {
    //************Tutaj umieszczaj kod, który należy wykonać po zakończeniu pętli głównej************

    delete sp;
}




//Procedura rysująca zawartość sceny
void drawScene(GLFWwindow* window,float angle_x,float angle_y) {
	//************Tutaj umieszczaj kod rysujący obraz******************l
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V=glm::lookAt(
         glm::vec3(0, 25, 50),
         glm::vec3(0,20,0),
         glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz widoku

    glm::mat4 P=glm::perspective(50.0f*PI/180.0f, aspectRatio, 0.1f, 100.0f); //Wylicz macierz rzutowania

    glm::mat4 M=glm::mat4(1.0f);
	//M=glm::rotate(M,angle_y,glm::vec3(1.0f,0.0f,0.0f)); //Wylicz macierz modelu
	//M=glm::rotate(M,angle_x,glm::vec3(0.0f,1.0f,0.0f)); //Wylicz macierz modelu

    sp->use();//Aktywacja programu cieniującego
    //Przeslij parametry programu cieniującego do karty graficznej
    glUniformMatrix4fv(sp->u("P"),1,false,glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"),1,false,glm::value_ptr(V));
    glUniformMatrix4fv(sp->u("M"),1,false,glm::value_ptr(M));

    glEnableVertexAttribArray(sp->a("vertex"));  //Włącz przesyłanie danych do atrybutu vertex
	glEnableVertexAttribArray(sp->a("color"));  //Włącz przesyłanie danych do atrybutu color
	glEnableVertexAttribArray(sp->a("normal"));  //Włącz przesyłanie danych do atrybutu color

    glVertexAttribPointer(sp->a("vertex"),3,GL_FLOAT,false,0,verticesHead); //Wskaż tablicę z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colorsHead); //Wskaż tablicę z danymi dla atrybutu color
	glVertexAttribPointer(sp->a("normal"), 3, GL_FLOAT, false, 0, normalsHead); //Wskaż tablicę z danymi dla atrybutu normal
    glDrawArrays(GL_TRIANGLES,0,vertexCountHead); //Narysuj obiekt

	glVertexAttribPointer(sp->a("vertex"), 3, GL_FLOAT, false, 0, verticesTorso); //Wskaż tablicę z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colorsTorso); //Wskaż tablicę z danymi dla atrybutu color
	glVertexAttribPointer(sp->a("normal"), 3, GL_FLOAT, false, 0, normalsTorso); //Wskaż tablicę z danymi dla atrybutu normal
	glDrawArrays(GL_TRIANGLES, 0, vertexCountTorso); //Narysuj obiekt

	glVertexAttribPointer(sp->a("vertex"), 3, GL_FLOAT, false, 0, verticesLeftThigh); //Wskaż tablicę z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colorsLeftThigh); //Wskaż tablicę z danymi dla atrybutu color
	glVertexAttribPointer(sp->a("normal"), 3, GL_FLOAT, false, 0, normalsLeftThigh); //Wskaż tablicę z danymi dla atrybutu normal
	glDrawArrays(GL_TRIANGLES, 0, vertexCountLeftThigh); //Narysuj obiekt

	glVertexAttribPointer(sp->a("vertex"), 3, GL_FLOAT, false, 0, verticesRightThigh); //Wskaż tablicę z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colorsRightThigh); //Wskaż tablicę z danymi dla atrybutu color
	glVertexAttribPointer(sp->a("normal"), 3, GL_FLOAT, false, 0, normalsRightThigh); //Wskaż tablicę z danymi dla atrybutu normal
	glDrawArrays(GL_TRIANGLES, 0, vertexCountRightThigh); //Narysuj obiekt

	glVertexAttribPointer(sp->a("vertex"), 3, GL_FLOAT, false, 0, verticesRightLeg); //Wskaż tablicę z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colorsRightLeg); //Wskaż tablicę z danymi dla atrybutu color
	glVertexAttribPointer(sp->a("normal"), 3, GL_FLOAT, false, 0, normalsRightLeg); //Wskaż tablicę z danymi dla atrybutu normal
	glDrawArrays(GL_TRIANGLES, 0, vertexCountRightLeg); //Narysuj obiekt

	glVertexAttribPointer(sp->a("vertex"), 3, GL_FLOAT, false, 0, verticesLeftLeg); //Wskaż tablicę z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colorsLeftLeg); //Wskaż tablicę z danymi dla atrybutu color
	glVertexAttribPointer(sp->a("normal"), 3, GL_FLOAT, false, 0, normalsLeftLeg); //Wskaż tablicę z danymi dla atrybutu normal
	glDrawArrays(GL_TRIANGLES, 0, vertexCountLeftLeg); //Narysuj obiekt

	glVertexAttribPointer(sp->a("vertex"), 3, GL_FLOAT, false, 0, verticesRightArm); //Wskaż tablicę z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colorsRightArm); //Wskaż tablicę z danymi dla atrybutu color
	glVertexAttribPointer(sp->a("normal"), 3, GL_FLOAT, false, 0, normalsRightArm); //Wskaż tablicę z danymi dla atrybutu normal
	glDrawArrays(GL_TRIANGLES, 0, vertexCountRightArm); //Narysuj obiekt

	glVertexAttribPointer(sp->a("vertex"), 3, GL_FLOAT, false, 0, verticesRightForearm); //Wskaż tablicę z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colorsRightForearm); //Wskaż tablicę z danymi dla atrybutu color
	glVertexAttribPointer(sp->a("normal"), 3, GL_FLOAT, false, 0, normalsRightForearm); //Wskaż tablicę z danymi dla atrybutu normal
	glDrawArrays(GL_TRIANGLES, 0, vertexCountRightForearm); //Narysuj obiekt

	M = glm::translate(M, glm::vec3(4.0f, 32.0f, 0.6f));
	M = glm::rotate(M, angle_y, glm::vec3(0.0f, 0.0f, 1.0f));
	M = glm::translate(M, glm::vec3(-4.0f, -32.0f, -0.6f));
	glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));

	glVertexAttribPointer(sp->a("vertex"), 3, GL_FLOAT, false, 0, verticesLeftArm); //Wskaż tablicę z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colorsLeftArm); //Wskaż tablicę z danymi dla atrybutu color
	glVertexAttribPointer(sp->a("normal"), 3, GL_FLOAT, false, 0, normalsLeftArm); //Wskaż tablicę z danymi dla atrybutu normal
	glDrawArrays(GL_TRIANGLES, 0, vertexCountLeftArm); //Narysuj obiekt

	glVertexAttribPointer(sp->a("vertex"), 3, GL_FLOAT, false, 0, verticesLeftForearm); //Wskaż tablicę z danymi dla atrybutu vertex
	glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colorsLeftForearm); //Wskaż tablicę z danymi dla atrybutu color
	glVertexAttribPointer(sp->a("normal"), 3, GL_FLOAT, false, 0, normalsLeftForearm); //Wskaż tablicę z danymi dla atrybutu normal
	glDrawArrays(GL_TRIANGLES, 0, vertexCountLeftForearm); //Narysuj obiekt

    glDisableVertexAttribArray(sp->a("vertex"));  //Wyłącz przesyłanie danych do atrybutu vertex
	glDisableVertexAttribArray(sp->a("color"));  //Wyłącz przesyłanie danych do atrybutu color
	glDisableVertexAttribArray(sp->a("normal"));  //Wyłącz przesyłanie danych do atrybutu color

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

	window = glfwCreateWindow(500, 500, "OpenGL", NULL, NULL);  //Utwórz okno 500x500 o tytule "OpenGL" i kontekst OpenGL.

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
