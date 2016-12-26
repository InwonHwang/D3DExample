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
	Matrix positionMatrix;

	//이동행렬 구하기	
	D3DXMatrixTranslation(&positionMatrix, 1, 2, 3);

	D3DXVECTOR3 s, t;
	D3DXQUATERNION r;
		
	D3DXMatrixDecompose(&s, &r, &t, &positionMatrix);
	cout << "position---------------------" << endl;
	cout << s.x << " " << s.y << " " << s.z << endl ;
	cout << r.x << " " << r.y << " " << r.z << endl ;
	cout << t.x << " " << t.y << " " << t.z << endl << endl ;

	//회전행렬 구하기	
	Matrix rotationMatrix;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, 45, 45, 45);

	//D3DXMatrixRotationX(&rotationMatrix, 45);
	////D3DXQuaternion(&m1, 45);

	//D3DXMatrixDecompose(&s, &r, &t, &rotationMatrix);
	//cout << "rotation---------------------" << endl;
	//cout << s.x << " " << s.y << " " << s.z << endl;
	//cout << r.x << " " << r.y << " " << r.z << endl;
	//cout << t.x << " " << t.y << " " << t.z << endl << endl;

	//D3DXMatrixRotationY(&rotationMatrix, 45);
	//D3DXMatrixRotationZ(&rotationMatrix, 45);

	cout << "rotation---------------------" << endl;
	cout << s.x << " " << s.y << " " << s.z << endl;
	cout << r.x << " " << r.y << " " << r.z << endl;
	cout << t.x << " " << t.y << " " << t.z << endl << endl;

	//크기 변환 구하기	
	Matrix scaleMatrix;
	D3DXMatrixScaling(&scaleMatrix, 2, 2, 2);

	scaleMatrix *= rotationMatrix;
	scaleMatrix *= positionMatrix;

	

	D3DXMatrixDecompose(&s, &r, &t, &scaleMatrix);
	cout << "scale---------------------" << endl;
	cout << s.x << " " << s.y << " " << s.z << endl;
	cout << r.x << " " << r.y << " " << r.z << endl;
	cout << t.x << " " << t.y << " " << t.z << endl << endl;

	//Matrix 연산은 크기, 회전, 이동 순으로 곱해주기
	
	//벡터 변환 D3DXVec3TransformCoord(결과, 변환할 벡터, 변환행렬) w가 1으로 인식
	//벡터 변환 D3DXVec3TransformNormal(결과, 변환할 벡터, 변환행렬) w가 0으로 인식
	

	return 0;
}