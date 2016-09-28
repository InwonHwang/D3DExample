#include <iostream>
#include "Vector3.h"
#include "Matrix.h"

using namespace std;

int main()
{
	Vector3 v1(1, 1, 1);
	Vector3 v2(1, 2, 3);

	// -------------------------- ���� --------------------------
	//����
	Vector3 result = v1 + v2;
	cout << "����: " << result.x << " " << result.y << " " << result.z << endl;

	//����
	result = v2 - v1;
	cout << "����: " << result.x << " " << result.y << " " << result.z << endl;

	//���� ũ��
	float length = D3DXVec3Length(&result);
	cout << "����ũ��: " << length << endl;

	//����ȭ
	D3DXVec3Normalize(&result, &v2);
	cout << "����ȭ: " << result.x << " " << result.y << " " << result.z << endl;

	//����
	result = v2 * 5;
	cout << "����: " << result.x << " " << result.y << " " << result.z << endl;

	//����	
	float dot = D3DXVec3Dot(&v1, &v2);
	cout << "����: " << dot << endl;

	//����	
	D3DXVec3Cross(&result, &v1, &v2);
	cout << "����: " << result.x << " " << result.y << " " << result.z << endl << endl;

	// -------------------------- ��� --------------------------
	Matrix m1;
	Matrix m2(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

	//�̵���� ���ϱ�	
	D3DXMatrixTranslation(&m1, 1, 2, 3);
	cout << m1._11 << " " << m1._12 << " " << m1._13 << " " << m1._14 << endl;
	cout << m1._21 << " " << m1._22 << " " << m1._23 << " " << m1._24 << endl;
	cout << m1._31 << " " << m1._32 << " " << m1._33 << " " << m1._34 << endl;
	cout << m1._41 << " " << m1._42 << " " << m1._43 << " " << m1._44 << endl << endl;

	//ȸ����� ���ϱ�	
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

	//ũ�� ��ȯ ���ϱ�	
	D3DXMatrixScaling(&m1, 2, 2, 2);
	cout << m1._11 << " " << m1._12 << " " << m1._13 << " " << m1._14 << endl;
	cout << m1._21 << " " << m1._22 << " " << m1._23 << " " << m1._24 << endl;
	cout << m1._31 << " " << m1._32 << " " << m1._33 << " " << m1._34 << endl;
	cout << m1._41 << " " << m1._42 << " " << m1._43 << " " << m1._44 << endl << endl;

	//Matrix ������ ũ��, ȸ��, �̵� ������ �����ֱ�
	
	//���� ��ȯ D3DXVec3TransformCoord(���, ��ȯ�� ����, ��ȯ���) w�� 1���� �ν�
	//���� ��ȯ D3DXVec3TransformNormal(���, ��ȯ�� ����, ��ȯ���) w�� 0���� �ν�
	

	return 0;
}