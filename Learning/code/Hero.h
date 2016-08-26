#pragma once

#include "Character.h"
#define TINY 1
#define MOSHOU 2



class Hero: public Character
{
private:
	int heroModal;
	float timeForAttack;
	float timeForRun;
	float timeForComeBackToLife;


public:
	Hero(int modal);
	~Hero();

	void Init(LPDIRECT3DDEVICE9	p_d3dDevice, DInputClass * p_Input , TerrainClass * p_Terrain ,CameraClass * p_Camera, std::vector<Character * > * p_Character);
	void Control(float f_TimeDelta);
	void Update();
	void Draw();

	void UserControl();
	void AutoControl();

};
