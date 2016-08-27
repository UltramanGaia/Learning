#include "Character.h"
#include "Tower.h"
static	int	totalHpForTower[15] = {4000,4200,4400,4600,4800,5000,5200,5400,5600,5800,6000,6200,6400,6600,6800};//���ȼ��ܵ�HP
static	int	totalAtkForTower[15] = {152,156,160,164,168,172,176,180,184,188,192,196,200,204,208};//���ȼ��ܵ�Atk
static	int	totalDefForTower[15] = {0};//���ȼ��ܵ�Def

Tower::Tower()
{
	timeForAttack = -1.0f;
	attackRadius = 2000;
	hp = totalHpForTower[level - 1];
	exp = 0;
	atk = totalAtkForTower[level - 1];
	def = totalDefForTower[level -1];
}

Tower::~Tower()
{

}



void Tower::Init(LPDIRECT3DDEVICE9	p_d3dDevice, Input * p_Input , Terrain * p_Terrain , Camera * p_Camera , std::vector<Character * > * p_Character)
{
	//InitByName(p_d3dDevice ,p_Input,p_Terrain,p_Camera,p_Character, L"tower3.X","idle","idle" ,"idle" ,"idle",-D3DX_PI / 2.0f ,D3DX_PI / 2.0f  , 0.0f ,2.5f);
	InitByName(p_d3dDevice ,p_Input,p_Terrain,p_Camera,p_Character, L"GameMedia\\tower11.X","idle","idle" ,"idle" ,"idle",-D3DX_PI / 2.0f ,D3DX_PI / 2.0f  , 0.0f ,0.1f);
}



void Tower::Control(float f_TimeDelta)
{
	fTimeDelta = f_TimeDelta;

	atk = totalAtkForTower[level - 1];
	def = totalDefForTower[level -1];

	if(!alive)
		return;

	bool canAttack  = false ;
	float lenCanAttack ;
	D3DXVECTOR3 tempTargetPosition = targetPosition;
	//�ܷ񹥻�,�ܷ�׷��
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
	}

	D3DXVECTOR3 vDiff;
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
	while(facingTarget > 2 * D3DX_PI)
		facingTarget -= 2 * D3DX_PI;
	while(facingTarget < 0)
		facingTarget += 2 * D3DX_PI;

	if(canAttack && timeForAttack <= 0.0f)
	{
		Attack();

		bool ok = false;
		for(int i = 0 ;  i < g_vFireBall.size() ; i ++)
		{
			if(g_vFireBall[i]->isDestroy)
			{
				g_vFireBall[i]->Init(team , id , 500.0f,position ,facingTarget);
				ok = true;
				break;
			}
		}
		if(!ok)
		{
			FireBall * temp = new FireBall(pd3dDevice);
			g_vFireBall.push_back(temp);

			g_vFireBall[g_vFireBall.size() - 1]->Init(team , id , 500.0f,position ,facingTarget);

		}

		timeForAttack = 3.0f;	
	}

}



void Tower::Update()
{
	if(timeForAttack > 0.0f)
		timeForAttack -= fTimeDelta;


	//����HP�ĳ���
	CUSTOMVERTEX *pVertices = NULL;
	HPBackVBuffer ->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = CUSTOMVERTEX(-totalHpForTower[level - 1] / 2, 0.0f, 0.0f, 0.0f, 1.0f);
	pVertices[1] = CUSTOMVERTEX(-totalHpForTower[level - 1] / 2, 40.0f, 0.0f, 0.0f, 0.0f); 
	pVertices[2] = CUSTOMVERTEX( totalHpForTower[level - 1] / 2, 0.0f, 0.0f, 1.0f, 1.0f); 
	pVertices[3] = CUSTOMVERTEX( totalHpForTower[level - 1] / 2, 40.0f, 0.0f, 1.0f, 0.0f);
	HPVBuffer ->Unlock();


	HPVBuffer ->Lock(0, 0, (void**)&pVertices, 0);
	pVertices[0] = CUSTOMVERTEX(-totalHpForTower[level - 1] / 2 , 0.0f, 0.0f, 0.0f, 1.0f);
	pVertices[1] = CUSTOMVERTEX(-totalHpForTower[level - 1] / 2 , 40.0f, 0.0f, 0.0f, 0.0f); 
	pVertices[2] = CUSTOMVERTEX( hp - totalHpForTower[level - 1] / 2 , 0.0f, 0.0f, 1.0f, 1.0f); 
	pVertices[3] = CUSTOMVERTEX( hp - totalHpForTower[level - 1] / 2 , 40.0f, 0.0f, 1.0f, 0.0f);
	HPVBuffer ->Unlock();


	///
	///pd3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);				//�رչ���
	///

	// ���ù��������ľ���
	D3DXMATRIX matFinal, mx;
	D3DXMatrixIdentity(&matFinal);
	//����λ�úͳ������任���� 
	D3DXMatrixRotationY( &matFinal, -facing );
	D3DXMatrixTranslation( &mx, position.x, position.y, position.z );
	D3DXMatrixMultiply( &matFinal, &matFinal, &mx );
	D3DXMatrixMultiply( &matFinal, &modelOrientation, &matFinal );


	pd3dDevice->SetTransform(D3DTS_WORLD, &matFinal);

	// ���¹�������
	pAnimController->AdvanceTime(fTimeDelta, NULL);	//���ù���������ʱ��

	UpdateFrameMatrices(pFrameRoot, &matFinal);		//���¿���еı任����

	///
	///pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);					//�򿪹���
	///


	//����ʱ��
	prvTime = curTime;
	curTime = prvTime + fTimeDelta ; 

}




void Tower::Draw()
{

	if(!alive)
		return;
	Character::Draw();
}