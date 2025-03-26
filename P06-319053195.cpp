/*
Práctica 6: Texturizado
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_m.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;
Texture dadoTexture;
Texture logofiTexture;

Model Kitt_M;
Model Llanta_M;
Model Dado_M;
Model CuerpoCarro_M;
Model BumperCarro_M;
Model CofreCarro_M;
Model LucesDelanterasCarro_M;
Model LucesTraserasCarro_M;
Model EspejosCarro_M;
Model Llanta1Carro_M;
Model Llanta2Carro_M;
Model Llanta3Carro_M;
Model Llanta4Carro_M;

Skybox skybox;

//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_texture.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_texture.frag";




//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}



void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);


	
	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}


void CreateShaders()
{
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}

void CrearDado()
{
	unsigned int octaedro_indices[] = {
		//Parte Superior
		//Adelante derecha
		0, 1, 2,
		//Adelante izquierda
		3, 4, 5,
		//Atras derecha
		6, 7, 8,
		//Atras izquierda
		9, 10, 11,

		//Parte Inferior
		//Adelante derecha
		12, 13, 14,
		//Adelante izquierda
		15, 16, 17,
		//Atras derecha
		18, 19, 20,
		//Atras izquierda
		21, 22, 23,

	};

	GLfloat octaedro_vertices[] = {
		//x		y		z		 S		 T			NX		NY		NZ
		//Parte Superior
		//Adelante Derecha
		0.0f, 1.0f,  0.0f,		0.03f,	0.35f,		0.0f,  0.0f, 0.0f,
		0.0f, 0.0f,  1.0f,		0.26f,	0.35f,		0.0f,  0.0f, 0.0f,
		1.0f, 0.0f,  0.0f,		0.14f,	0.64f,		0.0f,  0.0f, 0.0f,

		//Adelante izquierda
		0.0f, 1.0f,  0.0f,		0.28f,	0.36f,		0.0f,  0.0f, 0.0f,
		0.0f, 0.0f,  1.0f,		0.16f,	0.65f,		0.0f,  0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,		0.41f,	0.65f,		0.0f,  0.0f, 0.0f,

		//Atras derecha
		0.0f, 1.0f,  0.0f,		0.3f,	0.35f,		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f,  0.0f,		0.55f,	0.35f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,		0.425f,	0.64f,		0.0f, 0.0f, 0.0f,

		//Atras izquierda
		0.0f, 1.0f,  0.0f,		0.43f,	0.03f,		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,		0.3f,	0.32f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,		0.55f,	0.32f,		0.0f, 0.0f, 0.0f,

		//Parte Inferior
		//Adelante derecha
		0.0f, -1.0f, 0.0f,		0.44f,	0.675f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,  1.0f,		0.69f,	0.675f,		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f,  0.0f,		0.565f,	0.97f,		0.0f, 0.0f, 0.0f,

		//Adelante izquierda
		0.0f, -1.0f, 0.0f,		0.57f,	0.36f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,  1.0f,		0.44f,	0.65f,		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,		0.69f,	0.65f,		0.0f, 0.0f, 0.0f,

		//Atras derecha
		0.0f, -1.0f, 0.0f,		0.58f,	0.35f,		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f,  0.0f,		0.83f,	0.35f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,		0.71f,	0.63f,		0.0f, 0.0f, 0.0f,

		//Atras izquierda
		0.0f, -1.0f, 0.0f,		0.85f,	0.355f,		0.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,		0.73f,	0.65f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, -1.0f,		0.97f,	0.65f,		0.0f, 0.0f, 0.0f
	};

	Mesh* octaedro = new Mesh();
	octaedro->CreateMesh(octaedro_vertices, octaedro_indices, 192, 24);
	meshList.push_back(octaedro);

}


int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CrearDado();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.3f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();
	dadoTexture = Texture("Textures/octaedro.jpg");
	dadoTexture.LoadTexture();
	logofiTexture = Texture("Textures/escudo_fi_color.tga");
	logofiTexture.LoadTextureA();
	
	
	Kitt_M = Model();
	Kitt_M.LoadModel("Models/kitt_optimizado.obj");
	Llanta_M = Model();
	Llanta_M.LoadModel("Models/llanta_optimizada.obj");
	Dado_M = Model();
	Dado_M.LoadModel("Models/Dado_cars_obj.obj");
	CuerpoCarro_M = Model();
	CuerpoCarro_M.LoadModel("Models/CuerpoCarro_obj.obj");
	BumperCarro_M = Model();
	BumperCarro_M.LoadModel("Models/BumperCarro_obj.obj");
	CofreCarro_M = Model();
	CofreCarro_M.LoadModel("Models/CofreCarro_obj.obj");
	LucesDelanterasCarro_M = Model();
	LucesDelanterasCarro_M.LoadModel("Models/LucesDelanterasCarro_obj.obj");
	LucesTraserasCarro_M = Model();
	LucesTraserasCarro_M.LoadModel("Models/LucesTraserasCarro_obj.obj");
	EspejosCarro_M = Model();
	EspejosCarro_M.LoadModel("Models/EspejosCarro_obj.obj");
	Llanta1Carro_M = Model();
	Llanta1Carro_M.LoadModel("Models/Llanta1Carro_obj.obj");
	Llanta2Carro_M = Model();
	Llanta2Carro_M.LoadModel("Models/Llanta2Carro_obj.obj");
	Llanta3Carro_M = Model();
	Llanta3Carro_M.LoadModel("Models/Llanta3Carro_obj.obj");
	Llanta4Carro_M = Model();
	Llanta4Carro_M.LoadModel("Models/Llanta4Carro_obj.obj");

	
	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
	glm::mat4 model(1.0);
	glm::mat4 modelaux(1.0);
	glm::mat4 modelaux2(1.0);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformColor = shaderList[0].getColorLocation();
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		color = glm::vec3(1.0f, 1.0f, 1.0f);//color blanco, multiplica a la información de color de la textura

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		pisoTexture.UseTexture();
		meshList[2]->RenderMesh();

		/*
		//Ejercicio 1 Dado de 8 caras
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-1.5f, 4.5f, -2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		dadoTexture.UseTexture();
		meshList[4]->RenderMesh();
		*/

		//Ejercicio 2 y 3
		//Cuerpo del carro
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, 6.2f, 0.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		CuerpoCarro_M.RenderModel();

		//Bumper del carro
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -4.5f, 0.0f));
		modelaux2 = model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.0f, 0.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		BumperCarro_M.RenderModel();

		//Cofre del carro
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.8f, 6.3f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		CofreCarro_M.RenderModel();

		//Luces delanteras del carro
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 10.9f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		LucesDelanterasCarro_M.RenderModel();

		//Luces traseras del carro
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, -11.7f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 0.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		LucesTraserasCarro_M.RenderModel();

		//Espejos del carro
		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 1.0f, 5.2f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(0.667f, 0.859f, 0.992f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		EspejosCarro_M.RenderModel();

		//Llanta 1 del carro
		model = modelaux2;
		model = glm::translate(model, glm::vec3(4.4f, -1.3f, 9.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Llanta1Carro_M.RenderModel();

		//Llanta 2 del carro
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-4.4f, -1.3f, 9.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Llanta2Carro_M.RenderModel();

		//Llanta 3 del carro
		model = modelaux2;
		model = glm::translate(model, glm::vec3(4.1f, -1.3f, -8.7f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Llanta3Carro_M.RenderModel();

		//Llanta 4 del carro
		model = modelaux2;
		model = glm::translate(model, glm::vec3(-4.1f, -1.3f, -8.7f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		Llanta4Carro_M.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
/*
//blending: transparencia o traslucidez
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		logofiTexture.UseTexture(); //textura con transparencia o traslucidez
		FIGURA A RENDERIZAR de OpenGL, si es modelo importado no se declara UseTexture
		glDisable(GL_BLEND);
*/