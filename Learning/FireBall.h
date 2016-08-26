//=============================================================================
// Name: FireBall.h
//	Des: 火球系统类的头文件
//=============================================================================

#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "D3DUtil.h"
#include "Character.h"

//-------------------------------------------------------------------------------------------------
//粒子的FVF顶点结构和顶点格式
//-------------------------------------------------------------------------------------------------
//extern struct POINTVERTEX;

struct FIREPOINTVERTEX
{
	float x, y, z;													//顶点位置
	float u,v ;														//顶点纹理坐标
};

#define D3DFVF_POINTVERTEX (D3DFVF_XYZ|D3DFVF_TEX1)

#define	LIFETIME	1.0f										//总存活时间
#define FIRE_NUMBER 50										//粒子数量


//-------------------------------------------------------------------------------------------------
// Desc: 粒子结构体的定义
//-------------------------------------------------------------------------------------------------
struct  FIREPARTICLE
{
	float x, y, z;													//坐标位置
	float RotationY;												//绕自身Y轴旋转角度
	float RotationX;												//绕自身X轴旋转角度
	float BallSpeed;												//火球速度
	float RotationSpeed;											//旋转速度
	int   TextureIndex;												//纹理索引数

	bool isAlive;													//是否存在
	float	age;													//存活时间
};

//-------------------------------------------------------------------------------------------------
// Desc: 粒子系统类的定义
//-------------------------------------------------------------------------------------------------
class FireBall
{
private:
	LPDIRECT3DDEVICE9				m_pd3dDevice;					//D3D设备对象
	FIREPARTICLE					m_Fires[FIRE_NUMBER];		//火粒子数组
	LPDIRECT3DVERTEXBUFFER9			m_pVertexBuffer;				//粒子顶点缓存
	LPDIRECT3DTEXTURE9				m_pTexture[3];					//火星纹理数组

	Team							m_team;						//队伍
	int								index;							//哪个下标的人物发出的
	float							BallRadius;								//球半径
	D3DXVECTOR3						position;
	float							facing;

public:
	bool							isDestroy;

public:
	FireBall(LPDIRECT3DDEVICE9 pd3dDevice);							//构造函数
	~FireBall();													//析构函数
	HRESULT Init(Team team , int index,float radius,D3DXVECTOR3 pos ,float fac );										//粒子系统初始化函数
	HRESULT Update( float fElapsedTime);				//粒子系统更新函数
	HRESULT Render( );									//粒子系统渲染函数
};

