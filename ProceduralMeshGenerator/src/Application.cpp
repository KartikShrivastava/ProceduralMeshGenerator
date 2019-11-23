#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Renderer.h"
#include "Camera.h"
#include "CheckGLErrors.h"
#include "Shader.h"
#include "TextureStbImage.h"
#include "Model.h"
#include "expat/imgui/imgui.h"
#include "expat/imgui/imgui_impl_glfw_gl3.h"
#include "TrianglePicking.h"

void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
void ProcessInput(GLFWwindow* window);
static void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

Renderer renderer;
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));

bool stopRendering = false;
bool startPicking = false;
float scalePickFactor = 1.0f;

int main() {
	if(!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(renderer.width, renderer.height, "Procedural Mesh Generator", NULL, NULL);
	if (window == NULL) {
		std::cout << "Unable to create glfw window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetCursorPosCallback(window, MouseCallback);

	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Unable to intialize GLAD" << std::endl;
		return -1;
	}

	float verticesPlane[] = {
		-0.5f, -0.5f,  0.0f,	0.0f, 0.0f,
		 0.5f, -0.5f,  0.0f,	1.0f, 0.0f,
		-0.5f,  0.5f,  0.0f,	0.0f, 1.0f,
		 0.5f,  0.5f,  0.0f,	1.0f, 1.0f
	};

	float verticesBox[] = {
	-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f
	};
	
	unsigned int cubeVbo;
	GLCall(glGenBuffers(1, &cubeVbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, cubeVbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBox), verticesBox, GL_STATIC_DRAW));
	
	unsigned int texCubeVao;
	GLCall(glGenVertexArrays(1, &texCubeVao));
	GLCall(glBindVertexArray(texCubeVao));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glBindVertexArray(0));

	unsigned int lampVao;
	GLCall(glGenVertexArrays(1, &lampVao));
	GLCall(glBindVertexArray(lampVao));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))));
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glBindVertexArray(0));

	Shader lampShader("src/shaders/4_VertexShader.glsl", "src/shaders/3_LightFS.glsl");
	Shader mixedLightShader("src/shaders/1_VertexShader.glsl", "src/shaders/8_MixedLightFS.glsl");
	Shader trianglePickingShader("src/shaders/4_VertexShader.glsl", "src/shaders/12_PickingFS.glsl");

	TextureStbImage tex1("res/textures/wood.jpg", false);
	tex1.UnBind();

	glm::vec3 pointLightPosition[] = {
		glm::vec3(2.0f, -1.0f,  1.2f),
		glm::vec3(5.3f,  0.0f,  1.2f),
		glm::vec3(4.2f,  1.0f,  -2.0f),
		glm::vec3(3.1f,  2.5f,  1.2f)
	};

	glm::vec3 pointLightColors[] = {
		glm::vec3(1.0f, 0.5f, 0.2f),
		glm::vec3(0.2f, 1.0f, 1.0f),
		glm::vec3(0.8f, 0.2f, 0.5f),
		glm::vec3(0.2f, 0.2f, 1.0f)
	};
	
	{
		mixedLightShader.Bind();
		mixedLightShader.SetUniform1f("u_material.shininess", 64.0f);

		mixedLightShader.SetUniform3f("u_dLight.direction", -0.5f, -0.5f, -0.3f);
		mixedLightShader.SetUniform3f("u_dLight.ambient" , 0.1f, 0.1f, 0.1f);
		mixedLightShader.SetUniform3f("u_dLight.diffuse" , 0.4f, 0.4f, 0.4f);
		mixedLightShader.SetUniform3f("u_dLight.specular", 0.0f, 0.0f, 0.0f);

		mixedLightShader.SetUniform3fv("u_pLight[0].position", glm::value_ptr(pointLightPosition[0]));
		mixedLightShader.SetUniform3f("u_pLight[0].ambient", 0.0f, 0.0f, 0.0f);
		mixedLightShader.SetUniform3fv("u_pLight[0].diffuse", glm::value_ptr(pointLightColors[0]));
		mixedLightShader.SetUniform3fv("u_pLight[0].specular", glm::value_ptr(pointLightColors[0]));
		mixedLightShader.SetUniform1f("u_pLight[0].constant", 1.0f);
		mixedLightShader.SetUniform1f("u_pLight[0].linear", 0.045f);
		mixedLightShader.SetUniform1f("u_pLight[0].quadratic", 0.0075f);

		mixedLightShader.SetUniform3fv("u_pLight[1].position", glm::value_ptr(pointLightPosition[1]));
		mixedLightShader.SetUniform3f("u_pLight[1].ambient", 0.0f, 0.0f, 0.0f);
		mixedLightShader.SetUniform3fv("u_pLight[1].diffuse", glm::value_ptr(pointLightColors[1]));
		mixedLightShader.SetUniform3fv("u_pLight[1].specular", glm::value_ptr(pointLightColors[1]));
		mixedLightShader.SetUniform1f("u_pLight[1].constant", 1.0f);
		mixedLightShader.SetUniform1f("u_pLight[1].linear", 0.045f);
		mixedLightShader.SetUniform1f("u_pLight[1].quadratic", 0.0075f);

		mixedLightShader.SetUniform3fv("u_pLight[2].position", glm::value_ptr(pointLightPosition[2]));
		mixedLightShader.SetUniform3f("u_pLight[2].ambient", 0.0f, 0.0f, 0.0f);
		mixedLightShader.SetUniform3fv("u_pLight[2].diffuse", glm::value_ptr(pointLightColors[2]));
		mixedLightShader.SetUniform3fv("u_pLight[2].specular", glm::value_ptr(pointLightColors[2]));
		mixedLightShader.SetUniform1f("u_pLight[2].constant", 1.0f);
		mixedLightShader.SetUniform1f("u_pLight[2].linear", 0.045f);
		mixedLightShader.SetUniform1f("u_pLight[2].quadratic", 0.0075f);

		mixedLightShader.SetUniform3fv("u_pLight[3].position", glm::value_ptr(pointLightPosition[3]));
		mixedLightShader.SetUniform3f("u_pLight[3].ambient", 0.0f, 0.0f, 0.0f);
		mixedLightShader.SetUniform3fv("u_pLight[3].diffuse", glm::value_ptr(pointLightColors[3]));
		mixedLightShader.SetUniform3fv("u_pLight[3].specular", glm::value_ptr(pointLightColors[3]));
		mixedLightShader.SetUniform1f("u_pLight[3].constant", 1.0f);
		mixedLightShader.SetUniform1f("u_pLight[3].linear", 0.045f);
		mixedLightShader.SetUniform1f("u_pLight[3].quadratic", 0.0075f);

		mixedLightShader.SetUniform3f("u_sLight.ambient", 0.0f, 0.0f, 0.0f);
		mixedLightShader.SetUniform3f("u_sLight.diffuse", 0.8f, 0.8f, 0.8f);
		mixedLightShader.SetUniform3f("u_sLight.specular", 1.0f, 1.0f, 1.0f);
		mixedLightShader.SetUniform1f("u_sLight.constant", 1.0f);
		mixedLightShader.SetUniform1f("u_sLight.linear", 0.027f);
		mixedLightShader.SetUniform1f("u_sLight.quadratic", 0.0028f);
		mixedLightShader.SetUniform1f("u_sLight.innerCutoff", glm::cos(glm::radians(5.5f)));		//for smooth spotlight
		mixedLightShader.SetUniform1f("u_sLight.outerCutoff", glm::cos(glm::radians(10.5f)));		//for smooth spotlight
		mixedLightShader.UnBind();
	}

	Model crytek("res/EUL3/EUL5.obj");

	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glPointSize(1.5f);

	ImGui::CreateContext();
	ImGui_ImplGlfwGL3_Init(window, true);
	ImGui::StyleColorsDark();

	glm::vec3 modelPos[] = { glm::vec3(0.0f, 0.0f, 0.0f) };
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat4 mvp;

	TrianglePicking trianglePicking;
	if (!trianglePicking.Init(renderer.width, renderer.height)) {
		std::cout << "Triangle picking is not initialized" << std::endl;
	}

	while (!glfwWindowShouldClose(window)) {
		float currFrame = (float)glfwGetTime();
		renderer.deltaTime = currFrame - renderer.lastFrame;
		renderer.lastFrame = currFrame;

		ProcessInput(window);
		
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

		ImGui_ImplGlfwGL3_NewFrame();

		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.fov), (float)renderer.width / renderer.height, 0.1f, 100.0f);

		{//picking phase
			tex1.Bind(0);
			model = glm::translate(glm::mat4(1.0f), modelPos[0]);
			mvp = projection * view * model;

			trianglePicking.EnableWriting();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			trianglePickingShader.Bind();

			trianglePickingShader.SetUniform1ui("u_objectIndex", 0);
			trianglePickingShader.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
			trianglePickingShader.SetUniform3f("u_scale", 1.0f, 1.0f, 1.0f);
			crytek.Draw(GL_TRIANGLES, &trianglePickingShader);
			trianglePickingShader.UnBind();

			trianglePicking.DisableWriting();
		}

		{//render phase
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			if(glfwGetMouseButton(window, 0)==GLFW_PRESS && startPicking) {
			//if (renderer.mouseButtonPressed && startPicking) {
				TrianglePicking::Pixel pixel = trianglePicking.ReadPixel(renderer.lastX, renderer.height - renderer.lastY - 1);
				std::string str = std::to_string((unsigned int)pixel.objID) + std::to_string((unsigned int)pixel.drawID) + 
					std::to_string((unsigned int)pixel.primitiveID);
				if (renderer.primitiveIDStored.find(str) == renderer.primitiveIDStored.end()) {
					renderer.primitiveIDStored.insert(str);
					renderer.pixelID.push_back({ (unsigned int)pixel.objID, (unsigned int)pixel.drawID, (unsigned int)pixel.primitiveID });
				}
				//crytek.Draw(lampShader, projection, view, model, GL_TRIANGLES, pixel.drawID, pixel.primitiveID - 1);
			}

			for (int i = 0; i<renderer.pixelID.size(); ++i) {
				if (renderer.pixelID[i].primitiveID != 0) {

					lampShader.Bind();
					model = glm::translate(glm::mat4(1.0f), modelPos[renderer.pixelID[i].objID]);
					mvp = projection * view * model;
					lampShader.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
					lampShader.SetUniform3f("u_scale", 1.0f*scalePickFactor, 1.0f*scalePickFactor, 1.0f*scalePickFactor);
					lampShader.SetUniform3f("u_lightColor", 0.0f, 1.0f, 0.0f);

					glFlush();
					glFinish();

					glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

					crytek.Draw(lampShader, projection, view, model, GL_TRIANGLES, renderer.pixelID[i].drawID, renderer.pixelID[i].primitiveID - 1);
					lampShader.UnBind();
				}
			}

			if (!stopRendering) {
				mixedLightShader.Bind();

				mixedLightShader.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
				mixedLightShader.SetUniformMat4fv("u_model", glm::value_ptr(model));
				mixedLightShader.SetUniformMat4fv("u_view", glm::value_ptr(view));
				mixedLightShader.SetUniform3fv("u_sLight.direction", glm::value_ptr(camera.front));
				mixedLightShader.SetUniform3fv("u_camPos", glm::value_ptr(camera.position));

				crytek.Draw(mixedLightShader, GL_TRIANGLES);
				mixedLightShader.UnBind();
			}
		}
		
		/*{
			GLCall(glBindVertexArray(lampVao));
			lampShader.Bind();

			for (int i = 0; i < 4; ++i) {
				model = glm::translate(glm::mat4(1.0f), pointLightPosition[i]);
				model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
				mvp = projection * view * model;
				lampShader.SetUniform3fv("u_lightColor", glm::value_ptr(pointLightColors[i]));
				lampShader.SetUniformMat4fv("u_mvp", glm::value_ptr(mvp));
				GLCall(glDrawArrays(GL_TRIANGLES, 0, 36));
			}

			lampShader.UnBind();
		}*/

		{
			ImGui::Begin("Main menu");
			ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);
			if (ImGui::IsWindowHovered())
				renderer.isNotOverAnyWindow = false;
			else
				renderer.isNotOverAnyWindow = true;

			if (startPicking)
				ImGui::Text("is picking");
			else
				ImGui::Text("not picking");

			ImGui::SliderFloat("\nPalm\n", &scalePickFactor, 1.0f, 2.0f);
			ImGui::SliderFloat("\nHand\n", &scalePickFactor, 1.0f, 2.0f);
			ImGui::SliderFloat("\nStomach\n", &scalePickFactor, 1.0f, 2.0f);
			ImGui::SliderFloat("\nLegs\n", &scalePickFactor, 1.0f, 2.0f);

			if (renderer.mouseButtonPressed) {
				ImGui::Text("x:%.3f, y:%.3f",renderer.lastX,renderer.lastY);
			}

			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &texCubeVao);
	glDeleteVertexArrays(1, &lampVao);
	glDeleteBuffers(1, &cubeVbo);

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
	GLCall(glViewport(0, 0, width, height));
	renderer.width = width;
	renderer.height = height;
}

void ProcessInput(GLFWwindow* window) {
	float lightSpeed = 2.0f * renderer.deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, renderer.deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, renderer.deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, renderer.deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, renderer.deltaTime);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		scalePickFactor += 0.05f;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		scalePickFactor -= 0.05f;

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		stopRendering = true;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		stopRendering = false;
	
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		startPicking = !startPicking;

}

void MouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (renderer.initialMouse) {
		renderer.lastX = (float)xpos;
		renderer.lastY = (float)ypos;
		renderer.initialMouse = false;
	}
	float xoffset = (float)xpos - renderer.lastX;
	float yoffset = -(float)ypos + renderer.lastY;
	renderer.lastX = (float)xpos;
	renderer.lastY = (float)ypos;

	if (renderer.isNotOverAnyWindow && glfwGetMouseButton(window, 0) == GLFW_PRESS) {
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		camera.ProcessMouse(xoffset, yoffset);
		renderer.mouseRelease = false;
		renderer.mouseButtonPressed = true;
	}
	else if (!renderer.mouseRelease && glfwGetMouseButton(window, 0) == GLFW_RELEASE) {
		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		renderer.mouseRelease = true;
		renderer.mouseButtonPressed = false;
	}
}

void ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	camera.ProcessScroll((float)yoffset);
}