#include "MyBOClass.h"
//  MyBOClass
void MyBOClass::Init(void)
{
	m_m4ToWorld = IDENTITY_M4;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);

	m_v3CenterG = vector3(0.0f);
	m_v3MinG = vector3(0.0f);
	m_v3MaxG = vector3(0.0f);

	m_v3HalfWidth = vector3(0.0f);
	m_v3HalfWidthG = vector3(0.0f);

	m_fRadius = 0.0f;

	m_pMeshMngr = MeshManagerSingleton::GetInstance();
}
void MyBOClass::Swap(MyBOClass& other)
{
	std::swap(m_m4ToWorld, other.m_m4ToWorld);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);

	std::swap(m_v3CenterG, other.m_v3CenterG);
	std::swap(m_v3MinG, other.m_v3MinG);
	std::swap(m_v3MaxG, other.m_v3MaxG);

	std::swap(m_v3HalfWidth, other.m_v3HalfWidth);
	std::swap(m_v3HalfWidthG, other.m_v3HalfWidthG);

	std::swap(m_fRadius, other.m_fRadius);

	std::swap(m_pMeshMngr, other.m_pMeshMngr);
}
void MyBOClass::Release(void) {/* No pointers allocated yet */ }
//The big 3
MyBOClass::MyBOClass(MyBOClass const& other)
{
	m_m4ToWorld = other.m_m4ToWorld;

	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;

	m_v3CenterG = other.m_v3CenterG;
	m_v3MinG = other.m_v3MinG;
	m_v3MaxG = other.m_v3MaxG;

	m_v3HalfWidth = other.m_v3HalfWidth;
	m_v3HalfWidthG = other.m_v3HalfWidthG;

	m_fRadius = other.m_fRadius;

	m_pMeshMngr = other.m_pMeshMngr;
}
MyBOClass& MyBOClass::operator=(MyBOClass const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyBOClass temp(other);
		Swap(temp);
	}
	return *this;
}
MyBOClass::~MyBOClass() { Release(); };
//Accessors
float MyBOClass::GetRadius(void) { return m_fRadius; }
matrix4 MyBOClass::GetModelMatrix(void) { return m_m4ToWorld; }
vector3 MyBOClass::GetCenterLocal(void) { return m_v3Center; }
vector3 MyBOClass::GetCenterGlobal(void) { return m_v3CenterG; }
vector3 MyBOClass::GetHalfWidth(void) { return m_v3HalfWidth; }
vector3 MyBOClass::GetHalfWidthG(void) { return m_v3HalfWidthG; }
vector3 MyBOClass::GetMin(void) { return m_v3Min; }
vector3 MyBOClass::GetMax(void) { return m_v3Max; }
vector3 MyBOClass::GetMinG(void) { return m_v3MinG; }
vector3 MyBOClass::GetMaxG(void) { return m_v3MaxG; }
//--- Non Standard Singleton Methods
void MyBOClass::DisplaySphere(vector3 a_v3Color)
{
	m_pMeshMngr->AddSphereToRenderList(glm::translate(m_m4ToWorld, m_v3Center) *
		glm::scale(vector3(m_fRadius * 2.0f)), a_v3Color, WIRE);
}
void MyBOClass::DisplayOriented(vector3 a_v3Color)
{
	m_pMeshMngr->AddCubeToRenderList(glm::translate(m_m4ToWorld, m_v3Center) *
		glm::scale(m_v3HalfWidth * 2.0f), a_v3Color, WIRE);
}
void MyBOClass::DisplayReAlligned(vector3 a_v3Color)
{
	m_pMeshMngr->AddCubeToRenderList(glm::translate(IDENTITY_M4, m_v3CenterG) *
		glm::scale(m_v3HalfWidthG * 2.0f), a_v3Color, WIRE);
}
bool MyBOClass::IsColliding(MyBOClass* const a_pOther)
{
	//Get all vectors in global space
	vector3 v3Min = vector3(m_m4ToWorld * vector4(m_v3Min, 1.0f));
	vector3 v3Max = vector3(m_m4ToWorld * vector4(m_v3Max, 1.0f));

	vector3 v3MinO = vector3(a_pOther->m_m4ToWorld * vector4(a_pOther->m_v3Min, 1.0f));
	vector3 v3MaxO = vector3(a_pOther->m_m4ToWorld * vector4(a_pOther->m_v3Max, 1.0f));

	/*
	Are they colliding?
	For Objects we will assume they are colliding, unless at least one of the following conditions is not met
	*/
	//first check the bounding sphere, if that is not colliding we can guarantee that there are no collision
	if ((m_fRadius + a_pOther->m_fRadius) < glm::distance(m_v3CenterG, a_pOther->m_v3CenterG))
		return false;

	//If the distance was smaller it might be colliding

	bool bColliding = true;

	//Check for X
	if (m_v3MaxG.x < a_pOther->m_v3MinG.x)
		bColliding = false;
	if (m_v3MinG.x > a_pOther->m_v3MaxG.x)
		bColliding = false;

	//Check for Y
	if (m_v3MaxG.y < a_pOther->m_v3MinG.y)
		bColliding = false;
	if (m_v3MinG.y > a_pOther->m_v3MaxG.y)
		bColliding = false;

	//Check for Z
	if (m_v3MaxG.z < a_pOther->m_v3MinG.z)
		bColliding = false;
	if (m_v3MinG.z > a_pOther->m_v3MaxG.z)
		bColliding = false;

	return bColliding;
}
MyBOClass::MyBOClass(std::vector<vector3> a_lVectorList)
{
	//Init the default values
	Init();

	//Count the points of the incoming list
	uint nVertexCount = a_lVectorList.size();

	//If there are none just return, we have no information to create the BS from
	if (nVertexCount == 0)
		return;

	//Your code goes Here
	int count = a_lVectorList.size();
	m_v3Min = m_v3Max = a_lVectorList[0];

	//Generate the min and max based on all vectors in the object
	for (int i = 0; i < count; i++)
	{
		vector3 cur = a_lVectorList[i];	//Current vector being compared

		if (m_v3Min.x > cur.x)	//X check
			m_v3Min.x = cur.x;
		else if (m_v3Max.x < cur.x)
			m_v3Max.x = cur.x;

		if (m_v3Min.y > cur.y)	//Y check
			m_v3Min.y = cur.y;
		else if (m_v3Max.y < cur.y)
			m_v3Max.y = cur.y;

		if (m_v3Min.z > cur.z)	//Z check
			m_v3Min.z = cur.z;
		else if (m_v3Max.z < cur.z)
			m_v3Max.z = cur.z;
	}

	//Local center and halfwidths for OBB
	m_v3Center = (m_v3Max + m_v3Min) / 2.0f;
	m_v3HalfWidth = (m_v3Max - m_v3Min) / 2.0f;

	//Temporary non-global global values
	m_v3MinG = m_v3Min;
	m_v3MaxG = m_v3Max;
	m_v3CenterG = m_v3Center;
	m_v3HalfWidthG = m_v3HalfWidth;
}
void MyBOClass::SetModelMatrix(matrix4 a_m4ToWorld)
{
	//If there are no changes in the Model Matrix there is no need
	//of doing further calculations
	if (m_m4ToWorld == a_m4ToWorld)
		return;

	m_m4ToWorld = a_m4ToWorld;
	//Your code goes here
	m_v3CenterG = vector3(a_m4ToWorld * vector4(m_v3Center, 1.0f));		 //Global center
	m_v3MaxG = m_v3MinG = vector3(a_m4ToWorld * vector4(m_v3Min, 1.0f)); //Inital global min-max for comparison

	//For each corner in the OBB
	for (int i = 0; i < 8; i++)
	{
		vector3 temp = vector3(				//Different corner of the OBB
			i & 1 ? m_v3Min.x : m_v3Max.x,	//Binary flip between max and min x corners
			i & 2 ? m_v3Min.y : m_v3Max.y,	//Binary flip between max and min y corners
			i & 4 ? m_v3Min.z : m_v3Max.z);	//Binary flip between max and min z corners
		temp = vector3(a_m4ToWorld * vector4(temp, 1.0f));

		if (m_v3MinG.x > temp.x)	//X check
			m_v3MinG.x = temp.x;
		else if (m_v3MaxG.x < temp.x)
			m_v3MaxG.x = temp.x;

		if (m_v3MinG.y > temp.y)	//Y check
			m_v3MinG.y = temp.y;
		else if (m_v3MaxG.y < temp.y)
			m_v3MaxG.y = temp.y;

		if (m_v3MinG.z > temp.z)	//Z check
			m_v3MinG.z = temp.z;
		else if (m_v3MaxG.z < temp.z)
			m_v3MaxG.z = temp.z;
	}

	m_v3HalfWidthG = (m_v3MaxG - m_v3MinG) / 2.0f;	//Set half width based on global min/max
}