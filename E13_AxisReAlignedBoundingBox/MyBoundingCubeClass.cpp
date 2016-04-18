#include "MyBoundingCubeClass.h"
//  MyBoundingCubeClass
void MyBoundingCubeClass::Init(void)
{
	m_fRadius = 0.0f;
	m_m4ToWorld = IDENTITY_M4;

	m_v3Center = vector3(0.0f);
	m_v3Min = vector3(0.0f);
	m_v3Max = vector3(0.0f);
}
void MyBoundingCubeClass::Swap(MyBoundingCubeClass& other)
{
	std::swap(m_fRadius, other.m_fRadius);
	std::swap(m_m4ToWorld, other.m_m4ToWorld);

	std::swap(m_v3Center, other.m_v3Center);
	std::swap(m_v3Min, other.m_v3Min);
	std::swap(m_v3Max, other.m_v3Max);
}
void MyBoundingCubeClass::Release(void)
{

}
//The big 3
MyBoundingCubeClass::MyBoundingCubeClass(std::vector<vector3> a_lVectorList)
{
	m_vList = a_lVectorList;
	uint nVertexCount = m_vList.size();

	if (nVertexCount > 0)
	{
		m_v3Min = m_vList[0];
		m_v3Max = m_vList[0];
	}

	for (uint i = 0; i < nVertexCount; i++)
	{
		vector3 tempVect = m_vList[i];

		if (tempVect.x > m_v3Max.x)
			m_v3Max.x = tempVect.x;
		else if (tempVect.x < m_v3Min.x)
			m_v3Min.x = tempVect.x;

		if (tempVect.y > m_v3Max.y)
			m_v3Max.y = tempVect.y;
		else if (tempVect.y < m_v3Min.y)
			m_v3Min.y = tempVect.y;

		if (tempVect.z > m_v3Max.z)
			m_v3Max.z = tempVect.z;
		else if (tempVect.z < m_v3Min.z)
			m_v3Min.z = tempVect.z;
	}

	m_v3Center = (m_v3Max + m_v3Min) / 2.0f;
	m_fRadius = glm::distance(m_v3Center, m_v3Max);
	m_v3Size.x = glm::distance(vector3(m_v3Min.x, 0.0, 0.0), vector3(m_v3Max.x, 0.0, 0.0));
	m_v3Size.y = glm::distance(vector3(0.0, m_v3Min.y, 0.0), vector3(0.0, m_v3Max.y, 0.0));
	m_v3Size.z = glm::distance(vector3(0.0f, 0.0, m_v3Min.z), vector3(0.0, 0.0, m_v3Max.z));

	m_bList = std::vector<vector3>();
	m_bList.push_back(vector3(m_v3Size.x, m_v3Size.y, m_v3Size.z));
	m_bList.push_back(vector3(m_v3Size.x, m_v3Size.y, -m_v3Size.z));
	m_bList.push_back(vector3(m_v3Size.x, -m_v3Size.y, m_v3Size.z));
	m_bList.push_back(vector3(m_v3Size.x, -m_v3Size.y, -m_v3Size.z));
	m_bList.push_back(vector3(-m_v3Size.x, m_v3Size.y, m_v3Size.z));
	m_bList.push_back(vector3(-m_v3Size.x, m_v3Size.y, -m_v3Size.z));
	m_bList.push_back(vector3(-m_v3Size.x, -m_v3Size.y, m_v3Size.z));
	m_bList.push_back(vector3(-m_v3Size.x, -m_v3Size.y, -m_v3Size.z));

	SetCubeSize();
}
MyBoundingCubeClass::MyBoundingCubeClass(MyBoundingCubeClass const& other)
{
	m_fRadius = other.m_fRadius;
	m_m4ToWorld = other.m_m4ToWorld;

	m_v3Center = other.m_v3Center;
	m_v3Min = other.m_v3Min;
	m_v3Max = other.m_v3Max;
}
MyBoundingCubeClass& MyBoundingCubeClass::operator=(MyBoundingCubeClass const& other)
{
	if(this != &other)
	{
		Release();
		Init();
		MyBoundingCubeClass temp(other);
		Swap(temp);
	}
	return *this;
}
MyBoundingCubeClass::~MyBoundingCubeClass(){Release();};
void MyBoundingCubeClass::SetCubeSize()
{
	m_v3ChangingMin = vector3(0.0f);
	m_v3ChangingMax = vector3(0.0f);

	for (int i = 0; i < m_bList.size(); i++)
	{
		vector3 tempVect = vector3(GetCenterM() * vector4(m_bList[i], 1.0f)) - vector3(GetCenterM()[3]);

		if (tempVect.x > m_v3ChangingMax.x)
			m_v3ChangingMax.x = tempVect.x;
		else if (tempVect.x < m_v3ChangingMin.x)
			m_v3ChangingMin.x = tempVect.x;

		if (tempVect.y > m_v3ChangingMax.y)
			m_v3ChangingMax.y = tempVect.y;
		else if (tempVect.y < m_v3ChangingMin.y)
			m_v3ChangingMin.y = tempVect.y;

		if (tempVect.z > m_v3ChangingMax.z)
			m_v3ChangingMax.z = tempVect.z;
		else if (tempVect.z < m_v3ChangingMin.z)
			m_v3ChangingMin.z = tempVect.z;
	}

	m_v3ChangingMin /= 2;
	m_v3ChangingMax /= 2;

	m_v3ChangingSize.x = glm::distance(vector3(m_v3ChangingMin.x, 0.0, 0.0), vector3(m_v3ChangingMax.x, 0.0, 0.0));
	m_v3ChangingSize.y = glm::distance(vector3(0.0, m_v3ChangingMin.y, 0.0), vector3(0.0, m_v3ChangingMax.y, 0.0));
	m_v3ChangingSize.z = glm::distance(vector3(0.0, 0.0, m_v3ChangingMin.z), vector3(0.0, 0.0, m_v3ChangingMax.z));
}
//Accessors
void MyBoundingCubeClass::SetModelMatrix(matrix4 a_m4ToWorld){ m_m4ToWorld = a_m4ToWorld; }
vector3 MyBoundingCubeClass::GetCenterG(void){ return vector3(m_m4ToWorld * vector4(m_v3Center, 1.0f)); }
matrix4 MyBoundingCubeClass::GetCenterM(void) { return glm::translate(m_m4ToWorld, m_v3Center); }
float MyBoundingCubeClass::GetRadius(void) { return m_fRadius; }
vector3 MyBoundingCubeClass::GetSize(void) { return m_v3Size; };
vector3 MyBoundingCubeClass::GetChangingSize(void) { return m_v3ChangingSize; };
//--- Non Standard Singleton Methods
bool MyBoundingCubeClass::IsColliding(MyBoundingCubeClass* const a_pOther)
{
	//Collision check goes here
	vector3 v3Temp = GetCenterG();
	vector3 v3Temp1 = a_pOther->GetCenterG();
	
	bool bAreColliding = true;
	vector3 vMin1 = v3Temp + m_v3ChangingMin;
	vector3 vMax1 = v3Temp + m_v3ChangingMax;
	vector3 vMin2 = v3Temp1 + a_pOther->m_v3ChangingMin;
	vector3 vMax2 = v3Temp1 + a_pOther->m_v3ChangingMax;

	//Check for X
	if (vMax1.x < vMin2.x)
		bAreColliding = false;
	if (vMin1.x > vMax2.x)
		bAreColliding = false;

	//Check for Y
	if (vMax1.y < vMin2.y)
		bAreColliding = false;
	if (vMin1.y > vMax2.y)
		bAreColliding = false;

	//Check for Z
	if (vMax1.z < vMin2.z)
		bAreColliding = false;
	if (vMin1.z > vMax2.z)
		bAreColliding = false;

	return bAreColliding;
}
matrix4 MyBoundingCubeClass::GetModelMatrix(void) { return m_m4ToWorld; }