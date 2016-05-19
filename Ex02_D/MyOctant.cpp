#include "MyOctant.h"
using namespace ReEng;
//  MyOctant
void MyOctant::Init(void)
{
	m_v3Position = vector3(0.0f);
	m_pMeshMngr = MeshManagerSingleton::GetInstance();
	m_fSize = 0.0f;
	m_pBOMngr = MyBOManager::GetInstance();

	int nObjectCount = m_pBOMngr->GetObjectCount(); //Cont? Was this a spelling error?
	
	//YOUR CODE GOES HERE
	vector3 min = vector3(0.0f);
	vector3 max = vector3(0.0f);

	//For each object, compare its max and min values and generate a combined max and min
	for (int i = 0; i < nObjectCount; i++)
	{
		MyBOClass* cur = m_pBOMngr->GetBoundingObject(i);

		if (cur->GetCenterGlobal().x + cur->GetHalfWidthG().x > max.x) //X
			max.x = cur->GetCenterGlobal().x + cur->GetHalfWidthG().x;
		else if (cur->GetCenterGlobal().x - cur->GetHalfWidthG().x < min.x)
			min.x = cur->GetCenterGlobal().x - cur->GetHalfWidthG().x;

		if (cur->GetCenterGlobal().y + cur->GetHalfWidthG().y > max.y) //Y
			max.y = cur->GetCenterGlobal().y + cur->GetHalfWidthG().y;
		else if (cur->GetCenterGlobal().y - cur->GetHalfWidthG().y < min.y)
			min.y = cur->GetCenterGlobal().y - cur->GetHalfWidthG().y;

		if (cur->GetCenterGlobal().z + cur->GetHalfWidthG().z > max.z) //Z
			max.z = cur->GetCenterGlobal().z + cur->GetHalfWidthG().z;
		else if (cur->GetCenterGlobal().z - cur->GetHalfWidthG().z < min.z)
			min.z = cur->GetCenterGlobal().z - cur->GetHalfWidthG().z;
	}

	//Set position
	m_v3Position = (max + min) / 2.0f;

	//Set m_fSize to be the largest of the half widths
	vector3 m_v3Width = (max - min);
	m_fSize = m_v3Width.x;
	if (m_fSize < m_v3Width.y)
		m_fSize = m_v3Width.y;
	if (m_fSize < m_v3Width.z)
		m_fSize = m_v3Width.z;

}
void MyOctant::Release(void)
{
}
//The big 3
MyOctant::MyOctant(){Init();}
MyOctant::~MyOctant(){Release();};

void MyOctant::Display(void)
{
	m_pMeshMngr->AddCubeToRenderList(glm::translate(m_v3Position) * glm::scale(vector3(m_fSize)), REPURPLE, WIRE);
}


