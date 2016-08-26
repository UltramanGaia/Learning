//=============================================================================
// Name: FireParticleClass.cpp
//	Des: 火球粒子系统类的源文件
//=============================================================================

#include "d3dx9.h"
#include "FireBall.h"
#include <tchar.h>

//extern enum  Team;
//-------------------------------------------------------------------------------------------------
// Desc: 构造函数
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
// Desc: 粒子系统初始化函数
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Init(Team team , int heroindex,float radius,D3DXVECTOR3 pos ,float fac  )
{

	m_team = team;
	index = heroindex;
	BallRadius = radius;
	position = pos;
	facing = fac;

	isDestroy = false;

	//初始化火球粒子数组
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


	//创建火球粒子顶点缓存
	m_pd3dDevice->CreateVertexBuffer( 4*sizeof(FIREPOINTVERTEX), 0, 
		D3DFVF_POINTVERTEX,D3DPOOL_MANAGED, &m_pVertexBuffer, NULL );

	//填充火球粒子顶点缓存
	FIREPOINTVERTEX vertices[] =
	{
		{ -30.0f, 0.0f, 0.0f,   0.0f, 1.0f, },
		{ -30.0f, 60.0f, 0.0f,   0.0f, 0.0f, },
		{  30.0f, 0.0f, 0.0f,   1.0f, 1.0f, }, 
		{  30.0f, 60.0f, 0.0f,   1.0f, 0.0f, }
	};
	//加锁
	void* pVertices;
	m_pVertexBuffer->Lock( 0, sizeof(vertices), (void**)&pVertices, 0 );
	//访问
	memcpy( pVertices, vertices, sizeof(vertices) );
	//解锁
	m_pVertexBuffer->Unlock();

	//创建6种火球纹理
	D3DXCreateTextureFromFile( m_pd3dDevice, _T("1.bmp"), &m_pTexture[0] );
	D3DXCreateTextureFromFile( m_pd3dDevice, _T("2.bmp"), &m_pTexture[1] );
	D3DXCreateTextureFromFile( m_pd3dDevice, _T("3.bmp"), &m_pTexture[2] );

	return S_OK;
}


//-------------------------------------------------------------------------------------------------
// Name:  FireParticleClass::UpdateFireParticle( )
// Desc: 粒子系统更新函数
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Update( float fElapsedTime)
{
	isDestroy = true;
	//一个for循环，更新每个火球粒子的当前位置和角度
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

		//火球粒子
		m_Fires[i].age += fElapsedTime;
		if(m_Fires[i].age > LIFETIME)
		{
			m_Fires[i].isAlive = false;
			continue;
		}
	
		//更改自旋角度
		m_Fires[i].RotationY    += m_Fires[i].RotationSpeed * fElapsedTime;
		m_Fires[i].RotationX	+= m_Fires[i].RotationSpeed * fElapsedTime;

		isDestroy = false;
	}

	return S_OK;
}


//-------------------------------------------------------------------------------------------------
// Name:  FireParticleClass::RenderFireParticle( )
// Desc: 粒子系统渲染函数
//-------------------------------------------------------------------------------------------------
HRESULT FireBall::Render(  )
{
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
	for(int i=0; i<FIRE_NUMBER; i++)
	{
		//构造并设置当前火球粒子的世界矩阵
		static D3DXMATRIX matYaw, matPitch, matTrans, matWorld;
		D3DXMatrixRotationY(&matYaw, m_Fires[i].RotationY);
		D3DXMatrixRotationX(&matPitch, m_Fires[i].RotationX);
		D3DXMatrixTranslation(&matTrans, position.x + m_Fires[i].x,position.y + m_Fires[i].y  + 200 ,position.z + m_Fires[i].z);
		matWorld = matYaw * matPitch * matTrans;
		m_pd3dDevice->SetTransform( D3DTS_WORLD,  &matWorld);

		//渲染当前火球粒子
		m_pd3dDevice->SetTexture( 0, m_pTexture[m_Fires[i].TextureIndex] );			//设置纹理
		m_pd3dDevice->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(FIREPOINTVERTEX));  //把包含的几何体信息的顶点缓存和渲染流水线相关联  
		m_pd3dDevice->SetFVF(D3DFVF_POINTVERTEX);									//设置FVF灵活顶点格式
		m_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);						//绘制

	}

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