#include "Demo.h"



Demo::Demo() {

}


Demo::~Demo() {
}



void Demo::Init() {
	// build and compile our shader program
	// ------------------------------------
	shaderProgram = BuildShader("vertexShader.vert", "fragmentShader.frag", nullptr);

	BuildColoredCube();

	BuildColoredPlane();
}

void Demo::DeInit() {
	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO2);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &EBO2);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void Demo::ProcessInput(GLFWwindow *window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}



}

void Demo::Update(double deltaTime) {
}

void Demo::Render() {
	glViewport(0, 0, this->screenWidth, this->screenHeight);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_DEPTH_TEST);

	// Pass perspective projection matrix
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(this->shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// LookAt camera (position, target/direction, up)
	glm::mat4 view = glm::lookAt(glm::vec3(2, 2, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	GLint viewLoc = glGetUniformLocation(this->shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// set lighting attribute
	GLint lightPosLoc = glGetUniformLocation(this->shaderProgram, "lightPos");
	glUniform3f(lightPosLoc, 0, 1, 0);
	GLint viewPosLoc = glGetUniformLocation(this->shaderProgram, "viewPos");
	glUniform3f(viewPosLoc, 0, 2, 3);
	GLint lightColorLoc = glGetUniformLocation(this->shaderProgram, "lightColor");
	glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);
	
	DrawColoredCube();

	DrawColoredPlane();

	glDisable(GL_DEPTH_TEST);
}

void Demo::BuildColoredCube() {
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	float vertices[] = {
		// format position, tex coords
		// front
		-0.5, -0.5, 0.5,  0.0f,  0.0f,  1.0f, // 0
		0.5, -0.5, 0.5,   0.0f,  0.0f,  1.0f, // 1
		0.5,  0.5, 0.5,   0.0f,  0.0f,  1.0f, // 2
		-0.5,  0.5, 0.5,  0.0f,  0.0f,  1.0f, // 3

		// right
		0.5,  0.5,  0.5, 1.0f,  0.0f,  0.0f, // 4
		0.5,  0.5, -0.5, 1.0f,  0.0f,  0.0f, // 5
		0.5, -0.5, -0.5, 1.0f,  0.0f,  0.0f, // 6
		0.5, -0.5,  0.5, 1.0f,  0.0f,  0.0f, // 7

		// back
		-0.5, -0.5, -0.5, 0.0f,  0.0f,  -1.0f, // 8 
		0.5,  -0.5, -0.5, 0.0f,  0.0f,  -1.0f, // 9
		0.5,   0.5, -0.5, 0.0f,  0.0f,  -1.0f, // 10
		-0.5,  0.5, -0.5, 0.0f,  0.0f,  -1.0f, // 11

		// left
		-0.5, -0.5, -0.5, -1.0f,  0.0f,  0.0f, // 12
		-0.5, -0.5,  0.5, -1.0f,  0.0f,  0.0f, // 13
		-0.5,  0.5,  0.5, -1.0f,  0.0f,  0.0f, // 14
		-0.5,  0.5, -0.5, -1.0f,  0.0f,  0.0f, // 15

		// upper
		0.5, 0.5,  0.5, 0.0f,  1.0f,  0.0f, // 16
		-0.5, 0.5, 0.5, 0.0f,  1.0f,  0.0f, // 17
		-0.5, 0.5, -0.5,0.0f,  1.0f,  0.0f, // 18
		0.5, 0.5, -0.5, 0.0f,  1.0f,  0.0f, // 19

		// bottom
		-0.5, -0.5, -0.5, 0.0f,  -1.0f,  0.0f, // 20
		0.5, -0.5, -0.5,  0.0f,  -1.0f,  0.0f, // 21
		0.5, -0.5,  0.5,  0.0f,  -1.0f,  0.0f, // 22
		-0.5, -0.5,  0.5, 0.0f,  -1.0f,  0.0f, // 23
	};

	unsigned int indices[] = {
		0,  1,  2,  0,  2,  3,   // front
		4,  5,  6,  4,  6,  7,   // right
		8,  9,  10, 8,  10, 11,  // back
		12, 14, 13, 12, 15, 14,  // left
		16, 18, 17, 16, 19, 18,  // upper
		20, 22, 21, 20, 23, 22   // bottom
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// define position pointer layout 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	// define normal pointer layout 2
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	// remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void Demo::DrawColoredCube()
{
	UseShader(shaderProgram);

	glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
	
	GLint objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");

	float x1 = 0;
	float z1 = 0;
	float tinggiCagak = 0.5;
	float rotasi1 = 0;
	float ceil = 5;
	float widthScaleCeil = 0.3;
	float heightScaleCeil = 0.1;

	//atap
	for (int i = 0; i < ceil; i++) {
		objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
		glUniform3f(objectColorLoc, 0.6f, 0.5f, 0.4f);

		glm::mat4 model2;
		model2 = glm::translate(model2, glm::vec3(x1, tinggiCagak + (ceil - i - 2) * heightScaleCeil, z1));
		model2 = glm::scale(model2, glm::vec3((i + 1) * widthScaleCeil, heightScaleCeil, (i + 1) * widthScaleCeil));

		glGetUniformLocation(this->shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

	//cagak
	for (int i = 0; i < 4; i++) {
		objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
		glUniform3f(objectColorLoc, 0.4f, 0.3f, 0.2f);

		glm::mat4 model2;
		float kalix = i % 3 == 0 ? -1 : 1;
		float kaliz = i < 2 ? -1 : 1;
		model2 = glm::translate(model2, glm::vec3(x1 + (kalix * widthScaleCeil * (ceil - 3)), 0, z1 + (kaliz * widthScaleCeil * (ceil - 3))));
		model2 = glm::scale(model2, glm::vec3(0.2, tinggiCagak + 0.2, 0.2));

		glGetUniformLocation(this->shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

	//tembok depan
	float lebarTembok = widthScaleCeil * (ceil - 1);
	float margin = lebarTembok / 2;
	objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
	glUniform3f(objectColorLoc, 0.9f, 0.8f, 0.8f);

	glm::mat4 model3;
	model3 = glm::translate(model3, glm::vec3(x1, -0.05, z1 + margin));
	model3 = glm::scale(model3, glm::vec3(lebarTembok, tinggiCagak + 0.1, 0.1));

	glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model3));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//tembok kanan
	objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
	glUniform3f(objectColorLoc, 0.9f, 0.8f, 0.8f);

	glm::mat4 model4;
	model4 = glm::translate(model4, glm::vec3(x1 + margin, -0.05, z1));
	model4 = glm::scale(model4, glm::vec3(0.1, tinggiCagak + 0.1, lebarTembok));

	glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model4));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//tembok belakang
	objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
	glUniform3f(objectColorLoc, 0.9f, 0.8f, 0.8f);

	glm::mat4 model5;
	model5 = glm::translate(model5, glm::vec3(x1, -0.05, z1 - margin));
	model5 = glm::scale(model5, glm::vec3(lebarTembok, tinggiCagak + 0.1, 0.1));

	glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model5));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//tembok kiri
	objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
	glUniform3f(objectColorLoc, 0.9f, 0.8f, 0.8f);

	glm::mat4 model6;
	model6 = glm::translate(model6, glm::vec3(x1 - margin, -0.05, z1));
	model6 = glm::scale(model6, glm::vec3(0.1, tinggiCagak + 0.1, lebarTembok));

	glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model6));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	//lawang
	objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
	glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.2f);

	glm::mat4 model7;
	model7 = glm::translate(model7, glm::vec3(x1, -0.15, z1 + margin + 0.005));
	model7 = glm::scale(model7, glm::vec3(0.3, 0.4, 0.1));

	glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model7));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Demo::BuildColoredPlane()
{
	// Build geometry
	GLfloat vertices[] = {
		// format position, tex coords
		// bottom
		-50.0, -0.5, -50.0,  0.0f,  1.0f,  0.0f,
		 50.0, -0.5, -50.0,  0.0f,  1.0f,  0.0f,
		 50.0, -0.5,  50.0,  0.0f,  1.0f,  0.0f,
		-50.0, -0.5,  50.0,  0.0f,  1.0f,  0.0f,


	};

	GLuint indices[] = { 0,  2,  1,  0,  3,  2 };

	glGenVertexArrays(1, &VAO2);
	glGenBuffers(1, &VBO2);
	glGenBuffers(1, &EBO2);

	glBindVertexArray(VAO2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	// Normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO
}



void Demo::DrawColoredPlane()
{
	UseShader(shaderProgram);

	glBindVertexArray(VAO2); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized

	GLint objectColorLoc = glGetUniformLocation(this->shaderProgram, "objectColor");
	glUniform3f(objectColorLoc, 0.8f, 0.8f, 0.8f);

	glm::mat4 model;
	GLint modelLoc = glGetUniformLocation(this->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

int main(int argc, char** argv) {
	RenderEngine &app = Demo();
	app.Start("Basic Lighting: Phong Lighting Model", 800, 600, false, false);
}