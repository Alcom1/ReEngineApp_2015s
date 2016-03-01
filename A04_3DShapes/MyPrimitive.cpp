#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}

//This will make the triang A->B->C
void MyPrimitive::AddTri(vector3 a, vector3 b, vector3 c)
{
	AddVertexPosition(a);
	AddVertexPosition(b);
	AddVertexPosition(c);
}

void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}

void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}

void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	vector3 centerBot(0, -a_fHeight / 2, 0);					//Center bottom point
	vector3 centerTop(0, a_fHeight / 2, 0);						//Center top point, the peak
	vector3* radialPoints = new vector3[a_nSubdivisions];		//Points around the radius
	
	float angle = 0;											//Angle tracker
	for (int i = 0; i < a_nSubdivisions; i++)					//Generate radial points
	{
		radialPoints[i] = vector3(
			sin(angle) * a_fRadius,
			-a_fHeight / 2,
			cos(angle) * a_fRadius);
		angle += PI * 2 / a_nSubdivisions;
	}
	for (int i = 0; i < a_nSubdivisions; i++)					//Add tris
	{
		AddTri(													//Flat tris
			centerBot,
			radialPoints[(i + 1) % a_nSubdivisions],
			radialPoints[i % a_nSubdivisions]);
		AddTri(													//Peak tris
			radialPoints[i % a_nSubdivisions],
			radialPoints[(i + 1) % a_nSubdivisions],
			centerTop);
	}
	
	delete[] radialPoints;	//Memory management, yo!
	//Your code ends here
	CompileObject(a_v3Color);
}

void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	vector3 centerBot(0, -a_fHeight / 2, 0);					//Center bottom point
	vector3 centerTop(0, a_fHeight / 2, 0);						//Center top point, the peak
	vector3* radialPointsBot = new vector3[a_nSubdivisions];  //Lower points around the radius
	vector3* radialPointsTop = new vector3[a_nSubdivisions];  //Upper points around the radius
	
	float angle = 0;											//Angle tracker
	for (int i = 0; i < a_nSubdivisions; i++)					//Generate radial points
	{
		radialPointsBot[i] = vector3(
			sin(angle) * a_fRadius,
			-a_fHeight / 2,
			cos(angle) * a_fRadius);
		radialPointsTop[i] = vector3(
			sin(angle) * a_fRadius,
			a_fHeight / 2,
			cos(angle) * a_fRadius);
		angle += PI * 2 / a_nSubdivisions;
	}
	for (int i = 0; i < a_nSubdivisions; i++)					//Add tris and quads
	{
		AddTri(													//Bottom tris
			centerBot,
			radialPointsBot[(i + 1) % a_nSubdivisions],
			radialPointsBot[i % a_nSubdivisions]);
		AddTri(													//Top tris
			radialPointsTop[i % a_nSubdivisions],
			radialPointsTop[(i + 1) % a_nSubdivisions],
			centerTop);
		AddQuad(												//Curcumfrense or however you spell it
			radialPointsBot[i % a_nSubdivisions],
			radialPointsBot[(i + 1) % a_nSubdivisions],
			radialPointsTop[i % a_nSubdivisions],
			radialPointsTop[(i + 1) % a_nSubdivisions]);
	}
	
	delete[] radialPointsBot;	//Memory management, yo!
	delete[] radialPointsTop;
	//Your code ends here
	CompileObject(a_v3Color);
}

void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	vector3* radialPointsBotOuter = new vector3[a_nSubdivisions];  //Outer Lower Radial Points
	vector3* radialPointsTopOuter = new vector3[a_nSubdivisions];  //Outer Upper Radial Points
	vector3* radialPointsBotInner = new vector3[a_nSubdivisions];  //Inner Lower Radial Points
	vector3* radialPointsTopInner = new vector3[a_nSubdivisions];  //Inner Upper Radial Points

	float angle = 0;											//Angle tracker
	for (int i = 0; i < a_nSubdivisions; i++)					//Generate radial points
	{
		radialPointsBotOuter[i] = vector3(
			sin(angle) * a_fOuterRadius,
			-a_fHeight / 2,
			cos(angle) * a_fOuterRadius);
		radialPointsTopOuter[i] = vector3(
			sin(angle) * a_fOuterRadius,
			a_fHeight / 2,
			cos(angle) * a_fOuterRadius);
		radialPointsBotInner[i] = vector3(
			sin(angle) * a_fInnerRadius,
			-a_fHeight / 2,
			cos(angle) * a_fInnerRadius);
		radialPointsTopInner[i] = vector3(
			sin(angle) * a_fInnerRadius,
			a_fHeight / 2,
			cos(angle) * a_fInnerRadius);
		angle += PI * 2 / a_nSubdivisions;
	}
	for (int i = 0; i < a_nSubdivisions; i++)					//Add tris and quads
	{
		AddQuad(												//Outer
			radialPointsBotOuter[i % a_nSubdivisions],
			radialPointsBotOuter[(i + 1) % a_nSubdivisions],
			radialPointsTopOuter[i % a_nSubdivisions],
			radialPointsTopOuter[(i + 1) % a_nSubdivisions]);
		AddQuad(												//Inner
			radialPointsTopInner[(i + 1) % a_nSubdivisions],
			radialPointsBotInner[(i + 1) % a_nSubdivisions],
			radialPointsTopInner[i % a_nSubdivisions],
			radialPointsBotInner[i % a_nSubdivisions]);
		AddQuad(												//Top
			radialPointsTopInner[(i + 1) % a_nSubdivisions],
			radialPointsTopInner[i % a_nSubdivisions],
			radialPointsTopOuter[(i + 1) % a_nSubdivisions],
			radialPointsTopOuter[i % a_nSubdivisions]);
		AddQuad(												//Bottom
			radialPointsBotInner[(i + 1) % a_nSubdivisions],
			radialPointsBotOuter[(i + 1) % a_nSubdivisions],
			radialPointsBotInner[i % a_nSubdivisions],
			radialPointsBotOuter[i % a_nSubdivisions]);
	}

	delete[] radialPointsBotOuter;	//Memory management, yo!
	delete[] radialPointsTopOuter;
	delete[] radialPointsBotInner;
	delete[] radialPointsTopInner;
	//Your code ends here
	CompileObject(a_v3Color);
}

void MyPrimitive::GenerateTorus(float a_fOuterRadius, float a_fInnerRadius, int a_nSubdivisionsA, int a_nSubdivisionsB, vector3 a_v3Color)
{
	if (a_fOuterRadius <= a_fInnerRadius + 0.1f)
		return;

	if (a_nSubdivisionsA < 3)
		a_nSubdivisionsA = 3;
	if (a_nSubdivisionsA > 25)
		a_nSubdivisionsA = 25;

	if (a_nSubdivisionsB < 3)
		a_nSubdivisionsB = 3;
	if (a_nSubdivisionsB > 25)
		a_nSubdivisionsB = 25;

	Release();
	Init();

	//Your code starts here
	float radiusR = a_fOuterRadius + a_fInnerRadius / 2;				//Radius around the ring.
	float radiusL = a_fOuterRadius - a_fInnerRadius / 2;				//Radius of the segment.
	vector3* points = new vector3[a_nSubdivisionsB * a_nSubdivisionsA];	//Points, unlike the points I will be reward for this extra credit thingy.
	float angleB = 0;													//Angle around the ring.

	//For each segment of the ring.
	for (int j = 0; j < a_nSubdivisionsA; j++)
	{
		vector2 center = vector2(radiusR * sin(angleB), radiusR * cos(angleB)); //Center of the segment.
		float angleA = 0;														//Angle around the segment.

		//For each point of the segment.
		for (int i = 0; i < a_nSubdivisionsB; i++)
		{
			points[i + j * a_nSubdivisionsB] = vector3(	//Generate the point in the array
				center.x + radiusL * cos(angleA) * sin(angleB),
				sin(angleA) * radiusL,
				center.y + radiusL * cos(angleA) * cos(angleB));
			angleA += PI * 2 / a_nSubdivisionsB;		//Increment A angle for each point added.
		}
		angleB += PI * 2 / a_nSubdivisionsA;	//Increment B angle for each point added.
	}

	//Add quads.
	for (int j = 0; j < a_nSubdivisionsA; j++)	//For each segment in the ring.
	{
		for (int i = 0; i < a_nSubdivisionsB; i++)	//For each point in the segment.
		{
			AddQuad(
				points[
					i % a_nSubdivisionsB +
					j % a_nSubdivisionsA * a_nSubdivisionsB],
				points[
					i % a_nSubdivisionsB +
					(j + 1) % a_nSubdivisionsA * a_nSubdivisionsB],
				points[
					(i + 1) % a_nSubdivisionsB +
					j % a_nSubdivisionsA * a_nSubdivisionsB],
				points[
					(i + 1) % a_nSubdivisionsB +
					(j + 1) % a_nSubdivisionsA * a_nSubdivisionsB]);
		}
	}

	delete[] points;	//Memory management, yo!
	//Your code ends here
	CompileObject(a_v3Color);
}

void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisionsHorz, int a_nSubdivisionsVert, vector3 a_v3Color)
{
	//Sets minimum and maximum of subdivisions
	if (a_nSubdivisionsHorz < 1)
	{
		GenerateCube(a_fRadius * 2, a_v3Color);
		return;
	}
	if (a_nSubdivisionsHorz > 20)
		a_nSubdivisionsHorz = 20;

	Release();
	Init();

	//Your code starts here
	vector3 centerBot(0, -a_fRadius, 0);													//Center bottom point
	vector3 centerTop(0, a_fRadius, 0);														//Center top point, the peak
	vector3* radialPoints = new vector3[a_nSubdivisionsHorz * (a_nSubdivisionsVert - 1)]; //Array of points for all rings
	float angleVert = PI / a_nSubdivisionsVert;												//Vertical angle tracker

	//Generate each ring going up.
	for (int j = 1; j < a_nSubdivisionsVert; j++)
	{
		float yClimb = -cos(angleVert);					//Normalized Y position of the current ring
		float subRadius = sqrt(1 - yClimb * yClimb);	//Normalized radius of the current ring
		float angleHorz = 0;							//Horizontal angle tracker
		
		//Generate each point going counter-clockwise from the top.
		for (int i = 0; i < a_nSubdivisionsHorz; i++)
		{
			radialPoints[i + (j - 1) * a_nSubdivisionsHorz] = vector3(	//Generate the point in the array
				sin(angleHorz) * a_fRadius * subRadius,
				yClimb * a_fRadius,
				cos(angleHorz) * a_fRadius * subRadius);
			angleHorz += PI * 2 / a_nSubdivisionsHorz;	//Increment horizontal angle for each point added.
		}
		angleVert += PI / a_nSubdivisionsVert;	//Increment vertical angle for each point added.
	}

	//Add side quads
	for (int j = 0; j < a_nSubdivisionsVert - 2; j++)	//For each ring except the top one.
	{
		for (int i = 0; i < a_nSubdivisionsHorz; i++)	//For each point in a ring, draw a quad to the top right.
		{
			AddQuad(
				radialPoints[
					i % a_nSubdivisionsHorz +
					j * a_nSubdivisionsHorz],
				radialPoints[
					(i + 1) % a_nSubdivisionsHorz +
					j * a_nSubdivisionsHorz],
				radialPoints[
					i % a_nSubdivisionsHorz +
					(j + 1) * a_nSubdivisionsHorz],
				radialPoints[
					(i + 1) % a_nSubdivisionsHorz +
					(j + 1) * a_nSubdivisionsHorz]);
		}
	}

	//Add bottom tris
	for (int i = 0; i < a_nSubdivisionsHorz; i++)
	{
		AddTri(
			centerBot,
			radialPoints[(i + 1) % a_nSubdivisionsHorz],
			radialPoints[i % a_nSubdivisionsHorz]);
	}

	//Add top tris
	for (int i = 0; i < a_nSubdivisionsHorz; i++)
	{

		AddTri(
			radialPoints[(i % a_nSubdivisionsHorz) + a_nSubdivisionsHorz * (a_nSubdivisionsVert- 2)],
			radialPoints[((i + 1) % a_nSubdivisionsHorz) + a_nSubdivisionsHorz * (a_nSubdivisionsVert - 2)],
			centerTop);
	}

	delete[] radialPoints;	//Memory management, yo!
	//Your code ends here
	CompileObject(a_v3Color);
}