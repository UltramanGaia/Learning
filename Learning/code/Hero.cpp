#include "Character.h"
#include "Hero.h"
int	totalHpForHero[15] =  {616,700,784,869,953,1037,1121,1205,1289,1373,1457,1541,1625,1709,1793};//各等级总的HP
static	int	totalAtkForHero[15] = {57,62,67,72,77,82,87,92,97,102,107,112,117,122,127};//各等级总的Atk
static	int	totalDefForHero[15] = {0};//各等级总的Def
Hero::Hero(int modal)
{
	heroModal = modal;
	timeForAttack = -1.0f;
	timeForRun = -0.5f;
	timeForComeBackToLife = 30.0f;
	attackRadius = 1000;
	sightRadius = 1500;

	hp = totalHpForHero[level - 1];
	exp = 0;
	atk = totalAtkForHero[level - 1];
	def = totalDefForHero[level -1];
}

Hero::~Hero()
{
}



void Hero::Init(LPDIRECT3DDEVICE9	p_d3dDevice, Input * p_Input , Terrain * p_Terrain ,Camera * p_Camera, std::vector<Character * > * p_Character)
{
	if(heroModal == TINY)
		InitByName(p_d3dDevice ,p_Input,p_Terrain,p_Camera ,p_Character, L"GameMedia\\tiny_4anim - 副本.x","Walk","Jog" ,"Loiter" ,"Wave",-D3DX_PI / 2.0f , D3DX_PI / 2.0f, 0.0f , 2.0f);
	else 
		InitByName(p_d3dDevice ,p_Input,p_Terrain,p_Camera,p_Character, L"GameMedia\\moshou11.X","walk","run" ,"ready" ,"magicAttack",-D3DX_PI / 2.0f , 0.0f , 0.0f ,15.0f);
}


void Hero::Control(float f_TimeDelta)
{
	fTimeDelta = f_TimeDelta;

	atk = totalAtkForHero[level - 1];
	def = totalDefForHero[level -1];


	if(!alive)
		return;


	if(userControl)
		UserControl();
	else
		AutoControl();

	//	SmoothIdle();

}

void Hero::UserControl()
{
	if (pInput->IsKeyDown(DIK_J)&&timeForAttack <= 0.0f)  
	{
		Attack();
		///
		//		FireBall * temp = new FireBall(pd3dDevice);
		//		temp->Init(team , id , 500 , position ,facing);
		//		g_vFireBall.push_back(temp); 
		///
		mciSendString(L"play Fight from 0",NULL,0,NULL);
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


		timeForAttack = 1.0f;
	}
	else if (pInput->IsKeyDown(DIK_W))
	{
		if((pInput->IsKeyDown(DIK_LSHIFT))&&timeForRun > -1.0f && timeForRun < 1.0f )
		{
			Run();
			timeForRun = -5.0f;
		}
		else if(timeForRun < -1.0f)
		{
			Run();
		}
		else
		{
			Walk();
		}
	}
	else
	{
		if(currentAnim == attack)
			Attack();
		else
			Idle();
	}

	if (pInput->IsKeyDown(DIK_A))
	{
		setFacing(facing + turnSpeed);

	}
	if (pInput->IsKeyDown(DIK_D))
	{
		setFacing(facing - turnSpeed);
	}



}

void Hero::AutoControl()
{
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
		///
		//		FireBall * temp = new FireBall(pd3dDevice);
		//		temp->Init(team , id , 500 , position ,facing);
		//		g_vFireBall.push_back(temp); 
		///
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



	/*	if(abs(facing - facingTarget) > 2 * turnSpeed)
	{
	if(facing > facingTarget)
	setFacing(facing - turnSpeed);
	else
	setFacing(facing + turnSpeed);
	}
	*/
	if(!canAttack)
		Walk();

}



void Hero::Update()
{
	if(timeForAttack > 0.0f)
		timeForAttack -= fTimeDelta;
	if(timeForRun > 1.0f)
		timeForRun -= fTimeDelta;
	if(timeForRun < -1.0f)
	{
		timeForRun += fTimeDelta;
		if(timeForRun >= -1.0f)
			timeForRun = 10.0f;
	}

	if(!alive)
	{
		timeForComeBackToLife -= fTimeDelta;
		if(timeForComeBackToLife <= 0.0f)
		{
			alive = true;
			hp = totalHpForHero[level - 1];
			if(team == Red)
			{
				setPosition(D3DXVECTOR3(-7565.6,0.0,-7190.5));
			}
			else
			{
				setPosition(D3DXVECTOR3(3516.8,0.0,3393.6));
			}

		}
	}

	//设置HP的长度
	CUSTOMVERTEX *pVertices = NULL;
	HPBackVBuffer ->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = CUSTOMVERTEX(-totalHpForHero[level - 1] / 2, 0.0f, 0.0f, 0.0f, 1.0f);
	pVertices[1] = CUSTOMVERTEX(-totalHpForHero[level - 1] / 2, 40.0f, 0.0f, 0.0f, 0.0f); 
	pVertices[2] = CUSTOMVERTEX( totalHpForHero[level - 1] / 2, 0.0f, 0.0f, 1.0f, 1.0f); 
	pVertices[3] = CUSTOMVERTEX( totalHpForHero[level - 1] / 2, 40.0f, 0.0f, 1.0f, 0.0f);
	HPVBuffer ->Unlock();


	HPVBuffer ->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = CUSTOMVERTEX(-totalHpForHero[level - 1] / 2 , 0.0f, 0.0f, 0.0f, 1.0f);
	pVertices[1] = CUSTOMVERTEX(-totalHpForHero[level - 1] / 2 , 40.0f, 0.0f, 0.0f, 0.0f); 
	pVertices[2] = CUSTOMVERTEX( hp - totalHpForHero[level - 1] / 2 , 0.0f, 0.0f, 1.0f, 1.0f); 
	pVertices[3] = CUSTOMVERTEX( hp - totalHpForHero[level - 1] / 2 , 40.0f, 0.0f, 1.0f, 0.0f);
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

void Hero::Draw()
{
	if(!alive)
		return;
	Character::Draw();
}