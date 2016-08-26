//=============================================================================
// Name: FireBall.h
//	Des: ����ϵͳ���ͷ�ļ�
//=============================================================================

#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "D3DUtil.h"
#include "Character.h"


#define	LIFETIME	1.0f										//�ܴ��ʱ��



//-------------------------------------------------------------------------------------------------
// Desc: ��Ķ���
//-------------------------------------------------------------------------------------------------
class FireBall
{
private:
	LPDIRECT3DDEVICE9				m_pd3dDevice;					//D3D�豸����
	LPD3DXMESH 						m_sphere ;		//���������
	float							m_age;

	Team							m_team;						//����
	int								index;							//�ĸ��±�����﷢����
	float							BallRadius;								//��뾶
	D3DXVECTOR3						position;
	float							facing;

public:
	bool							isDestroy;

public:
	FireBall(LPDIRECT3DDEVICE9 pd3dDevice);							//���캯��
	~FireBall();													//��������
	HRESULT Init(Team team , int index,float radius,D3DXVECTOR3 pos ,float fac );										//����ϵͳ��ʼ������
	HRESULT Update( float fElapsedTime);				//����ϵͳ���º���
	HRESULT Render( );									//����ϵͳ��Ⱦ����
};

