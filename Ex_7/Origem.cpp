#include <glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <assert.h>
#include <math.h>

# define PI           3.14159265358979323846

using namespace std;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int setupShader();
int setupGeometry();
GLuint generateCircle(float radius, int nPoints);
int nPoints = 3600;

const GLuint WIDTH = 800, HEIGHT = 600;

const GLchar* vertexShaderSource = "#version 450\n"
"layout (location = 0) in vec3 position;\n"
"void main()\n"
"{\n"
//...pode ter mais linhas de código aqui!
"gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";

const GLchar* fragmentShaderSource = "#version 450\n"
"uniform vec4 inputColor;\n"
"out vec4 color;\n"
"void main()\n"
"{\n"
"color = inputColor;\n"
"}\n\0";


int main()
{
	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ex_6! - Otavio P Forest", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	GLuint shaderID = setupShader();


	//float radius = 1;
	//int nPoints = 20;
	GLuint VAO = NULL;
	//GLuint VAO = generateCircle(radius, nPoints);



	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");
	assert(colorLoc > -1);

	glUseProgram(shaderID);


	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.8f, 0.8f, 0.8f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		VAO = generateCircle(1, nPoints);

		glLineWidth(2);
		glPointSize(1);

		// Chamada de desenho - drawcall
		// Poligono Preenchido - GL_TRIANGLES
		glUniform4f(colorLoc, 1.0f, 0.0f, 1.0f, 1.0f); //enviando cor para variável uniform inputColor
		glBindVertexArray(VAO);
		
		glDrawArrays(GL_LINE_STRIP_ADJACENCY, 0, nPoints);//trocar aqui para pacman

		glUniform4f(colorLoc, 1.0f, 0.0f, 1.0f, 1.0f);
		glDrawArrays(GL_POINTS, 0, nPoints + 1);
		
		glBindVertexArray(0);
	
		// Chamada de desenho - drawcall
		// CONTORNO - GL_LINE_LOOP
		// PONTOS - GL_POINTS
		 //enviando cor para variável uniform inputColor



		glfwSwapBuffers(window);
	}
	glDeleteVertexArrays(1, &VAO);
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

}

int setupShader()
{
	// Vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// Checando erros de compilação (exibição via log no terminal)
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// Checando erros de compilação (exibição via log no terminal)
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Linkando os shaders e criando o identificador do programa de shader
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// Checando por erros de linkagem
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

GLuint generateCircle(float radius, int Points) {
	int totalSize = Points * 3;//trocar aqui para pacman
	GLfloat* vertices = new GLfloat[totalSize];
	//GLfloat* vertices2 = new GLfloat[totalSize];

	double angle = 0.0;
	GLfloat slice = 1;

	vertices[0] = 0.0;
	vertices[1] = 0.0;
	vertices[2] = 0.0;

	double rad = 1;

	for (int i = 3; i < totalSize; i += 3) {
		vertices[i] = rad * cos(angle);
		vertices[i + 1] = rad * sin(angle);
		vertices[i + 2] = 0.0;

		//vertices2[totalSize -i+2] = 0.0;
		//vertices2[totalSize -i+1] = rad * sin(angle);
		//vertices2[totalSize -i+0] = rad * cos(angle);

		angle -= 0.2;
		rad = rad / 1.01;
	}

	//vertices2[0] = 0.0;
	//vertices2[1] = 0.0;
	//vertices2[2] = 0.0;
	



	GLuint VBO, VAO;

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, totalSize * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}
