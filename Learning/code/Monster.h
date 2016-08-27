#pragma once

#include "Character.h"




class Monster: public Character

{
public:
	D3DXVECTOR3 areaCenterPosition;//领地中心位置
	float	timeForComeBackToLife;//复活时间
	float	timeForAttack;//发起攻击的时间间隔
	float	areaRaidus;//领地半径



public:
	Monster(D3DXVECTOR3 pos);
	~Monster();

	void Init(LPDIRECT3DDEVICE9	p_d3dDevice, Input * p_Input , Terrain * p_Terrain ,Camera * p_Camera , std::vector<Character * > * p_Character);
	void Control(float f_TimeDelta);
	void Update();
	void Draw();

};
