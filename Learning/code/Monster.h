#pragma once

#include "Character.h"




class Monster: public Character

{
public:
	D3DXVECTOR3 areaCenterPosition;//�������λ��
	float	timeForComeBackToLife;//����ʱ��
	float	timeForAttack;//���𹥻���ʱ����
	float	areaRaidus;//��ذ뾶



public:
	Monster(D3DXVECTOR3 pos);
	~Monster();

	void Init(LPDIRECT3DDEVICE9	p_d3dDevice, DInputClass * p_Input , TerrainClass * p_Terrain ,CameraClass * p_Camera , std::vector<Character * > * p_Character);
	void Control(float f_TimeDelta);
	void Update();
	void Draw();

};