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
	Matrix positionMatrix;

	//�̵���� ���ϱ�	
	D3DXMatrixTranslation(&positionMatrix, 1, 2, 3);

	D3DXVECTOR3 s, t;
	D3DXQUATERNION r;
		
	D3DXMatrixDecompose(&s, &r, &t, &positionMatrix);
	cout << "position---------------------" << endl;
	cout << s.x << " " << s.y << " " << s.z << endl ;
	cout << r.x << " " << r.y << " " << r.z << endl ;
	cout << t.x << " " << t.y << " " << t.z << endl << endl ;

	//ȸ����� ���ϱ�	
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

	//ũ�� ��ȯ ���ϱ�	
	Matrix scaleMatrix;
	D3DXMatrixScaling(&scaleMatrix, 2, 2, 2);

	scaleMatrix *= rotationMatrix;
	scaleMatrix *= positionMatrix;

	

	D3DXMatrixDecompose(&s, &r, &t, &scaleMatrix);
	cout << "scale---------------------" << endl;
	cout << s.x << " " << s.y << " " << s.z << endl;
	cout << r.x << " " << r.y << " " << r.z << endl;
	cout << t.x << " " << t.y << " " << t.z << endl << endl;

	//Matrix ������ ũ��, ȸ��, �̵� ������ �����ֱ�
	
	//���� ��ȯ D3DXVec3TransformCoord(���, ��ȯ�� ����, ��ȯ���) w�� 1���� �ν�
	//���� ��ȯ D3DXVec3TransformNormal(���, ��ȯ�� ����, ��ȯ���) w�� 0���� �ν�
	

	return 0;
}