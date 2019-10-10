#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

enum CameraMovement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera {
private:

public:
	glm::vec3 position;
	glm::vec3 up;
	glm::vec3 front;
	glm::vec3 right;
	float movementSpeed = 2.5f;
	float mouseSensitivity = 0.05f;
	float yaw = -90.0f;
	float pitch = 0.0f;
	float fov = 45.0f;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(CameraMovement movement, float deltaTime);
	
	void ProcessMouse(float xoffset, float yoffset);

	void ProcessScroll(float yoffset);

	void UpdateVectors();

	glm::mat4 MyLookAt(const glm::vec3& cameraPos, const glm::vec3& cameraTarget, const glm::vec3& worldUp);
};