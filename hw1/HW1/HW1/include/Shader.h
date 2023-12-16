#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

using namespace std;

class Shader
{
public:
	unsigned int program;

	Shader(const string& vertPath, const string& fragPath)
	{
		unsigned int vertexShader = createShader(vertPath);
		unsigned int fragmentShader = createShader(fragPath);

		program = createProgram(vertexShader, fragmentShader);
	}

private:
	unsigned int createShader(const string& filename)
	{
		unsigned int shader;

		// Find the type of shader
		int split = filename.find('.') + 1;
		string type = filename.substr(split);

		if (type == "vert")
			shader = glCreateShader(GL_VERTEX_SHADER);
		else if (type == "frag")
			shader = glCreateShader(GL_FRAGMENT_SHADER);
		else
		{
			cout << "Unknown Shader Type.\n";
			return 0;
		}

		// Read the code of shader file
		ifstream fs(filename);
		stringstream ss;
		string s;
		while (getline(fs, s))
		{
			ss << s << "\n";
		}
		string temp = ss.str();
		const char* source = temp.c_str();

		// Compile shader
		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		// Debug info
		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			cout << "ERROR::SHADER::" << type << "::COMPLIATION_FAILED\n" << infoLog << "\n";
			return 0;
		}

		return shader;
	}

	unsigned int createProgram(unsigned int vertexShader, unsigned int fragmentShader)
	{
		unsigned int program = glCreateProgram();

		//Attach our shaders to our program
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);

		//Link our program
		glLinkProgram(program);

		//Note the different functions here: glGetProgram* instead of glGetShader*.
		int success = 0;
		char infoLog[512];
		glGetProgramiv(program, GL_LINK_STATUS, &success);

		if (!success)
		{
			glGetProgramInfoLog(program, 512, NULL, infoLog);

			//We don't need the program anymore.
			glDeleteProgram(program);
			//Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << "\n";
			return -1;
		}

		//Always detach shaders after a successful link.
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);

		return program;
	}
};