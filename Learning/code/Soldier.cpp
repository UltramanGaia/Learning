#include "Character.h"
#include "Soldier.h"
static	int	totalHpForSoldier[15] = {445,465,485,505,525,545,565,585,605,625,645,665,685,705,725};//各等级总的HP
static	int	totalAtkForSoldier[15] = {12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};//各等级总的Atk
static	int	totalDefForSoldier[15] = {0};//各等级总的Def

Soldier::Soldier(int i)
{
	roadNum = i;

	timeForAttack = -1.0f;
	timeForComeBackToLife = 30.0f;

	attackRadius = 1000;
	sightRadius = 1500;

	hp = totalHpForSoldier[level - 1];
	exp = 0;
	atk = totalAtkForSoldier[level - 1];
	def = totalDefForSoldier[level -1];

}

Soldier::~Soldier()
{

}



void Soldier::Init(LPDIRECT3DDEVICE9	p_d3dDevice, DInputClass * p_Input , TerrainClass * p_Terrain , CameraClass * p_Camera , std::vector<Character * > * p_Character)
{
	if(rand()%2 == 0)
		InitByName(p_d3dDevice ,p_Input,p_Terrain,p_Camera ,p_Character, L"GameMedia\\tiny_4anim - 副本.x","Walk","Jog" ,"Loiter" ,"Wave",-D3DX_PI / 2.0f , D3DX_PI / 2.0f, 0.0f , 1.5f);
	else 
		InitByName(p_d3dDevice ,p_Input,p_Terrain,p_Camera,p_Character, L"GameMedia\\moshou11.X","walk","run" ,"ready" ,"magicAttack",-D3DX_PI / 2.0f , 0.0f , 0.0f ,11.0f);

	if(team == Red)
	{
		switch(roadNum)
		{
		case 0:
			position = road0[0];
			targetPosition = road0[1];
			break;
		case 1:
			position = road1[0];
			targetPosition = road1[1];
			break;
		case 2:
			position = road2[0];
			targetPosition = road2[1];
			break;
		}
	}
	else
	{
		switch(roadNum)
		{
		case 0:
			position = road0[2];
			targetPosition = road0[1];
			break;
		case 1:
			position = road1[1];
			targetPosition = road1[0];
			break;
		case 2:
			position = road2[2];
			targetPosition = road2[1];
			break;
		}
	}
}

void Soldier::Control(float f_TimeDelta)
{
	fTimeDelta = f_TimeDelta;	

	atk = totalAtkForSoldier[level - 1];
	def = totalDefForSoldier[level -1];

	//到了一个中转站
	if(D3DXVec3Length(&(targetPosition - position)) < 2 * modalRadius)
	{
		if(team == Red)
		{
			switch(roadNum)
			{
			case 0:
				targetPosition = road0[2];
				break;
			case 1:
				targetPosition = road1[1];
				break;
			case 2:
				targetPosition = road2[2];
				break;
			}
		}
		else
		{
			switch(roadNum)
			{
			case 0:
				targetPosition = road0[0];
				break;
			case 1:
				targetPosition = road1[0];
				break;
			case 2:
				targetPosition = road2[0];
				break;
			}
		}
	}


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

	D3DXVECTOR3 vDiff;
	if(tempTargetPosition == targetPosition)
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

	Walk();
}

void Soldier::Update()
{
	if(timeForAttack > 0.0f)
		timeForAttack -= fTimeDelta;

	if(!alive)
	{
		timeForComeBackToLife -= fTimeDelta;
		if(timeForComeBackToLife <= 0.0f)
		{
			alive = true;
			hp = totalHpForSoldier[level - 1];

			if(team == Red)
			{
				switch(roadNum)
				{
				case 0:
					position = road0[0];
					targetPosition = road0[1];
					break;
				case 1:
					position = road1[0];
					targetPosition = road1[1];
					break;
				case 2:
					position = road2[0];
					targetPosition = road2[1];
					break;
				}
			}
			else
			{
				switch(roadNum)
				{
				case 0:
					position = road0[2];
					targetPosition = road0[1];
					break;
				case 1:
					position = road1[1];
					targetPosition = road1[0];
					break;
				case 2:
					position = road2[2];
					targetPosition = road2[1];
					break;
				}
			}

		}
	}

	//设置HP的长度
	CUSTOMVERTEX *pVertices = NULL;
	HPBackVBuffer ->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = CUSTOMVERTEX(-totalHpForSoldier[level - 1] / 2, 0.0f, 0.0f, 0.0f, 1.0f);
	pVertices[1] = CUSTOMVERTEX(-totalHpForSoldier[level - 1] / 2, 40.0f, 0.0f, 0.0f, 0.0f); 
	pVertices[2] = CUSTOMVERTEX( totalHpForSoldier[level - 1] / 2, 0.0f, 0.0f, 1.0f, 1.0f); 
	pVertices[3] = CUSTOMVERTEX( totalHpForSoldier[level - 1] / 2, 40.0f, 0.0f, 1.0f, 0.0f);
	HPVBuffer ->Unlock();


	HPVBuffer ->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = CUSTOMVERTEX(-totalHpForSoldier[level - 1] / 2 , 0.0f, 0.0f, 0.0f, 1.0f);
	pVertices[1] = CUSTOMVERTEX(-totalHpForSoldier[level - 1] / 2 , 40.0f, 0.0f, 0.0f, 0.0f); 
	pVertices[2] = CUSTOMVERTEX( hp - totalHpForSoldier[level - 1] / 2 , 0.0f, 0.0f, 1.0f, 1.0f); 
	pVertices[3] = CUSTOMVERTEX( hp - totalHpForSoldier[level - 1] / 2 , 40.0f, 0.0f, 1.0f, 0.0f);
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


void Soldier::Draw()
{
	if(!alive)
		return;
	Character::Draw();

}