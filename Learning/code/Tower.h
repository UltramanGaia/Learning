#pragma once

#include "Character.h"

class Tower: public Character

{
private:
	float timeForAttack;



public:
	Tower();
	~Tower();

	void Init(LPDIRECT3DDEVICE9	p_d3dDevice, DInputClass * p_Input , TerrainClass * p_Terrain ,CameraClass * p_Camera , std::vector<Character * > * p_Character);
	void Control(float f_TimeDelta);
	void Update();
	void Draw();

};
