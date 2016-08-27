//=============================================================================
// Name: FireParticleClass.cpp
//	Des: ��������ϵͳ���Դ�ļ�
//=============================================================================

#include "d3dx9.h"
#include "FireBall.h"
#include <tchar.h>

//extern enum  int;
//-------------------------------------------------------------------------------------------------
// Desc: ���캯��
//-------------------------------------------------------------------------------------------------
FireBall::	FireBall(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice=pd3dDevice;
	m_pVertexBuffer=NULL;	
	for(int i=0; i<3; i++)
		m_pTexture[i] = NULL; 
}

//-------------------------------------------------------------------------------------------------
// Name:  FireParticleClass::InitFireParticle( )
// Desc: ����ϵͳ��ʼ������
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Init(int team , int heroindex,float radius,D3DXVECTOR3 pos ,float fac  )
{

	m_team = team;
	index = heroindex;
	BallRadius = radius;
	position = pos;
	facing = fac;

	isDestroy = false;

	//��ʼ��������������
//	srand(GetTickCount());



	m_Fires.RotationY		=	(rand()%100)/50.0f*D3DX_PI;
	m_Fires.RotationX		=	(rand()%100)/50.0f*D3DX_PI;
	m_Fires.BallSpeed		=	30.0f;
	m_Fires.RotationSpeed	=	30.0f +  rand()%10/10.0f;
	m_Fires.TextureIndex		=	rand()%3 ;
	m_Fires.age				=	0.0f; 


	//�����������Ӷ��㻺��
	m_pd3dDevice->CreateVertexBuffer( 4*sizeof(FIREPOINTVERTEX), 0, 
		D3DFVF_POINTVERTEX,D3DPOOL_MANAGED, &m_pVertexBuffer, NULL );

	//���������Ӷ��㻺��
	FIREPOINTVERTEX vertices[] =
	{
		{ -1200.0f, -1200.0f, 0.0f,   0.0f, 1.0f, },
		{ -1200.0f, 1200.0f, 0.0f,   0.0f, 0.0f, },
		{  1200.0f, -1200.0f, 0.0f,   1.0f, 1.0f, }, 
		{  1200.0f, 1200.0f, 0.0f,   1.0f, 0.0f, }
	};
	//����
	void* pVertices;
	m_pVertexBuffer->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 );
	//����
	memcpy( pVertices, vertices, sizeof(vertices) );
	//����
	m_pVertexBuffer->Unlock();

	//����3�ֻ�������
	D3DXCreateTextureFromFile( m_pd3dDevice, _T("GameMedia\\1.bmp"), &m_pTexture[0] );
	D3DXCreateTextureFromFile( m_pd3dDevice, _T("GameMedia\\2.bmp"), &m_pTexture[1] );
	D3DXCreateTextureFromFile( m_pd3dDevice, _T("GameMedia\\3.bmp"), &m_pTexture[2] );

	return S_OK;
}


//-------------------------------------------------------------------------------------------------
// Name:  FireParticleClass::UpdateFireParticle( )
// Desc: ����ϵͳ���º���
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Update( float fElapsedTime)
{

	if(isDestroy)
		return S_FALSE;

	D3DXMATRIX m;
	D3DXVECTOR3 pV = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	D3DXVec3TransformCoord( &pV, &pV, D3DXMatrixRotationY( &m, -facing ) );
	position += m_Fires.BallSpeed * pV;

	//��������
	m_Fires.age += fElapsedTime;
	if(m_Fires.age > LIFETIME)
	{
		isDestroy = true;
	}

	//���������Ƕ�
	m_Fires.RotationY    += m_Fires.RotationSpeed * fElapsedTime;
	m_Fires.RotationX	+= m_Fires.RotationSpeed * fElapsedTime;


	return S_OK;
}


//-------------------------------------------------------------------------------------------------
// Name:  FireParticleClass::RenderFireParticle( )
// Desc: ����ϵͳ��Ⱦ����
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Render(  )
{
	if(isDestroy)
		return S_FALSE;

	//��������Ч��
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, false );

	//��������״̬
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);	//��������ɫ��ϵĵ�һ����������ɫֵ�������
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );		//������ɫ��ϵĵ�һ��������ֵ��ȡ������ɫֵ
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );			//��С����״̬���������������
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );			//�Ŵ����״̬���������������

	//����Alpha���ϵ��
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);						//��Alpha���
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);						//Դ���ϵ����Ϊ1
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);					//Ŀ����ϵ����Ϊ1

	//�����޳�ģʽΪ���޳��κ���
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	//��Ⱦ����
	//���첢���õ�ǰ�������ӵ��������
	static D3DXMATRIX matYaw, matPitch, matTrans, matWorld;
	D3DXMatrixRotationY(&matYaw, m_Fires.RotationY);
	D3DXMatrixRotationX(&matPitch, m_Fires.RotationX);
	D3DXMatrixTranslation(&matTrans, position.x + m_Fires.x,position.y + m_Fires.y  + 500 ,position.z + m_Fires.z);
	matWorld = matYaw * matPitch * matTrans;
	m_pd3dDevice->SetTransform( D3DTS_WORLD,  &matWorld);

	//��Ⱦ��ǰ��������
	m_pd3dDevice->SetTexture( 0, m_pTexture[m_Fires.TextureIndex] );			//��������
	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(FIREPOINTVERTEX));  //�Ѱ����ļ�������Ϣ�Ķ��㻺�����Ⱦ��ˮ�������  
	m_pd3dDevice->SetFVF(D3DFVF_POINTVERTEX);									//����FVF�����ʽ
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);						//����


	//�ָ������Ⱦ״̬��Alpha��� ���޳�״̬������
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, true );

	return S_OK;
}




//-------------------------------------------------------------------------------------------------
// Desc: ��������
//-------------------------------------------------------------------------------------------------
FireBall::~FireBall()
{
	SAFE_RELEASE(m_pVertexBuffer);

	for(int i=0;i<3; i++)
	{
		SAFE_RELEASE(m_pTexture[i]);
	}
}