//TODO:
//
// Potentially Have The Projection Matrix And View Matrix Be Public So That I Can Access Them From Other Classes
//
#include"Camera.h"

Camera::Camera(unsigned int camWidth, unsigned int camHeight, float FOVDeg, float nearClip, float farClip, glm::vec3 camPosition) 
{
	Camera::width = camWidth;
	Camera::height = camHeight;
	Camera::aspectRatio = (float)width / (float)height;

	Camera::fieldOfView = glm::radians(FOVDeg);

	Camera::nearClipPlane = nearClip;
	Camera::farClipPlane = farClip;

	Camera::camPosition = camPosition;
}

void Camera::UpdateViewportSize(unsigned int camWidth, unsigned int camHeight) 
{
	Camera::width = camWidth;
	Camera::height = camHeight;
	Camera::aspectRatio = (float)width / (float)height;
}

void Camera::UpdateFieldOfView(float FOVDeg) 
{
	Camera::fieldOfView = glm::radians(FOVDeg);
}

void Camera::UpdateClippingPlanes(float nearPlane, float farPlane) 
{
	Camera::nearClipPlane = nearPlane;
	Camera::farClipPlane = farPlane;
}

void Camera::UpdateMatrix()
{
	//Initialize The MVP Matrix And Set Their Values To Be Identities
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	glm::mat4 model = glm::mat4(1.0f);

	//Apply The Model Matrix In The TRS Order (Translation, Rotation, Scale) 
	//(MOVE THIS INTO THE MESH CLASS RATHER THAN HAVING OBJECTS RELY ON THE CAMERA CLASS)
	//model = glm::translate(model, glm::vec3(sin((float)glfwGetTime()), 0.0f, 0.0f));
	//model = glm::rotate(model, glm::radians((float)glfwGetTime() * 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::scale(model, glm::vec3(sin((float)glfwGetTime()), sin((float)glfwGetTime()), sin((float)glfwGetTime())));

	//Apply The Camera's View Matrix (Applies The Camera's Position And Orientation Relative To The World)
	view = glm::lookAt(camPosition, camPosition + camForwardDirection, camUpDirection);

	//Apply The Camera's Projection Settings (In This Case, A Perspective Projection)
	projection = glm::perspective(fieldOfView, aspectRatio, nearClipPlane, farClipPlane);

	//Multiply The Projection And View Matrices (In That Order) In Order To Update The Camera Matrix
	//(Model Matrix Will Be Handled By The Model Itself, So In The Model's Vertex Shader, 
	//Its Final NDC Vertex Position Will Be The Camera Matrix (This) * The Model Matrix, In That Order)
	cameraMatrix = projection * view;
}

void Camera::SetMatrix(Shader& shader, const char* uniform)
{
	//Multiply All Three Matrices Together To Get The MVP Matrix, And Send It To The Vertex Shader Via Uniforms
	shader.setMatrix4x4(uniform, cameraMatrix);
}

void Camera::PollCameraEvents(GLFWwindow* window, float deltaTime) 
{
//	--Camera Movement Controls-- (Tied To Delta Time)

	//Forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
	{
		camPosition += camForwardDirection * camMovementSpeed * deltaTime;
	}
	//Back
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camPosition -= camForwardDirection * camMovementSpeed * deltaTime;
	}
	//Left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camPosition -= glm::normalize(glm::cross(camForwardDirection, camUpDirection)) * camMovementSpeed * deltaTime;
	}
	//Right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camPosition += glm::normalize(glm::cross(camForwardDirection, camUpDirection)) * camMovementSpeed * deltaTime;
	}
	//Up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		camPosition += camUpDirection * camMovementSpeed * deltaTime;
	}
	//Down
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		camPosition -= camUpDirection * camMovementSpeed * deltaTime;
	}

	//Toggle "Sprint"
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		camMovementSpeed = 3.5f;
	}
	else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
	{
		camMovementSpeed = 1.0f;
	}

//	--Camera Rotation/Orientation Controls-- (Not Tied To Delta Time)

	//If We Press The Left Mouse Button, Update The Camera's Rotation/Orientation
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) 
	{
		//Hide The Cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		//Small Check To Prevent The Camera From Jumping To A New Rotation/Orientation When We First Click The Mouse
		if (firstClick) 
		{
			glfwSetCursorPos(window, width / 2.0f, height / 2.0f);
			firstClick = false;
		}

		double mouseX;
		double mouseY;

		//Store The Current Mouse Position In Window Space (Top Left = 0,0	 Bottom Right = Display Resolution Width, Display Resolution Height)
		//(Note That Since The Coordinates Are In Window Space, If The Window Isnt In The Middle Of The Screen, 
		// There Will Be An Offset To The Coordinates By The Same Amount Of Pixels The Window Is Offset By) 
		glfwGetCursorPos(window, &mouseX, &mouseY);

		//Calculate How Much We Should Rotate The Camera Based Off Of How Far It Moved From The Display Origin * The Mouse Sensitivity
		float camPitchAngle = camRotationSensitivity * ((float)mouseY - (height / 2)) / height;
		float camYawAngle = camRotationSensitivity * ((float)mouseX - (width / 2)) / width;

		//Calculate The New Pitch Angle Before Applying It To The Camera
		glm::vec3 newPitchAngle = glm::rotate(camForwardDirection, glm::radians(-camPitchAngle), glm::normalize(glm::cross(camForwardDirection, camUpDirection)));

		//Check To Make Sure The New Pitch Angle Doesn't Go Outside The -175 - 175 Degree Range (5 Degree Barrier Between Max Allowed Angle And 180 To Prevent Gimbal Locking)
		if (!((glm::angle(newPitchAngle, camUpDirection) <= glm::radians(5.0f)) || (glm::angle(newPitchAngle, -camUpDirection) <= glm::radians(5.0f)))) 
		{
			//If The Check Passes, Apply The New Rotation/Orientation
			camForwardDirection = newPitchAngle;
		}

		//Apply The Yaw Angle Calculated, No Checks Needed
		camForwardDirection = glm::rotate(camForwardDirection, glm::radians(-camYawAngle), camUpDirection);

		//Return The Cursor Back To The Center Of The Screen Before The Next Frame
		glfwSetCursorPos(window, width / 2.0f, height / 2.0f);
	}
	//If We Release The Left Mouse Button, Reveal The Cursor And Reset The "firstClick" Check
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		firstClick = true;
	}
}