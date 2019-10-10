#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up) : position(position), up(up) {
	UpdateVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::ProcessKeyboard(CameraMovement movement, float deltaTime) {
	float cameraSpeed = movementSpeed * deltaTime;
	if(movement == FORWARD)
		position += cameraSpeed * front;
	if (movement == BACKWARD)
		position -= cameraSpeed * front;
	if (movement == LEFT)
		position -= cameraSpeed * right;
	if (movement == RIGHT)
		position += cameraSpeed * right;
	
}

void Camera::ProcessMouse(float xoffset, float yoffset){
	xoffset *= mouseSensitivity;
	yoffset *= mouseSensitivity;

	yaw = std::fmod((yaw + xoffset), 360.0f);
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	UpdateVectors();
}

void Camera::ProcessScroll(float yoffset){
	if (fov >= 1.0f && fov <= 45.0f)
		fov -= yoffset;
	if (fov <= 1.0f)
		fov = 1.0f;
	if (fov >= 45.0f)
		fov = 45.0f;
}

void Camera::UpdateVectors(){
	glm::vec3 frontDir;
	frontDir.x = cos(glm::radians(yaw));
	frontDir.y = sin(glm::radians(pitch));
	frontDir.z = cos(glm::radians(pitch))*sin(glm::radians(yaw));
	front = frontDir;

	right = glm::normalize(glm::cross(front, up));
	//up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::MyLookAt(const glm::vec3& cameraPos, const glm::vec3& cameraTarget, const glm::vec3& worldUp) {
	glm::vec3 cameraZ = glm::normalize(cameraPos - cameraTarget);	//direction vector
	glm::vec3 cameraX = glm::cross(worldUp, cameraZ);				//right
	glm::vec3 cameraY = glm::cross(cameraZ, cameraX);				//UP
	glm::mat4 viewMatrix(cameraX.x, cameraX.y, cameraX.z, 0,
		cameraY.x, cameraY.y, cameraY.z, 0,
		cameraZ.x, cameraZ.y, cameraZ.z, 0,
		0, 0, 0, 1);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), -cameraPos);
	view = glm::transpose(viewMatrix) * view;
	return view;
}