#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void frame_resize(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
}

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main() {

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "thefirstone", NULL, NULL);
	if (window == NULL) {
		std::cout << "windowing didnt opened" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, frame_resize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "failed to init glad" << std::endl;
		return -1;
	}

	unsigned int ver_shader;
	ver_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(ver_shader, 1, &vertexShaderSource, NULL);
	glCompileShader(ver_shader);

	int success;
	char infoLog[512];
	glGetShaderiv(ver_shader, GL_COMPILE_STATUS, &success);
	if(success){
		glGetShaderInfoLog(ver_shader, 512, NULL, infoLog);
		std::cout << "ver_success: " << infoLog << std::endl;
	}
	else {
		glGetShaderInfoLog(ver_shader, 512, NULL, infoLog);
		std::cout << "frag_failed: " << infoLog << std::endl;
	}

	unsigned int frag_shader;
	frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(frag_shader, 1, &fragmentShaderSource, NULL);
	glCompileShader(frag_shader);

	glGetShaderiv(ver_shader, GL_COMPILE_STATUS, &success);
	if (success) {
		glGetShaderInfoLog(ver_shader, 512, NULL, infoLog);
		std::cout << "frag_success: " << infoLog << std::endl;
	}
	else {
		glGetShaderInfoLog(ver_shader, 512, NULL, infoLog);
		std::cout << "frag_failed: " << infoLog << std::endl;
	}

	unsigned int shader_program;
	shader_program = glCreateProgram();

	glAttachShader(shader_program, ver_shader);
	glAttachShader(shader_program, frag_shader);
	glLinkProgram(shader_program);

	glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
	if (success) {
		glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
		std::cout << "linking success: " << infoLog << std::endl;
	}
	else {
		glGetShaderInfoLog(ver_shader, 512, NULL, infoLog);
		std::cout << "linking failed: " << infoLog << std::endl;
	}

	glDeleteShader(ver_shader);
	glDeleteShader(frag_shader);

	float vertices[] = {
		 0.5f,  0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		-0.5f,  0.5f, 0.0f
		 
	};

	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0); // voa

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	unsigned int ebo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render-loop or frame
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader_program);

		glUseProgram(shader_program);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		process_input(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteProgram(shader_program);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glfwTerminate();
	return 0;
}