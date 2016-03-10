#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	//Points
	points = new vector3[pointsCount];
	points[0] = vector3(-4.0f, -2.0f, 5.0f);
	points[1] = vector3(1.0f, -2.0f, 5.0f);
	points[2] = vector3(-3.0f, -1.0f, 3.0f);
	points[3] = vector3(2.0f, -1.0f, 3.0f);
	points[4] = vector3(-2.0f, 0.0f, 0.0f);
	points[5] = vector3(3.0f, 0.0f, 0.0f);
	points[6] = vector3(-1.0f, 1.0f, -3.0f);
	points[7] = vector3(4.0f, 1.0f, -3.0f);
	points[8] = vector3(0.0f, 2.0f, -5.0f);
	points[9] = vector3(5.0f, 2.0f, -5.0f);
	points[10] = vector3(1.0f, 3.0f, -5.0f);

	//Spheres
	m_pSpheres = new PrimitiveClass[pointsCount];
	m_pMatricies = new matrix4[pointsCount];
	for (int i = 0; i < pointsCount; i++)
	{
		m_pSpheres[i].GenerateSphere(0.1, 4, vector3(1.0, 0.0, 0.0));
		m_pMatricies[i] = glm::translate(points[i]);
	}

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

	//cumulative time
	static double fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan;

	//Use distScale and length between points for constant speed
	fRunSpan += fTimeSpan * distScale / glm::length(points[(runCounter + 1) % pointsCount] - points[runCounter % pointsCount]);
#pragma endregion

#pragma region Your Code goes here 
	//I'll put my code wherever I feel like, thank you.
	m_pMeshMngr->SetModelMatrix(
		glm::translate(
			glm::lerp(										//Lerp translation between two points
				points[runCounter % pointsCount],			//Point 1 (Mod wrapped to be between points)
				points[(runCounter + 1) % pointsCount],		//Point 2 (Mod wrapped to be between points)
				fRunSpan)),
			"WallEye");

	//Go to the next set of points after hitting the second point
	if (fRunSpan > 1)
	{
		runCounter++;
		fRunSpan = 0.0;
	}
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("fRunTime:");
	m_pMeshMngr->PrintLine(std::to_string(fRunTime), REWHITE);
	m_pMeshMngr->Print("fRunSpan:");
	m_pMeshMngr->PrintLine(std::to_string(fRunSpan), REWHITE);
	m_pMeshMngr->Print("Distance:");
	m_pMeshMngr->PrintLine(std::to_string(glm::length(points[(runCounter + 1) % pointsCount] - points[runCounter % pointsCount])), REWHITE);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->PrintLine(std::to_string(nFPS), RERED);
#pragma endregion
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
	
	for (int i = 0; i < pointsCount; i++)
	{
		m_pSpheres[i].Render(
			m_pCameraMngr->GetProjectionMatrix(),
			m_pCameraMngr->GetViewMatrix(),
			m_pMatricies[i]);
	}
	
	m_pMeshMngr->Render(); //renders the render list

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
	delete[] m_pSpheres;	//GARBAGE DAY!
	delete[] m_pMatricies;
}