/*---------------------------------------------------------*/
/* ----------------  Proyecto Final Teoría                  -----------*/
/*-----------------    2023-1   ---------------------------*/
/*------------- Alumno:	PEÑALOZA LUGO TANIA LIZETH  
						HERNANDEZ CASTAÑEDA KEVIN REYNALDO---------------*/
/*------------- No. Cuenta: 316013929
							315084003			---------------*/

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
#include <irrKlang-32bit-1.6.0/irrKlang-1.6.0/include/irrKlang.h>






using namespace irrklang;


//#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor* monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(1600.0f, -100.0f, 3500.0f));
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
float	rotAlaD = 0.0f,
rotAlaI = 0.0f,
orienta = 0.0f;
bool	animacion = true,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;
//float x = 0.0f;
//float y = 0.0f;



//Keyframes (ManipulaciÃ³n y dibujo)
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
float	movBird_x = 240.0f,
movBird_y = -63.2f,
movBird_z = -4310.0f,
giroBird = 45.0f,
PI = 3.14159265359f;

float torsoLeonaY = 0.0f, //-209.0f
torsoLeonaZ = 3521.0f, //3521.0f
movPatasLeona1 = 0.0f,
movPatasLeona2 = 0.0f,
rodPatasLeona1 = 0.0f,
rodPatasLeona2 = 0.0f,
cabezaLeona = 0.0f,
mandibula = 0.0f,
cabezaLeonaZ;

float inctorsoLeonaY = 0.0f,
incTorsoLeonaZ = 0.0f,
incPatasLeona1 = 0.0f,
incPatasLeona2 = 0.0f,
incRodPatasLeona1 = 0.0f,
incRodPatasLeona2 = 0.0f,
incCabezaLeona = 0.0f,
incMandibula = 0.0f,
incCabezaLeonaZ;

//Mono
float rotaHD, rotaMD, rotaHI, rotaMI;

//Pinguino
float rotaPataP = 0, rotaPataP2 = 0,
rotaCuerpoP = 0, rotaCuerpoP2 = 0, rotaCabezap = 0, movXP = 0.0f, movYP = 0, movZP = 0;
bool pingu = true, animPingu = true, animPingu1 = false, animPingu2 = false, animPingu3 = false, animPingu4 = false; //Bandera
float tiempoP = 0.0f; // tiempo actual
float velocidadP = 0.7f; // velocidad de la animaciÃ³n
float radioP = 100.0f; // radio de la semi-circunferencia

//Elefante
float rotaE = 0, rotaE2 = 1;

#define MAX_FRAMES 44
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

	float torsoLeonaZ;	//
	float torsoLeonaY;
	float movPatasLeona1;
	float movPatasLeona2;
	float rodPatasLeona1;
	float rodPatasLeona2;
	float cabezaLeona;
	float mandibula;
	float cabezaLeonaZ;


}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 28;			//introducir datos
bool play = false;
int playIndex = 0;



void saveFrame(void)
{
	std::cout << "Frame Index = " << FrameIndex << std::endl;
	std::cout << "torsoLeonaZ = " << torsoLeonaZ << std::endl;
	std::cout << "torsoLeonaY = " << torsoLeonaY << std::endl;
	std::cout << "movPatasLeona1 = " << movPatasLeona1 << std::endl;
	std::cout << "movPatasLeona2 = " << movPatasLeona2 << std::endl;
	std::cout << "rodPatasLeona1 = " << rodPatasLeona1 << std::endl;
	std::cout << "rodPatasLeona2 = " << rodPatasLeona2 << std::endl;
	std::cout << "cabezaLeona = " << cabezaLeona << std::endl;
	std::cout << "mandibulaLeona = " << mandibula << std::endl;
	std::cout << "cabezaLeonaZ = " << cabezaLeonaZ << std::endl;

	KeyFrame[FrameIndex].movBird_x = movBird_x;
	KeyFrame[FrameIndex].movBird_y = movBird_y;
	KeyFrame[FrameIndex].movBird_z = movBird_z;
	KeyFrame[FrameIndex].giroBird = giroBird;

	KeyFrame[FrameIndex].torsoLeonaZ = torsoLeonaZ;
	KeyFrame[FrameIndex].torsoLeonaY = torsoLeonaY;
	KeyFrame[FrameIndex].movPatasLeona1 = movPatasLeona1;
	KeyFrame[FrameIndex].movPatasLeona2 = movPatasLeona2;
	KeyFrame[FrameIndex].rodPatasLeona1 = rodPatasLeona1;
	KeyFrame[FrameIndex].rodPatasLeona2 = rodPatasLeona2;
	KeyFrame[FrameIndex].cabezaLeona = cabezaLeona;
	KeyFrame[FrameIndex].mandibula = mandibula;
	KeyFrame[FrameIndex].cabezaLeonaZ = cabezaLeonaZ;

	FrameIndex++;
}

void resetElements(void)
{
	movBird_x = KeyFrame[0].movBird_x;
	movBird_y = KeyFrame[0].movBird_y;
	movBird_z = KeyFrame[0].movBird_z;
	giroBird = KeyFrame[0].giroBird;

	torsoLeonaZ = KeyFrame[0].torsoLeonaZ;
	torsoLeonaY = KeyFrame[0].torsoLeonaY;
	movPatasLeona1 = KeyFrame[0].movPatasLeona1;
	movPatasLeona2 = KeyFrame[0].movPatasLeona2;
	rodPatasLeona1 = KeyFrame[0].rodPatasLeona1;
	rodPatasLeona2 = KeyFrame[0].rodPatasLeona2;
	cabezaLeona = KeyFrame[0].cabezaLeona;
	mandibula = KeyFrame[0].mandibula;
	cabezaLeonaZ = KeyFrame[0].cabezaLeonaZ;
}

void interpolation(void)
{
	KeyFrame[playIndex].movBird_xInc = (KeyFrame[playIndex + 1].movBird_x - KeyFrame[playIndex].movBird_x) / i_max_steps;
	KeyFrame[playIndex].movBird_yInc = (KeyFrame[playIndex + 1].movBird_y - KeyFrame[playIndex].movBird_y) / i_max_steps;
	KeyFrame[playIndex].movBird_zInc = (KeyFrame[playIndex + 1].movBird_z - KeyFrame[playIndex].movBird_z) / i_max_steps;
	KeyFrame[playIndex].giroBirdInc = (KeyFrame[playIndex + 1].giroBird - KeyFrame[playIndex].giroBird) / i_max_steps;
	incTorsoLeonaZ = (KeyFrame[playIndex + 1].torsoLeonaZ - KeyFrame[playIndex].torsoLeonaZ) / i_max_steps;
	inctorsoLeonaY = (KeyFrame[playIndex + 1].torsoLeonaY - KeyFrame[playIndex].torsoLeonaY) / i_max_steps;
	incPatasLeona1 = (KeyFrame[playIndex + 1].movPatasLeona1 - KeyFrame[playIndex].movPatasLeona1) / i_max_steps;
	incPatasLeona2 = (KeyFrame[playIndex + 1].movPatasLeona2 - KeyFrame[playIndex].movPatasLeona2) / i_max_steps;
	incRodPatasLeona1 = (KeyFrame[playIndex + 1].rodPatasLeona1 - KeyFrame[playIndex].rodPatasLeona1) / i_max_steps;
	incRodPatasLeona2 = (KeyFrame[playIndex + 1].rodPatasLeona2 - KeyFrame[playIndex].rodPatasLeona2) / i_max_steps;
	incCabezaLeona = (KeyFrame[playIndex + 1].cabezaLeona - KeyFrame[playIndex].cabezaLeona) / i_max_steps;
	incMandibula = (KeyFrame[playIndex + 1].mandibula - KeyFrame[playIndex].mandibula) / i_max_steps;
	incCabezaLeonaZ = (KeyFrame[playIndex + 1].cabezaLeonaZ - KeyFrame[playIndex].cabezaLeonaZ) / i_max_steps;


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

			torsoLeonaZ += incTorsoLeonaZ;
			torsoLeonaY += inctorsoLeonaY;
			movPatasLeona1 += incPatasLeona1;
			movPatasLeona2 += incPatasLeona2;
			rodPatasLeona1 += incRodPatasLeona1;
			rodPatasLeona2 += incRodPatasLeona2;
			cabezaLeona += incCabezaLeona;
			mandibula += incMandibula;
			cabezaLeonaZ += incCabezaLeonaZ;

			i_curr_steps++;
		}
	}

	//Alas halcon
	if (animacion)
	{
		// Aumentar o disminuir el Ã¡ngulo de rotaciÃ³n de las alas
		rotAlaD += 0.1f;
		rotAlaI -= 0.1f;

		// Si el Ã¡ngulo alcanza los lÃ­mites, cambiar la direcciÃ³n
		if (rotAlaD >= 6.0f || rotAlaI <= -6.0f)
		{
			float temp = rotAlaD;
			rotAlaD = rotAlaI;
			rotAlaI = temp;
		}
	}




	//PingÃ¼ino//////////////////////////

	if (pingu) {
		rotaPataP -= 1.0f;
		rotaPataP2 += 1.0f;

		if (rotaPataP <= -50.0f) {
			pingu = false;
		}
	}
	else {
		rotaPataP += 1.0f;
		rotaPataP2 -= 1.0f;
		if (rotaPataP >= 0.0f) {
			pingu = true;
		}
	}


	if (animPingu) {
		movXP += velocidadP;
		if (movXP >= 70.0f) {
			movXP += velocidadP;
			movYP += -.3 * velocidadP - 2.f;
			rotaCuerpoP = 135.0f;
			rotaCabezap = -45.f;
			if (movYP <= -100.0f) {
				animPingu1 = true;
				animPingu = false;
			}
		}
	}

	if (animPingu1 == true) {
		movXP += velocidadP * 3;
		movZP += velocidadP;
		rotaCuerpoP = 90.0f;
		rotaCuerpoP2 += 60.f;

		if (movXP > 360.f) {
			animPingu1 = false;
			animPingu2 = true;
		}
	}
	if (animPingu2 == true) {
		rotaCuerpoP = 180.f;
		rotaCuerpoP2 = 0.f;
		movYP -= 10.0f;

		if (movYP < -250.f) {
			animPingu1 = false;
			animPingu2 = true;
			movXP = 0;
		}
	}
	if (animPingu1 == true) {
		movXP += velocidadP * 3;
		movZP += velocidadP;
		rotaCuerpoP = 90.0f;
		rotaCuerpoP2 += 60.f;

		if (movXP > 360.f) {
			animPingu1 = false;
			animPingu2 = true;
		}
	}
	if (animPingu2 == true) {
		rotaCuerpoP = 90.f;
		rotaCuerpoP2 = 0.f;
		movYP -= 10.0f;

		if (movYP < -400.f) {
			animPingu2 = false;
			animPingu3 = true;
			movXP = -200;
			movZP = 0;
			movYP = -200;
		}
	}
	if (animPingu3 == true) {
		rotaCuerpoP = 0.f;
		rotaCuerpoP2 = 45.f;
		movXP += .3 * cos(velocidadP);
		movYP += 6 * velocidadP;
		if (movYP > 0.f) {
			rotaCabezap = 0;
			animPingu3 = false;
			animPingu4 = true;
		}

	}
	if (animPingu4 == true) {
		movXP += 2 * velocidadP;
		if (movXP >= 0.f) {
			animPingu4 = false;
			animPingu = true;
		}

	}

	//Elefante
	rotaE += rotaE2 * 0.5f;

	// Si la trompa ha oscilado lo suficiente en una direcciÃ³n, cambia la direcciÃ³n
	if (abs(rotaE) > 50.f) {
		rotaE2 *= -1;
	}



}


//Mono
void AnimateMonkey() {
	if (rotaHD >= 180.0f) rotaHD = -180.0f;
	else rotaHD += 2.0f * deltaTime;
	if (rotaMD >= 180.0f) rotaMD = -180.0f;
	else rotaMD += 3.0f * deltaTime;
	if (rotaHI >= 180.0f) rotaHI = -180.0f;
	else rotaHI += 2.0f * deltaTime;
	if (rotaMI >= 180.0f) rotaMI = -180.0f;
	else rotaMI += 3.0f * deltaTime;
}






void getResolution()
{
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}


int main()
{
	if (!engine)
	{
		printf("Could not startup engine\n");
		return 0; // error starting up the engine
	}

	engine->play2D("include/irrKlang-32bit-1.6.0/audioZoo.mp3", true);
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
		/*"resources/skybox/mySkybox/right.png"
		"resources/skybox/mySkybox/left.png"
		"resources/skybox/mySkybox/top.png"
		"resources/skybox/mySkybox/bottom.png"
		"resources/skybox/mySkybox/front.png"
		"resources/skybox/mySkybox/back.png"*/
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
	Model arboles("resources/objects/arboles/arbolesJuntos.obj");
	//Leon
	Model caraLeona("resources/objects/leones/caraLeona.obj");
	Model mandibulaLeona("resources/objects/leones/bocaLeona.obj");
	Model cuerpoLeona("resources/objects/leones/cuerpoLeona.obj");
	Model pataDelanteraDerLeona("resources/objects/leones/pataDelanteraDerLeona.obj");
	Model piernaDelanteraDerLeona("resources/objects/leones/piernaDelanteraDerLeona.obj");
	Model piernaDelanteraIzqLeona("resources/objects/leones/piernaDelanteraIzqLeona.obj");
	Model pataDelanteraIzqLeona("resources/objects/leones/pataDelanteraIzqLeona.obj");
	Model pataTraseraDerLeona("resources/objects/leones/pataTraseraDerLeona.obj");
	Model piernaTraseraDerLeona("resources/objects/leones/piernaTraseraDerLeona.obj");
	Model piernaTraseraIzqLeona("resources/objects/leones/piernaTraseraIzqLeona.obj");
	Model pataTraseraIzqLeona("resources/objects/leones/pataTraseraIzqLeona.obj");
	//Escenarios
	Model iceberg("resources/objects/Pinguino/HabitadPin.obj");
	Model lobo("resources/objects/escenariolobo/escenariolobo.obj");
	Model escenariomono("resources/objects/escenariomono/escenariomono.obj");
	Model escenarioEle("resources/objects/elefante/E_Habitad.obj");
	//Mono
	Model c_cuerpo("resources/objects/Mono/cuerpo.obj");
	Model c_cola1("resources/objects/Mono/cola1.obj");
	Model c_cola2("resources/objects/Mono/cola2.obj");
	Model c_cola3("resources/objects/Mono/cola3.obj");
	Model c_hombroDer("resources/objects/Mono/hombroder.obj");
	Model c_hombroIzq("resources/objects/Mono/hombroizq.obj");
	Model c_manoDer("resources/objects/Mono/brazoder.obj");
	Model c_manoIzq("resources/objects/Mono/brazoizq.obj");

	//Pinguino
	Model P_cuerpo("resources/objects/Pinguino/cuerpoPin.obj");
	Model P_cabeza("resources/objects/Pinguino/cabezaPin.obj");
	Model P_pata("resources/objects/Pinguino/PataDerPin.obj");

	//HalcÃ³n 
	Model halcon("resources/objects/Halcon/Falcon1.obj");
	Model AlaD("resources/objects/Halcon/AlaDer.obj");
	Model AlaI("resources/objects/Halcon/AlaIzq.obj");

	//Elefante
	Model Trompa1("resources/objects/elefante/Trompa4.obj");
	Model Trompa2("resources/objects/elefante/Trompa3.obj");
	Model Trompa3("resources/objects/elefante/Trompa1.obj");
	Model OrejaDer("resources/objects/elefante/oidoDer.obj");
	Model OrejaIzq("resources/objects/elefante/oidoIzq.obj");
	Model Pelota("resources/objects/elefante/pelota.obj");



	//InicializaciÃ³n de KeyFrames

	KeyFrame[0].movBird_x = 240.0f;
	KeyFrame[0].movBird_y = -63.2f;
	KeyFrame[0].movBird_z = -4310.0f;
	KeyFrame[0].giroBird = 45.0f;

	KeyFrame[0].torsoLeonaZ = 3521;
	KeyFrame[0].torsoLeonaY = 0;
	KeyFrame[0].movPatasLeona1 = 0;
	KeyFrame[0].movPatasLeona2 = 0;
	KeyFrame[0].rodPatasLeona1 = 0;
	KeyFrame[0].rodPatasLeona2 = 0;
	KeyFrame[0].cabezaLeona = 0;
	KeyFrame[0].mandibula = 0;
	KeyFrame[0].cabezaLeonaZ = 0;
	// 1
	KeyFrame[1].torsoLeonaZ = 3525;
	KeyFrame[1].torsoLeonaY = 0;
	KeyFrame[1].movPatasLeona1 = 0.2;
	KeyFrame[1].movPatasLeona2 = 0;
	KeyFrame[1].rodPatasLeona1 = 0.2;
	KeyFrame[1].rodPatasLeona2 = 0;
	KeyFrame[1].cabezaLeona = 0;
	KeyFrame[1].mandibula = 0;
	KeyFrame[1].cabezaLeonaZ = 0;
	//  2
	KeyFrame[2].torsoLeonaZ = 3529;
	KeyFrame[2].torsoLeonaY = 0;
	KeyFrame[2].movPatasLeona1 = 0.4;
	KeyFrame[2].movPatasLeona2 = 0;
	KeyFrame[2].rodPatasLeona1 = 0.4;
	KeyFrame[2].rodPatasLeona2 = 0;
	KeyFrame[2].cabezaLeona = 0;
	KeyFrame[2].mandibula = 0;
	KeyFrame[2].cabezaLeonaZ = 0;
	//  3
	KeyFrame[3].torsoLeonaZ = 3533;
	KeyFrame[3].torsoLeonaY = 0;
	KeyFrame[3].movPatasLeona1 = 0.2;
	KeyFrame[3].movPatasLeona2 = 0;
	KeyFrame[3].rodPatasLeona1 = 0.2;
	KeyFrame[3].rodPatasLeona2 = 0;
	KeyFrame[3].cabezaLeona = 0;
	KeyFrame[3].mandibula = 0;
	KeyFrame[3].cabezaLeonaZ = 0;
	// 4
	KeyFrame[4].torsoLeonaZ = 3537;
	KeyFrame[4].torsoLeonaY = 0;
	KeyFrame[4].movPatasLeona1 = 1.49012e-08;
	KeyFrame[4].movPatasLeona2 = 0;
	KeyFrame[4].rodPatasLeona1 = 1.49012e-08;
	KeyFrame[4].rodPatasLeona2 = 0;
	KeyFrame[4].cabezaLeona = 0;
	KeyFrame[4].mandibula = 0;
	KeyFrame[4].cabezaLeonaZ = 0;
	//  5
	KeyFrame[5].torsoLeonaZ = 3540;
	KeyFrame[5].torsoLeonaY = 0;
	KeyFrame[5].movPatasLeona1 = -0.2;
	KeyFrame[5].movPatasLeona2 = 0;
	KeyFrame[5].rodPatasLeona1 = -0.2;
	KeyFrame[5].rodPatasLeona2 = 0;
	KeyFrame[5].cabezaLeona = 0;
	KeyFrame[5].mandibula = 0;
	KeyFrame[5].cabezaLeonaZ = 0;
	//  6
	KeyFrame[6].torsoLeonaZ = 3544;
	KeyFrame[6].torsoLeonaY = 0;
	KeyFrame[6].movPatasLeona1 = -0.4;
	KeyFrame[6].movPatasLeona2 = 0;
	KeyFrame[6].rodPatasLeona1 = -0.4;
	KeyFrame[6].rodPatasLeona2 = 0;
	KeyFrame[6].cabezaLeona = 0;
	KeyFrame[6].mandibula = 0;
	KeyFrame[6].cabezaLeonaZ = 0;
	//  7;
	KeyFrame[7].torsoLeonaZ = 3548;
	KeyFrame[7].torsoLeonaY = 0;
	KeyFrame[7].movPatasLeona1 = -0.2;
	KeyFrame[7].movPatasLeona2 = 0;
	KeyFrame[7].rodPatasLeona1 = -0.2;
	KeyFrame[7].rodPatasLeona2 = 0;
	KeyFrame[7].cabezaLeona = 0;
	KeyFrame[7].mandibula = 0;
	KeyFrame[7].cabezaLeonaZ = 0;
	//  8;
	KeyFrame[8].torsoLeonaZ = 3552;
	KeyFrame[8].torsoLeonaY = 0;
	KeyFrame[8].movPatasLeona1 = 1.49012e-08;
	KeyFrame[8].movPatasLeona2 = 0;
	KeyFrame[8].rodPatasLeona1 = 1.49012e-08;
	KeyFrame[8].rodPatasLeona2 = 0;
	KeyFrame[8].cabezaLeona = 0;
	KeyFrame[8].mandibula = 0;
	KeyFrame[8].cabezaLeonaZ = 0;
	//  9;
	KeyFrame[9].torsoLeonaZ = 3556;
	KeyFrame[9].torsoLeonaY = 0;
	KeyFrame[9].movPatasLeona1 = 0.2;
	KeyFrame[9].movPatasLeona2 = 0;
	KeyFrame[9].rodPatasLeona1 = 0.2;
	KeyFrame[9].rodPatasLeona2 = 0;
	KeyFrame[9].cabezaLeona = 0;
	KeyFrame[9].mandibula = 0;
	KeyFrame[9].cabezaLeonaZ = 0;
	//  10;
	KeyFrame[10].torsoLeonaZ = 3561;
	KeyFrame[10].torsoLeonaY = 0;
	KeyFrame[10].movPatasLeona1 = 0.4;
	KeyFrame[10].movPatasLeona2 = 0;
	KeyFrame[10].rodPatasLeona1 = 0.4;
	KeyFrame[10].rodPatasLeona2 = 0;
	KeyFrame[10].cabezaLeona = 0;
	KeyFrame[10].mandibula = 0;
	KeyFrame[10].cabezaLeonaZ = 0;
	//  11;
	KeyFrame[11].torsoLeonaZ = 3565;
	KeyFrame[11].torsoLeonaY = 0;
	KeyFrame[11].movPatasLeona1 = 0.2;
	KeyFrame[11].movPatasLeona2 = 0;
	KeyFrame[11].rodPatasLeona1 = 0.2;
	KeyFrame[11].rodPatasLeona2 = 0;
	KeyFrame[11].cabezaLeona = 0;
	KeyFrame[11].mandibula = 0;
	KeyFrame[11].cabezaLeonaZ = 0;
	//  12
	KeyFrame[12].torsoLeonaZ = 3570;
	KeyFrame[12].torsoLeonaY = 0;
	KeyFrame[12].movPatasLeona1 = 1.49012e-08;
	KeyFrame[12].movPatasLeona2 = 0;
	KeyFrame[12].rodPatasLeona1 = 1.49012e-08;
	KeyFrame[12].rodPatasLeona2 = 0;
	KeyFrame[12].cabezaLeona = 0;
	KeyFrame[12].mandibula = 0;
	KeyFrame[12].cabezaLeonaZ = 0;
	//  13
	KeyFrame[13].torsoLeonaZ = 3574;
	KeyFrame[13].torsoLeonaY = 0;
	KeyFrame[13].movPatasLeona1 = -0.2;
	KeyFrame[13].movPatasLeona2 = 0;
	KeyFrame[13].rodPatasLeona1 = -0.2;
	KeyFrame[13].rodPatasLeona2 = 0;
	KeyFrame[13].cabezaLeona = 0;
	KeyFrame[13].mandibula = 0;
	KeyFrame[13].cabezaLeonaZ = 0;
	//  14
	KeyFrame[14].torsoLeonaZ = 3579;
	KeyFrame[14].torsoLeonaY = 0;
	KeyFrame[14].movPatasLeona1 = -0.4;
	KeyFrame[14].movPatasLeona2 = 0;
	KeyFrame[14].rodPatasLeona1 = -0.4;
	KeyFrame[14].rodPatasLeona2 = 0;
	KeyFrame[14].cabezaLeona = 0;
	KeyFrame[14].mandibula = 0;
	KeyFrame[14].cabezaLeonaZ = 0;
	//  15
	KeyFrame[15].torsoLeonaZ = 3583;
	KeyFrame[15].torsoLeonaY = 0;
	KeyFrame[15].movPatasLeona1 = -0.2;
	KeyFrame[15].movPatasLeona2 = 0;
	KeyFrame[15].rodPatasLeona1 = -0.2;
	KeyFrame[15].rodPatasLeona2 = 0;
	KeyFrame[15].cabezaLeona = 0;
	KeyFrame[15].mandibula = 0;
	KeyFrame[15].cabezaLeonaZ = 0;
	//  16
	KeyFrame[16].torsoLeonaZ = 3587;
	KeyFrame[16].torsoLeonaY = 0;
	KeyFrame[16].movPatasLeona1 = 1.49012e-08;
	KeyFrame[16].movPatasLeona2 = 0;
	KeyFrame[16].rodPatasLeona1 = 1.49012e-08;
	KeyFrame[16].rodPatasLeona2 = 0;
	KeyFrame[16].cabezaLeona = 0;
	KeyFrame[16].mandibula = 0;
	KeyFrame[16].cabezaLeonaZ = 0;
	//  17
	KeyFrame[17].torsoLeonaZ = 3591;
	KeyFrame[17].torsoLeonaY = 0;
	KeyFrame[17].movPatasLeona1 = 0.2;
	KeyFrame[17].movPatasLeona2 = 0;
	KeyFrame[17].rodPatasLeona1 = 0.2;
	KeyFrame[17].rodPatasLeona2 = 0;
	KeyFrame[17].cabezaLeona = 0;
	KeyFrame[17].mandibula = 0;
	KeyFrame[17].cabezaLeonaZ = 0;
	//  18
	KeyFrame[18].torsoLeonaZ = 3591;
	KeyFrame[18].torsoLeonaY = 0;
	KeyFrame[18].movPatasLeona1 = 1.49012e-08;
	KeyFrame[18].movPatasLeona2 = 0;
	KeyFrame[18].rodPatasLeona1 = 1.49012e-08;
	KeyFrame[18].rodPatasLeona2 = 0;
	KeyFrame[18].cabezaLeona = 0;
	KeyFrame[18].mandibula = 0;
	KeyFrame[18].cabezaLeonaZ = 0;
	//  19
	KeyFrame[19].torsoLeonaZ = 3591;
	KeyFrame[19].torsoLeonaY = -3;
	KeyFrame[19].movPatasLeona1 = -0.2;
	KeyFrame[19].movPatasLeona2 = 0.1;
	KeyFrame[19].rodPatasLeona1 = -0.2;
	KeyFrame[19].rodPatasLeona2 = 0;
	KeyFrame[19].cabezaLeona = 0.1;
	KeyFrame[19].mandibula = 0;
	KeyFrame[19].cabezaLeonaZ = 0;
	//  20
	KeyFrame[20].torsoLeonaZ = 3591;
	KeyFrame[20].torsoLeonaY = -9;
	KeyFrame[20].movPatasLeona1 = -0.2;
	KeyFrame[20].movPatasLeona2 = 0.1;
	KeyFrame[20].rodPatasLeona1 = -0.2;
	KeyFrame[20].rodPatasLeona2 = 0;
	KeyFrame[20].cabezaLeona = 0.3;
	KeyFrame[20].mandibula = -0.1;
	KeyFrame[20].cabezaLeonaZ = -9.1;
	//  21
	KeyFrame[21].torsoLeonaZ = 3591;
	KeyFrame[21].torsoLeonaY = -8;
	KeyFrame[21].movPatasLeona1 = -0.2;
	KeyFrame[21].movPatasLeona2 = 0.1;
	KeyFrame[21].rodPatasLeona1 = -0.2;
	KeyFrame[21].rodPatasLeona2 = 0;
	KeyFrame[21].cabezaLeona = 0.3;
	KeyFrame[21].mandibula = -1.8;
	KeyFrame[21].cabezaLeonaZ = -12;
	//  22
	KeyFrame[22].torsoLeonaZ = 3591;
	KeyFrame[22].torsoLeonaY = -8;
	KeyFrame[22].movPatasLeona1 = -0.2;
	KeyFrame[22].movPatasLeona2 = 0.1;
	KeyFrame[22].rodPatasLeona1 = -0.2;
	KeyFrame[22].rodPatasLeona2 = 0;
	KeyFrame[22].cabezaLeona = 0.3;
	KeyFrame[22].mandibula = -0.6;
	KeyFrame[22].cabezaLeonaZ = -12;
	//  23
	KeyFrame[23].torsoLeonaZ = 3591;
	KeyFrame[23].torsoLeonaY = -8;
	KeyFrame[23].movPatasLeona1 = -0.2;
	KeyFrame[23].movPatasLeona2 = 0.1;
	KeyFrame[23].rodPatasLeona1 = -0.2;
	KeyFrame[23].rodPatasLeona2 = 0;
	KeyFrame[23].cabezaLeona = 0.3;
	KeyFrame[23].mandibula = -1.8;
	KeyFrame[23].cabezaLeonaZ = -12;
	//  24
	KeyFrame[24].torsoLeonaZ = 3591;
	KeyFrame[24].torsoLeonaY = -8;
	KeyFrame[24].movPatasLeona1 = -0.2;
	KeyFrame[24].movPatasLeona2 = 0.1;
	KeyFrame[24].rodPatasLeona1 = -0.2;
	KeyFrame[24].rodPatasLeona2 = 0;
	KeyFrame[24].cabezaLeona = 0.3;
	KeyFrame[24].mandibula = -0.5;
	KeyFrame[24].cabezaLeonaZ = -12;
	//  25
	KeyFrame[25].torsoLeonaZ = 3591;
	KeyFrame[25].torsoLeonaY = -8;
	KeyFrame[25].movPatasLeona1 = -0.2;
	KeyFrame[25].movPatasLeona2 = 0.1;
	KeyFrame[25].rodPatasLeona1 = -0.2;
	KeyFrame[25].rodPatasLeona2 = 0;
	KeyFrame[25].cabezaLeona = 0.3;
	KeyFrame[25].mandibula = -1.6;
	KeyFrame[25].cabezaLeonaZ = -12;
	//  26
	KeyFrame[26].torsoLeonaZ = 3591;
	KeyFrame[26].torsoLeonaY = -8;
	KeyFrame[26].movPatasLeona1 = -0.2;
	KeyFrame[26].movPatasLeona2 = 0.1;
	KeyFrame[26].rodPatasLeona1 = -0.2;
	KeyFrame[26].rodPatasLeona2 = 0;
	KeyFrame[26].cabezaLeona = 0.3;
	KeyFrame[26].mandibula = -0.4;
	KeyFrame[26].cabezaLeonaZ = -12;
	//  27
	KeyFrame[27].torsoLeonaZ = 3590.98;
	KeyFrame[27].torsoLeonaY = -1.23978e-05;
	KeyFrame[27].movPatasLeona1 = 1.3411e-07;
	KeyFrame[27].movPatasLeona2 = 0.0999999;
	KeyFrame[27].rodPatasLeona1 = 1.3411e-07;
	KeyFrame[27].rodPatasLeona2 = 0;
	KeyFrame[27].cabezaLeona = -2.23517e-07;
	KeyFrame[27].mandibula = -0.7;
	KeyFrame[27].cabezaLeonaZ = -0.0999791;

	//Frames HalcÃ³n
	
	float yOffset = 4.0f; // Desplazamiento en el eje y

	for (int i = 1; i < MAX_FRAMES; i++) {
		float angle = 2 * PI * i / MAX_FRAMES;  // Ãngulo de la trayectoria

		KeyFrame[i].movBird_x = 210.0f + 100.0f * cos(angle);
		KeyFrame[i].movBird_y = -63.2f + yOffset;
		KeyFrame[i].movBird_z = -4312.0f - 100.0f * sin(angle);

		float prevAngle = atan2(KeyFrame[i - 1].movBird_z - (-4312.0f), KeyFrame[i - 1].movBird_x - 210.0f);
		float currentAngle = atan2(KeyFrame[i].movBird_z - (-4312.0f), KeyFrame[i].movBird_x - 210.0f);

		// InterpolaciÃ³n lineal para calcular el Ã¡ngulo de giro
		KeyFrame[i].giroBird = 2 * (currentAngle - prevAngle);

		yOffset += 4.0f; // Incremento del desplazamiento en el eje y
	}

	KeyFrame[MAX_FRAMES - 1].movBird_x = KeyFrame[0].movBird_x;
	KeyFrame[MAX_FRAMES - 1].movBird_y = -63.2f;
	KeyFrame[MAX_FRAMES - 1].movBird_z = KeyFrame[0].movBird_z;
	KeyFrame[MAX_FRAMES - 1].giroBird = KeyFrame[0].giroBird;
	

	


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
		AnimateMonkey();

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
		glm::mat4 tmpPin = glm::mat4(1.0f);
		glm::mat4 tempLeona = glm::mat4(1.0f);
		glm::mat4 tempPataLeona = glm::mat4(1.0f);
		glm::mat4 modelaux = glm::mat4(1.0);

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

		// -------------------------------------------------------------------------------------------------------------------------
		// Halcon
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(movBird_x, movBird_y, -movBird_z));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(20.0f));
		tmp = model = glm::rotate(model, glm::radians(giroBird), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		halcon.Draw(staticShader);

		//AlaDer

		model = glm::translate(tmp, glm::vec3(0.65f, 1.0f, -1.7f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotAlaD), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		AlaD.Draw(staticShader);

		//AlaIzq

		model = glm::translate(tmp, glm::vec3(0.85f, 0.85f, -1.4));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(rotAlaI), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		AlaI.Draw(staticShader);



		//AnimaciÃ³n Mono
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f, -200.0f, 2850.0f));
		model = glm::scale(model, glm::vec3(250.0f));
		staticShader.setMat4("model", model);
		c_cuerpo.Draw(staticShader);
		modelaux = model;

		model = glm::translate(model, glm::vec3(-0.080135f, 0.425874f, -0.021222f));
		model = glm::rotate(model, glm::radians(-16.5f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::sin(glm::radians(rotaHD)) / 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		c_hombroDer.Draw(staticShader);

		model = glm::translate(model, glm::vec3(0.080135f - 0.130399f, 0.327911f - 0.425874f, 0.021222f - 0.009231f));
		model = glm::rotate(model, glm::sin(glm::radians(-rotaMD)) / 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::sin(glm::radians(-rotaHD + 10.0f)) / 3, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(-16.5f - 19.4f + 90), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(-8.9f + 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(87.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		c_manoDer.Draw(staticShader);

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.077942f, 0.429474f, -0.012812f));
		model = glm::rotate(model, glm::radians(-186.8f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::sin(glm::radians(rotaHI)) / 4.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		c_hombroIzq.Draw(staticShader);

		model = glm::translate(model, glm::vec3(0.134954f - 0.077942f, 0.429474f - 0.562912f, 0.012812f - 0.001025f));
		model = glm::rotate(model, glm::sin(glm::radians(rotaMI)) / 4, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::sin(glm::radians(rotaHI - 10.0f)) / 3, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(186.8f - 93.0f - 90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(157.0f + 90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(74.8f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		c_manoIzq.Draw(staticShader);

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.0f, 0.24621f, -0.013774f));
		staticShader.setMat4("model", model);
		c_cola1.Draw(staticShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.013774f - 0.091316f));
		staticShader.setMat4("model", model);
		c_cola2.Draw(staticShader);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.091316f - 0.227569f));
		staticShader.setMat4("model", model);
		c_cola3.Draw(staticShader);


		// AnimaciÃ³n leona
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		arboles.Draw(staticShader);

		tempLeona = model = glm::translate(glm::mat4(1.0f), glm::vec3(310.0f, -209.0f, torsoLeonaZ)); //100
		model = glm::translate(tempLeona, glm::vec3(0.0f, torsoLeonaY, 0.0f)); //100
		model = glm::scale(model, glm::vec3(37.0f));
		staticShader.setMat4("model", model);
		cuerpoLeona.Draw(staticShader);

		tempPataLeona = model = glm::translate(tempLeona, glm::vec3(0.0f, 38.0f, 25.5f)); //0.0 10.0 5.0
		model = glm::scale(model, glm::vec3(37.0f));
		model = glm::rotate(model, movPatasLeona1, glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaDelanteraIzqLeona.Draw(staticShader);
		model = glm::translate(tempPataLeona, glm::vec3(0.0f, 0.5f, -1.5f));
		model = glm::translate(tempPataLeona, glm::vec3(0.0f, 0.5f, movPatasLeona2));
		model = glm::scale(model, glm::vec3(37.0f));
		model = glm::rotate(model, rodPatasLeona1, glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pataDelanteraIzqLeona.Draw(staticShader);

		tempPataLeona = glm::translate(tempLeona, glm::vec3(0.0f, -38.0f, -25.5f)); //regresa a 0
		tempPataLeona = model = glm::translate(tempLeona, glm::vec3(0.0f, 38.0f, 25.5f));
		model = glm::scale(model, glm::vec3(37.0f));
		model = glm::rotate(model, -movPatasLeona1, glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaDelanteraDerLeona.Draw(staticShader);
		model = glm::translate(tempPataLeona, glm::vec3(0.0f, 0.5f, 0.5f));
		model = glm::translate(tempPataLeona, glm::vec3(0.0f, 0.5f, -movPatasLeona2));
		model = glm::scale(model, glm::vec3(37.0f));
		model = glm::rotate(model, -rodPatasLeona1, glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pataDelanteraDerLeona.Draw(staticShader);

		tempPataLeona = glm::translate(tempLeona, glm::vec3(0.0f, -38.0f, -25.5f)); //regresa a 0
		tempPataLeona = model = glm::translate(tempLeona, glm::vec3(0.0f, 38.0f, -23.0f));
		model = glm::scale(model, glm::vec3(37.0f));
		model = glm::rotate(model, movPatasLeona1, glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaTraseraDerLeona.Draw(staticShader);
		model = glm::translate(tempPataLeona, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::translate(tempPataLeona, glm::vec3(0.0f, 0.5f, movPatasLeona2));
		model = glm::scale(model, glm::vec3(37.0f));
		model = glm::rotate(model, rodPatasLeona1, glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pataTraseraDerLeona.Draw(staticShader);

		tempPataLeona = glm::translate(tempLeona, glm::vec3(0.0f, -38.0f, 23.0f)); //regresa a 0
		tempPataLeona = model = glm::translate(tempLeona, glm::vec3(0.0f, 48.0f, -25.0f));
		model = glm::scale(model, glm::vec3(37.0f));
		model = glm::rotate(model, -movPatasLeona1, glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		piernaTraseraIzqLeona.Draw(staticShader);
		model = glm::translate(tempPataLeona, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::translate(tempPataLeona, glm::vec3(0.0f, 0.5f, -movPatasLeona2));
		model = glm::scale(model, glm::vec3(37.0f));
		model = glm::rotate(model, -rodPatasLeona1, glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		pataTraseraIzqLeona.Draw(staticShader);

		model = glm::translate(tempLeona, glm::vec3(0.0f, 0.0f, cabezaLeonaZ));
		model = glm::scale(model, glm::vec3(37.0f));
		model = glm::rotate(model, cabezaLeona, glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		caraLeona.Draw(staticShader);

		model = glm::translate(tempLeona, glm::vec3(0.0f, mandibula, cabezaLeonaZ));
		model = glm::scale(model, glm::vec3(37.0f));
		model = glm::rotate(model, cabezaLeona, glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		mandibulaLeona.Draw(staticShader);




		//Pinguino
															//z es fondo -	//x es arriba, en medio
		// AnimaciÃ³n PingÃ¼ino
			//Cuerpo

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-550.0f + movZP, -25.0f + movYP, 4000.0f + movXP));
		model = glm::rotate(model, glm::radians(0.0f + rotaCuerpoP2), glm::vec3(.0f, 1.0f, .0f));
		tmpPin = model = glm::rotate(model, glm::radians(0.0f + rotaCuerpoP), glm::vec3(1.0f, .0f, 0.0f));
		model = glm::scale(model, glm::vec3(250.0f));
		staticShader.setMat4("model", model);
		P_cuerpo.Draw(staticShader);
		//Cabeza
		model = glm::translate(tmpPin, glm::vec3(3.0f, 36.2f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f + rotaCabezap), glm::vec3(1.0f, .0f, 0.0f));
		model = glm::scale(model, glm::vec3(250.0f));
		staticShader.setMat4("model", model);
		P_cabeza.Draw(staticShader);
		//PieIzq
		model = glm::translate(tmpPin, glm::vec3(-5.0f, -25.0f, 2.0f));
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, .0f, 0.0f));
		model = glm::rotate(model, glm::radians(rotaPataP2), glm::vec3(1.0f, .0f, .0f));
		model = glm::scale(model, glm::vec3(250.0f));
		staticShader.setMat4("model", model);
		P_pata.Draw(staticShader);
		//PieDer
		model = glm::translate(tmpPin, glm::vec3(5.0f, -27.0f, -4.0f));
		model = glm::rotate(model, glm::radians(rotaPataP), glm::vec3(1.0f, 0.0f, .0f));
		model = glm::scale(model, glm::vec3(250.0f));
		staticShader.setMat4("model", model);
		P_pata.Draw(staticShader);

		//Elefante

		model = glm::translate(glm::mat4(1.0f), glm::vec3(950.0f, -85.0f, 3923.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(.0f, 1.0f, .0f));
		tmpPin = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(.0f, .0f, 1.0f));
		model = glm::scale(model, glm::vec3(250.0f));
		staticShader.setMat4("model", model);
		Trompa1.Draw(staticShader);

		model = glm::translate(tmpPin, glm::vec3(0.0f, -38.0f, 6.0f));
		tmp = model = glm::rotate(model, glm::radians(0.f + rotaE), glm::vec3(.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Trompa2.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(.0f, -40.0f, -10.0f));
		model = glm::rotate(model, glm::radians(0.f + rotaE), glm::vec3(1.0f, 0.0f, .0f));
		model = glm::scale(model, glm::vec3(50.0f));
		staticShader.setMat4("model", model);
		Trompa3.Draw(staticShader);

		model = glm::translate(tmpPin, glm::vec3(20.0f, 30.0f, -55.0f));
		model = glm::rotate(model, glm::radians(0.f + rotaPataP + 5), glm::vec3(1.0f, .0f, .0f));
		model = glm::scale(model, glm::vec3(250.0f));
		staticShader.setMat4("model", model);
		OrejaDer.Draw(staticShader);
		model = glm::translate(tmpPin, glm::vec3(-20.0f, 30.0f, -55.0f));
		model = glm::rotate(model, glm::radians(0.f + rotaPataP + 2), glm::vec3(1.0f, .0f, .0f));
		model = glm::scale(model, glm::vec3(250.0f));
		staticShader.setMat4("model", model);
		OrejaIzq.Draw(staticShader);

		//Escenario PingÃ¼ino

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-450.0f/*eje z*/, -210.0f/*eje y*/, 4100.0f/*eje x*/));
		model = glm::scale(model, glm::vec3(400.0f));
		staticShader.setMat4("model", model);
		iceberg.Draw(staticShader);



		//Escenario lobo
		model = glm::translate(glm::mat4(1.0f), glm::vec3(950.0f, -210.0f, 2850.0f));
		model = glm::scale(model, glm::vec3(250.0f));
		staticShader.setMat4("model", model);
		lobo.Draw(staticShader);

		//Escenario Mono
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-500.0f/*eje z*/, -210.0f/*eje y*/, 2850.0f/*eje x*/));
		model = glm::scale(model, glm::vec3(300.0f));
		staticShader.setMat4("model", model);
		escenariomono.Draw(staticShader);
		//Escenario Elefante
		model = glm::translate(glm::mat4(1.0f), glm::vec3(950.0f, -210.0f, 4000.0f));
		model = glm::rotate(model, glm::radians(180.f), glm::vec3(.0f, 1.0f, .0f));
		model = glm::scale(model, glm::vec3(300.0f));
		staticShader.setMat4("model", model);
		escenarioEle.Draw(staticShader);

		//PISO
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -2.75f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente PrÃ¡ctica
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
void my_input(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime + 100);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime + 100);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime + 100);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime + 100);



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