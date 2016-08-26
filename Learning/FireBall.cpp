//=============================================================================
// Name: FireParticleClass.cpp
//	Des: ��������ϵͳ���Դ�ļ�
//=============================================================================

#include "d3dx9.h"
#include "FireBall.h"
#include <tchar.h>

//extern enum  Team;
//-------------------------------------------------------------------------------------------------
// Desc: ���캯��
//-------------------------------------------------------------------------------------------------
FireBall::	FireBall(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice=pd3dDevice;
	m_pVertexBuffer=NULL;	
	for(int i=0; i<5; i++)
		m_pTexture[i] = NULL; 
}

//-------------------------------------------------------------------------------------------------
// Name:  FireParticleClass::InitFireParticle( )
// Desc: ����ϵͳ��ʼ������
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Init(Team team , int heroindex,float radius,D3DXVECTOR3 pos ,float fac  )
{

	m_team = team;
	index = heroindex;
	BallRadius = radius;
	position = pos;
	facing = fac;

	isDestroy = false;

	//��ʼ��������������
	srand(GetTickCount());
	for(int i=0; i<FIRE_NUMBER; i++)
	{
		do
		{
		m_Fires[i].x				=	float(rand()%(int)(2*BallRadius)) - BallRadius;
		m_Fires[i].z				=	float(rand()%(int)(2*BallRadius)) - BallRadius;
		}while(m_Fires[i].x	* m_Fires[i].x	 + m_Fires[i].z * m_Fires[i].z > BallRadius * BallRadius);

		m_Fires[i].y				=	sqrt(BallRadius * BallRadius - m_Fires[i].x	* m_Fires[i].x	 + m_Fires[i].z * m_Fires[i].z ) * (rand()%2==0)? 1 : -1;

		m_Fires[i].RotationY		=	(rand()%100)/50.0f*D3DX_PI;
		m_Fires[i].RotationX		=	(rand()%100)/50.0f*D3DX_PI;
		m_Fires[i].BallSpeed		=	30.0f;
		m_Fires[i].RotationSpeed	=	5.0f +  rand()%10/10.0f;
		m_Fires[i].TextureIndex		=	rand()%3 + 1;
		m_Fires[i].age				=   rand()%(int(LIFETIME * 5)) / 10.0f;
		m_Fires[i].isAlive			=	true;
	}


	//�����������Ӷ��㻺��
	m_pd3dDevice->CreateVertexBuffer( 4*sizeof(FIREPOINTVERTEX), 0, 
		D3DFVF_POINTVERTEX,D3DPOOL_MANAGED, &m_pVertexBuffer, NULL );

	//���������Ӷ��㻺��
	FIREPOINTVERTEX vertices[] =
	{
		{ -30.0f, 0.0f, 0.0f,   0.0f, 1.0f, },
		{ -30.0f, 60.0f, 0.0f,   0.0f, 0.0f, },
		{  30.0f, 0.0f, 0.0f,   1.0f, 1.0f, }, 
		{  30.0f, 60.0f, 0.0f,   1.0f, 0.0f, }
	};
	//����
	void* pVertices;
	m_pVertexBuffer->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 );
	//����
	memcpy( pVertices, vertices, sizeof(vertices) );
	//����
	m_pVertexBuffer->Unlock();

	//����6�ֻ�������
	D3DXCreateTextureFromFile( m_pd3dDevice, _T("1.bmp"), &m_pTexture[0] );
	D3DXCreateTextureFromFile( m_pd3dDevice, _T("2.bmp"), &m_pTexture[1] );
	D3DXCreateTextureFromFile( m_pd3dDevice, _T("3.bmp"), &m_pTexture[2] );

	return S_OK;
}


//-------------------------------------------------------------------------------------------------
// Name:  FireParticleClass::UpdateFireParticle( )
// Desc: ����ϵͳ���º���
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Update( float fElapsedTime)
{
	isDestroy = true;
	//һ��forѭ��������ÿ���������ӵĵ�ǰλ�úͽǶ�
	for(int i=0; i<FIRE_NUMBER; i++)
	{
		if(m_Fires[i].isAlive == false)
		{
			continue;
		}
		
		m_Fires[i].y -= m_Fires[i].BallSpeed*fElapsedTime;


		D3DXMATRIX m;
		D3DXVECTOR3 pV = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
		D3DXVec3TransformCoord( &pV, &pV, D3DXMatrixRotationY( &m, -facing ) );
		position += pV;
///		position += 

		//��������
		m_Fires[i].age += fElapsedTime;
		if(m_Fires[i].age > LIFETIME)
		{
			m_Fires[i].isAlive = false;
			continue;
		}
	
		//���������Ƕ�
		m_Fires[i].RotationY    += m_Fires[i].RotationSpeed * fElapsedTime;
		m_Fires[i].RotationX	+= m_Fires[i].RotationSpeed * fElapsedTime;

		isDestroy = false;
	}

	return S_OK;
}


//-------------------------------------------------------------------------------------------------
// Name:  FireParticleClass::RenderFireParticle( )
// Desc: ����ϵͳ��Ⱦ����
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Render(  )
{
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
	for(int i=0; i<FIRE_NUMBER; i++)
	{
		//���첢���õ�ǰ�������ӵ��������
		static D3DXMATRIX matYaw, matPitch, matTrans, matWorld;
		D3DXMatrixRotationY(&matYaw, m_Fires[i].RotationY);
		D3DXMatrixRotationX(&matPitch, m_Fires[i].RotationX);
		D3DXMatrixTranslation(&matTrans, position.x + m_Fires[i].x,position.y + m_Fires[i].y  + 200 ,position.z + m_Fires[i].z);
		matWorld = matYaw * matPitch * matTrans;
		m_pd3dDevice->SetTransform( D3DTS_WORLD,  &matWorld);

		//��Ⱦ��ǰ��������
		m_pd3dDevice->SetTexture( 0, m_pTexture[m_Fires[i].TextureIndex] );			//��������
		m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(FIREPOINTVERTEX));  //�Ѱ����ļ�������Ϣ�Ķ��㻺�����Ⱦ��ˮ�������  
		m_pd3dDevice->SetFVF(D3DFVF_POINTVERTEX);									//����FVF�����ʽ
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);						//����

	}

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