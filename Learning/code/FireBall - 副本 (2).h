//=============================================================================
// Name: FireBall.h
//	Des: 火球系统类的头文件
//=============================================================================

#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "D3DUtil.h"
#include "Character.h"


#define	LIFETIME	1.0f										//总存活时间



//-------------------------------------------------------------------------------------------------
// Desc: 类的定义
//-------------------------------------------------------------------------------------------------
class FireBall
{
private:
	LPDIRECT3DDEVICE9				m_pd3dDevice;					//D3D设备对象
	LPD3DXMESH 						m_sphere ;		//球面体对象
	float							m_age;

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

