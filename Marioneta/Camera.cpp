#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	position = startPosition- glm::vec3(0.0f, 0.0f, -45.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys, GLfloat deltaTime)
{
	GLfloat velocity = 4*moveSpeed * deltaTime;

	if (keys[GLFW_KEY_W])
	{
		position += front * velocity;
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * velocity;
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * velocity;
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * velocity;
	}
	if ((keys[GLFW_KEY_SPACE]) == GLFW_PRESS)
	{
		position += velocity * worldUp;
	}
	if ((keys[GLFW_KEY_LEFT_CONTROL]) == GLFW_PRESS)
	{
		position += velocity * -worldUp;
	}
	if ((keys[GLFW_KEY_RIGHT]) == GLFW_PRESS)
	{
		yaw += 1.0f;
		update();
	}
	if ((keys[GLFW_KEY_LEFT]) == GLFW_PRESS)
	{
		yaw -= 1.0f;
		update();
	}
	if ((keys[GLFW_KEY_UP]) == GLFW_PRESS)
	{
		pitch += 1.0f;
		update();
	}
	if ((keys[GLFW_KEY_DOWN]) == GLFW_PRESS)
	{
		pitch -= 1.0f;
		update();
	}



	
} 

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::update()
{
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}


Camera::~Camera()
{
}
