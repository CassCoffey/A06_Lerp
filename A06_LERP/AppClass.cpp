#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	// define the points to be lerping between
	locations = new vector3[11] { vector3(-4.0f,-2.0f, 5.0f),
		vector3(1.0f,-2.0f, 5.0f),
		vector3(-3.0f,-1.0f, 3.0f),
		vector3(2.0f,-1.0f, 3.0f),
		vector3(-2.0f, 0.0f, 0.0f),
		vector3(3.0f, 0.0f, 0.0f),
		vector3(-1.0f, 1.0f,-3.0f),
		vector3(4.0f, 1.0f,-3.0f),
		vector3(0.0f, 2.0f,-5.0f),
		vector3(5.0f, 2.0f,-5.0f),
		vector3(1.0f, 3.0f,-5.0f) };

	spheres = new PrimitiveClass[11];
	sphereMatrices = new matrix4[11];

	// Create all of the spheres on the locations
	for (int i = 0; i < 11; i++)
	{
		spheres[i].GenerateSphere(0.1f, 5, vector3(100.0f, 0.0f, 0.0f));
		sphereMatrices[i][3].x = locations[i].x;
		sphereMatrices[i][3].y = locations[i].y;
		sphereMatrices[i][3].z = locations[i].z;
		sphereMatrices[i][3].w = 1;
	}

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
#pragma endregion

#pragma region Your Code goes here
	
	matrix4 enemyLoc;
	float percent = MapValue((float)fRunTime, 0.0f, fDuration, 0.0f, 1.0f); // calculate percent with mapvalue
	int index = (int)percent % 11; // get the current index using the percent

	vector4 start = vector4(locations[index], 1); // Define the start vector
	vector4 end = vector4(locations[(index + 1) % 11], 1); // Define the end vector
	vector4 lerp = glm::lerp(start, end, std::fmodf(percent, 1)); // Use glm::lerp to lerp between the start and end vectors, using the percent

	enemyLoc[3] = lerp;

	m_pMeshMngr->SetModelMatrix(enemyLoc, "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW);
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();

	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list

	for (int i = 0; i < 11; i++)
	{
		spheres[i].Render(m_pCameraMngr->GetProjectionMatrix(), m_pCameraMngr->GetViewMatrix(), sphereMatrices[i]);
	}

	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields

	delete[] locations;
	delete[] spheres;
	delete[] sphereMatrices;
}