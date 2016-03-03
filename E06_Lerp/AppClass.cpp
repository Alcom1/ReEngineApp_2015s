#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Bronner Example"); // Window Name

	m_v4ClearColor = vector4(0.4f, 0.6f, 0.9f, 0.0f);
}

void AppClass::InitVariables(void)
{
	//Camera position
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 2.5f, 12.0f), vector3(0.0f, 2.5f, 11.0f), REAXISY);

	//Load Steve
	m_pMeshMngr->LoadModel("Minecraft\\Steve.obj", "Steve");
}

void AppClass::Update(void)
{
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();

	//First person camera movement
	if (m_bFPC == true)
		CameraRotation();

	//Call the arcball method
	ArcBall();
	
	double dt = m_pSystem->LapClock();
	static double fRunTime = 0.0f;
	fRunTime += dt;

	vector3 v3Start = vector3(-5.0f, 0.0f, 0.0f);
	vector3 v3End = vector3(5.0f, 0.0f, 0.0f);
	static float fp = MapValue(fRunTime, 0.0, 10.0, 0.0, 1.0);		//When it's static, if it's already declared, it will reuse the previous value. It keeps saving the value! :O

	vector3 v3Int = glm::lerp(v3Start, v3End, fp);

	matrix4 m4Temp = glm::translate(v3Int);

	/*
	//Steve matrix
	matrix4 m4Temp = glm::translate(vector3(-5.0f, 0.0f, 0.0f));

	//Inverse, expensive
	m4Temp = m4Temp * glm::inverse(m4Temp);

	//Transpose-inverse. Is not.
	matrix3 m3Temp(m4Temp);
	m3Temp = glm::transpose(m3Temp);			//Transpose of a rotation matrix is its inverse
	m4Temp = m4Temp * matrix4(m3Temp);			//Apply transposed rotation to mat4
	m4Temp[3] = m4Temp[3] + (m4Temp[3] * -1);	//Inverse translation
	m4Temp[3][3] = 1;							//Bottom corner fix for matrix
	*/

	m_pMeshMngr->SetModelMatrix(m4Temp, "Steve"); //m_pMeshMngr is a singleton, it exists everywhere.
	
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();
	//print info into the console
	//printf("FPS: %d            \r", nFPS);//print the Frames per Second
	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);

	m_pMeshMngr->Print("Selection: ");
	m_pMeshMngr->PrintLine(m_pMeshMngr->GetInstanceGroupName(m_selection.first, m_selection.second), REYELLOW);
	
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	switch (m_pCameraMngr->GetCameraMode())
	{
	default: //Perspective
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY); //renders the XY grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOX:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::YZ, RERED * 0.75f); //renders the YZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOY:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XZ, REGREEN * 0.75f); //renders the XZ grid with a 100% scale
		break;
	case CAMERAMODE::CAMROTHOZ:
		m_pMeshMngr->AddGridToQueue(1.0f, REAXIS::XY, REBLUE * 0.75f); //renders the XY grid with a 100% scale
		break;
	}
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}