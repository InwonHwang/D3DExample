#include <iostream>
#include "Vector3.h"
#include "Matrix.h"

using namespace std;

int main()
{
	Vector3 v1(1, 1, 1);
	Vector3 v2(1, 2, 3);

	// -------------------------- 벡터 --------------------------
	//덧셈
	Vector3 result = v1 + v2;
	cout << "덧셈: " << result.x << " " << result.y << " " << result.z << endl;

	//뺄셈
	result = v2 - v1;
	cout << "뺄셈: " << result.x << " " << result.y << " " << result.z << endl;

	//벡터 크기
	float length = D3DXVec3Length(&result);
	cout << "벡터크기: " << length << endl;

	//정규화
	D3DXVec3Normalize(&result, &v2);
	cout << "정규화: " << result.x << " " << result.y << " " << result.z << endl;

	//곱셈
	result = v2 * 5;
	cout << "곱셈: " << result.x << " " << result.y << " " << result.z << endl;

	//내적	
	float dot = D3DXVec3Dot(&v1, &v2);
	cout << "내적: " << dot << endl;

	//외적	
	D3DXVec3Cross(&result, &v1, &v2);
	cout << "외적: " << result.x << " " << result.y << " " << result.z << endl << endl;

	// -------------------------- 행렬 --------------------------
	Matrix m1;
	Matrix m2(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

	//이동행렬 구하기	
	D3DXMatrixTranslation(&m1, 1, 2, 3);
	cout << m1._11 << " " << m1._12 << " " << m1._13 << " " << m1._14 << endl;
	cout << m1._21 << " " << m1._22 << " " << m1._23 << " " << m1._24 << endl;
	cout << m1._31 << " " << m1._32 << " " << m1._33 << " " << m1._34 << endl;
	cout << m1._41 << " " << m1._42 << " " << m1._43 << " " << m1._44 << endl << endl;

	//회전행렬 구하기	
	D3DXMatrixRotationX(&m1, 45);
	cout << m1._11 << " " << m1._12 << " " << m1._13 << " " << m1._14 << endl;
	cout << m1._21 << " " << m1._22 << " " << m1._23 << " " << m1._24 << endl;
	cout << m1._31 << " " << m1._32 << " " << m1._33 << " " << m1._34 << endl;
	cout << m1._41 << " " << m1._42 << " " << m1._43 << " " << m1._44 << endl << endl;

	D3DXMatrixRotationY(&m1, 45);
	cout << m1._11 << " " << m1._12 << " " << m1._13 << " " << m1._14 << endl;
	cout << m1._21 << " " << m1._22 << " " << m1._23 << " " << m1._24 << endl;
	cout << m1._31 << " " << m1._32 << " " << m1._33 << " " << m1._34 << endl;
	cout << m1._41 << " " << m1._42 << " " << m1._43 << " " << m1._44 << endl << endl;

	D3DXMatrixRotationZ(&m1, 45);
	cout << m1._11 << " " << m1._12 << " " << m1._13 << " " << m1._14 << endl;
	cout << m1._21 << " " << m1._22 << " " << m1._23 << " " << m1._24 << endl;
	cout << m1._31 << " " << m1._32 << " " << m1._33 << " " << m1._34 << endl;
	cout << m1._41 << " " << m1._42 << " " << m1._43 << " " << m1._44 << endl << endl;

	//크기 변환 구하기	
	D3DXMatrixScaling(&m1, 2, 2, 2);
	cout << m1._11 << " " << m1._12 << " " << m1._13 << " " << m1._14 << endl;
	cout << m1._21 << " " << m1._22 << " " << m1._23 << " " << m1._24 << endl;
	cout << m1._31 << " " << m1._32 << " " << m1._33 << " " << m1._34 << endl;
	cout << m1._41 << " " << m1._42 << " " << m1._43 << " " << m1._44 << endl << endl;

	//Matrix 연산은 크기, 회전, 이동 순으로 곱해주기
	
	//벡터 변환 D3DXVec3TransformCoord(결과, 변환할 벡터, 변환행렬) w가 1으로 인식
	//벡터 변환 D3DXVec3TransformNormal(결과, 변환할 벡터, 변환행렬) w가 0으로 인식
	

	return 0;
}