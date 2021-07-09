#include <files.hpp>
#include <model.hpp>
#include <cam.hpp>
#include <vector>
#include <stdlib.h> 
#include<figures.h>
#include<hitbox.h>

const u32 FSIZE = sizeof(f32);
const u32 ISIZE = sizeof(u32);
const u32 SCR_WIDTH = 1280;
const u32 SCR_HEIGHT = 720;
const f32 ASPECT = (f32)SCR_WIDTH / (f32)SCR_HEIGHT;

Cam* cam;

f32  deltaTime = 0.0f;
f32  lastFrame = 0.0f;
bool wireframe = false;

int n = 5;
glm::vec3 base = glm::vec3(0.5f, -1.25f, -4.0f);
glm::vec3 posRock = glm::vec3(0.80f, -1.10f, -4.0f);
int bala = 0;
int estado = 0;

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam->processKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam->processKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam->processKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam->processKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		bala = 1;
		estado = 1;
    }
    //if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		
		//bala = 2;
    
    //}
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (base.x - deltaTime * 0.5 >= -2.5f && estado==0) {
            base.x -= deltaTime * 5;
			posRock.x -= deltaTime * 5;
        }
		else
		{
			if (base.x - deltaTime * 0.5 >= -2.5f)
				base.x -= deltaTime * 5;
		}
    }
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		if (base.x + deltaTime * 0.5 < 2.5f && estado ==0) {
			base.x += deltaTime * 5;
			posRock.x += deltaTime * 5;
		}
		else
		{
			if (base.x + deltaTime * 0.5 < 2.5f && estado == 0)
				base.x += deltaTime * 5;
		}
	}
}
void key_callback(GLFWwindow*, int key, int, int act, int) {
	wireframe ^= key == GLFW_KEY_E && act == GLFW_PRESS;
}
void mouse_callback(GLFWwindow* window, f64 xpos, f64 ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		cam->movePov(xpos, ypos);
	}
	else {
		cam->stopPov();
	}
}
void scroll_callback(GLFWwindow*, f64, f64 yoffset) {
	cam->processScroll((f32)yoffset);
}

i32 main() {
	GLFWwindow* window = glutilInit(3, 3, SCR_WIDTH, SCR_HEIGHT, "Cubito");
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	cam = new Cam();

	Files* files = new Files("bin", "resources/textures", "resources/objects");

	Shader* shader = new Shader(files, "shader.vert", "shader.frag");
	Model* monito = new Model(files, "window/window.obj");
	Model* rock = new Model(files, "planet/planet.obj");
	Shader* bloques = new Shader(files, "shader2.vert", "shader2.frag");
	
	Cube* cubex = new Cube(0.99f, 0.99f, 0.99f);

	glm::vec3 lightPos = glm::vec3(1.0f);
	glm::vec3 lightColor = glm::vec3(1.0f);
	glm::vec3 dx(0.5f, -1.25f, -4.0f); 

	srand(time(NULL));
	int n = 3;
	int count = 0;
	std::vector<glm::vec3> positions(n * (n-1));
	for (u32 i = 0; i < n; ++i) {
		positions[count] = glm::vec3(i - 1.0f, -1.00f, -11.0f);  //(i-n/2.0f, sin(i*j/50.0f), j-n/2.0f)
		count++;
		
	}

	//roca
	//auto rndb = [](f32 a, f32 b) -> f32 {
	//	f32 d = b - a;
	//	return (rand() % 1000) / 1000.0f * d + a;
	//};
	//
	//posRock.x = rndb(1.0, n - 1.0);
	//posRock.y = rndb(1.0, n - 1.0);
	//
	//u32 bx = rand() % 2;
	//u32 by = rand() % 2;
	//glm::vec3 velocidad = glm::vec3(rndb(0.02, 0.03), rndb(0.02, 0.03), -0.04f);
	//if (bx) velocidad.x *= -1;
	//if (by) velocidad.y *= -1;

	u32 cubeVao, lightCubeVao, vbo, ebo;
	glGenVertexArrays(1, &cubeVao);
	glGenVertexArrays(1, &lightCubeVao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(cubeVao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBufferData(GL_ARRAY_BUFFER, cubex->getVSize() * FSIZE,
		cubex->getVertices(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, cubex->getISize() * ISIZE,
		cubex->getIndices(), GL_STATIC_DRAW);

	// posiciones
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cubex->len(), cubex->skip(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, cubex->len(), cubex->skip(3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, cubex->len(), cubex->skip(6));
	glEnableVertexAttribArray(2);

	glBindVertexArray(lightCubeVao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cubex->len(), cubex->skip(0));
	glEnableVertexAttribArray(0);

	int cambiox = 1;
	int cambioy = 1;

	int cambioz = 1;

	int change = 1;
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		f32 currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);

		shader->use();
		lightPos.x = 2.0f * (cos(currentFrame) - sin(currentFrame));
		lightPos.z = 2.0f * (cos(currentFrame) + sin(currentFrame));
		shader->setVec3("xyz", lightPos);
		shader->setVec3("xyzColor", lightColor);
		shader->setVec3("xyzView", cam->pos);
		glm::mat4 proj = glm::perspective(cam->zoom, ASPECT, 0.1f, 100.0f);
		shader->setMat4("proj", proj);
		shader->setMat4("view", cam->getViewM4());

		//rock
		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, posRock);  //base
		if (bala == 1) { posRock.z -= deltaTime*2; }
		//if (cambioy == 1) { dx.y += deltaTime; }
		//if (cambioy == 0) { dx.y -= deltaTime; }
		//if (dx.y > 1.80f) { cambioy = 0; }
		//if (dx.y < -1.50f) { cambioy = 1; }
		//std::cout << dx.x<<'\n';
		model = scale(model, glm::vec3(0.05f));
		shader->setMat4("model", model);
		rock->Draw(shader);

		//base
		model = glm::mat4(1.0f);
		model = glm::translate(model, base);
		model = scale(model, glm::vec3(0.50f));
		model = rotate(model, 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		model = rotate(model, 30.0f, glm::vec3(0.0f, 0.0f, 0.5f));
		shader->setMat4("model", model);
		monito->Draw(shader);

		//bloques
		glm::vec3 z(0.0f, 0.0f, 0.001f);

		bloques->use();
		glBindVertexArray(cubeVao);
		model = glm::mat4(1.0f);
		bloques->setVec3("xyz", lightPos);
		bloques->setVec3("xyzColor", lightColor);
		bloques->setVec3("xyzView", cam->pos);
		bloques->setMat4("proj", proj);
		bloques->setMat4("view", cam->getViewM4());
		for (int i = 0;i< positions.size();i++) {
			model = glm::mat4(1.0f);
			positions[i] += z;
			model = glm::translate(model, positions[i]);
			bloques->setMat4("proj", proj);
			bloques->setMat4("view", cam->getViewM4());
			model = scale(model, glm::vec3(0.20f));
			bloques->setMat4("model", model);

			if (abs(positions[i].x - posRock.x) < 0.45f && abs(positions[i].z - posRock.z) < 0.30f) {
				positions.erase(positions.begin() + i);
				posRock = base + glm::vec3(0.30f, 0.15,0.0f);   //glm::vec3(0.80f, -1.10f, -4.0f);  //base: glm::vec3(0.5f, -1.25f, -4.0f);
				bala = 0;
				estado = 0;
			}

			glDrawElements(GL_TRIANGLES, cubex->getISize(), GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	};

	glDeleteVertexArrays(1, &cubeVao);
	glDeleteVertexArrays(1, &lightCubeVao);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);

	delete cam;
	delete shader;
	delete monito;
	delete bloques;
	delete rock;
	delete cubex;

	return 0;
}
