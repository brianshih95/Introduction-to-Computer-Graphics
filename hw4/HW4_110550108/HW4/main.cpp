#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Object.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
unsigned int createShader(const char* filename, const char* type);
unsigned int createProgram(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader);
unsigned int ModelVAO(Object* model);
void LoadTexture(unsigned int& texture, const char* tFileName);
glm::mat4 getPerspective();
glm::mat4 getView();

Object* duckModel = new Object("obj/duck.obj");
Object* dolphinModel = new Object("obj/dolphin.obj");
Object* bombModel = new Object("obj/Atomic bomb.obj");

int windowWidth = 800, windowHeight = 600;

bool drop = false;
bool bigger = false;
bool scaling = false;
bool explosion = false;
float height = 0.0f;
float magnitude = 0.0f;
float scaleFactor = 1.0f;
float explosionTime = 0.0f;
int squirt = 0;
int onFire = 0;
int strength;

int main()
{
	// Initialization
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFE_OPENGL_FORWARD_COMPACT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "HW4", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSwapInterval(1);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Shaders
	unsigned int vertexShader, fragmentShader, geometryShader, shaderProgram;
	vector<unsigned int> duckPrograms, dolphinPrograms, bombPrograms;
	vertexShader = createShader("shaders/explosion.vert", "vert");
	geometryShader = createShader("shaders/explosion.geom", "geom");
	fragmentShader = createShader("shaders/explosion.frag", "frag");
	shaderProgram = createProgram(vertexShader, geometryShader, fragmentShader);
	duckPrograms.push_back(shaderProgram);
	dolphinPrograms.push_back(shaderProgram);
	bombPrograms.push_back(shaderProgram);

	// Texture
	unsigned int duckTexture, dolphinTexture, bombTexture;
	LoadTexture(duckTexture, "obj/duck_diffuse.jpg");
	LoadTexture(dolphinTexture, "obj/dolphin_diffuse.jpg");
	LoadTexture(bombTexture, "obj/Atomic bomb.jpg");

	// VAO, VBO
	unsigned int duckVAO, dolphinVAO, bombVAO;
	duckVAO = ModelVAO(duckModel);
	dolphinVAO = ModelVAO(dolphinModel);
	bombVAO = ModelVAO(bombModel);

	// Display loop
	glfwGetFramebufferSize(window, &windowWidth, &windowHeight);
	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	double lastTime = glfwGetTime();
	double currentTime;
	unsigned int mLoc, vLoc, pLoc;

	while (!glfwWindowShouldClose(window))
	{
		if (explosion) {
			float color = max(1 - (lastTime - explosionTime) / 10, 0.1);
			glClearColor(color, color, color, 0.0f);
		}
		else {
			glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		}

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 perspective = getPerspective();
		glm::mat4 view = getView();
		glm::mat4 duck = glm::mat4(1.0f);
		glm::mat4 dolphin = glm::mat4(1.0f);
		glm::mat4 bomb = glm::mat4(1.0f);

		duck = glm::translate(duck, glm::vec3(0.5, -0.5, 0.0));
		duck = glm::rotate(duck, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		duck = glm::rotate(duck, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		duck = glm::scale(duck, glm::vec3(0.015, 0.015, 0.015));

		dolphin = glm::translate(dolphin, glm::vec3(-0.5, -0.8, 0.0));
		dolphin = glm::rotate(dolphin, glm::radians(-120.0f), glm::vec3(0, 1, 0));
		dolphin = glm::rotate(dolphin, glm::radians(-90.0f), glm::vec3(1, 0, 0));
		dolphin = glm::scale(dolphin, glm::vec3(0.01, 0.01, 0.01));

		bomb = glm::translate(bomb, glm::vec3(0, 3 - height, 0));
		bomb = glm::scale(bomb, glm::vec3(0.0005, 0.0005, 0.0005));

		for (unsigned int program : duckPrograms)  
		{
			glUseProgram(program);
			mLoc = glGetUniformLocation(program, "M");
			vLoc = glGetUniformLocation(program, "V");
			pLoc = glGetUniformLocation(program, "P");

			glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(duck));
			glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(perspective));

			glUniform1f(glGetUniformLocation(program, "scaleFactor"), scaleFactor);
			glUniform1f(glGetUniformLocation(program, "magnitude"), magnitude);
			glUniform1f(glGetUniformLocation(program, "time"), lastTime);
			glUniform1i(glGetUniformLocation(program, "squirt"), 0);
			glUniform1i(glGetUniformLocation(program, "onFire"), 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, duckTexture);

			glBindVertexArray(duckVAO);
			glDrawArrays(GL_TRIANGLES, 0, duckModel->positions.size());
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
		}

		for (unsigned int program : dolphinPrograms)
		{
			glUseProgram(program);
			mLoc = glGetUniformLocation(program, "M");
			vLoc = glGetUniformLocation(program, "V");
			pLoc = glGetUniformLocation(program, "P");

			glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(dolphin));
			glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(perspective));

			glUniform1f(glGetUniformLocation(program, "scaleFactor"), scaleFactor);
			glUniform1f(glGetUniformLocation(program, "magnitude"), magnitude);
			glUniform1f(glGetUniformLocation(program, "time"), lastTime);
			glUniform1i(glGetUniformLocation(program, "squirt"), squirt);
			glUniform1i(glGetUniformLocation(program, "onFire"), 0);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, dolphinTexture);

			glBindVertexArray(dolphinVAO);
			glDrawArrays(GL_TRIANGLES, 0, dolphinModel->positions.size());
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
		}

		for (unsigned int program : bombPrograms)
		{
			glUseProgram(program);
			mLoc = glGetUniformLocation(program, "M");
			vLoc = glGetUniformLocation(program, "V");
			pLoc = glGetUniformLocation(program, "P");

			glUniformMatrix4fv(mLoc, 1, GL_FALSE, glm::value_ptr(bomb));
			glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(pLoc, 1, GL_FALSE, glm::value_ptr(perspective));

			glUniform1f(glGetUniformLocation(program, "scaleFactor"), 1);
			glUniform1f(glGetUniformLocation(program, "magnitude"), magnitude);
			glUniform1f(glGetUniformLocation(program, "time"), 0);
			glUniform1i(glGetUniformLocation(program, "squirt"), 0);
			glUniform1i(glGetUniformLocation(program, "onFire"), onFire);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, bombTexture);

			glBindVertexArray(bombVAO);
			glDrawArrays(GL_TRIANGLES, 0, bombModel->positions.size());
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindVertexArray(0);
		}

		if (drop && !explosion) {
			height += 0.02;
			if (height > 3.5) {
				explosion = true;					
				drop = false;
			}
		}

		if (explosion) {
			if (explosionTime == 0.0f)
				explosionTime = lastTime;
			scaling = false;
			squirt = 0;
			onFire = 1;

			if (magnitude < 1)
				strength = 5;
			else if (magnitude < 2)
				strength = 3;
			else
				strength = 1;

			magnitude += 0.01f * strength;
		}
		else if (scaling) {
			if (bigger) {
				scaleFactor += 0.01;
				if (scaleFactor > 2.0f)
					bigger = false;
			}
			else {
				scaleFactor -= 0.01;
				if (scaleFactor < 0.1f)
					bigger = true;
			}
		}

		// Status update
		currentTime = glfwGetTime();
		lastTime = currentTime;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

// Add key callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		cout << "KEY 1 PRESSED\n";
		if (!explosion) {
			squirt = !squirt;
			if (squirt)
				cout << "Start squirting\n\n";
			else
				cout << "Stop squirting\n\n";
		}
	}

	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		scaling = true;
		bigger = !bigger;
		cout << "KEY 2 PRESSED\n";

		if (!explosion)
			cout << "Start zooming\n\n";
	}

	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		drop = true;
		cout << "KEY 3 PRESSED\n";

		if (!explosion)
			cout << "Drop the atomic bomb\n\n";
	}
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	windowWidth = width;
	windowHeight = height;
}

unsigned int createShader(const char* filename, const char* type)
{
	unsigned int shader;
	if (strcmp(type, "vert") == 0)
		shader = glCreateShader(GL_VERTEX_SHADER);
	else if (strcmp(type, "frag") == 0)
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	else if (strcmp(type, "geom") == 0)
		shader = glCreateShader(GL_GEOMETRY_SHADER);
	else
	{
		cout << "Unknown Shader Type.\n";
		return 0;
	}

	FILE* fp = fopen(filename, "rb");
	fseek(fp, 0, SEEK_END);
	long fsize = ftell(fp);
	fseek(fp, 0, SEEK_SET);  //same as rewind(fp);

	char* source = (char*)malloc(sizeof(char) * (fsize + 1));
	fread(source, fsize, 1, fp);
	fclose(fp);

	source[fsize] = 0;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		cout << "ERROR::SHADER::" << type << "::COMPLIATION_FAILED\n" << infoLog << endl;
		return -1;
	}

	return shader;
}

unsigned int createProgram(unsigned int vertexShader, unsigned int geometryShader, unsigned int fragmentShader)
{
	unsigned int program = glCreateProgram();

	//Attach our shaders to our program
	glAttachShader(program, vertexShader);
	glAttachShader(program, geometryShader);
	glAttachShader(program, fragmentShader);

	//Link our program
	glLinkProgram(program);

	//Note the different functions here: glGetProgram* instead of glGetShader*.
	int success = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	if (!success)
	{
		int maxLength = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

		//The maxLength includes the NULL character
		char* infoLog = (char*)malloc(sizeof(char) * (maxLength));
		glGetProgramInfoLog(program, maxLength, &maxLength, infoLog);

		//We don't need the program anymore.
		glDeleteProgram(program);
		//Don't leak shaders either.
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);

		cout << "LINKING ERROR: ";
		puts(infoLog);
		free(infoLog);

		return -1;
	}

	//Always detach shaders after a successful link.
	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	return program;
}

unsigned int ModelVAO(Object* model)
{
	unsigned int VAO, VBO[3];
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(3, VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->positions.size()), &(model->positions[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->normals.size()), &(model->normals[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * (model->texcoords.size()), &(model->texcoords[0]), GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

void LoadTexture(unsigned int& texture, const char* tFileName) {
	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(tFileName, &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(data);
}

glm::mat4 getPerspective()
{
	return glm::perspective(
		glm::radians(45.0f),
		(float)windowWidth / (float)windowHeight, 
		1.0f, 100.0f);
}

glm::mat4 getView()
{
	return glm::lookAt(
		glm::vec3(0, 4, 4),
		glm::vec3(0, 0, 0),
		glm::vec3(0, 1, 0));
}
