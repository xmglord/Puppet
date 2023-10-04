#include <stdio.h>
#include <string.h>
#include <cmath>
#include <ctime>
#include <vector>
#include <stack>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>

#include "Window.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"

const float toRadians = 3.14159265f / 180.0f;

Window mainWindow;
std::vector<Mesh*> ListaMesh;
std::vector<Shader> ListaShader;
Camera camera;

GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;


// Vertex Shader
static const char* vShader = "Shaders/shader.vert";

// Fragment Shader
static const char* fShader = "Shaders/shader.frag";


// Angulo de articulaciones
float angCabeza = 0.0f;
float angCabeza2 = 0.0f;
float angTorzo = 0.0f;
float angCadera = 0.0f;
float angHombro = 0.0f;
float angTobilloR = 0.0f;
float angTobilloL = 0.0f;
float angRodillaR = 0.0f;
float angRodillaL = 0.0f;
float angCaderaR = 0.0f;
float angCaderaL = 0.0f;
float angHombroR = 0.0f;
float angHombroL = 0.0f;
float angCodoR = 0.0f;
float angCodoR2 = 0.0f;
float angCodoL = 0.0f;
float angCodoL2 = 0.0f;
float angMunecaR = 0.0f;
float angMunecaL = 0.0f;

bool drawFigure = true;
bool reset = false;
bool randomize = false;
float color[4] = { 0.1f, 0.0f, 0.0f, 1.0f };

void CreateObjects() {
	unsigned int indices[] = { // definimos el cubo unitario
		0, 1, 5,
		0, 4, 5,
		3, 0, 1,
		3, 2, 1,
		3, 7, 6,
		3, 2, 6,
		6, 5, 1,
		6, 2, 1,
		7, 4, 5,
		7, 6, 5,
		7, 4, 0,
		7, 3, 0
	};

	GLfloat vertices[] = {
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f
	};

	Mesh* obj1 = new Mesh(); 
	obj1->CreateMesh(vertices, indices, 36, 36);
	ListaMesh.push_back(obj1); 
}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	ListaShader.push_back(*shader1);
}

int main(){
	// Inicializar
	mainWindow = Window(1500, 800);
	mainWindow.Initialise();
	srand(time(NULL));

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(20.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsClassic();
	ImGui_ImplGlfw_InitForOpenGL(mainWindow.getmainWin(), true);
	ImGui_ImplOpenGL3_Init("#version 330");


	// Loop until window closed
	while (!mainWindow.getShouldClose()){
		GLfloat now = glfwGetTime(); // SDL_GetPerformanceCounter();
		deltaTime = now - lastTime; // (now - lastTime)*1000/SDL_GetPerformanceFrequency();
		lastTime = now;

		// Get + Handle User Input
		glfwPollEvents();

		camera.keyControl(mainWindow.getsKeys(), deltaTime);

		// Clear the window
		glClearColor(color[0], color[1], color[2], color[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glEnable(GL_DEPTH_TEST);

		// Tell OpenGL a new frame is about to begin
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame(); 

		//Usar Shader
		ListaShader[0].UseShader();
		uniformModel = ListaShader[0].GetModelLocation();
		uniformProjection = ListaShader[0].GetProjectionLocation();
		uniformView = ListaShader[0].GetViewLocation();

		// ImGUI window creation
		ImGui::Begin("Controls");
		// Text that appears in the window
		ImGui::Text("Puppet");
		// Checkbox that appears in the window
		ImGui::Checkbox("Lights On!", &drawFigure);
		if(ImGui::Button("Reset Position")){
			reset = true;
		}
		if (ImGui::Button("Randomize Values")) {
			randomize = true;
		}
		// Slider that appears in the window
		ImGui::SliderFloat("Torso", &angTorzo, -75.0f, 75.0f);
		ImGui::SliderFloat("Hombro", &angHombro, -65.0f, 65.0f);
		ImGui::SliderFloat("Cabeza", &angCabeza, -75.0f, 75.0f);
		ImGui::SliderFloat("Cabeza vertical", &angCabeza2, -40.0f, 40.0f);
		ImGui::SliderFloat("Cadera", &angCadera, -65.0f, 65.0f);
		ImGui::SliderFloat("Pierna derecha", &angCaderaR, -65.0f, 65.0f);
		ImGui::SliderFloat("Pierna izquierda", &angCaderaL, -65.0f, 65.0f);
		ImGui::SliderFloat("Rodilla derecha", &angRodillaR, 0.0f, 65.0f);
		ImGui::SliderFloat("Rodilla izquierda", &angRodillaL, 0.0f, 65.0f);
		ImGui::SliderFloat("Tobillo derecho", &angTobilloR, -10.0f, 65.0f);
		ImGui::SliderFloat("Tobillo izquierdo", &angTobilloL, -10.0f, 65.0f);
		ImGui::SliderFloat("Brazo derecho", &angHombroR, -130.0f, 65.0f);
		ImGui::SliderFloat("Brazo izquierdo", &angHombroL, -130.0f, 65.0f);
		ImGui::SliderFloat("Antebrazo derecho", &angCodoR, 0.0f, 65.0f);
		ImGui::SliderFloat("Antebrazo derecho rotar", &angCodoR2, -90.0f, 90.0f);
		ImGui::SliderFloat("Antebrazo izquierdo", &angCodoL, 0.0f, -65.0f);
		ImGui::SliderFloat("Antebrazo izquierdo rotar", &angCodoL2, -90.0f, 90.0f);
		ImGui::SliderFloat("Mano derecha", &angMunecaR, -10.0f, 65.0f);
		ImGui::SliderFloat("Mano izquierda", &angMunecaL, -65.0f, 10.0f);

		ImGui::ColorEdit4("Color", color);

		// Ends the window
		ImGui::End();

		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// posicion random
		if (randomize) {
			angTorzo = (rand() % 151) - 75.0f;
			angHombro = (rand() % 131) - 65.0f;
			angCabeza = (rand() % 151) - 75.0f;
			angCabeza2 = (rand() % 81) - 40.0f;
			angCadera = (rand() % 131) - 65.0f;
			angCaderaR = (rand() % 131) - 65.0f;
			angCaderaL = (rand() % 131) - 65.0f;
			angRodillaR = (rand() % 66);
			angRodillaL = (rand() % 66);
			angTobilloR = (rand() % 76) - 10.0f;
			angTobilloL = (rand() % 76) - 10.0f;
			angHombroR = (rand() % 196) - 130.0f;
			angHombroL = (rand() % 196) - 130.0f;
			angCodoR = (rand() % 66);
			angCodoR2 = (rand() % 181) - 90.0f;
			angCodoL = -1 * (rand() % 66);
			angCodoL2 = (rand() % 181) - 90.0f;
			angMunecaR = (rand() % 76) - 10.0f;
			angMunecaL = (rand() % 76) - 65.0f;

			randomize = false;  // Reset the flag
		}

		// Reinicia la marioneta
		if (reset) {
			angCabeza = 0.0f;
			angCabeza2 = 0.0f;
			angTorzo = 0.0f;
			angCadera = 0.0f;
			angTobilloR = 0.0f;
			angTobilloL = 0.0f;
			angRodillaR = 0.0f;
			angRodillaL = 0.0f;
			angCaderaR = 0.0f;
			angCaderaL = 0.0f;
			angHombro = 0.0f;
			angHombroL = 0.0f;
			angHombroR = 0.0f;
			angCodoR = 0.0f;
			angCodoR2 = 0.0f;
			angCodoL = 0.0f;
			angCodoL2 = 0.0f;
			angMunecaR = 0.0f;
			angMunecaR = 0.0f;
			angMunecaL = 0.0f;


			drawFigure = true;
			reset = false;
		}


		if (drawFigure) {
			// Matrices
			glm::mat4 Identidad(1.0f);
			glm::mat4 aux(1.0f);

			std::stack<glm::mat4> Tranformaciones;

			Tranformaciones.push(Identidad);

			// Torso
			aux = Identidad;
			aux = glm::translate(aux, glm::vec3(2.5, 0, 1.5));
			aux = glm::rotate(aux, angTorzo * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-2.5, 0, -1.5));  

			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(5.0f, 7.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();	

			// Hombros
			aux = Identidad;
			aux = glm::translate(aux, glm::vec3(2.5f, 7.0f, 1.5f));
			aux = glm::rotate(aux, angHombro * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-5.0f, 0.0f, -1.5f)); 

			Tranformaciones.push(Tranformaciones.top()*aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(10.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();

			//Cuello  
			aux = glm::translate(Tranformaciones.top(), glm::vec3(5.0f, 2.0f, 1.25f));
			aux = glm::rotate(aux, angCabeza * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-0.75f, 0.0f, -0.75));

			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(1.5f, 1.8f, 1.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();

			//Cabeza
			aux = glm::translate(Tranformaciones.top(), glm::vec3(0.75f, 1.8f, 0.75f));
			aux = glm::rotate(aux, angCabeza2 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-1.75f, 0.0f, -1.75));
			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(3.5f, 4.0f, 3.5f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh(); 

			Tranformaciones.pop();

			Tranformaciones.pop();

			// Brazo derecho
			aux = glm::translate(Tranformaciones.top(), glm::vec3(1.25f, 0.0f, 1.5f));
			aux = glm::rotate(aux, angHombroR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-1.25f, -3.5, -1.5));
			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 3.5f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();

			// Antebrazo derecho 
			aux = glm::translate(Tranformaciones.top(), glm::vec3(1.0f, 0.0f, 1.25f));
			aux = glm::rotate(aux, angCodoR * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
			aux = glm::rotate(aux, angCodoR2 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-1.0f, -4.0, -1.2));
			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 4.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();

			//Mano derecha
			aux = glm::translate(Tranformaciones.top(), glm::vec3(1.0f, -0.7f, 0.5f));
			aux = glm::rotate(aux, angMunecaR * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			aux = glm::translate(aux, glm::vec3(-1.0f, -1.3, -0.5));
			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();
			Tranformaciones.pop();

			Tranformaciones.pop();

			Tranformaciones.pop();

			// Brazo izquierdo 
			aux = glm::translate(Tranformaciones.top(), glm::vec3(8.75f, 0.0f, 1.5f));
			aux = glm::rotate(aux, angHombroL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-0.8f, -3.5, -1.5));
			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 3.5f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();

			// Antebrazo izquierdo	
			aux = glm::translate(Tranformaciones.top(), glm::vec3(1.0f, 0.0f, 1.25f));
			aux = glm::rotate(aux, angCodoL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			aux = glm::rotate(aux, angCodoL2 * toRadians, glm::vec3(0.0f, -1.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-1.0f, -4.0, -1.2));
			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 4.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();

			// Mano izquierda
			aux = glm::translate(Tranformaciones.top(), glm::vec3(1.0f, -1.0f, 0.5f));
			aux = glm::rotate(aux, angMunecaL * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
			aux = glm::translate(aux, glm::vec3(-1.0f, -1.0, -0.5));
			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 2.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();
			Tranformaciones.pop();

			Tranformaciones.pop();

			Tranformaciones.pop();

			Tranformaciones.pop();

			// Cadera 
			aux = glm::translate(Tranformaciones.top(), glm::vec3(2.5, 0.0f, 1.5));
			aux = glm::rotate(aux, angCadera * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-2.5, -1.5, -1.5));
			Tranformaciones.push(aux);

			aux=glm::scale(Tranformaciones.top(), glm::vec3(5.0f, 1.5f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();

			// Muslo izquierdo
			aux = glm::translate(Tranformaciones.top(), glm::vec3(4.25f, 0.0f, 1.5f));
			aux = glm::rotate(aux, angCaderaL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-1.25f, -5.0f, -1.5));
			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 5.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();

			// Pantorrilla izquierda	
			aux = glm::translate(Tranformaciones.top(), glm::vec3(1.0f, 0.0f, 1.5f));
			aux = glm::rotate(aux, angRodillaL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-1.0f, -4.5f, -1.5f));
			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 4.5f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();

			// Pie izquierdo 
			aux = glm::translate(Tranformaciones.top(), glm::vec3(1.0f, 0.0f, 1.5f));
			aux = glm::rotate(aux, angTobilloL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-1.0f, -1.0f, -1.5f));
			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 1.0f, 4.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();
			Tranformaciones.pop();

			Tranformaciones.pop(); 
			Tranformaciones.pop();
			// Muslo derecho
			aux = glm::translate(Tranformaciones.top(), glm::vec3(1.25f, 0.0f, 1.5f));
			aux = glm::rotate(aux, angCaderaR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-1.25f, -5.0f, -1.5));
			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 5.0f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();

			// Pantorrilla derecha	
			aux = glm::translate(Tranformaciones.top(), glm::vec3(1.0f, 0.0f, 1.5f));
			aux = glm::rotate(aux, angRodillaR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-1.0f, -4.5f, -1.5f));
			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 4.5f, 2.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh(); 

			// Pie derecho
			aux = glm::translate(Tranformaciones.top(), glm::vec3(1.0f, 0.0f, 1.5f));
			aux = glm::rotate(aux, angTobilloR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
			aux = glm::translate(aux, glm::vec3(-1.0f, -1.0f, -1.5f));
			Tranformaciones.push(aux);

			aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 1.0f, 4.0f));
			glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
			glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
			ListaMesh[0]->RenderMesh();


			Tranformaciones.pop();

			Tranformaciones.pop(); 

			Tranformaciones.pop();
		}
		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
