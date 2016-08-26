//=============================================================================
// Name: FireParticleClass.cpp
//	Des: 火球 系统类的源文件
//=============================================================================

#include "d3dx9.h"
#include "FireBall.h"
#include "D3DUtil.h"
#include <tchar.h>

//extern enum  Team;
//-------------------------------------------------------------------------------------------------
// Desc: 构造函数
//-------------------------------------------------------------------------------------------------
FireBall::	FireBall(LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_pd3dDevice=pd3dDevice;
	m_sphere = NULL;
}

//-------------------------------------------------------------------------------------------------
// Name:  FireParticleClass::InitFireParticle( )
// Desc:  系统初始化函数
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
	if(FAILED(D3DXCreateSphere(m_pd3dDevice,radius , 50, 50,					//球面体的创建
		&m_sphere, NULL))) return false;

	// 设置材质
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
// Desc:  系统更新函数
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
// Desc:  系统渲染函数
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Render(  )
{
	//禁用照明效果
	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				//关闭光照
	m_pd3dDevice->SetRenderState(D3DRS_FILLMODE,D3DFILL_SOLID);

	// 进行球面体的绘制
	static D3DXMATRIX matTrans,matWorld;
	D3DXMatrixTranslation(&matTrans, position.x,position.y + 200, position.z);
	m_pd3dDevice->SetTransform(D3DTS_WORLD, &matWorld);

	m_sphere->DrawSubset(0);

	m_pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);				//打开光照

	return S_OK;
}




//-------------------------------------------------------------------------------------------------
// Desc: 析构函数
//-------------------------------------------------------------------------------------------------
FireBall::~FireBall()
{
	SAFE_RELEASE(m_sphere);
}