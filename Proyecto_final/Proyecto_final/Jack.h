#ifndef jack
#define jack_h
#include <Windows.h>




class Jack
{
public:
	float posx, posy, posz;
	int estadosJack;
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
	void animacion_jack(void) {
		switch (estadosJack) {
		case 0:
			elevacionJack = posy;
			rotacion_tapa = 0.0f;
			break;
		case 1:
			rotacion_manivela = rotacion_manivela + 5;
			if (rotacion_manivela >= 360) {
				rotacion_manivela = 0;
				estadosJack = 2;
			}
			break;
		case 2:
			cajaPosition.x = cajaPosition.x + 0.5;
			if (cajaPosition.x >= posx + 4) {
				estadosJack = 3;
			}
			break;
		case 3:
			cajaPosition.x = cajaPosition.x - 0.5;
			if (cajaPosition.x <= posx - 4) {
				estadosJack = 4;
			}
			break;

		case 4:
			elevacionJack = elevacionJack + 1;
			rotacion_tapa = rotacion_tapa + 5;
			if (rotacion_tapa >= 90) {
				estadosJack = 5;
			}
			break;
		case 5:
			break;
		}

	}
};

#endif