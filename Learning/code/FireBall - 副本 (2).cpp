//=============================================================================
// Name: FireParticleClass.cpp
//	Des: ���� ϵͳ���Դ�ļ�
//=============================================================================

#include "d3dx9.h"
#include "FireBall.h"
#include "D3DUtil.h"
#include <tchar.h>

//extern enum  Team;
//-------------------------------------------------------------------------------------------------
// Desc: ���캯��
//-------------------------------------------------------------------------------------------------
FireBall::	FireBall(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice=pd3dDevice;
	m_sphere = NULL;
}

//-------------------------------------------------------------------------------------------------
// Name:  FireParticleClass::InitFireParticle( )
// Desc:  ϵͳ��ʼ������
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Init(Team team , int heroindex,float radius,D3DXVECTOR3 pos ,float fac  )
{

	m_team = team;
	index = heroindex;
	BallRadius = radius;
	position = pos;
	facing = fac;

	m_age = 0.0f;
	isDestroy = false;

	HRESULT hr;
	if(FAILED(D3DXCreateSphere(m_pd3dDevice,radius , 50, 50,					//������Ĵ���
		&m_sphere, NULL))) return false;

	// ���ò���
/*	D3DMATERIAL9 mtrl;
	::ZeroMemory(&mtrl, sizeof(mtrl));
	mtrl.Ambient  = D3DXCOLOR(0.5f, 0.5f, 0.7f, 1.0f);
	mtrl.Diffuse  = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f);
	mtrl.Specular = D3DXCOLOR(0.3f, 0.3f, 0.3f, 0.3f);
	mtrl.Emissive = D3DXCOLOR(0.3f, 0.0f, 0.1f, 1.0f);
	m_pd3dDevice->SetMaterial(&mtrl);
*/
	return S_OK;
}


//-------------------------------------------------------------------------------------------------
// Name:  FireParticleClass::UpdateFireParticle( )
// Desc:  ϵͳ���º���
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Update( float fElapsedTime)
{

	D3DXMATRIX m;
	D3DXVECTOR3 pV = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	D3DXVec3TransformCoord( &pV, &pV, D3DXMatrixRotationY( &m, -facing ) );
	position +=  pV;

	//
	m_age += fElapsedTime;
	if(m_age > LIFETIME)
	{
		isDestroy = true;
	}


	return S_OK;
}


//-------------------------------------------------------------------------------------------------
// Name:  FireParticleClass::RenderFireParticle( )
// Desc:  ϵͳ��Ⱦ����
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Render(  )
{
	//��������Ч��
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				//�رչ���
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);

	// ����������Ļ���
	static D3DXMATRIX matTrans,matWorld;
	D3DXMatrixTranslation(&matTrans, position.x,position.y + 200, position.z);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_sphere->DrawSubset(0);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);				//�򿪹���

	return S_OK;
}




//-------------------------------------------------------------------------------------------------
// Desc: ��������
//-------------------------------------------------------------------------------------------------
FireBall::~FireBall()
{
	SAFE_RELEASE(m_sphere);
}