//=============================================================================
// Name: FireParticleClass.cpp
//	Des: 火球粒子系统类的源文件
//=============================================================================

#include "d3dx9.h"
#include "FireBall.h"
#include <tchar.h>

//extern enum  int;
//-------------------------------------------------------------------------------------------------
// Desc: 构造函数
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
// Desc: 粒子系统初始化函数
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Init(int team , int heroindex,float radius,D3DXVECTOR3 pos ,float fac  )
{

	m_team = team;
	index = heroindex;
	BallRadius = radius;
	position = pos;
	facing = fac;

	isDestroy = false;

	//初始化火球粒子数组
//	srand(GetTickCount());



	m_Fires.RotationY		=	(rand()%100)/50.0f*D3DX_PI;
	m_Fires.RotationX		=	(rand()%100)/50.0f*D3DX_PI;
	m_Fires.BallSpeed		=	30.0f;
	m_Fires.RotationSpeed	=	30.0f +  rand()%10/10.0f;
	m_Fires.TextureIndex		=	rand()%3 ;
	m_Fires.age				=	0.0f; 


	//创建火球粒子顶点缓存
	m_pd3dDevice->CreateVertexBuffer( 4*sizeof(FIREPOINTVERTEX), 0, 
		D3DFVF_POINTVERTEX,D3DPOOL_MANAGED, &m_pVertexBuffer, NULL );

	//填充火球粒子顶点缓存
	FIREPOINTVERTEX vertices[] =
	{
		{ -1200.0f, -1200.0f, 0.0f,   0.0f, 1.0f, },
		{ -1200.0f, 1200.0f, 0.0f,   0.0f, 0.0f, },
		{  1200.0f, -1200.0f, 0.0f,   1.0f, 1.0f, }, 
		{  1200.0f, 1200.0f, 0.0f,   1.0f, 0.0f, }
	};
	//加锁
	void* pVertices;
	m_pVertexBuffer->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 );
	//访问
	memcpy( pVertices, vertices, sizeof(vertices) );
	//解锁
	m_pVertexBuffer->Unlock();

	//创建3种火球纹理
	D3DXCreateTextureFromFile( m_pd3dDevice, _T("GameMedia\\1.bmp"), &m_pTexture[0] );
	D3DXCreateTextureFromFile( m_pd3dDevice, _T("GameMedia\\2.bmp"), &m_pTexture[1] );
	D3DXCreateTextureFromFile( m_pd3dDevice, _T("GameMedia\\3.bmp"), &m_pTexture[2] );

	return S_OK;
}


//-------------------------------------------------------------------------------------------------
// Name:  FireParticleClass::UpdateFireParticle( )
// Desc: 粒子系统更新函数
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Update( float fElapsedTime)
{

	if(isDestroy)
		return S_FALSE;

	D3DXMATRIX m;
	D3DXVECTOR3 pV = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
	D3DXVec3TransformCoord( &pV, &pV, D3DXMatrixRotationY( &m, -facing ) );
	position += m_Fires.BallSpeed * pV;

	//火球粒子
	m_Fires.age += fElapsedTime;
	if(m_Fires.age > LIFETIME)
	{
		isDestroy = true;
	}

	//更改自旋角度
	m_Fires.RotationY    += m_Fires.RotationSpeed * fElapsedTime;
	m_Fires.RotationX	+= m_Fires.RotationSpeed * fElapsedTime;


	return S_OK;
}


//-------------------------------------------------------------------------------------------------
// Name:  FireParticleClass::RenderFireParticle( )
// Desc: 粒子系统渲染函数
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Render(  )
{
	if(isDestroy)
		return S_FALSE;

	//禁用照明效果
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, false );

	//设置纹理状态
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_SELECTARG1);	//将纹理颜色混合的第一个参数的颜色值用于输出
	m_pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );		//纹理颜色混合的第一个参数的值就取纹理颜色值
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );			//缩小过滤状态采用线性纹理过滤
	m_pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );			//放大过滤状态采用线性纹理过滤

	//设置Alpha混合系数
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);						//打开Alpha混合
	m_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);						//源混合系数设为1
	m_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);					//目标混合系数设为1

	//设置剔出模式为不剔除任何面
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );

	//渲染火球
	//构造并设置当前火球粒子的世界矩阵
	static D3DXMATRIX matYaw, matPitch, matTrans, matWorld;
	D3DXMatrixRotationY(&matYaw, m_Fires.RotationY);
	D3DXMatrixRotationX(&matPitch, m_Fires.RotationX);
	D3DXMatrixTranslation(&matTrans, position.x + m_Fires.x,position.y + m_Fires.y  + 500 ,position.z + m_Fires.z);
	matWorld = matYaw * matPitch * matTrans;
	m_pd3dDevice->SetTransform( D3DTS_WORLD,  &matWorld);

	//渲染当前火球粒子
	m_pd3dDevice->SetTexture( 0, m_pTexture[m_Fires.TextureIndex] );			//设置纹理
	m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(FIREPOINTVERTEX));  //把包含的几何体信息的顶点缓存和渲染流水线相关联  
	m_pd3dDevice->SetFVF(D3DFVF_POINTVERTEX);									//设置FVF灵活顶点格式
	m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);						//绘制


	//恢复相关渲染状态：Alpha混合 、剔除状态、光照
	m_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	m_pd3dDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	m_pd3dDevice->SetRenderState( D3DRS_LIGHTING, true );

	return S_OK;
}




//-------------------------------------------------------------------------------------------------
// Desc: 析构函数
//-------------------------------------------------------------------------------------------------
FireBall::~FireBall()
{
	SAFE_RELEASE(m_pVertexBuffer);

	for(int i=0;i<3; i++)
	{
		SAFE_RELEASE(m_pTexture[i]);
	}
}