//=============================================================================
// Name: Camera.h
//	Des: ʵ��������������ͷ�ļ�
//=============================================================================

#pragma once

#include <d3d9.h>
#include <d3dx9.h>

class Camera
{
private:
    D3DXVECTOR3				m_vRightVector;						//�ҷ�������
    D3DXVECTOR3				m_vUpVector;						//�Ϸ�������
    D3DXVECTOR3				m_vLookVector;						//�۲췽������
    D3DXVECTOR3				m_vCameraPosition;					//�����λ�õ�����
	D3DXVECTOR3				m_vTargetPosition;					//Ŀ��۲�λ�õ�����
    D3DXMATRIX				m_matView;							//ȡ���任����
    D3DXMATRIX				m_matProj;							//ͶӰ�任����   
    LPDIRECT3DDEVICE9		m_pd3dDevice;						//Direct3D�豸����

public:
    void CalculateViewMatrix(D3DXMATRIX *pMatrix);				//����ȡ���任����
    void GetProjMatrix(D3DXMATRIX *pMatrix);					//���ص�ǰͶӰ����
    void GetCameraPosition(D3DXVECTOR3 *pVector);				//���ص�ǰ�����λ�þ���
	void GetTargetPosition(D3DXVECTOR3 *pVector);				//���ص�ǰĿ��λ��
	void GetRightVrctor(D3DXVECTOR3 *pVector);					//�����ҷ�������	
	void GetUpVector(D3DXVECTOR3 *pVector);						//�����Ϸ�������
    void GetLookVector(D3DXVECTOR3 *pVector);					//���ع۲췽������

    void SetTargetPosition(D3DXVECTOR3 *pLookat = NULL);		//�����������Ŀ��۲�λ������
    void SetCameraPosition(D3DXVECTOR3 *pVector = NULL);		//������������ڵ�λ������
	void SetRighthVector(D3DXVECTOR3 *vRight);					//�����ҷ�������
	void SetUpVector(D3DXVECTOR3 *vUp);							//�����Ϸ�������
	void SetLookVector(D3DXVECTOR3 *vLook);						//���ù۲췽������
    void SetViewMatrix(D3DXMATRIX *pMatrix = NULL);				//����ȡ���任����
    void SetProjMatrix(D3DXMATRIX *pMatrix = NULL);				//����ͶӰ�任����

public:
    void MoveAlongRightVec(float fUnits);						// ��right�����ƶ�
    void MoveAlongUpVec(float fUnits);							// ��up�����ƶ�
    void MoveAlongLookVec(float fUnits);						// ��look�����ƶ�

    void RotationRightVec(float fAngle);						// ��right������ת
    void RotationUpVec(float fAngle);							// ��up������ת
    void RotationLookVec(float fAngle);							// ��look������ת

	void FollowPosition(D3DXVECTOR3 *vCharPos);                 // ���������ĳ��λ�õ��ϵ��ӽ�

public:
	//���캯������������
	Camera(IDirect3DDevice9 *pd3dDevice);					//���캯��
	virtual ~Camera(void);									//��������

};
