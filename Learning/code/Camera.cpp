//=============================================================================
// Name: Camera.cpp
//	Des: ʵ����������������Դ�ļ�
//=============================================================================
#include "Camera.h"
#include <fstream>
using namespace std;

#ifndef WINDOW_WIDTH
#define WINDOW_WIDTH	800					//���ڿ��
#define WINDOW_HEIGHT	600					//���ڸ߶�
#endif

//-----------------------------------------------------------------------------
// Desc: ���캯��
//-----------------------------------------------------------------------------
Camera::Camera(IDirect3DDevice9 *pd3dDevice)
{
	m_pd3dDevice		=	pd3dDevice;
	m_vRightVector		=	D3DXVECTOR3(1.0f, 0.0f, 0.0f);				//Ĭ����������X�������غ�
	m_vUpVector			=	D3DXVECTOR3(0.0f, 1.0f, 0.0f);				//Ĭ����������Y�������غ�
	m_vLookVector		=	D3DXVECTOR3(0.0f, 0.0f, 1.0f);				//Ĭ�Ϲ۲�������Z�������غ�
	m_vCameraPosition   =	D3DXVECTOR3(0.0f, 0.0f, -250.0f);			//Ĭ�����������Ϊ(0.0f, 0.0f, -250.0f)
	m_vTargetPosition   =	D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//Ĭ�Ϲ۲�Ŀ��λ��Ϊ(0.0f, 0.0f, 0.0f);

}


//-----------------------------------------------------------------------------
// Name��Camera::CalculateViewMatrix( )
// Desc: ���ݸ����ľ�������ȡ���任����
//-----------------------------------------------------------------------------
void Camera::CalculateViewMatrix(D3DXMATRIX *pMatrix) 
{
	//1.�Ȱ�3���������淶����ʹ���໥��ֱ����Ϊһ����������
	D3DXVec3Normalize(&m_vLookVector, &m_vLookVector);					//�淶���۲����
	D3DXVec3Cross(&m_vUpVector, &m_vLookVector, &m_vRightVector);		//��������۲�������ֱ
	D3DXVec3Normalize(&m_vUpVector, &m_vUpVector);						//�淶��������
	D3DXVec3Cross(&m_vRightVector, &m_vUpVector, &m_vLookVector);		//����������������ֱ
	D3DXVec3Normalize(&m_vRightVector, &m_vRightVector);				//�淶��������


	// 2.������ȡ���任����
	//����д��ȡ���任����ĵ�һ��
	pMatrix->_11 = m_vRightVector.x;									// Rx
	pMatrix->_12 = m_vUpVector.x;										// Ux
	pMatrix->_13 = m_vLookVector.x;										// Lx
	pMatrix->_14 = 0.0f;
	//����д��ȡ���任����ĵڶ���
	pMatrix->_21 = m_vRightVector.y;									// Ry
	pMatrix->_22 = m_vUpVector.y;										// Uy
	pMatrix->_23 = m_vLookVector.y;										// Ly
	pMatrix->_24 = 0.0f;
	//����д��ȡ���任����ĵ�����
	pMatrix->_31 = m_vRightVector.z;									// Rz
	pMatrix->_32 = m_vUpVector.z;										// Uz
	pMatrix->_33 = m_vLookVector.z;										// Lz
	pMatrix->_34 = 0.0f;
	//����д��ȡ���任����ĵ�����
	pMatrix->_41 = -D3DXVec3Dot(&m_vRightVector, &m_vCameraPosition);    // -P*R
	pMatrix->_42 = -D3DXVec3Dot(&m_vUpVector, &m_vCameraPosition);       // -P*U
	pMatrix->_43 = -D3DXVec3Dot(&m_vLookVector, &m_vCameraPosition);     // -P*L
	pMatrix->_44 = 1.0f;
}

//-----------------------------------------------------------------------------
// Name��GetProjMatrix()
// Desc: ���ص�ǰͶӰ����
//-----------------------------------------------------------------------------
void Camera::GetProjMatrix(D3DXMATRIX *pMatrix)		
{

	*pMatrix = m_matProj;

}				

//-----------------------------------------------------------------------------
// Name��GetCameraPosition()
// Desc: ���ص�ǰ�����λ�þ���
//-----------------------------------------------------------------------------
void Camera::GetCameraPosition(D3DXVECTOR3 *pVector)	
{

	*pVector = m_vCameraPosition;

}		

//-----------------------------------------------------------------------------
// Name��GetTargetPosition()
// Desc: ���ص�ǰĿ��λ��
//-----------------------------------------------------------------------------
void Camera::GetTargetPosition(D3DXVECTOR3 *pVector)	
{

	*pVector = m_vTargetPosition; 

}	

//-----------------------------------------------------------------------------
// Name��GetRightVrctor()
// Desc: �����ҷ�������	
//-----------------------------------------------------------------------------
void Camera::GetRightVrctor(D3DXVECTOR3 *pVector)		
{

	*pVector = m_vRightVector;

}			

//-----------------------------------------------------------------------------
// Name��GetUpVector()
// Desc: �����Ϸ�������	
//-----------------------------------------------------------------------------
void Camera::GetUpVector(D3DXVECTOR3 *pVector)			
{

	*pVector = m_vUpVector;

}				

//-----------------------------------------------------------------------------
// Name��GetLookVector()
// Desc: ���ع۲췽������	
//-----------------------------------------------------------------------------
void Camera::GetLookVector(D3DXVECTOR3 *pVector)		
{

	*pVector = m_vLookVector;

}			


//-----------------------------------------------------------------------------
// Name��Camera::SetTargetPosition( )
// Desc: ����������Ĺ۲�λ��
//-----------------------------------------------------------------------------
void Camera::SetTargetPosition(D3DXVECTOR3 *pLookat) 
{
	//�ȿ���pLookat�Ƿ�ΪĬ��ֵNULL
	if (pLookat != NULL)  m_vTargetPosition = (*pLookat);
	else m_vTargetPosition = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_vLookVector = m_vTargetPosition - m_vCameraPosition;				//�۲��λ�ü������λ�ã��õ��۲췽������
	D3DXVec3Normalize(&m_vLookVector, &m_vLookVector);					//�淶��m_vLookVector����

	//�������淶��m_vUpVector��m_vRightVector
	D3DXVec3Cross(&m_vUpVector, &m_vLookVector, &m_vRightVector);
	D3DXVec3Normalize(&m_vUpVector, &m_vUpVector);
	D3DXVec3Cross(&m_vRightVector, &m_vUpVector, &m_vLookVector);
	D3DXVec3Normalize(&m_vRightVector, &m_vRightVector);
}

//-----------------------------------------------------------------------------
// Name��Camera::SetCameraPosition( )
// Desc: ������������ڵ�λ��
//-----------------------------------------------------------------------------
void Camera::SetCameraPosition(D3DXVECTOR3 *pVector) 
{
	D3DXVECTOR3 V = D3DXVECTOR3(0.0f, 0.0f, -250.0f);
	m_vCameraPosition = pVector ? (*pVector) : V;						//��Ŀ����������pVectorΪ��Ļ���
	//����*pVector��ֵ����m_vCameraPosition=*pVector����
	//���򷵻�V��ֵ����m_vCameraPosition=V��
}

//-----------------------------------------------------------------------------
// Name��Camera::SetViewMatrix( )
// Desc: ����ȡ���任����
//-----------------------------------------------------------------------------
void Camera::SetViewMatrix(D3DXMATRIX *pMatrix) 
{
	//����pMatrix��ֵ����һ���ж�
	if (pMatrix) m_matView = *pMatrix;
	else CalculateViewMatrix(&m_matView);
	m_pd3dDevice->SetTransform(D3DTS_VIEW, &m_matView);
	//��ȡ���任�����ֵ�������ֱ���ҷ������Ϸ������͹۲����
	m_vRightVector = D3DXVECTOR3(m_matView._11, m_matView._12, m_matView._13);
	m_vUpVector    = D3DXVECTOR3(m_matView._21, m_matView._22, m_matView._23);
	m_vLookVector  = D3DXVECTOR3(m_matView._31, m_matView._32, m_matView._33);
}

//-----------------------------------------------------------------------------
// Name��Camera::SetProjMatrix( )
// Desc: ����ͶӰ�任����
//-----------------------------------------------------------------------------
void Camera::SetProjMatrix(D3DXMATRIX *pMatrix) 
{
	//�ж�ֵ��û�У�û�еĻ��ͼ���һ��
	if (pMatrix != NULL) m_matProj = *pMatrix;
	else D3DXMatrixPerspectiveFovLH(&m_matProj, D3DX_PI / 4.0f, (float)((double)WINDOW_WIDTH/WINDOW_HEIGHT), 1.0f, 300000.0f);//�ӽ���Զ�����һ�㣬�����Ͳ��¿�����Զ����������
	m_pd3dDevice->SetTransform(D3DTS_PROJECTION, &m_matProj);		//����ͶӰ�任����
}

//-----------------------------------------------------------------------------
// Name��Camera::SetRighthVector( )
// Desc: �����ҷ�������
//-----------------------------------------------------------------------------
void Camera::SetRighthVector(D3DXVECTOR3 *vRight)					//�����ҷ�������
{
	m_vRightVector = *vRight;
}

//-----------------------------------------------------------------------------
// Name��Camera::SetUpVector( )
// Desc: �����Ϸ�������
//-----------------------------------------------------------------------------
void Camera::SetUpVector(D3DXVECTOR3 *vUp)							//�����Ϸ�������
{
	m_vUpVector = *vUp;
}

//-----------------------------------------------------------------------------
// Name��Camera::SetLookVector( )
// Desc: ���ù۲췽������
//-----------------------------------------------------------------------------
void Camera::SetLookVector(D3DXVECTOR3 *vLook)						//���ù۲췽������
{
	m_vLookVector = *vLook;
}

//-----------------------------------------------------------------------------
// Name��Camera::MoveAlongRightVec( )
// Desc: ��������ƽ��fUnits����λ
//-----------------------------------------------------------------------------
void Camera::MoveAlongRightVec(float fUnits) 
{
	//ֱ�ӳ���fUnits�������ۼӾ�����
	m_vCameraPosition += m_vRightVector * fUnits;
	m_vTargetPosition   += m_vRightVector * fUnits;
}

//-----------------------------------------------------------------------------
// Name��Camera::MoveAlongUpVec( )
// Desc:  ��������ƽ��fUnits����λ
//-----------------------------------------------------------------------------
void Camera::MoveAlongUpVec(float fUnits) 
{
	//ֱ�ӳ���fUnits�������ۼӾ�����
	m_vCameraPosition += m_vUpVector * fUnits;
	m_vTargetPosition   += m_vUpVector * fUnits;
}

//-----------------------------------------------------------------------------
// Name��Camera::MoveAlongLookVec( )
// Desc:  �ع۲�����ƽ��fUnits����λ
//-----------------------------------------------------------------------------
void Camera::MoveAlongLookVec(float fUnits) 
{
	//ֱ�ӳ���fUnits�������ۼӾ�����
	m_vCameraPosition += m_vLookVector * fUnits;
	m_vTargetPosition   += m_vLookVector * fUnits;
}

//-----------------------------------------------------------------------------
// Name��Camera::RotationRightVec( )
// Desc:  ����������תfAngle�����ȵ�λ�ĽǶ�
//-----------------------------------------------------------------------------
void Camera::RotationRightVec(float fAngle) 
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vRightVector, fAngle);				//��������m_vRightVector��תfAngle���Ƕȵ�R����
	D3DXVec3TransformCoord(&m_vUpVector, &m_vUpVector, &R);				//��m_vUpVector������m_vRightVector��תfAngle���Ƕ�
	D3DXVec3TransformCoord(&m_vLookVector, &m_vLookVector, &R);			//��m_vLookVector������m_vRightVector��תfAngle���Ƕ�

	m_vTargetPosition = m_vLookVector * D3DXVec3Length(&m_vCameraPosition);//����һ�¹۲�����λ�ã��������ģ=������
}

//-----------------------------------------------------------------------------
// Name��Camera::RotationUpVec( )
// Desc:  ����������תfAngle�����ȵ�λ�ĽǶ�
//-----------------------------------------------------------------------------
void Camera::RotationUpVec(float fAngle) 
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vUpVector, fAngle);					//��������m_vUpVector��תfAngle���Ƕȵ�R����
	D3DXVec3TransformCoord(&m_vRightVector, &m_vRightVector, &R);		//��m_vRightVector������m_vUpVector��תfAngle���Ƕ�
	D3DXVec3TransformCoord(&m_vLookVector, &m_vLookVector, &R);			//��m_vLookVector������m_vUpVector��תfAngle���Ƕ�

	m_vTargetPosition = m_vLookVector * D3DXVec3Length(&m_vCameraPosition);//����һ�¹۲�����λ�ã��������ģ=������
}

//-----------------------------------------------------------------------------
// Name��Camera::RotationLookVec( )
// Desc:  �ع۲�������תfAngle�����ȵ�λ�ĽǶ�
//-----------------------------------------------------------------------------
void Camera::RotationLookVec(float fAngle) 
{
	D3DXMATRIX R;
	D3DXMatrixRotationAxis(&R, &m_vLookVector, fAngle);					//��������m_vLookVector��תfAngle���Ƕȵ�R����
	D3DXVec3TransformCoord(&m_vRightVector, &m_vRightVector, &R);		//��m_vRightVector������m_vLookVector��תfAngle���Ƕ�
	D3DXVec3TransformCoord(&m_vUpVector, &m_vUpVector, &R);				//��m_vUpVector������m_vLookVector��תfAngle���Ƕ�

	m_vTargetPosition = m_vLookVector * D3DXVec3Length(&m_vCameraPosition);//����һ�¹۲�����λ�ã��������ģ=������
}


//-----------------------------------------------------------------------------
// Name��Camera::FollowPosition( )
// Desc:  ���������ĳ��λ�õ��ϵ��ӽ�
//-----------------------------------------------------------------------------
void Camera::FollowPosition(D3DXVECTOR3 *vCharPos)                 // ���������ĳ��λ�õ��ϵ��ӽ�
{


	static bool haveLoad = false;
	static float x[4] ,y[4] ,z[4];
	if(!haveLoad)
	{
		ifstream infile("GameMedia\\CameraFollow.txt");
		for(int i = 0 ; i < 4 ; i++)
			infile>>x[i]>>y[i]>>z[i];
		haveLoad = true;
	}
	m_vRightVector		=	D3DXVECTOR3(x[0], y[0], z[0]);				//Ĭ����������X�������غ�
	m_vUpVector			=	D3DXVECTOR3(x[1], y[1], z[1]);				//Ĭ����������Y�������غ�
	m_vLookVector		=	D3DXVECTOR3(x[2], y[2], z[2]);				//Ĭ�Ϲ۲�������Z�������غ�
	m_vCameraPosition   =	D3DXVECTOR3(x[3], y[3], z[3]);			//Ĭ�����������Ϊ(0.0f, 0.0f, -250.0f)

	//	m_vCameraPosition   =	D3DXVECTOR3(-595.0f, 4244.3f, -4085.7f);	//Ĭ�����������Ϊ(0.0f, 0.0f, -250.0f)
	//	m_vTargetPosition   =	D3DXVECTOR3(0.0f, 0.0f, 0.0f);				//Ĭ�Ϲ۲�Ŀ��λ��Ϊ(0.0f, 0.0f, 0.0f);
	//5327.3   -    5922.3 = -595
	//4244.3   -    0.0    = 4244.3
	//1554.3   -    5640.0 = -4085.7
	m_vCameraPosition  += *vCharPos;
}

//-----------------------------------------------------------------------------
// Desc: ��������
//-----------------------------------------------------------------------------
Camera::~Camera(void)
{
}
