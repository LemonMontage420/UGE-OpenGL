#pragma once

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"ShaderClass.h"

class Camera 
{
	public:
		glm::vec3 camPosition;
		glm::vec3 camForwardDirection = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 camUpDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::mat4 cameraMatrix = glm::mat4(1.0f);

		bool firstClick = true;

		unsigned int width;
		unsigned int height;
		float aspectRatio;

		float fieldOfView;

		float nearClipPlane;
		float farClipPlane;

		float camMovementSpeed = 1.0f;
		float camRotationSensitivity = 50.0f;

		Camera(unsigned int camWidth, unsigned int camHeight, float FOVDeg, float nearPlane, float farPlane, glm::vec3 camPosition);

		void UpdateViewportSize(unsigned int camWidth, unsigned int camHeight);
		void UpdateFieldOfView(float FOVDeg);
		void UpdateClippingPlanes(float nearPlane, float farPlane);
		void SetMatrix(Shader& shader, const char* uniform);
		void UpdateMatrix();
		void PollCameraEvents(GLFWwindow* window, float deltaTime);
};