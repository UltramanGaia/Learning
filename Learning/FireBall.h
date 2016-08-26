//=============================================================================
// Name: FireBall.h
//	Des: ����ϵͳ���ͷ�ļ�
//=============================================================================

#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "D3DUtil.h"
#include "Character.h"

//-------------------------------------------------------------------------------------------------
//���ӵ�FVF����ṹ�Ͷ����ʽ
//-------------------------------------------------------------------------------------------------
//extern struct POINTVERTEX;

struct FIREPOINTVERTEX
{
	float x, y, z;													//����λ��
	float u,v ;														//������������
};

#define D3DFVF_POINTVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

#define	LIFETIME	1.0f										//�ܴ��ʱ��
#define FIRE_NUMBER 50										//��������


//-------------------------------------------------------------------------------------------------
// Desc: ���ӽṹ��Ķ���
//-------------------------------------------------------------------------------------------------
struct  FIREPARTICLE
{
	float x, y, z;													//����λ��
	float RotationY;												//������Y����ת�Ƕ�
	float RotationX;												//������X����ת�Ƕ�
	float BallSpeed;												//�����ٶ�
	float RotationSpeed;											//��ת�ٶ�
	int   TextureIndex;												//����������

	bool isAlive;													//�Ƿ����
	float	age;													//���ʱ��
};

//-------------------------------------------------------------------------------------------------
// Desc: ����ϵͳ��Ķ���
//-------------------------------------------------------------------------------------------------
class FireBall
{
private:
	LPDIRECT3DDEVICE9				m_pd3dDevice;					//D3D�豸����
	FIREPARTICLE					m_Fires[FIRE_NUMBER];		//����������
	LPDIRECT3DVERTEXBUFFER9			m_pVertexBuffer;				//���Ӷ��㻺��
	LPDIRECT3DTEXTURE9				m_pTexture[3];					//������������

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

