#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "AllocateHierarchyClass.h"										//骨骼动画的类
#include "DirectInputClass.h"
#include "TerrainClass.h"
#include "CameraClass.h"
#include "Character.h"
#include <vector>
#include "FireBall.h"

static	int	totalExp[15] = {100,120,140,160,180,200,220,240,260,280,300,320,340,360,380};//各等级总的Exp//所爆经验为其一半

// 地板的顶点结构
struct CUSTOMVERTEX
{
	float _x, _y, _z;
	float _u, _v ;
	CUSTOMVERTEX(float x, float y, float z, float u, float v)
		: _x(x), _y(y), _z(z), _u(u), _v(v) {}
};
#define D3DFVF_CUSTOMVERTEX  (D3DFVF_XYZ | D3DFVF_TEX1)

enum Team{Red,Blue,Other};//队伍枚举类型

extern  std:: vector < FireBall * >			g_vFireBall;

class Character
{
public:
	LPDIRECT3DDEVICE9						pd3dDevice;
	DInputClass *							pInput;
	TerrainClass *							pTerrain;
	CameraClass *                           pCamera;

	std::vector<Character * > *				pCharacter;

	float									fTimeDelta;

	//骨骼动画的
	LPD3DXFRAME								pFrameRoot;
	CAllocateHierarchy*						pAllocateHier;
	LPD3DXANIMATIONCONTROLLER				pAnimController;

	//HP的
	LPDIRECT3DVERTEXBUFFER9					HPVBuffer;    // HP顶点缓存对象
	LPDIRECT3DTEXTURE9						HPTexture;    // HP纹理对象
	LPDIRECT3DVERTEXBUFFER9					HPBackVBuffer;    // HP顶点缓存对象背景
	LPDIRECT3DTEXTURE9						HPBackTexture;    // HP纹理对象背景


	D3DXMATRIX								modelOrientation;  
	DWORD									walk;
	DWORD									run;
	DWORD									idle;
	DWORD									attack;

	DWORD									currentAnim;
	UINT									currentTrack;

	bool									userControl;

	D3DXVECTOR3								position;
	D3DXVECTOR3								targetPosition;

	float									facing;
	float									facingTarget;
	float									alongXRotation;
	float									alongYRotation;
	float									alongZRotation;

	float									scaling;

	float									prvTime;
	float									curTime;


	//--------------角色属性-------------------//
	int										id;//下标
	Team									team;//队伍
	int										species;//0 hero,1 tower,2 soliders,3 monster

	bool									alive;//是否活着
	int										level;//等级
	int										hp;//血量
	int										exp;//经验值
	int										atk;//攻击力
	int										def;//防御力

	float									walkSpeed;//走路速度
	float									runSpeed; //跑步速度
	float									turnSpeed;//转向速度
	float									mySpeed;//我的速度，用于自动控制
	float									modalRadius;//模型半径
	float									sightRadius;//视野半径
	float									attackRadius;//攻击半径

	float									timeForComeBackToLife;

public:
	Character();
	~Character();


	void setControlByUser(bool userCon);
	void setPosition(D3DXVECTOR3);
	void setPositonChange(float x, float y, float z);
	void setFacing(float);
	void setScaling(float);
	void setWalkSpeed(float wSpeed);
	void setRunSpeed(float rSpeed);
	void setTurnSpeed(float);


	void getPosition(D3DXVECTOR3 *);
	void getTargetPosition(D3DXVECTOR3 *);
	void getFacing(D3DXVECTOR3* pV );
	float getScaling();
	float getWalkSpeed();
	float getRunSpeed();
	float getTurnSpeed();	
	int getCurrentTrack();
	int getCurrentAnim();

	DWORD GetAnimIndex( char sString[] );


	virtual void Character::Init(LPDIRECT3DDEVICE9	p_d3dDevice, DInputClass * p_Input , TerrainClass * p_Terrain , CameraClass * p_Camera , std::vector<Character * > * p_Character);

	void InitByName(LPDIRECT3DDEVICE9	p_d3dDevice, DInputClass * p_Input , TerrainClass * p_Terrain ,CameraClass * p_Camera, std::vector<Character * > * p_Character  ,LPCWSTR filename , char * walkAnim ,char * runAnim ,char * idleAnim ,char * attackAnim, float xRotation ,float yRotation , float zRotation ,float sca );


	virtual void Control(float f_TimeDelta);
	void UserControl();
	void AutoControl();
	void ChooseNewTargetPosition();
	void SmoothIdle();

	virtual void Update();
	void ChangeTrack(const char  *name);
	void ChangeTrack(DWORD name);
	virtual void Draw();

	void Walk();
	void Run();
	void Idle();
	void Attack();
	bool BeingBlocked();

};