//=============================================================================
// Name: SnowParticle.h
//	Des: ѩ������ϵͳ���ͷ�ļ�
//=============================================================================


#pragma once
#include "D3DUtil.h"
#define  PARTICLE_NUMBER					1000					//ѩ����������
#define  SNOW_SYSTEM_LENGTH_X				20000					//ѩ����������ĳ���
#define  SNOW_SYSTEM_WIDTH_Z				20000					//ѩ����������Ŀ���
#define  SNOW_SYSTEM_HEIGHT_Y				20000					//ѩ����������ĸ߶�

//-------------------------------------------------------------------------------------------------
//ѩ�����ӵ�FVF����ṹ�Ͷ����ʽ
//-------------------------------------------------------------------------------------------------
struct POINTVERTEX
{
	float x, y, z;													//����λ��
	float u,v ;														//������������
};
#define D3DFVF_POINTVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)


//-------------------------------------------------------------------------------------------------
// Desc: ѩ�����ӽṹ��Ķ���
//-------------------------------------------------------------------------------------------------
struct  SNOWPARTICLE
{
	float x, y, z;													//����λ��
	float RotationY;												//ѩ��������Y����ת�Ƕ�
	float RotationX;												//ѩ��������X����ת�Ƕ�
	float FallSpeed;												//ѩ���½��ٶ�
	float RotationSpeed;											//ѩ����ת�ٶ�
	int   TextureIndex;												//����������
};

//-------------------------------------------------------------------------------------------------
// Desc: ����ϵͳ��Ķ���
//-------------------------------------------------------------------------------------------------
class SnowParticle
{
private:
	LPDIRECT3DDEVICE9				m_pd3dDevice;					//D3D�豸����
	SNOWPARTICLE					m_Snows[PARTICLE_NUMBER];		//ѩ����������
	LPDIRECT3DVERTEXBUFFER9			m_pVertexBuffer;				//���Ӷ��㻺��
	LPDIRECT3DTEXTURE9				m_pTexture[6];					//ѩ����������

public:
	SnowParticle(LPDIRECT3DDEVICE9 pd3dDevice);				//���캯��
	~SnowParticle();											//��������
	HRESULT InitSnowParticle();										//����ϵͳ��ʼ������
	HRESULT UpdateSnowParticle( float fElapsedTime);				//����ϵͳ���º���
	HRESULT RenderSnowParticle( );									//����ϵͳ��Ⱦ����
};
