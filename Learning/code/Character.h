#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include "AllocateHierarchyClass.h"										//������������
#include "DirectInputClass.h"
#include "TerrainClass.h"
#include "CameraClass.h"
#include "Character.h"
#include <vector>
#include "FireBall.h"

static	int	totalExp[15] = {100,120,140,160,180,200,220,240,260,280,300,320,340,360,380};//���ȼ��ܵ�Exp//��������Ϊ��һ��

// �ذ�Ķ���ṹ
struct CUSTOMVERTEX
{
	float _x, _y, _z;
	float _u, _v ;
	CUSTOMVERTEX(float x, float y, float z, float u, float v)
		: _x(x), _y(y), _z(z), _u(u), _v(v) {}
};
#define D3DFVF_CUSTOMVERTEX  (D3DFVF_XYZ | D3DFVF_TEX1)

enum Team{Red,Blue,Other};//����ö������

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

	//����������
	LPD3DXFRAME								pFrameRoot;
	CAllocateHierarchy*						pAllocateHier;
	LPD3DXANIMATIONCONTROLLER				pAnimController;

	//HP��
	LPDIRECT3DVERTEXBUFFER9					HPVBuffer;    // HP���㻺�����
	LPDIRECT3DTEXTURE9						HPTexture;    // HP�������
	LPDIRECT3DVERTEXBUFFER9					HPBackVBuffer;    // HP���㻺����󱳾�
	LPDIRECT3DTEXTURE9						HPBackTexture;    // HP������󱳾�


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


	//--------------��ɫ����-------------------//
	int										id;//�±�
	Team									team;//����
	int										species;//0 hero,1 tower,2 soliders,3 monster

	bool									alive;//�Ƿ����
	int										level;//�ȼ�
	int										hp;//Ѫ��
	int										exp;//����ֵ
	int										atk;//������
	int										def;//������

	float									walkSpeed;//��·�ٶ�
	float									runSpeed; //�ܲ��ٶ�
	float									turnSpeed;//ת���ٶ�
	float									mySpeed;//�ҵ��ٶȣ������Զ�����
	float									modalRadius;//ģ�Ͱ뾶
	float									sightRadius;//��Ұ�뾶
	float									attackRadius;//�����뾶

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