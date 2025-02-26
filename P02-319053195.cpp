//Práctica 2: índices, mesh, proyecciones, transformaciones geométricas
#include <stdio.h>
#include <string.h>
#include<cmath>
#include<vector>
#include <glew.h>
#include <glfw3.h>
//glm
#include<glm.hpp>
#include<gtc\matrix_transform.hpp>
#include<gtc\type_ptr.hpp>
//clases para dar orden y limpieza al código
#include"Mesh.h"
#include"Shader.h"
#include"Window.h"
//Dimensiones de la ventana
const float toRadians = 3.14159265f / 180.0; //grados a radianes
Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<MeshColor*> meshColorList;
std::vector<Shader>shaderList;
//Vertex Shader
static const char* vShaderRojo = "shaders/shader_rojo.vert";
static const char* fShader = "shaders/shader.frag";
static const char* vShaderColor = "shaders/shadercolor.vert";
static const char* fShaderColor = "shaders/shadercolor.frag";
//shaders nuevos se crearían acá
static const char* vShaderVerde = "shaders/shader_verde.vert";
static const char* vShaderAzul = "shaders/shader_azul.vert";
static const char* vShaderCafe = "shaders/shader_cafe.vert";
static const char* vShaderVerdeOscuro = "shaders/shader_verde_oscuro.vert";

float angulo = 0.0f;

//color café en RGB : 0.478, 0.255, 0.067

//Pirámide triangular regular
void CreaPiramide()
{
	unsigned int indices[] = {
		0,1,2,
		1,3,2,
		3,0,2,
		1,0,3

	};
	GLfloat vertices[] = {
		-0.5f, -0.5f,0.0f,	//0
		0.5f,-0.5f,0.0f,	//1
		0.0f,0.5f, -0.25f,	//2
		0.0f,-0.5f,-0.5f,	//3

	};
	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 12, 12);
	meshList.push_back(obj1);
}

//Pirámide cuadrangular
void CreaPiramideCuadrangular()
{
	unsigned int cuadrangular_indices[] = {
		0,1,4,//Cara trasera
		0,2,4,//Cara izquierda
		1,3,4,//Cara derecha
		2,3,4,//Cara frontal
		0,1,2,//Base1
		1,2,3//Base2

	};
	GLfloat cuadrangular_vertices[] = {
		-1.0f,	-1.0f,	1.0f,	//0: Base, arriba, izquierda
		1.0f,	-1.0f,	1.0f,	//1: Base, arriba, derecha
		-1.0f,	-1.0f,	-1.0f,	//2: Base, abajo, izquierda
		1.0f,	-1.0f,	-1.0f,	//3: Base, abajo, derecha
		0.0f,	0.8f,	0.0f,	//4: Pico superior

	};
	Mesh* piramidecuadrangular = new Mesh();
	piramidecuadrangular->CreateMesh(cuadrangular_vertices, cuadrangular_indices, 15, 18);
	meshList.push_back(piramidecuadrangular);
}

//Vértices de un cubo
void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3
	};

	GLfloat cubo_vertices[] = {
		// front
		-0.5f, -0.5f,  0.5f,
		0.5f, -0.5f,  0.5f,
		0.5f,  0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,
		// back
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f,  0.5f, -0.5f,
		-0.5f,  0.5f, -0.5f
	};
	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 24, 36);
	meshList.push_back(cubo);
}

void CrearLetrasyFiguras()
{
	GLfloat vertices_L[] = {
		// X         Y            Z		 R		 G		 B
		//Letra L [0]
		-0.9f,	   0.9f,		0.0f,	1.0f,	1.0f,	0.0f,
		-0.7f,	   0.9f,		0.0f,	1.0f,	1.0f,	0.0f,
		-0.9f,	   0.1f,		0.0f,	1.0f,	1.0f,	0.0f,
		
		-0.7f,	   0.9f,		0.0f,	1.0f,	1.0f,	0.0f,
		-0.9f,	   0.1f,		0.0f,	1.0f,	1.0f,	0.0f,
		-0.7f,	   0.1f,		0.0f,	1.0f,	1.0f,	0.0f,

		-0.7f,	   0.1f,		0.0f,	1.0f,	1.0f,	0.0f,
		-0.1f,	   0.1f,		0.0f,	1.0f,	1.0f,	0.0f,
		-0.7f,	   0.3f,		0.0f,	1.0f,	1.0f,	0.0f,

		-0.7f,	   0.3f,		0.0f,	1.0f,	1.0f,	0.0f,
		-0.1f,	   0.3f,		0.0f,	1.0f,	1.0f,	0.0f,
		-0.1f,	   0.1f,		0.0f,	1.0f,	1.0f,	0.0f,
	};
	MeshColor* letra_L = new MeshColor();
	letra_L->CreateMeshColor(vertices_L, 72);
	meshColorList.push_back(letra_L);

	GLfloat vertices_C[] = {
		// X         Y            Z		R		G		B
		//Letra C [1]
		0.9f,	   0.9f,		0.0f,	1.0f,	0.64f,	0.0f,
		0.3f,	   0.9f,		0.0f,	1.0f,	0.64f,	0.0f,
		0.9f,	   0.7f,		0.0f,	1.0f,	0.64f,	0.0f,

		0.9f,	   0.7f,		0.0f,	1.0f,	0.64f,	0.0f,
		0.3f,	   0.7f,		0.0f,	1.0f,	0.64f,	0.0f,
		0.3f,	   0.9f,		0.0f,	1.0f,	0.64f,	0.0f,

		0.1f,	   0.9f,		0.0f,	1.0f,	0.64f,	0.0f,
		0.1f,	   0.1f,		0.0f,	1.0f,	0.64f,	0.0f,
		0.3f,	   0.9f,		0.0f,	1.0f,	0.64f,	0.0f,

		0.1f,	   0.1f,		0.0f,	1.0f,	0.64f,	0.0f,
		0.3f,	   0.1f,		0.0f,	1.0f,	0.64f,	0.0f,
		0.3f,	   0.9f,		0.0f,	1.0f,	0.64f,	0.0f,

		0.9f,	   0.3f,		0.0f,	1.0f,	0.64f,	0.0f,
		0.3f,	   0.3f,		0.0f,	1.0f,	0.64f,	0.0f,
		0.9f,	   0.1f,		0.0f,	1.0f,	0.64f,	0.0f,

		0.9f,	   0.1f,		0.0f,	1.0f,	0.64f,	0.0f,
		0.3f,	   0.1f,		0.0f,	1.0f,	0.64f,	0.0f,
		0.3f,	   0.3f,		0.0f,	1.0f,	0.64f,	0.0f
	};
	MeshColor* letra_C = new MeshColor();
	letra_C->CreateMeshColor(vertices_C, 108);
	meshColorList.push_back(letra_C);

	GLfloat vertices_H[] = {
		// X         Y            Z		R		G		B
		//Letra H [2]
		-0.9f,	  -0.9f,		0.0f,	0.6f,	0.6f,	0.6f,
		-0.7f,	  -0.9f,		0.0f,	0.6f,	0.6f,	0.6f,
		-0.9f,	  -0.1f,		0.0f,	0.6f,	0.6f,	0.6f,

		-0.7f,	  -0.9f,		0.0f,	0.6f,	0.6f,	0.6f,
		-0.9f,	  -0.1f,		0.0f,	0.6f,	0.6f,	0.6f,
		-0.7f,	  -0.1f,		0.0f,	0.6f,	0.6f,	0.6f,

		-0.3f,	  -0.4f,		0.0f,	0.6f,	0.6f,	0.6f,
		-0.3f,	  -0.6f,		0.0f,	0.6f,	0.6f,	0.6f,
		-0.7f,	  -0.4f,		0.0f,	0.6f,	0.6f,	0.6f,

		-0.7f,	  -0.6f,		0.0f,	0.6f,	0.6f,	0.6f,
		-0.7f,	  -0.4f,		0.0f,	0.6f,	0.6f,	0.6f,
		-0.3f,	  -0.6f,		0.0f,	0.6f,	0.6f,	0.6f,

		-0.3f,	  -0.9f,		0.0f,	0.6f,	0.6f,	0.6f,
		-0.1f,	  -0.9f,		0.0f,	0.6f,	0.6f,	0.6f,
		-0.3f,	  -0.1f,		0.0f,	0.6f,	0.6f,	0.6f,

		-0.1f,	  -0.9f,		0.0f,	0.6f,	0.6f,	0.6f,
		-0.3f,	  -0.1f,		0.0f,	0.6f,	0.6f,	0.6f,
		-0.1f,	  -0.1f,		0.0f,	0.6f,	0.6f,	0.6f
	};
	MeshColor* letra_H = new MeshColor();
	letra_H->CreateMeshColor(vertices_H, 108);
	meshColorList.push_back(letra_H);
}

void CreateShaders()
{
	//Rojo
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShaderRojo, fShader);
	shaderList.push_back(*shader1);

	Shader* shader2 = new Shader();//shader para usar color como parte del VAO: letras 
	shader2->CreateFromFiles(vShaderColor, fShaderColor);
	shaderList.push_back(*shader2);

	//Verde
	Shader* shader3 = new Shader();
	shader3->CreateFromFiles(vShaderVerde, fShader);
	shaderList.push_back(*shader3);

	//Azul
	Shader* shader4 = new Shader();
	shader4->CreateFromFiles(vShaderAzul, fShader);
	shaderList.push_back(*shader4);

	//Cafe
	Shader* shader5 = new Shader();
	shader5->CreateFromFiles(vShaderCafe, fShader);
	shaderList.push_back(*shader5);

	//Verde oscuro
	Shader* shader6 = new Shader();
	shader6->CreateFromFiles(vShaderVerdeOscuro, fShader);
	shaderList.push_back(*shader6);

}


int main()
{
	mainWindow = Window(800, 600);
	mainWindow.Initialise();
	CreaPiramideCuadrangular(); //índice 0 en MeshList
	CrearCubo();//índice 1 en MeshList
	CrearLetrasyFiguras(); //usa MeshColor, índices en MeshColorList
	CreateShaders();
	GLuint uniformProjection = 0;
	GLuint uniformModel = 0;
	//Projection: Matriz de Dimensión 4x4 para indicar si vemos en 2D( orthogonal) o en 3D) perspectiva
	//glm::mat4 projection = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 100.0f);
	glm::mat4 projection = glm::perspective(glm::radians(60.0f),
		mainWindow.getBufferWidth() / mainWindow.getBufferHeight(),
		0.1f, 100.0f);

	//Model: Matriz de Dimensión 4x4 en la cual se almacena la multiplicación de las transformaciones geométricas.
	glm::mat4 model(1.0); //fuera del while se usa para inicializar la matriz con una identidad

	//Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		//Recibir eventos del usuario
		glfwPollEvents();
		//Limpiar la ventana
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Se agrega limpiar el buffer de profundidad


		//Para las letras hay que usar el segundo set de shaders con índice 1 en ShaderList 
		shaderList[1].useShader();
		uniformModel = shaderList[1].getModelLocation();
		uniformProjection = shaderList[1].getProjectLocation();

		//Letras

		//Letra L
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 0.7f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[0]->RenderMeshColor();

		//Letra C
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(1.0f, 0.7f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[1]->RenderMeshColor();

		//Letra H
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.0f, 0.7f, -3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshColorList[2]->RenderMeshColor();

		//Casa

		//Techo
		//Para la piramide azul, se utiliza el cuarto set de shaders con índice 3 en ShaderList
		shaderList[3].useShader();
		uniformModel = shaderList[3].getModelLocation();
		uniformProjection = shaderList[3].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 0.7f, -3.0f));
		model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();


		//Cubo
		shaderList[0].useShader();
		uniformModel = shaderList[0].getModelLocation();
		uniformProjection = shaderList[0].getProjectLocation();
		angulo += 0.1;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, -2.3f));
		//model = glm::scale(model, glm::vec3(0.9f, 0.9f, 0.9f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Ventana 1
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.22f, -0.22f, -1.8f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Ventana 2
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.24f, -0.22f, -1.8f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Puerta
		shaderList[2].useShader();
		uniformModel = shaderList[2].getModelLocation();
		uniformProjection = shaderList[2].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -0.727f, -1.8f));
		model = glm::scale(model, glm::vec3(0.35f, 0.35f, 0.35f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Tronco 1
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.75f, -0.67f, -1.5f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Tronco 2
		shaderList[4].useShader();
		uniformModel = shaderList[4].getModelLocation();
		uniformProjection = shaderList[4].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, -0.67f, -1.5f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[1]->RenderMesh();

		//Pino 1
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-0.75f, -0.4f, -1.55f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		//Pino 2
		shaderList[5].useShader();
		uniformModel = shaderList[5].getModelLocation();
		uniformProjection = shaderList[5].getProjectLocation();
		angulo += 0.01;
		//Inicializar matriz de dimensión 4x4 que servirá como matriz de modelo para almacenar las transformaciones geométricas
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.75f, -0.4f, -1.55f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		meshList[0]->RenderMesh();

		glUseProgram(0);
		mainWindow.swapBuffers();

	}
	return 0;
}
// inicializar matriz: glm::mat4 model(1.0);
// reestablecer matriz: model = glm::mat4(1.0);
//Traslación
//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
//////////////// ROTACIÓN //////////////////
//model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
////////////////  ESCALA ////////////////
//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
///////////////////// T+R////////////////
/*model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::rotate(model, glm::radians(angulo), glm::vec3(0.0f, 1.0f, 0.0f));
*/
/////////////R+T//////////
/*model = glm::rotate(model, 45 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
model = glm::translate(model, glm::vec3(valor, 0.0f, 0.0f));
*/