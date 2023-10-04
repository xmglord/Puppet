#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <stack>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include <glm\glm\glm.hpp>
#include <glm\glm\gtc\matrix_transform.hpp>
#include <glm\glm\gtc\type_ptr.hpp>
 
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

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
float angCodoL = 0.0f;
float angMunecaR = 0.0f;
float angMunecaL = 0.0f;

//
bool drawFigure = true;
bool reset = false;
float color[4] = { 12/255.0f, 24/255.0f, 40/255.0f, 1.0f };

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
	mainWindow = Window(800, 800);
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.5f);

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0;
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 100.0f);

	// Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
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
		ImGui::Begin("Controles");
		// Text that appears in the window
		ImGui::Text("Marioneta...");
		// Checkbox that appears in the window
		ImGui::Checkbox("Dibujar", &drawFigure);
		ImGui::Checkbox("Reset", &reset);
		// Slider that appears in the window
		ImGui::SliderFloat("Torso", &angTorzo, -75.0f, 75.0f);
		ImGui::SliderFloat("Hombro", &angHombro, -65.0f, 65.0f);
		ImGui::SliderFloat("Cabeza", &angCabeza, -75.0f, 75.0f);
		ImGui::SliderFloat("Cabeza vert", &angCabeza2, -40.0f, 40.0f);
		ImGui::SliderFloat("Cadera", &angCadera, -65.0f, 65.0f);
		ImGui::SliderFloat("Pierna d", &angCaderaR, -65.0f, 65.0f);
		ImGui::SliderFloat("Pierna i", &angCaderaL, -65.0f, 65.0f);
		ImGui::SliderFloat("Rodilla d", &angRodillaR, 0.0f, 65.0f);
		ImGui::SliderFloat("Rodilla i", &angRodillaL, 0.0f, 65.0f);
		ImGui::SliderFloat("Tobillo d", &angTobilloR, -10.0f, 65.0f);
		ImGui::SliderFloat("Tobillo i", &angTobilloL, -10.0f, 65.0f);
		ImGui::SliderFloat("Brazo d", &angHombroR, -130.0f, 65.0f);
		ImGui::SliderFloat("Brazo i", &angHombroL, -130.0f, 65.0f);
		ImGui::SliderFloat("Antebrazo d", &angCodoR, 0.0f, 65.0f);
		ImGui::SliderFloat("Antebrazo i", &angCodoL, 0.0f, -65.0f);
		ImGui::SliderFloat("Mano d", &angMunecaR, -10.0f, 65.0f);
		ImGui::SliderFloat("Mano i", &angMunecaL, -10.0f, 65.0f);

		ImGui::ColorEdit4("Color", color);

		// Ends the window
		ImGui::End();

		// Renders the ImGUI elements
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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
			angCodoL = 0.0f;
			angMunecaR = 0.0f;
			angMunecaL = 0.0f;


			drawFigure = true;
			reset = false;
		}


		if (drawFigure) {
			// Matrices
			glm::mat4 Identidad(1.0f); /////// This
			glm::mat4 aux(1.0f); /////// This 

			std::stack<glm::mat4> Tranformaciones; /////// This 
			
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
					aux = glm::translate(Tranformaciones.top(), glm::vec3(5.0f, 2.0f, 1.5f));
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
					
					// Brazo Derecho
					aux = glm::translate(Tranformaciones.top(), glm::vec3(1.25f, 0.0f, 1.5f));
					aux = glm::rotate(aux, angHombroR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
					aux = glm::translate(aux, glm::vec3(-1.25f, -3.5, -1.5));
					Tranformaciones.push(aux);

					aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 3.5f, 2.0f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
					glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
					glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
					ListaMesh[0]->RenderMesh();

						// Antebrazo Derecho 
						aux = glm::translate(Tranformaciones.top(), glm::vec3(1.25f, 0.0f, 1.5f));
						aux = glm::rotate(aux, angCodoR * toRadians, glm::vec3(-1.0f, 0.0f, 0.0f));
						aux = glm::translate(aux, glm::vec3(-1.2f, -4.0, -1.2));
						Tranformaciones.push(aux);

						aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 4.0f, 2.0f));
						glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
						glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
						glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
						ListaMesh[0]->RenderMesh();

							//Mano Derecha
							aux = glm::translate(Tranformaciones.top(), glm::vec3(1.0f, 0.0f, 1.2f));
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
					
					// Brazo Izquierdo 
					aux = glm::translate(Tranformaciones.top(), glm::vec3(8.75f, 0.0f, 1.5f));
					aux = glm::rotate(aux, angHombroL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
					aux = glm::translate(aux, glm::vec3(-0.8f, -3.5, -1.5));
					Tranformaciones.push(aux);

					aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 3.5f, 2.0f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
					glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
					glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
					ListaMesh[0]->RenderMesh();

						// Antebrazo Izquierdo	
						aux = glm::translate(Tranformaciones.top(), glm::vec3(1.25f, 0.0f, 1.5f));
						aux = glm::rotate(aux, angCodoL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
						aux = glm::translate(aux, glm::vec3(-1.2f, -4.0, -1.2));
						Tranformaciones.push(aux);

						aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 4.0f, 2.0f));
						glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
						glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
						glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
						ListaMesh[0]->RenderMesh();

							// Mano Izquierda
							aux = glm::translate(Tranformaciones.top(), glm::vec3(1.0f, 0.0f, 1.2f));
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
				
					// Pierna Izquierda
					aux = glm::translate(Tranformaciones.top(), glm::vec3(3.75f, 0.0f, 1.5f));
					aux = glm::rotate(aux, angCaderaL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
					aux = glm::translate(aux, glm::vec3(-1.25f, -5.0f, -1.5));
					Tranformaciones.push(aux);

					aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 5.0f, 2.0f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
					glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
					glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
					ListaMesh[0]->RenderMesh();
					
						// Pantorrilla Izquierda	
						aux = glm::translate(Tranformaciones.top(), glm::vec3(1.25f, 0.0f, 1.5f));
						aux = glm::rotate(aux, angRodillaL * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
						aux = glm::translate(aux, glm::vec3(-1.0f, -4.5f, -1.5f));
						Tranformaciones.push(aux);
						 
						aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 4.5f, 2.0f));
						glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
						glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
						glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
						ListaMesh[0]->RenderMesh();
							
							// Pie Izquierdo 
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
					// Pierna Derecha	 
					aux = glm::translate(Tranformaciones.top(), glm::vec3(1.25f, 0.0f, 1.5f));
					aux = glm::rotate(aux, angCaderaR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
					aux = glm::translate(aux, glm::vec3(-1.25f, -5.0f, -1.5));
					Tranformaciones.push(aux);

					aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 5.0f, 2.0f));
					glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
					glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
					glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
					ListaMesh[0]->RenderMesh();

						// Pantorrilla Derecha	
						aux = glm::translate(Tranformaciones.top(), glm::vec3(1.25f, 0.0f, 1.5f));
						aux = glm::rotate(aux, angRodillaR * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
						aux = glm::translate(aux, glm::vec3(-1.0f, -4.5f, -1.5f));
						Tranformaciones.push(aux);
						 
						aux = glm::scale(Tranformaciones.top(), glm::vec3(2.0f, 4.5f, 2.0f));
						glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(aux));
						glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
						glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
						ListaMesh[0]->RenderMesh(); 

							// Pie DERECHOO :D
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