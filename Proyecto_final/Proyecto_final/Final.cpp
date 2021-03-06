/*---------------------------------------------------------*/
/* ----------------Proyecto final             -----------*/
/*-----------------    2022-1   ---------------------------*/
/*------------- Alumno:Juan Dario Villa Vega---------------*/
/*------------- No. Cuenta:315015401        ---------------*/
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <cstdlib>

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



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

//----------------------------------------------------------------------------declaracion_animacion_jack
void animacion_jack(void);
// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(200.0f, 10.0f, 0.0f));
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
glm::vec3 lightDirection(-1.0f, 0.0f, 0.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
int cont1, cont2, cont3, cont4 = 0;

bool	animacion = false,
recorrido1 = true,
recorrido2 = false,
recorrido3 = false,
recorrido4 = false;
int estado_carro = 1;
//Variables jack, temporales-----------------------------------------------------------------------------------------------------
int estadosJack = 0;
int estadosPistola=0,estadoPelota,estadoCarro = 0;
//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
posY = 0.0f,
posZ = 0.0f,
rotRodIzq = 0.0f,
giroMonito = 0.0f,
giro_llanta = 0.0f;
float	incX = 0.0f,
incY = 0.0f,
incZ = 0.0f,
rotInc = 0.0f,
giroMonitoInc = 0.0f;
float rot_x, rot_y, rot_z, luz_x, luz_y, luz_z = 0.0f;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;
class Jack
{
public:
	float posx, posy, posz;
	float elevacionJack, rotacion_tapa, rotacion_manivela;
	glm::vec3 cajaPosition;
	glm::mat4 model, tmp;
	Model jackBox = Model("resources/objects/jack/jack_box_obj.obj");
	Model jack = Model("resources/objects/jack/jack.obj");
	Model jack_Manibela = Model("resources/objects/jack/manibela.obj");
	Model jack_tapa = Model("resources/objects/jack/top_jack_box.obj");
	Jack(float x, float y, float z) {
		posx = x;
		posy = y;
		posz = z;
		cajaPosition = glm::vec3(x, y, z);
		elevacionJack=-2.5;
	}

	void dibujar(Shader staticShader) {
		model = glm::translate(glm::mat4(1.0f), glm::vec3(posx, posy, posz));
		tmp = model = glm::translate(model, cajaPosition);//manejo de una variable
		staticShader.setMat4("model", model);
		jackBox.Draw(staticShader);

		tmp = glm::translate(model, glm::vec3(0.0f, 6.0f, 5.0f));
		tmp = glm::rotate(tmp, glm::radians(rotacion_manivela), glm::vec3(0.0f, 0.0f, 1.0f));//rotacion a apartir de una variable
		staticShader.setMat4("model", tmp);
		jack_Manibela.Draw(staticShader);

		tmp = glm::translate(model, glm::vec3(-5.0f, 10.0f, 0.0f));
		tmp = glm::rotate(tmp, glm::radians(rotacion_tapa), glm::vec3(0.0f, 0.0f, 1.0f));//rotacion a apartir de una variable
		staticShader.setMat4("model", tmp);
		jack_tapa.Draw(staticShader);

		tmp = glm::translate(model, glm::vec3(0.0f, elevacionJack, 0.0f));//se utiliza variable para la altura
		staticShader.setMat4("model", tmp);
		jack.Draw(staticShader);
	}
	void animacion_jack(int *estadosJack) {
		switch (*estadosJack) {
		case 0:
			elevacionJack = -2.5;
			rotacion_tapa = 0.0f;
			break;
		case 1:
			rotacion_manivela = rotacion_manivela + 15;
			if (rotacion_manivela >= 360) {
				rotacion_manivela = 0;
				*estadosJack = 2;
			}
			break;
		case 2:
			cajaPosition.x = cajaPosition.x + 1;
			if (cajaPosition.x >= posx + 4) {
				*estadosJack = 3;
			}
			break;
		case 3:
			cajaPosition.x = cajaPosition.x - 1;
			if (cajaPosition.x <= posx - 4) {
				*estadosJack = 4;
			}
			break;

		case 4:
			elevacionJack = elevacionJack + 1;
			rotacion_tapa = rotacion_tapa + 5;
			if (rotacion_tapa >= 90) {
				*estadosJack = 5;
			}
			break;
		case 5:
			break;
		}

	}
};
class pistolaTenis {
	float x, y, z;
	float rotacion, presion_gatillo;
	float rotacionP;
	float movGatillo,movPx,movPy;
	
	glm::vec3 posPelota;
	Model pistola=Model ("resources/objects/pistola_tenis/pistola.obj");
	Model gatillo=Model ("resources/objects/pistola_tenis/gatillo.obj");
	Model pelota = Model("resources/objects/pistola_tenis/pelota.obj");
	glm::mat4 model, tmp;
public:
	pistolaTenis(float posx,float posy,float posz) {
		x = posx;
		y = posy;
		z = posz;
		posPelota = glm::vec3(posx,posy,posz-1.5);
	}
	void dibujar(Shader staticShader) {
		tmp=model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));
		tmp = glm::rotate(model, glm::radians(-rotacion), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", tmp);
		pistola.Draw(staticShader);
		model = glm::translate(tmp, glm::vec3(x + movGatillo+10.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		gatillo.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model,posPelota);
		model = glm::rotate(model, glm::radians(rotacionP), glm::vec3(0.0f,0.0f,1.0f));
		staticShader.setMat4("model", model);
		pelota.Draw(staticShader);
	}
	void animar(int* estadoPistola, int* estadoPelota) {
		switch (*estadoPistola) {
		case 0:
			rotacion =0;
			movGatillo = 0.0f;
			break;
		case 1:
			movGatillo=movGatillo+0.2f;
			if (movGatillo >=0.5f) {
				*estadoPistola = 2;
			}
			break;
		case 2:
			movGatillo=movGatillo-0.2f;
			if (movGatillo <= 0) {
				*estadoPistola = 3;
				*estadoPelota = 1;
			}
			break;
		case 3:
			rotacion+=7;
			if (rotacion >=45) {
				*estadoPistola = 4;
				
			}
			break;

		case 4:

			break;
		}
		switch (*estadoPelota) {
		case 0:
			posPelota.x = x;
			posPelota.y = y;
			movPx = 0;
			movPy = 0;
			break;
		case 1:
			movPx -= 2;
			posPelota.x -= 2;
			if (movPx <= -62.0f)
				*estadoPelota = 2;
			break;
		case 2:
			movPy += 2;
			posPelota.y += 2;
			if (movPy >= 22.0f) {
				*estadoPelota = 3;
				movPy = 0;
			}
			break;
		case 3:
			movPy -= 2;
			posPelota.y -= 2;
			if (movPy <= -53.0f) {
				*estadoPelota = 4;
				movPx = 0;
			}
		case 4:
			movPx++;
			posPelota.x++;
			rotacionP += 5;
			if (movPx > -10)
				*estadoPelota = 5;
			break;
		case 5:
			break;
		}

	}
};
class	Santa
{
public:
	float posx, posy, posz;
	float rotacionPiernas,rotacionTorzo,rotacionBrazoIx,rotacionBrazoDx, rotacionBrazoIy, rotacionBrazoDy,rotacionCabeza,rot;
	float dz, dx;
	int edoPies, edoTraslacion, edoSaludo,edobrazo,contador;
	glm::vec3 traslacion;
	glm::mat4 model, tmp;
	Model cabeza = Model("resources/objects/santa/cabeza.obj");
	Model torzo = Model("resources/objects/santa/torzo.obj");
	Model pie = Model("resources/objects/santa/pie.obj");
	Model cintura= Model("resources/objects/santa/cintura.obj");
	Model brazoi= Model("resources/objects/santa/brazo_i.obj");
	Model brazod= Model("resources/objects/santa/brazo_d.obj");
	
	
	Santa(float x, float y, float z) {
		posx = x;
		posy = y;
		posz = z;
		
	}

	void dibujar(Shader staticShader) {
		model = glm::translate(glm::mat4(1.0f), glm::vec3(posx+dx, posy, posz+dz));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cintura.Draw(staticShader);//cintura base animacion inferior

		tmp= glm::translate(model, glm::vec3(0.64f, 0.488f, 1.571f));
		tmp = glm::rotate(tmp, glm::radians(rotacionPiernas), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", tmp);
		pie.Draw(staticShader);

		tmp = glm::translate(model, glm::vec3(0.64f, 0.488f, -1.571f));
		tmp = glm::rotate(tmp, glm::radians(-rotacionPiernas), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", tmp);
		pie.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(posx+dx, posy, posz+dz));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model= glm::translate(model, glm::vec3(0.0f, 3.24f, 0.0f));//Ubicacion torzo
		model = glm::rotate(model, glm::radians(rotacionTorzo), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		torzo.Draw(staticShader);
		
		tmp = glm::translate(model, glm::vec3(0.0f, 5.73f, 0.43f));//Ubicacion cabeza
		tmp = glm::rotate(tmp, glm::radians(rotacionCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", tmp);
		cabeza.Draw(staticShader);

		tmp = glm::translate(model, glm::vec3(0.0f, 4.773f, -1.05f));//Ubicacion brazo I
		tmp = glm::rotate(tmp, glm::radians(rotacionBrazoDx), glm::vec3(1.0f, 0.0f, 0.0f));
		tmp = glm::rotate(tmp, glm::radians(rotacionBrazoDy), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", tmp);
		brazod.Draw(staticShader);

		tmp = glm::translate(model, glm::vec3(0.0f, 4.773f, +1.05f));//Ubicacion brazo I
		tmp = glm::rotate(tmp, glm::radians(-rotacionBrazoIx), glm::vec3(1.0f, 0.0f, 0.0f));
		tmp = glm::rotate(tmp, glm::radians(rotacionBrazoIy), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", tmp);
		brazoi.Draw(staticShader);
	}
	void animacion() {
		switch (edoTraslacion) {
		case 0:
			dx+=5;
			if (dx > 100) {
				edoTraslacion = 4;
				
				edoSaludo = 3;
			}
			break;
		case 1:
			dz-=5;
			if (dz < -100) {
				edoTraslacion = 2;
				rot += 90;
			}
			break;
		case 2:
			dx-=5;
			if (dx <= 0) {
				edoTraslacion = 3;
				rot += 90;
			}
			break;
		case 3:
			dz+=5;
			if (dz >= 0) {
				edoTraslacion = 0;
				rot +=90;
			}
			break;
		case 4://estado espera

			break;
		}
		
		//movimientos piernas
		if (edoPies == 0&& edoTraslacion!=4) {
			rotacionPiernas+=4;
			if (rotacionPiernas > 45)
				edoPies = 1;
		}else {
			if (edoTraslacion != 4) {
				rotacionPiernas -= 4;
				if (rotacionPiernas < -45)
					edoPies = 0;
			}
			else
				rotacionPiernas = 0;
		}
		switch (edoSaludo)
		{
			case 0:
				rotacionBrazoDx = -45;
				rotacionBrazoIx = -45;
				rotacionTorzo = 0;
				break;
			case 1:
				rotacionTorzo += 2;
				rotacionBrazoDx += 2.5;
				if (rotacionTorzo >= 20)
					edoSaludo = 2;
				break;
			case 2:
				rotacionTorzo -= 2;
				rotacionBrazoDx -= 2.5;
				if (rotacionTorzo <= -20) {
					edoSaludo = 1;
					contador++;
					if (contador == 2) {
						edoSaludo = 4;
						rotacionTorzo =0;
						contador = 0;
					}
				}
				break;
			case 3:
				rotacionBrazoDx += 5;
				if (rotacionBrazoDx >= 30)
					edoSaludo = 1;
				break;
			case 4:
				rotacionBrazoDx -= 5;
				if (rotacionBrazoDx <= -45) {

					edoSaludo = 0;
					edoTraslacion = 1;
					rot = +90;
				}
				break;
		default:
			break;
		}
		
		///Movimientos brazos al caminar
		if (edoSaludo == 0) {
			if (edobrazo == 0) {
				rotacionBrazoDy += 2;
				rotacionBrazoIy += 2;
				if (rotacionBrazoIy >= 45)
					edobrazo = 1;
			}
			else {
				rotacionBrazoDy -= 2;
				rotacionBrazoIy -= 2;
				if (rotacionBrazoIy <= -45)
					edobrazo = 0;
			}
		}
		else {
			rotacionBrazoDy = 0;
			rotacionBrazoIy = 0;
		}
	}
};
class carro
{
public:
	float posx, posy, posz,movz,movx,movy,rot,roty;
	float rotLlanta;
	glm::vec3 cajaPosition;
	glm::mat4 model, tmp;
	Model cuerpo = Model("resources/objects/carro/cuerpo.obj");
	Model rueda = Model("resources/objects/carro/llanata.obj");
	carro(float x, float y, float z) {
		posx = x;
		posy = y;
		posz = z;

	}

	void dibujar(Shader staticShader) {
		model = glm::translate(glm::mat4(1.0f), glm::vec3(posx+movx, posy+movy, posz+movz));
		model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(roty), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", model);
		cuerpo.Draw(staticShader);

		tmp = glm::translate(model, glm::vec3(8.539f,0.0f,-4.715f));
		tmp = glm::rotate(tmp, glm::radians(rotLlanta), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", tmp);
		rueda.Draw(staticShader);

		tmp = glm::translate(model, glm::vec3(8.539f, 0.0f, 4.715f));
		tmp = glm::rotate(tmp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = glm::rotate(tmp, glm::radians(-rotLlanta), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", tmp);
		rueda.Draw(staticShader);

		tmp = glm::translate(model, glm::vec3(33.193f, 0.0f, -4.715f));
		tmp = glm::rotate(tmp, glm::radians(rotLlanta), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", tmp);
		rueda.Draw(staticShader);

		tmp = glm::translate(model, glm::vec3(33.193f, 0.0f, 4.715f));
		tmp = glm::rotate(tmp, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp = glm::rotate(tmp, glm::radians(-rotLlanta), glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setMat4("model", tmp);
		rueda.Draw(staticShader);
		
	}
	void animacion(int *estadosCarro) {
		switch (*estadosCarro) {
		case 0:
			movx = 0;
			movz = 0;
			movy = 0;
			rot = 0;
			roty = 0;
			break;

		case 1:
			rotLlanta++;
			movx -= 5;
			
			if (movx <= -130) {
				*estadosCarro = 2;
				rot = 90;
			}
			break;
		case 2:
			rotLlanta--;
			movz -= 5;

			if (movz <= -286) {
				*estadosCarro = 3;
			}
			break;
		case 3:
			rotLlanta--;
			movz += 5;

			if (movz >= 0) {
				*estadosCarro = 4;
			}
			break;
		case 4:
			movz += 1.98;
			movy -= 1;
			roty += 4.7368;
			if (movy <= -35)
				*estadosCarro = 5;
			break;
		}
		if (*estadosCarro==1||*estadosCarro==4) {
			rotLlanta+=5;
		}else
			rotLlanta -= 5;
	}
};
void saveFrame(void)
{
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;

}
void change_ligth(void);

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

			i_curr_steps++;
		}

	}


	change_ligth();
	
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
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
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
	// -----------sala
	Model piso("resources/objects/piso_escena/piso.obj");
	Model arboles("resources/objects/piso_escena/snowtree.obj");
	Model snowman("resources/objects/piso_escena/snowman.obj");
	Model estructura_inferior("resources/objects/estructura_piso_inferior/estructura.obj");
	Model mesa_sala("resources/objects/Elementos_sala/mesa.obj");
	
	
	Model estructura_inferior2("resources/objects/estructura_pso2/estructura.obj");
	Model puerta("resources/objects/estructura_pso2/puerta.obj");
	Model cortina("resources/objects/estructura_pso2/cortina.obj");
	Model regadera("resources/objects/estructura_pso2/regadera.obj");
	Model cama("resources/objects/estructura_pso2/cama.obj");
	Model armario("resources/objects/estructura_pso2/armario.obj");
	Model adorno("resources/objects/estructura_pso2/adorno.obj");
	Model ebanio("resources/objects/estructura_pso2/ebanio.obj");
	Model techo("resources/objects/estructura_pso2/techo.obj");

	Model arbol_sala("resources/objects/Elementos_sala/arbol_navidad.obj");
	Model sillon1("resources/objects/Elementos_sala/sillon1.obj");

	Model regalos("resources/objects/Elementos_sala/regalos.obj");
	
	//--------------garage

	Model estanterias("resources/objects/Elemenos_garage/Estanteria.obj");
	//Model carro("resources/objects/Elemenos_garage/carro.obj");
	//--------------------------------+++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//--------------------comedor
	Model ecomedor("resources/objects/Elementos_comedor/Ecomedor.obj");
	Model ecocina("resources/objects/elementos cocina/ecocina.obj");
	
	//Elementos animacion
	
	Jack jack(-10.0f, 3.0f, 0.0f);
	pistolaTenis pistola(-10.582f, 34.957f, 158.88f);
	carro car(-11.497,84.0,131.0);
	Santa santa(-65.0f, 9.0f, -118.0f);

	//musica
	PlaySound(TEXT("jingle-bell-rock-lyrics.wav"), NULL, SND_FILENAME|SND_LOOP | SND_ASYNC); // Background music
	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

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


		animate();
		jack.animacion_jack(&estadosJack);
		pistola.animar(&estadosPistola,&estadoPelota);
		car.animacion(&estadoCarro);
		santa.animacion();
		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 0.5f, 0.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(rot_x - 40, 0.0, rot_z - 40));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.09f);
		staticShader.setFloat("pointLight[1].quadratic", 0.064f);

		staticShader.setVec3("pointLight[2].position", glm::vec3(80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].ambient", glm::vec3(0.0f, 0.0f, 1.0f));
		staticShader.setVec3("pointLight[2].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[2].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[2].constant", 1.0f);
		staticShader.setFloat("pointLight[2].linear", 0.009f);
		staticShader.setFloat("pointLight[2].quadratic", 0.032f);

		staticShader.setVec3("pointLight[3].position", glm::vec3(0.0f, 0.0f, 40.0f));
		staticShader.setVec3("pointLight[3].ambient", glm::vec3(luz_x, luz_y, luz_z));
		staticShader.setVec3("pointLight[3].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[3].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[3].constant", 1.0f);
		staticShader.setFloat("pointLight[3].linear", 0.009f);
		staticShader.setFloat("pointLight[3].quadratic", 0.032f);

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


		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------


		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		
		
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		jack.dibujar(staticShader);

		pistola.dibujar(staticShader);

		car.dibujar(staticShader);
		santa.dibujar(staticShader);

		
		model = glm::mat4(1.0f);
		model = glm::translate(model,glm::vec3(0.0f,-2.0f,0.0f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);
		arboles.Draw(staticShader);
		snowman.Draw(staticShader);
		model = glm::translate(model, glm::vec3(-400.0f, 0.0f, 0.00f));
		staticShader.setMat4("model", model);
		arboles.Draw(staticShader);
		
		
		//-----plantabaja+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		model = glm::mat4(1.0f);
		staticShader.setMat4("model", model);
		estructura_inferior.Draw(staticShader);
		model = glm::mat4(1.0f);
		staticShader.setMat4("model", model);
		regalos.Draw(staticShader);
		mesa_sala.Draw(staticShader);
		estanterias.Draw(staticShader);
		
		model = glm::translate(model, glm::vec3(0.298, 76.26f ,0.0f));
		staticShader.setMat4("model", model);
		estructura_inferior2.Draw(staticShader);
		model = glm::mat4(1.0f);
		staticShader.setMat4("model", model);
		arbol_sala.Draw(staticShader);
		
		model = glm::mat4(1.0f);
		staticShader.setMat4("model", model);
		sillon1.Draw(staticShader);
		
		
		
		
		//carro.Draw(staticShader);



		model = glm::mat4(1.0f);
		staticShader.setMat4("model", model);
		ecomedor.Draw(staticShader);
		ecocina.Draw(staticShader);

		//++++++++++++++++++++++++++++++++++++++++++++++++++++++piso2
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-90.882f, 80.432f, -8.93f));
		staticShader.setMat4("model", model);
		puerta.Draw(staticShader);
		model = glm::translate(model, glm::vec3(0.0f,0.0f, -106.12f));
		staticShader.setMat4("model", model);
		puerta.Draw(staticShader);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -85.12f));
		staticShader.setMat4("model", model);
		puerta.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model,glm::vec3(3.03f,93.741f,215.753f));
		staticShader.setMat4("model", model);
		cortina.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(40.457f, 144.967f, 240.792f));
		staticShader.setMat4("model", model);
		regadera.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.244f, 93.143f, -214.69f));
		staticShader.setMat4("model", model);
		cama.Draw(staticShader);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 92.5f));
		staticShader.setMat4("model", model);
		cama.Draw(staticShader);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 100.6f));
		staticShader.setMat4("model", model);
		cama.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-83.132f, 117.53f, 29.716f));
		staticShader.setMat4("model", model);
		armario.Draw(staticShader);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -97.022f));
		staticShader.setMat4("model", model);
		armario.Draw(staticShader);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -93.387f));
		staticShader.setMat4("model", model);
		armario.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-161.404f, 130.759f, -65.158f));
		staticShader.setMat4("model", model);
		adorno.Draw(staticShader);

		model = glm::mat4(1.0f);
		staticShader.setMat4("model", model);
		ebanio.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-62.587f, 156.039f, 0.0f));
		staticShader.setMat4("model", model);
		techo.Draw(staticShader);
		
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
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		if (estadosJack == 0)
			estadosJack = 1;
		if (estadosJack == 5)
			estadosJack = 0;
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		if (estadosPistola == 0)
			estadosPistola = 1;
		else {
			estadosPistola = 0;
			estadoPelota = 0;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		if (estadoCarro == 0)
			estadoCarro = 1;
		else {
			estadoCarro = 0;
		}
	}
	
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
	
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;
	//-------------------------------------------------activacion Jack

	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		animacion ^= true;

	}
	if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
		

	}
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
void change_ligth(void) {

	if (cont1++ == 20) {
		cont1 = 0;
		luz_x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);;
		luz_y = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);;
		luz_z = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);;
		rot_x = glm::sin(cont2++) * 5;
		rot_z = glm::cos(cont2) * 5;

	}
	if (cont3++ == 50) {
		cont3 = 0;
		lightDirection.x = glm::sin(cont4++);
		lightDirection.y = glm::cos(cont4);
	}


}




