#include "Character.h"
#include "Monster.h"
using namespace std;

static	int	totalHpForMonster[15] = {800,810,820,830,840,850,860,870,880,890,900,920,940,960,1000};//各等级总的HP
static	int	totalAtkForMonster[15] = {54,55,56,57,58,59,60,61,62,63,64,65,66,67,68};//各等级总的Atk
static	int	totalDefForMonster[15] = {0};//各等级总的Def

Monster::Monster(D3DXVECTOR3 pos):
	timeForComeBackToLife(50),
	timeForAttack(2),
	areaRaidus(1000.0f)
{
	areaCenterPosition = pos;
	sightRadius = 400;
	attackRadius = 200;

	timeForAttack = -1.0f;
	timeForComeBackToLife = 50.0f;
	attackRadius = 1000;
	sightRadius = 1500;

	hp = totalHpForMonster[level - 1];
	exp = 0;
	atk = totalAtkForMonster[level - 1];
	def = totalDefForMonster[level -1];
}

Monster::~Monster()
{

}

void Monster::Control(float f_TimeDelta)
{
	fTimeDelta = f_TimeDelta;	

	atk = totalAtkForMonster[level - 1];
	def = totalDefForMonster[level -1];



	bool canAttack  = false , canFollow = false ;
	float lenCanAttack , lenCanFollow;
	D3DXVECTOR3 tempTargetPosition = targetPosition;

	//能否攻击,能否追击
	for(int i = 0 ; i < (pCharacter)->size();i++)
	{
		if(!(*pCharacter)[i]->alive)
			continue;
		if((*pCharacter)[i]->team  == team)
			continue;
		float length = D3DXVec3Length(&((*pCharacter)[i]->position - position));
		if(length < attackRadius)
		{
			tempTargetPosition = (*pCharacter)[i]->position;
			canAttack = true;
			break;
		}
		else if(length < sightRadius)
		{
			tempTargetPosition = (*pCharacter)[i]->position;
			canFollow = true;
		}
	}

	//无目标，回领地中央
	if(!canAttack && !canFollow)
		targetPosition = areaCenterPosition;

	D3DXVECTOR3 vDiff;
	//离开了领地，回去
	if(D3DXVec3Length(&(position - areaCenterPosition)) > areaRaidus)
	{
		targetPosition = areaCenterPosition;
		tempTargetPosition = targetPosition;
	}
	else if(tempTargetPosition == targetPosition)
		D3DXVec3Subtract( &vDiff, &targetPosition, &position );
	else
		D3DXVec3Subtract( &vDiff, &tempTargetPosition, &position );

	D3DXVec3Normalize( &vDiff, &vDiff );

	if( vDiff.z == 0.f )
	{
		if( vDiff.x > 0.f )
			facingTarget = 0.0f;
		else
			facingTarget = D3DX_PI;
	}
	else if( vDiff.z > 0.f )
		facingTarget = acosf( vDiff.x );
	else
		facingTarget = acosf( - vDiff.x ) + D3DX_PI;

	//	setFacing(facingTarget);
	if(facing > facingTarget && (facing - facingTarget > 2*turnSpeed || facingTarget + 2 * D3DX_PI - facing > 2*turnSpeed ))
		setFacing(facing - turnSpeed);	
	if(facing - facingTarget > 3 * turnSpeed)
		setFacing(facing - turnSpeed);	
	else
		setFacing(facingTarget);
	if(facing < facingTarget && ( -facing + facingTarget > 2*turnSpeed || -facingTarget - 2 * D3DX_PI + facing > 2*turnSpeed ))
		if(facingTarget - facing > 3 * turnSpeed)
			setFacing(facing + turnSpeed);	
		else
			setFacing(facingTarget);

	if(canAttack && timeForAttack <= 0.0f)
	{
		Attack();

		bool ok = false;
		for(int i = 0 ;  i < g_vFireBall.size() ; i ++)
		{
			if(g_vFireBall[i]->isDestroy)
			{
				g_vFireBall[i]->Init(team , id , 500.0f,position ,facing);
				ok = true;
				break;
			}
		}
		if(!ok)
		{
			FireBall * temp = new FireBall(pd3dDevice);
			g_vFireBall.push_back(temp);

			g_vFireBall[g_vFireBall.size() - 1]->Init(team , id , 500.0f,position ,facing);

		}

		timeForAttack = 3.0f;	
	}
	//if(currentAnim != attack)
	if(!canAttack)
		Walk();

}


void Monster::Init(LPDIRECT3DDEVICE9	p_d3dDevice, Input * p_Input , Terrain * p_Terrain , Camera * p_Camera , std::vector<Character * > * p_Character)
{
	InitByName(p_d3dDevice ,p_Input,p_Terrain,p_Camera,p_Character, L"GameMedia\\YaSeWang3.X","idle","idle" ,"idle" ,"idle",-D3DX_PI / 2.0f ,D3DX_PI / 2.0f  , 0.0f ,2.5f);
	//InitByName(p_d3dDevice ,p_Input,p_Terrain,p_Camera,p_Character, L"Dragon.X","Anim-1","Anim-1" ,"Anim-1" ,"Anim-1",0.0f ,D3DX_PI / 2.0f  , 0.0f ,20.f);
}



void Monster::Update()
{

	if(timeForAttack > 0.0f)
		timeForAttack -= fTimeDelta;

	if(!alive)
	{
		timeForComeBackToLife -= fTimeDelta;
		if(timeForComeBackToLife <= 0.0f)
		{
			alive = true;
			hp = totalHpForMonster[level - 1];
			setPosition(areaCenterPosition);

		}
	}

	//设置HP的长度
	CUSTOMVERTEX *pVertices = NULL;
	HPBackVBuffer ->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = CUSTOMVERTEX(-totalHpForMonster[level - 1] / 2, 0.0f, 0.0f, 0.0f, 1.0f);
	pVertices[1] = CUSTOMVERTEX(-totalHpForMonster[level - 1] / 2, 40.0f, 0.0f, 0.0f, 0.0f); 
	pVertices[2] = CUSTOMVERTEX( totalHpForMonster[level - 1] / 2, 0.0f, 0.0f, 1.0f, 1.0f); 
	pVertices[3] = CUSTOMVERTEX( totalHpForMonster[level - 1] / 2, 40.0f, 0.0f, 1.0f, 0.0f);
	HPVBuffer ->Unlock();


	HPVBuffer ->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = CUSTOMVERTEX(-totalHpForMonster[level - 1] / 2 , 0.0f, 0.0f, 0.0f, 1.0f);
	pVertices[1] = CUSTOMVERTEX(-totalHpForMonster[level - 1] / 2 , 40.0f, 0.0f, 0.0f, 0.0f); 
	pVertices[2] = CUSTOMVERTEX( hp - totalHpForMonster[level - 1] / 2 , 0.0f, 0.0f, 1.0f, 1.0f); 
	pVertices[3] = CUSTOMVERTEX( hp - totalHpForMonster[level - 1] / 2 , 40.0f, 0.0f, 1.0f, 0.0f);
	HPVBuffer ->Unlock();


	///
	///pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				//关闭光照
	///

	// 设置骨骼动画的矩阵
	D3DXMATRIX matFinal, mx;
	D3DXMatrixIdentity(&matFinal);
	//根据位置和朝向计算变换矩阵 
	D3DXMatrixRotationY( &matFinal, -facing );
	D3DXMatrixTranslation( &mx, position.x, position.y, position.z );
	D3DXMatrixMultiply( &matFinal, &matFinal, &mx );
	D3DXMatrixMultiply( &matFinal, &modelOrientation, &matFinal );


	pd3dDevice->SetTransform(D3DTS_WORLD, &matFinal);

	// 更新骨骼动画
	pAnimController->AdvanceTime(fTimeDelta, NULL);	//设置骨骼动画的时间

	UpdateFrameMatrices(pFrameRoot, &matFinal);		//更新框架中的变换矩阵

	///
	///pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);					//打开光照
	///


	//更新时间
	prvTime = curTime;
	curTime = prvTime + fTimeDelta ; 



}
void Monster::Draw()
{
	if(!alive)
		return;
	Character::Draw();
}