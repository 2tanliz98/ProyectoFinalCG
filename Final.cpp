/*---------------------------------------------------------*/
/* ----------------  Práctica                   -----------*/
/*-----------------    2023-1   ---------------------------*/
/*------------- Alumno:                     ---------------*/
/*------------- No. Cuenta                  ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 90.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	movAuto_x = 0.0f,
		movAuto_z = 0.0f,
		orienta = 0.0f;
bool	animacion = false,
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;


//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;
float	movBird_x = 0.0f, 
		movBird_y = 0.0f, 
		movBird_z = 0.0f,
		giroBird = 0;


#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	float movBird_x;		//Variable para PosicionX
	float movBird_y;		//Variable para PosicionY
	float movBird_z;		//Variable para PosicionZ
	float movBird_xInc;		//Variable para IncrementoX
	float movBird_yInc;		//Variable para IncrementoY
	float movBird_zInc;		//Variable para IncrementoZ
	float giroBird;
	float giroBirdInc;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void)
{
	printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].movBird_x = movBird_x;
	KeyFrame[FrameIndex].movBird_y = movBird_y;
	KeyFrame[FrameIndex].movBird_z = movBird_z;
	KeyFrame[FrameIndex].giroBird = giroBird;

	FrameIndex++;
}

void resetElements(void)
{
	movBird_x = KeyFrame[0].movBird_x;
	movBird_y = KeyFrame[0].movBird_y;
	movBird_z = KeyFrame[0].movBird_z;
	giroBird = KeyFrame[0].giroBird;
}

void interpolation(void)
{
	KeyFrame[playIndex].movBird_xInc = (KeyFrame[playIndex + 1].movBird_x - KeyFrame[playIndex].movBird_x) / i_max_steps;
	KeyFrame[playIndex].movBird_yInc = (KeyFrame[playIndex + 1].movBird_y - KeyFrame[playIndex].movBird_y) / i_max_steps;
	KeyFrame[playIndex].movBird_zInc = (KeyFrame[playIndex + 1].movBird_z - KeyFrame[playIndex].movBird_z) / i_max_steps;
	KeyFrame[playIndex].giroBirdInc = (KeyFrame[playIndex + 1].giroBird - KeyFrame[playIndex].giroBird) / i_max_steps;

}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;
			movBird_x += KeyFrame[playIndex].movBird_xInc;
			movBird_y += KeyFrame[playIndex].movBird_yInc;
			movBird_z += KeyFrame[playIndex].movBird_zInc;
			giroBird += KeyFrame[playIndex].giroBirdInc;

			i_curr_steps++;
		}
	}

	//Vehículo
	if (animacion)
	{
		movAuto_z += 3.0f;
	}
}

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/Piso.obj");
	

	
	

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		
	}
	/*
	KeyFrame[0].movBird_x = 0.0f;
	KeyFrame[0].movBird_y = 0.0f;
	KeyFrame[0].movBird_z = 0.0f;
	KeyFrame[0].giroBird = -90;


	KeyFrame[1].movBird_x = 20.0f;
	KeyFrame[1].movBird_y = 0.0f;
	KeyFrame[1].movBird_z = 0.0f;
	KeyFrame[1].giroBird = 0;

	KeyFrame[2].movBird_x = 30.0f;
	KeyFrame[2].movBird_y = 0.0f;
	KeyFrame[2].movBird_z = -5.5f;
	KeyFrame[2].giroBird = 10;

	KeyFrame[3].movBird_x = 40.0f;
	KeyFrame[3].movBird_y = 0.0f;
	KeyFrame[3].movBird_z = -5.5f;
	KeyFrame[3].giroBird = 15;

	KeyFrame[4].movBird_x = 45.f;
	KeyFrame[4].movBird_y = 0.0f;
	KeyFrame[4].movBird_z = -10.0f;
	KeyFrame[4].giroBird = 20;

	KeyFrame[5].movBird_x = 50.0f;
	KeyFrame[5].movBird_y = 0.0f;
	KeyFrame[5].movBird_z = -10.5f;
	KeyFrame[5].giroBird = 25;

	KeyFrame[6].movBird_x = 59.0f;
	KeyFrame[6].movBird_y = 0.0f;
	KeyFrame[6].movBird_z = -20.0f;
	KeyFrame[6].giroBird = 25;

	KeyFrame[7].movBird_x = 64.0f;
	KeyFrame[7].movBird_y = 0.0f;
	KeyFrame[7].movBird_z = -20.5f;
	KeyFrame[7].giroBird = 30;

	KeyFrame[8].movBird_x = 69.0f;
	KeyFrame[8].movBird_y = 0.0f;
	KeyFrame[8].movBird_z = -30.0f;
	KeyFrame[8].giroBird = 35;

	KeyFrame[9].movBird_x = 75.0f;
	KeyFrame[9].movBird_y = 0.0f;
	KeyFrame[9].movBird_z = -30.5f;
	KeyFrame[9].giroBird = 40;

	KeyFrame[10].movBird_x = 8.0f;
	KeyFrame[10].movBird_y = 0.0f;
	KeyFrame[10].movBird_z = -40.0f;
	KeyFrame[10].giroBird = 45;

	KeyFrame[11].movBird_x = 90.0f;
	KeyFrame[11].movBird_y = 0.0f;
	KeyFrame[11].movBird_z = -60.0f;
	KeyFrame[11].giroBird = 60;

	KeyFrame[12].movBird_x = 95.0f;
	KeyFrame[12].movBird_y = 0.0f;
	KeyFrame[12].movBird_z = -80.0f;
	KeyFrame[12].giroBird = 75;

	KeyFrame[13].movBird_x = 100.0f;
	KeyFrame[13].movBird_y = 0.0f;
	KeyFrame[13].movBird_z = -100.0f;
	KeyFrame[13].giroBird = 85;

	KeyFrame[14].movBird_x = 95.0f;
	KeyFrame[14].movBird_y = 0.0f;
	KeyFrame[14].movBird_z = -120.0f;
	KeyFrame[14].giroBird = 100;

	KeyFrame[15].movBird_x = 90.0f;
	KeyFrame[15].movBird_y = 0.0f;
	KeyFrame[15].movBird_z = -130.5f;
	KeyFrame[15].giroBird = 110;

	KeyFrame[16].movBird_x = 85.0f;
	KeyFrame[16].movBird_y = 0.0f;
	KeyFrame[16].movBird_z = -140.0f;
	KeyFrame[16].giroBird = 120;

	KeyFrame[17].movBird_x = 80.0f;
	KeyFrame[17].movBird_y = 0.0f;
	KeyFrame[17].movBird_z = -140.5f;
	KeyFrame[17].giroBird = 130;

	KeyFrame[18].movBird_x = 75.0f;
	KeyFrame[18].movBird_y = 0.0f;
	KeyFrame[18].movBird_z = -150.5f;
	KeyFrame[18].giroBird = 135;

	KeyFrame[19].movBird_x = 70.0f;
	KeyFrame[19].movBird_y = 0.0f;
	KeyFrame[19].movBird_z = -160.0f;
	KeyFrame[19].giroBird = 140;

	KeyFrame[20].movBird_x = 65.0f;
	KeyFrame[20].movBird_y = 0.0f;
	KeyFrame[20].movBird_z = -160.5f;
	KeyFrame[20].giroBird = 145;

	KeyFrame[21].movBird_x = 60.0f;
	KeyFrame[21].movBird_y = 0.0f;
	KeyFrame[21].movBird_z = -170.0f;
	KeyFrame[21].giroBird = 150;

	KeyFrame[22].movBird_x = 55.0f;
	KeyFrame[22].movBird_y = 0.0f;
	KeyFrame[22].movBird_z = -170.5f;
	KeyFrame[22].giroBird = 160;

	KeyFrame[23].movBird_x = 40.0f;
	KeyFrame[23].movBird_y = 0.0f;
	KeyFrame[23].movBird_z = -180.0f;
	KeyFrame[23].giroBird = 170;

	KeyFrame[24].movBird_x = 20.0f;
	KeyFrame[24].movBird_y = 0.0f;
	KeyFrame[24].movBird_z = -180.f;
	KeyFrame[24].giroBird = 180;

	KeyFrame[25].movBird_x = -20.0f;
	KeyFrame[25].movBird_y = 0.0f;
	KeyFrame[25].movBird_z = -170.5f;
	KeyFrame[25].giroBird = 195;

	KeyFrame[26].movBird_x = -40.0f;
	KeyFrame[26].movBird_y = 0.0f;
	KeyFrame[26].movBird_z = -170.0f;
	KeyFrame[26].giroBird = 205;

	KeyFrame[27].movBird_x = -55.0f;
	KeyFrame[27].movBird_y = 0.0f;
	KeyFrame[27].movBird_z = -160.5f;
	KeyFrame[27].giroBird = 215;

	KeyFrame[28].movBird_x = -70.0f;
	KeyFrame[28].movBird_y = 0.0f;
	KeyFrame[28].movBird_z = -150.0f;
	KeyFrame[28].giroBird = 230;

	KeyFrame[29].movBird_x = -75.0f;
	KeyFrame[29].movBird_y = 0.0f;
	KeyFrame[29].movBird_z = -140.0f;
	KeyFrame[29].giroBird = 240;

	KeyFrame[30].movBird_x = -80.0f;
	KeyFrame[30].movBird_y = 0.0f;
	KeyFrame[30].movBird_z = -130.0f;
	KeyFrame[30].giroBird = 250;

	KeyFrame[31].movBird_x = -85.5f;
	KeyFrame[31].movBird_y = 0.0f;
	KeyFrame[31].movBird_z = -120.0f;
	KeyFrame[31].giroBird = 260;

	KeyFrame[32].movBird_x = -90.0f;
	KeyFrame[32].movBird_y = 0.0f;
	KeyFrame[32].movBird_z = -100.0f;
	KeyFrame[32].giroBird = 270;

	KeyFrame[33].movBird_x = -85.5f;
	KeyFrame[33].movBird_y = 0.0f;
	KeyFrame[33].movBird_z = -80.0f;
	KeyFrame[33].giroBird = 275;

	KeyFrame[34].movBird_x = -80.0f;
	KeyFrame[34].movBird_y = 0.0f;
	KeyFrame[34].movBird_z = -60.0f;
	KeyFrame[34].giroBird = 280;

	KeyFrame[35].movBird_x = -75.5f;
	KeyFrame[35].movBird_y = 0.0f;
	KeyFrame[35].movBird_z = -40.0f;
	KeyFrame[35].giroBird = 290;

	KeyFrame[36].movBird_x = -70.0f;
	KeyFrame[36].movBird_y = 0.0f;
	KeyFrame[36].movBird_z = -30.5f;
	KeyFrame[36].giroBird = 300;

	KeyFrame[37].movBird_x = -60.0f;
	KeyFrame[37].movBird_y = 0.0f;
	KeyFrame[37].movBird_z = -30.0f;
	KeyFrame[37].giroBird = 330;

	KeyFrame[38].movBird_x = -30.0f;
	KeyFrame[38].movBird_y = 0.0f;
	KeyFrame[38].movBird_z = -10.0f;
	KeyFrame[38].giroBird = 350;

	KeyFrame[39].movBird_x = 0.0f;
	KeyFrame[39].movBird_y = 0.0f;
	KeyFrame[39].movBird_z = 0.0f;
	KeyFrame[39].giroBird = 360;*/
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.2f, 0.2f, 0.2f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.6f, 0.6f, 0.6f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.00032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.00032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(10.0f, 0.0f, -1.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.2f, 0.2f, 0.2f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(40.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(40.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);


		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		glm::vec3 lowerLight = camera.Position;

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f));
		staticShader.setMat4("model", model);
		//aviario.Draw(staticShader);


		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 10.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.0f + movAuto_x, -1.0f, movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		//carro.Draw(staticShader);


		model = glm::mat4(1.0);
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(lowerLight.x,10, lowerLight.z));
		model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
		staticShader.setMat4("model", model);
		//carro.Draw(staticShader);

		
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(posX-100, posY+60, posZ));
		model = glm::scale(model, glm::vec3(20.0f));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		//halcon.Draw(staticShader);

		
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente Práctica
		// -------------------------------------------------------------------------------------------------------------------------
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime+100);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime+100);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime+100);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime+100);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}