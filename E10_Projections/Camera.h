#ifndef __CAMERA_H_
#define __CAMERA_H_

#include "RE\ReEngAppClass.h"
#include <SFML\Graphics.hpp>

class Camera
{
	static Camera* instance;
public:

	//Constructor
	static Camera* GetInstance()
	{
		if (instance == nullptr)
			instance = new Camera();
		return instance;
	}

	//Destructor
	static void ReleaseInstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}
	
	//Return the view matrix of the camera
	matrix4 GetView()
	{
		return glm::lookAt(
			glm::vec3(0.0f, 5.0f, 5.0f),
			glm::vec3(0.0f, 0.0f, 0.0f), 
			glm::vec3(0.0f, 1.0f, 0.0f));
	}
	
	//Return the projection matrix of the camera
	matrix4 GetProjection(bool bOrthographic)
	{
		return glm::perspective(90.0f, 1080.0f / 768.0f, 0.01f, 1000.0f);
	}
	
	//Set the position of the camera
	void SetPosition(vector3 v3Position)
	{

	}

	//Set the target of the camera
	void SetTarget(vector3 v3Target)
	{

	}

	//Set the up-vector of the camera
	void SetUp(vector3 v3Up)
	{

	}

	//Move the camera forward by an increment
	void MoveForward(float fIncrement)
	{

	}

	//Move the camera sideways by an increment
	void MoveSideways(float fIncrement)
	{

	}

	//Move the camera vertically by an increment
	void MoveVertical(float fIncrement)
	{

	}

	//Pitch the gamera up or down
	void ChangePitch(float fIncrement)
	{

	}

	//Roll the camera (Aileron, not Barrel)
	void ChangeRoll(float fIncrement)
	{

	}

	//Yaw the camera sideways
	void ChangeYaw(float fIncrement)
	{

	}
private:
	Camera() {};
};
#endif