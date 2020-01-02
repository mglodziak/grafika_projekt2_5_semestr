#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include <string>
#include <iostream>
#include <fstream>
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include "shaderLoader.h" //narzŕdzie do │adowania i kompilowania shaderˇw z pliku
#include "tekstura.h"


//funkcje algebry liniowej
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective

using namespace std;
//Wymiary okna
int screen_width = 1024;
int screen_height = 768;

int opcja;

int pozycjaMyszyX; // na ekranie
int pozycjaMyszyY;
int mbutton; // wcisiety klawisz myszy

double kameraX = 45.0;
double kameraZ = -52.0;
double kameraY = 0.0;
double kameraD = -15.0;
double kameraPredkosc;
double kameraKat = 20;
double kameraPredkoscObrotu;
double poprzednie_kameraX;
double poprzednie_kameraZ;
double poprzednie_kameraD;

double rotation = 0;

double max;
double min;

GLuint ebo;
unsigned int VAO;
unsigned int VBO;
unsigned int vtex;

float textureCoords[] = {
	0,0,
	1,0,
	1,1,
	0,1,
};
GLuint texid;
//GLuint sampler_id;

//macierze
glm::mat4 MV; //modelview - macierz modelu i świata
glm::mat4 P;  //projection - macierz projekcji, czyli naszej perspektywy


float * vertices;
float * vertices_texture;
int * elements;


//shaders
GLuint programID = 0;
GLuint programID2 = 0;
GLuint programID3 = 0;


/*###############################################################*/
void mysz(int button, int state, int x, int y)
{
	mbutton = button;
	switch (state)
	{
	case GLUT_UP:
		break;
	case GLUT_DOWN:
		pozycjaMyszyX = x;
		pozycjaMyszyY = y;
		poprzednie_kameraX = kameraX;
		poprzednie_kameraZ = kameraZ;
		poprzednie_kameraD = kameraD;
		break;

	}
}
/*******************************************/
void mysz_ruch(int x, int y)
{
	if (mbutton == GLUT_LEFT_BUTTON)
	{
		kameraX = poprzednie_kameraX - (pozycjaMyszyX - x) * 0.1;
		kameraZ = poprzednie_kameraZ - (pozycjaMyszyY - y) * 0.1;
	}
	if (mbutton == GLUT_RIGHT_BUTTON)
	{
		kameraD = poprzednie_kameraD + (pozycjaMyszyY - y) * 0.1;
	}

}
/******************************************/

double dx = 0;
double skala = 0;
double obr_x = 0;
double obr_y = 0;
double obr_z = 0;

void klawisz(GLubyte key, int x, int y)
{
	switch (key) {

	case 27:    /* Esc - koniec */
		exit(1);
		break;

	case 'x':
		dx += 0.1;
		break;
	case 't':
		skala += 0.03;
		break;
	case 'y':
		skala -= 0.03;
		break;
	case 'q':
		obr_x += 1;
		break;
	case 'w':
		obr_x -= 1;
		break;
	case 'a':
		obr_y += 1;
		break;
	case 's':
		obr_y -= 1;
		break;
	case 'e':
		obr_z += 1;
		break;
	case 'r':
		obr_z -= 1;
		break;

	}


}
/*###############################################################*/
void rysuj(void)
{

	//GLfloat color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	//glClearBufferfv(GL_COLOR, 0, color);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Kasowanie ekranu



	MV = glm::mat4(1.0f);  //macierz jednostkowa
	MV = glm::translate(MV, glm::vec3(dx, 0, kameraD));
	MV = glm::rotate(MV, (float)glm::radians(kameraZ + obr_x), glm::vec3(1, 0, 0));
	MV = glm::rotate(MV, (float)glm::radians(kameraX + obr_y), glm::vec3(0, 0, 1));
	MV = glm::rotate(MV, (float)glm::radians(kameraY + obr_y), glm::vec3(0, 1, 0));

	MV = glm::scale(MV, glm::vec3(1 + skala, 1 + skala, 1 + skala));

	glm::mat4 MVP = P * MV;

	/*Zmienne   jednorodne   (ang. uniform variable), zwane  także  zmiennym  globalnymi,sązmiennymi,
	których  wartośc  jest  stała  w  obrębie  obiektu programu. Shadery  mogą zmienne jednorodne tylko  odczytywac */
	if (opcja == 1) //siatka trójkątów
	{
		glUseProgram(programID); //u┐yj programu, czyli naszego shadera       
		GLuint MVP_id = glGetUniformLocation(programID, "MVP"); // pobierz lokalizację zmiennej 'uniform' "MV" w programie
		glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));     // wyślij tablicę mv do lokalizacji "MV", która jest typu mat4    
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, 200 * 200 * 2 * 3, GL_UNSIGNED_INT, 0); // Zaczynamy od 0 i rysujemy wszystkie wierzcho│ki
	}

	if (opcja == 2) //tekstura
	{

		glUseProgram(programID3); //u┐yj programu, czyli naszego shadera
		GLuint MVP_id = glGetUniformLocation(programID3, "MVP"); // pobierz lokalizację zmiennej 'uniform' "MV" w programie
		glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));     // wyślij tablicę mv do lokalizacji "MV", która jest typu mat4    

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //wypełnienie

		glBindTexture(GL_TEXTURE_2D, texid); //wydaje się zbędne
											 //glVertexAttrib3f(1, 1, 0, 0);
		glBindVertexArray(VAO);
		//glActiveTexture(GL_TEXTURE0); //to też

		glDrawElements(GL_TRIANGLES, 200 * 200 * 2 * 3, GL_UNSIGNED_INT, 0);

		;
	}

	if (opcja == 3) //wypełnienie kolorami
	{
		glUseProgram(programID2); //u┐yj programu, czyli naszego shadera      
		GLuint MVP_id = glGetUniformLocation(programID2, "MVP"); // pobierz lokalizację zmiennej 'uniform' "MV" w programie
		glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));     // wyślij tablicę mv do lokalizacji "MV", która jest typu mat4    
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //wypełnienie
		glDrawElements(GL_TRIANGLES, 200 * 200 * 2 * 3, GL_UNSIGNED_INT, 0);
	}


	glFlush();
	glutSwapBuffers();

}
/*###############################################################*/
void rozmiar(int width, int height)
{
	screen_width = width;
	screen_height = height;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screen_width, screen_height);

	P = glm::perspective(glm::radians(60.0f), (GLfloat)screen_width / (GLfloat)screen_height, 1.0f, 1000.0f);

	glutPostRedisplay(); // Przerysowanie sceny
}

/*###############################################################*/
void idle()
{

	glutPostRedisplay();
}

/*###############################################################*/
GLfloat k = 0.05;
GLfloat ad = 0.0;

void timer(int value) {

	ad += k;

	if (ad>1 || ad<0)
		k = -k;

	//GLfloat attrib[] = { ad, 0.0f,0.0f };
	// Aktualizacja wartości atrybutu wejściowego 1.
	//glVertexAttrib3fv(1, attrib);

	/*

	W vertex_shader np:
	layout (location = 1) in vec3 incolor;

	*/
	glutTimerFunc(20, timer, 0);
}
/*###############################################################*/
int main(int argc, char *argv[])
{
	string name = "tatry.txt";
	ifstream file(name);
	if (!file.is_open())
	{
		perror("error file");
		getchar();
		getchar();
		return -1;
	}
	cout << "Wybierz numer i zatwierdz klawiszem Enter" << endl;
	cout << "1. Rysowanie konturu" << endl;
	cout << "2. Rysowanie wraz z tekstura" << endl;
	cout << "3. Rysowanie wypelnienia, kolor zalezny od wysokosci" << endl;

	//cout << "1. Rysowanie konturu" << endl;
	cin >> opcja;


	string word;
	float x = 0;
	float y = 0;
	float last_x = 0;
	float last_y = 49;
	//file >> word;
	vertices = new float[200 * 200 * 3];
	vertices_texture = new float[200 * 200 * 2];
	elements = new int[200 * 200 * 6];
	//vertices[0] = 0;
	int i = 0;
	float x_add = 0;
	float y_add = 0;
	int licznik = 1;

	while (word != "757.15455348527")
	{
		//file >> word;
		if (i % 3 == 0)
		{
			file >> word;
			last_x = x;
			x = stof(word);
			if (x - last_x < 0)
			{
				x_add = 0;
			}
			vertices[i] = x_add;
			//cout << vertices[i] << endl;
			x_add += 145;
			i++;
		}
		else if (i % 3 == 1)
		{
			file >> word;
			last_y = y;
			if (licznik == 200)
			{
				y_add += 220;
				licznik = 0;
			}
			//y
			vertices[i] = y_add;
			licznik++;
			//cout << vertices[i] << endl;
			i++;
		}
		else if (i % 3 == 2)
		{
			file >> word;
			vertices[i] = stof(word);
			//cout << vertices[i] << endl;
			i++;
		}
	}
	//cout << vertices[3];
	max = vertices[0];
	min = max;
	for (int i = 0; i < 200 * 200 * 3; i++)
	{
		vertices[i] = vertices[i] * 0.001;
	}

	for (int i = 2; i < 200 * 200 * 3; i += 3)
	{
		//cout << vertices[i] <<' '<< vertices[i+1] << ' '<< vertices[i+2] << endl;
		if (max < vertices[i])
			max = vertices[i];

		if (min > vertices[i])
			min = vertices[i];
	}


	cout << max << ' ' << min << endl;

	//elements[0] = 0;
	//elements[1] = 1;
	//lements[2] = 201;
	int n = 0;

	int xd = 0;
	for (int i = 0; i < 199; i++)
	{
		for (int j = 0; j < 199; j++)
		{
			elements[xd++] = i * 200 + j;
			elements[xd++] = i * 200 + j + 1;
			elements[xd++] = i * 200 + j + 200;
			elements[xd++] = i * 200 + j + 1;
			elements[xd++] = i * 200 + j + 200;
			elements[xd++] = i * 200 + j + 201;

			//xd++;
		}
	}


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Przyklad 3");

	glewInit(); //init rozszerzeszeń OpenGL z biblioteki GLEW

	glutDisplayFunc(rysuj);                 // def. funkcji rysuj¦cej
	glutIdleFunc(idle);                     // def. funkcji rysuj¦cej w czasie wolnym procesoora (w efekcie: ci¦gle wykonywanej)
	glutTimerFunc(20, timer, 0);
	glutReshapeFunc(rozmiar); // def. obs-ugi zdarzenia resize (GLUT)

	glutKeyboardFunc(klawisz);              // def. obsługi klawiatury
	glutMouseFunc(mysz);            // def. obsługi zdarzenia przycisku myszy (GLUT)
	glutMotionFunc(mysz_ruch); // def. obsługi zdarzenia ruchu myszy (GLUT)


	glEnable(GL_DEPTH_TEST);

	//glPointSize(3.0f);

	if (opcja == 2)
	{
		
		int ix = 0;
		for (int i = 0; i < 200 * 200 * 3; i++)
		{
			if (i % 3 == 0)
				vertices_texture[ix++] = vertices[i] / 28.855;
			if (i % 3 == 1)
				//vertices_texture[ix++] = vertices[i] / 43.780;
				vertices_texture[ix++] = vertices[i] / 44;
			//if (i % 3 == 2)
			//vertices_texture[i] = vertices[i] / 1.66988;

		}
		double mx = vertices_texture[0];
		double mn = vertices_texture[0];
		for (int i = 0; i < 200 * 200 * 2; i++)
		{
			//cout << vertices_texture[i] << endl;
			if (mx < vertices_texture[i])
				mx = vertices_texture[i];
			if (mn > vertices_texture[i])
				mn = vertices_texture[i];

		}
		cout << "mx=" << mx << endl;
		cout << "mn=" << mn << endl;


		//######################

		texid = WczytajTeksture("tatry_popr.bmp");
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texid);

		GLuint sampler_id = glGetUniformLocation(programID3, "myTextureSampler");
		glUniform1i(sampler_id, 0);

		glGenBuffers(1, &vtex);
		glBindBuffer(GL_ARRAY_BUFFER, vtex);
		glBufferData(GL_ARRAY_BUFFER, 200 * 200 * 2 * sizeof(float), vertices_texture, GL_STATIC_DRAW);


		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 200 * 200 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);

		//GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 200 * 200 * 6 * sizeof(int), elements, GL_STATIC_DRAW);


		

		programID3 = loadShaders("vertex_shader.glsl", "fs3.glsl");


		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, vtex);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);



		
		

		/*
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0); glBindBuffer(GL_ARRAY_BUFFER, VBO);


		glEnableVertexAttribArray(0); // pierwszy buform atrybuˇw: wierzcho│ki
		glVertexAttribPointer(
		0,                  // atrybut 0. musi odpowiadaŠ atrybutowi w programie shader
		3,                  // wielkoťŠ (x,y,z)
		GL_FLOAT,           // typ
		GL_FALSE,           // czy znormalizowany [0-1]?
		0,                  // stride
		(void*)0            // array buffer offset
		);
		*/



		//glBufferData(GL_ARRAY_BUFFER, 200 * 200 * 3 * sizeof(float), vertices_texture, GL_STATIC_DRAW);


		//glGenBuffers(1, &VBO);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBufferData(GL_ARRAY_BUFFER, 200 * 200 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);




	}
	if (opcja == 1 || opcja == 3)
	{
		//tworzenie bufora wierzcholków
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 200 * 200 * 3 * sizeof(float), vertices, GL_STATIC_DRAW);


		GLuint ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 200 * 200 * 6 * sizeof(int), elements, GL_STATIC_DRAW);

		/* úadowanie shadera, tworzenie programu *************************/
		/* i linkowanie go oraz sprawdzanie b│ŕdˇw! **********************/

		glEnableVertexAttribArray(0); // pierwszy buform atrybuˇw: wierzcho│ki
		glVertexAttribPointer(
			0,                  // atrybut 0. musi odpowiadaŠ atrybutowi w programie shader
			3,                  // wielkoťŠ (x,y,z)
			GL_FLOAT,           // typ
			GL_FALSE,           // czy znormalizowany [0-1]?
			0,                  // stride
			(void*)0            // array buffer offset
			);
	}



	programID = loadShaders("vertex_shader.glsl", "fragment_shader.glsl");
	programID2 = loadShaders("vertex_shader.glsl", "fs2.glsl");


	//glBindVertexArray(VAO[0]);



	if (opcja == 1 || opcja == 3)
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glEnableVertexAttribArray(0); // pierwszy buform atrybuˇw: wierzcho│ki
		glVertexAttribPointer(
			0,                  // atrybut 0. musi odpowiadaŠ atrybutowi w programie shader
			3,                  // wielkoťŠ (x,y,z)
			GL_FLOAT,           // typ
			GL_FALSE,           // czy znormalizowany [0-1]?
			0,                  // stride
			(void*)0            // array buffer offset
			);
	}

	glutMainLoop();                                 // start

	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VAO);
	//glDeleteBuffers(1, &ebo[0]);

	free(vertices);
	free(vertices_texture);
	free(elements);

	return(0);
}