/*
* Laboratorio de Computación Gráfica e Interacción Humano-Computadora
* 06 - Modelos de Iluminación
*/

#include <iostream>
#include <stdlib.h>

// GLAD: Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator
// https://glad.dav1d.de/
#include <glad/glad.h>

// GLFW: https://www.glfw.org/
#include <GLFW/glfw3.h>

// GLM: OpenGL Math library
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Model loading classes
#include <shader_m.h>
#include <camera.h>
#include <model.h>
#include <material.h>
#include <light.h>
#include <cubemap.h>

#include <irrKlang.h>
using namespace irrklang;

// Max number of bones
#define MAX_RIGGING_BONES 100

// Functions
bool Start();
bool Update();

// Definición de callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// Gobals
GLFWwindow* window;

// Tamaño en pixeles de la ventana
const unsigned int SCR_WIDTH = 1024;
const unsigned int SCR_HEIGHT = 768;

// Definición de cámara (posición en XYZ)
Camera camera(glm::vec3(0.0f, 2.0f, 10.0f));

// Controladores para el movimiento del mouse
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// Variables para la velocidad de reproducción
// de la animación
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float elapsedTime = 0.0f;

glm::vec3 position(0.0f,0.0f, 0.0f);
glm::vec3 forwardView(0.0f, 0.0f, 1.0f);
float     scaleV = 0.005f;
float     rotateCharacter = 0.0f;
float     rotateTable = 0.0f;
glm::vec3 positionTable(0.0f, 0.0f, 0.0f);
glm::vec3 boundingboxMin(-9.0f, 0.0f, -9.0f);
glm::vec3 boundingboxMax( 9.0f, 0.0f,  9.0f);

// Shaders
Shader *ourShader;
Shader *staticShader;
Shader *phongShader;
Shader *fresnelShader;
Shader *cubemapShader;

// Carga la información del modelo
Model	*character;
Model	*house;
Model	*table;
Model   *sphere;
Model   *barra;

float tradius = 10.0f;
float theta = 0.0f;
float alpha = 0.0f;

// Cubemap
CubeMap *mainCubeMap;

// Materiales
Material material01;
Light    light01;

// Pose inicial del modelo
glm::mat4 gBones[MAX_RIGGING_BONES];
glm::mat4 gBonesBar[MAX_RIGGING_BONES];

float	fps = 0.0f, fpsBar = 0.0f;
int		keys = 0, keysBar = 0;
int		animationCount = 0, animationCountBar = 0;

glm::vec3 force(0.0f,0.0f,0.0f);
glm::vec3 acceleration(0.0f, 0.0f, 0.0f);
glm::vec3 velocity(0.0f, 0.0f, 0.0f);
glm::vec3 gravity(0.0f, -9.81f, 0.0f);
float mass = 1.0f;

// Audio
ISoundEngine *SoundEngine = createIrrKlangDevice();

// Entrada a función principal
int main()
{
	if (!Start())
		return -1;

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		if (!Update())
			break;
	}

	glfwTerminate();
	return 0;

}

bool Start() {
	// Inicialización de GLFW

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creación de la ventana con GLFW
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "FBX Animation with OpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Ocultar el cursor mientras se rota la escena
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: Cargar todos los apuntadores
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return false;
	}

	// Activación de buffer de profundidad
	glEnable(GL_DEPTH_TEST);

	// Compilación y enlace de shaders
	ourShader     = new Shader("shaders/10_vertex_skinning-IT.vs", "shaders/10_fragment_skinning-IT.fs");
	staticShader  = new Shader("shaders/10_vertex_simple.vs", "shaders/10_fragment_simple.fs");
	phongShader   = new Shader("shaders/11_BasicPhongShader.vs", "shaders/11_BasicPhongShader.fs");
	fresnelShader = new Shader("shaders/11_Fresnel.vs", "shaders/11_Fresnel.fs");
	cubemapShader = new Shader("shaders/10_vertex_cubemap.vs", "shaders/10_fragment_cubemap.fs");

	// Máximo número de huesos: 100
	ourShader->setBonesIDs(MAX_RIGGING_BONES);

	// Dibujar en malla de alambre
	// glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	house = new Model("models/interaction/House02.fbx");
	table = new Model("models/interaction/Table02.fbx");
	character = new Model("models/interaction/Maria02.fbx");
	sphere = new Model("models/interaction/Sphere.fbx");
	barra = new Model("models/interaction/Character.fbx");

	// Cubemap
	vector<std::string> faces
	{
		"textures/cubemap/01/posx.png",
		"textures/cubemap/01/negx.png",
		"textures/cubemap/01/posy.png",
		"textures/cubemap/01/negy.png",
		"textures/cubemap/01/posz.png",
		"textures/cubemap/01/negz.png"
	};
	mainCubeMap = new CubeMap();
	mainCubeMap->loadCubemap(faces);
	
	// time, arrays
	character->SetPose(0.0f, gBones);
	barra->SetPose(0.0f, gBonesBar);

	fps = (float)character->getFramerate();
	keys = (int)character->getNumFrames();

	fpsBar = (float)barra->getFramerate();
	keysBar = (int)barra->getNumFrames();

	SoundEngine->play2D("sound/EternalGarden.mp3", true);

	return true;
}

bool Update() {
	// Cálculo del framerate
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	elapsedTime += deltaTime;
	if (elapsedTime > 1.0f / fps) {
		animationCount++;
		if (animationCount > keys - 1) {
			animationCount = 0;
		}
		animationCountBar++;
		if (animationCountBar > keysBar - 1) {
			animationCountBar = 0;
		}
		// Configuración de la pose en el instante t
		character->SetPose((float)animationCount, gBones);
		barra->SetPose((float)animationCountBar, gBonesBar);
		elapsedTime = 0.0f;
	}

	// Procesa la entrada del teclado o mouse
	processInput(window);

	// Renderizado R - G - B - A
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Cubemap (fondo)
	{
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		mainCubeMap->drawCubeMap(*cubemapShader, projection, view);
	}
	
	// Objeto estático (casa)
	{
		// Activamos el shader del plano
		staticShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader->setMat4("projection", projection);
		staticShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		staticShader->setMat4("model", model);

		house->Draw(*staticShader);
	}

	glUseProgram(0);

	// Objeto animado
	{
		// Activación del shader del personaje
		ourShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader->setMat4("projection", projection);
		ourShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position); // translate it down so it's at the center of the scene
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down

		ourShader->setMat4("model", model);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBones);

		// Dibujamos el modelo
		character->Draw(*ourShader);
	}

	// Ciclo de animación de la barra
	{
		// Activación del shader del personaje
		ourShader->use();

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader->setMat4("projection", projection);
		ourShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, position); // translate it down so it's at the center of the scene
		//model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));	// it's a bit too big for our scene, so scale it down

		ourShader->setMat4("model", model);

		ourShader->setMat4("gBones", MAX_RIGGING_BONES, gBonesBar);

		// Dibujamos el modelo
		barra->Draw(*ourShader);
	}

	// Desactivamos el shader actual
	glUseProgram(0);

	/*{
		// Activamos el shader de Phong
		phongShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		phongShader->setMat4("projection", projection);
		phongShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotateTable), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		phongShader->setMat4("model", model);

		// Configuramos propiedades de fuentes de luz
		phongShader->setVec4("LightColor", light01.Color);
		phongShader->setVec4("LightPower", light01.Power);
		phongShader->setInt("alphaIndex", light01.alphaIndex);
		phongShader->setFloat("distance", light01.distance);
		phongShader->setVec3("lightPosition", light01.Position);
		phongShader->setVec3("lightDirection", light01.Direction);
		phongShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales
		phongShader->setVec4("MaterialAmbientColor", material01.ambient);
		phongShader->setVec4("MaterialDiffuseColor", material01.diffuse);
		phongShader->setVec4("MaterialSpecularColor", material01.specular);
		phongShader->setFloat("transparency", material01.transparency);

		table->Draw(*phongShader);

	}*/

	// Dibujamos una mesa
	{
		// Activamos el shader de fresnel
		fresnelShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		fresnelShader->setMat4("projection", projection);
		fresnelShader->setMat4("view", view);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, positionTable);
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotateTable), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		fresnelShader->setMat4("model", model);
		fresnelShader->setFloat("transparency", material01.transparency);

		glDepthMask(GL_FALSE);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mainCubeMap->textureID);
		glDepthMask(GL_TRUE);
		table->Draw(*fresnelShader);

	}

	glUseProgram(0);

	// Despliegue del objeto esfera
	{
		// Activamos el shader de la esfera
		phongShader->use();

		// Activamos para objetos transparentes
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Aplicamos transformaciones de proyección y cámara (si las hubiera)
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		phongShader->setMat4("projection", projection);
		phongShader->setMat4("view", view);

		phongShader->setFloat("tradius", tradius);
		phongShader->setFloat("theta", theta);
		phongShader->setFloat("alpha", alpha);

		// Aplicamos transformaciones del modelo
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(tradius * glm::cos(theta), 0.0f, tradius * glm::sin(theta))); // translate it down so it's at the center of the scene
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
		phongShader->setMat4("model", model);

		alpha += 0.01f;
		theta += 0.0001f;

		// Configuramos propiedades de fuentes de luz
		phongShader->setVec4("LightColor", light01.Color);
		phongShader->setVec4("LightPower", light01.Power);
		phongShader->setInt("alphaIndex", light01.alphaIndex);
		phongShader->setFloat("distance", light01.distance);
		phongShader->setVec3("lightPosition", light01.Position);
		phongShader->setVec3("lightDirection", light01.Direction);
		phongShader->setVec3("eye", camera.Position);

		// Aplicamos propiedades materiales
		phongShader->setVec4("MaterialAmbientColor", material01.ambient);
		phongShader->setVec4("MaterialDiffuseColor", material01.diffuse);
		phongShader->setVec4("MaterialSpecularColor", material01.specular);
		phongShader->setFloat("transparency", material01.transparency);

		sphere->Draw(*phongShader);

	}
	
	glUseProgram(0);

	// glfw: swap buffers 
	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}

// Procesamos entradas del teclado
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);

	// Character movement
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position = position + scaleV * forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position = position - scaleV * forwardView;
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		if (positionTable.z > boundingboxMin.z) {
			positionTable.z -= 0.01;
		}
		else {
			//positionTable.z = boundingboxMin.z;
			//light01.Color = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f);

			// fuerzas actuantes
			force = mass * gravity;
			acceleration = force / mass;
			velocity += acceleration * deltaTime;
			positionTable += velocity * deltaTime;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		if (positionTable.z < boundingboxMax.z)
			positionTable.z += 0.01;
		else
			positionTable.z = boundingboxMax.z;
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		positionTable.x -= 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
		positionTable.x += 0.01;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		rotateCharacter += 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		rotateCharacter -= 0.5f;

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(rotateCharacter), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::vec4 viewVector = model * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
		forwardView = glm::vec3(viewVector);
		forwardView = glm::normalize(forwardView);
	}

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	{
		rotateTable += 0.05f;
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	{
		rotateTable -= 0.05f;
	}
	
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
		light01.Position.x += 0.1;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		light01.Position.x -= 0.1;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		light01.Position.y += 0.1;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		light01.Position.y -= 0.1;
	
}

// glfw: Actualizamos el puerto de vista si hay cambios del tamaño
// de la ventana
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

// glfw: Callback del movimiento y eventos del mouse
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = (float)xpos;
		lastY = (float)ypos;
		firstMouse = false;
	}

	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos; 

	lastX = (float)xpos;
	lastY = (float)ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: Complemento para el movimiento y eventos del mouse
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll((float)yoffset);
}
