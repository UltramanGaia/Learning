#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <time.h> 
//#include "CLD3D.h"
#include <stdio.h>
#include "D3DUtil.h"
#include <Windows.h>
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "dinput8.lib")  
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "winmm.lib") 


//常量
const wchar_t wndName[50] = L"蒙皮骨骼动画演示";
const wchar_t wndTitle[100] = L"【Direct3D】——蒙皮骨骼动画演示-coderling";
//const wchar_t xFileName[20] = L"tiny.x";
const wchar_t xFileName[20] = L"tiny_4anim.x";
const unsigned int wndWidth = 800;
const unsigned int wndHeight = 600;

//为每个骨骼添加一个控制矩阵
struct D3DXFRAME_EX: public D3DXFRAME
{
	D3DXMATRIXA16 combinedTransformtionMatrix; 
};

//保存蒙皮骨骼动画的结构体，包含网格模型
struct D3DXMESHCONTAINER_EX: public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9* ppTextures; //指向纹理信息指针的指针数组

	//蒙皮骨骼信息
	LPD3DXMESH pOrigMesh;
	LPD3DXATTRIBUTERANGE pAttributeTable; //子集分布表格？
	DWORD numAttributeGroups;
	DWORD numInfl;
	LPD3DXBUFFER pBoneCombinatonBuf; //骨骼名字缓冲区
	D3DXMATRIX** ppBoneMatrixPtrs; //最终变换矩阵
	D3DXMATRIX* pBoneOffsetMatrices; //offset矩阵数组指针
	DWORD numPaletteEntries;
	//bool useSoftwareVP;


};

HRESULT GenerateSkinnedMesh( D3DXMESHCONTAINER_EX* );

//重写ID3DXAllocateHierarchy接口
class CAllocateHierarchy: public ID3DXAllocateHierarchy
{
public:
	STDMETHOD(CreateFrame)( THIS_ LPCSTR name, LPD3DXFRAME* ppNewFrame );
	STDMETHOD(CreateMeshContainer)( THIS_
		LPCSTR Name,
		CONST D3DXMESHDATA* pMeshData,
		CONST D3DXMATERIAL* pMaterials,
		CONST D3DXEFFECTINSTANCE* pEffectInstance,
		DWORD numMaterials,
		CONST DWORD* pAdjacency,
		LPD3DXSKININFO pSkinInfo,
		LPD3DXMESHCONTAINER* ppNewMeshContainer );
	STDMETHOD(DestroyFrame)( THIS_ LPD3DXFRAME pFrameToFree );
	STDMETHOD(DestroyMeshContainer)( THIS_ LPD3DXMESHCONTAINER pMeshContainer );

};

//创建新的帧
HRESULT CAllocateHierarchy::CreateFrame( LPCSTR name, LPD3DXFRAME* ppNewFrame )
{
	HRESULT hr = S_OK;

	D3DXFRAME_EX *pFrame;

	*ppNewFrame = NULL;
	
	pFrame = new D3DXFRAME_EX;
	if( pFrame == NULL )
	{
		hr = E_OUTOFMEMORY;
		return hr;
	}

	if( name )
	{
		unsigned int len = strlen( name ) + 1;
		pFrame->Name = new char[len];
		memcpy( pFrame->Name, name, sizeof(char)*len );
	}

	//初始化frame
	D3DXMatrixIdentity( &pFrame->TransformationMatrix );
	D3DXMatrixIdentity( &pFrame->combinedTransformtionMatrix );

	pFrame->pMeshContainer = NULL;
	pFrame->pFrameFirstChild = NULL;
	pFrame->pFrameSibling = NULL;

	*ppNewFrame = pFrame; //返回

	pFrame = NULL;

	return hr;
}


//创建新的meshcontainner
HRESULT CAllocateHierarchy::CreateMeshContainer(
	LPCSTR name,
	CONST D3DXMESHDATA* pMeshData,
	CONST D3DXMATERIAL* pMaterials,
	CONST D3DXEFFECTINSTANCE* pEffectInstances,
	DWORD numMaterials,
	CONST DWORD* pAdjacency,
	LPD3DXSKININFO pSkinInfo,
	LPD3DXMESHCONTAINER* ppNewMeshContainer )
{
	D3DXMESHCONTAINER_EX* pMeshContainer = NULL;
	unsigned int numFaces;
	unsigned int iMaterial;
	unsigned int iBone, cBones;
	LPDIRECT3DDEVICE9 pD3DDevice = NULL;

	LPD3DXMESH pMesh = NULL;
	
	*ppNewMeshContainer = NULL;

	if( pMeshData->Type != D3DXMESHTYPE_MESH )
		return E_FAIL;

	pMesh = pMeshData->pMesh;

	if( pMesh->GetFVF() == 0 )
		return E_FAIL;

	pMeshContainer = new D3DXMESHCONTAINER_EX;
	if( pMeshContainer == NULL )
		return E_FAIL;
	memset( pMeshContainer, 0, sizeof(D3DXMESHCONTAINER_EX) );

	if( name )
	{
		unsigned int len = strlen( name ) + 1;
		pMeshContainer->Name = new char[len];
		memcpy( pMeshContainer->Name, name, sizeof(char)*len );
	}

	pMesh->GetDevice( &pD3DDevice );
	numFaces = pMesh->GetNumFaces();

	//添加顶点法线
	if( !( pMesh->GetFVF() & D3DFVF_NORMAL ) )
	{
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		HRESULT hr = pMesh->CloneMeshFVF( pMesh->GetOptions(), pMesh->GetFVF() | D3DFVF_NORMAL,
										  pD3DDevice, &pMeshContainer->MeshData.pMesh );
		if( FAILED( hr ) )
			return E_FAIL;

		pMesh = pMeshContainer->MeshData.pMesh;

		D3DXComputeNormals( pMesh, NULL );

	}
	else
	{
		pMeshContainer->MeshData.pMesh = pMesh;
		pMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

		pMesh->AddRef(); //增加引用计数
	}

	pMeshContainer->NumMaterials = max( 1, numMaterials );
	pMeshContainer->pMaterials = new D3DXMATERIAL[pMeshContainer->NumMaterials];
	pMeshContainer->ppTextures = new LPDIRECT3DTEXTURE9[pMeshContainer->NumMaterials];
	pMeshContainer->pAdjacency = new DWORD[numFaces*3];
	if( (pMeshContainer->pAdjacency == NULL ) || ( pMeshContainer->pMaterials == NULL ) )
		return E_FAIL;

	memcpy( pMeshContainer->pAdjacency, pAdjacency, sizeof(DWORD) * numFaces*3 );
	memset( pMeshContainer->ppTextures, 0, sizeof(LPDIRECT3DTEXTURE9) * pMeshContainer->NumMaterials );

	if( numMaterials > 0 )
	{
		memcpy( pMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pMeshContainer->NumMaterials );
		
		for( iMaterial = 0; iMaterial < numMaterials; iMaterial++ )
		{
			if( pMeshContainer->pMaterials[iMaterial].pTextureFilename != NULL )
			{
				HRESULT hr = D3DXCreateTextureFromFileA( pD3DDevice,
								pMeshContainer->pMaterials[iMaterial].pTextureFilename,
								&pMeshContainer->ppTextures[iMaterial] );
				if( FAILED( hr ) )
					pMeshContainer->ppTextures[iMaterial] = NULL;
			}
		}
	}
	else
	{//默认
		pMeshContainer->pMaterials[0].pTextureFilename = NULL;
		memset( &pMeshContainer->pMaterials[0].MatD3D, 0, sizeof(D3DMATERIAL9) );
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.r = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.g = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Diffuse.b = 0.5f;
		pMeshContainer->pMaterials[0].MatD3D.Specular = pMeshContainer->pMaterials[0].MatD3D.Diffuse;
	}

	if( pSkinInfo != NULL )
	{
		pMeshContainer->pSkinInfo = pSkinInfo;
		pSkinInfo->AddRef();

		pMeshContainer->pOrigMesh = pMesh;
		pMesh->AddRef();

		//
		cBones = pSkinInfo->GetNumBones();
		pMeshContainer->pBoneOffsetMatrices = new D3DXMATRIX[cBones];
		if( pMeshContainer->pBoneOffsetMatrices == NULL )
			return E_FAIL;

		//获得offsetmatrix
		for( iBone = 0; iBone < cBones; iBone++ )
		{
			pMeshContainer->pBoneOffsetMatrices[iBone] = *( pMeshContainer->pSkinInfo->GetBoneOffsetMatrix( iBone ) );
		}

		HRESULT hr = GenerateSkinnedMesh( pMeshContainer );
		if( FAILED( hr ) )
			return E_FAIL;
	}

	*ppNewMeshContainer = pMeshContainer;

	//SAFE_RELEASE( pD3DDevice );

	//if( pMeshContainer != NULL )
		//DestroyMeshContainer( pMeshContainer );

	return S_OK;
}

//销毁Frame
HRESULT CAllocateHierarchy::DestroyFrame( LPD3DXFRAME pFrameToFree )
{
	SAFE_DELETE_ARRAY( pFrameToFree->Name );
	SAFE_DELETE( pFrameToFree );

	return S_OK;
}

//销毁container
HRESULT CAllocateHierarchy::DestroyMeshContainer( LPD3DXMESHCONTAINER pMeshContainerBase )
{
	unsigned int iMaterial;
	D3DXMESHCONTAINER_EX* pMeshContainer = (D3DXMESHCONTAINER_EX*)pMeshContainerBase;

	SAFE_DELETE_ARRAY( pMeshContainer->Name );
	SAFE_DELETE_ARRAY( pMeshContainer->pAdjacency );
	SAFE_DELETE_ARRAY( pMeshContainer->pMaterials );
	SAFE_DELETE_ARRAY( pMeshContainer->pBoneOffsetMatrices );

	if( pMeshContainer->ppTextures != NULL )
	{
		for( iMaterial = 0; iMaterial< pMeshContainer->NumMaterials; iMaterial++ )
		{
			SAFE_RELEASE( pMeshContainer->ppTextures[iMaterial] );
		}
	}

	SAFE_DELETE_ARRAY( pMeshContainer->ppTextures );
	SAFE_DELETE_ARRAY( pMeshContainer->ppBoneMatrixPtrs );
	SAFE_RELEASE( pMeshContainer->pBoneCombinatonBuf );
	SAFE_RELEASE( pMeshContainer->pSkinInfo );
	SAFE_RELEASE( pMeshContainer->pOrigMesh );
	SAFE_DELETE( pMeshContainer );

	return S_OK;
}


//---------------我是分割线---------------------------------------------------------------------------------------------

//全局变量
LPDIRECT3D9 g_pD3D = NULL; //direct3d对象用于创建direct设备
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL; //direct3d设备指针
LPD3DXFONT g_pText = NULL;
LPD3DXFRAME g_pFrameRoot = NULL; //骨骼的根
LPD3DXANIMATIONCONTROLLER g_pAnimController; //动画控制句柄
double g_startTime = 0.0;
wchar_t* g_pAdapter = NULL;
unsigned int g_numBoneMatricesMax = 0;
D3DXMATRIXA16* g_pBoneMatrices = NULL;

//相关函数声明
LRESULT WINAPI MsgProc( HWND, UINT, WPARAM, LPARAM );
HRESULT InitApp( HWND, HINSTANCE );
HRESULT InitGeometry(); //场景数据
VOID InitTranformMatrix(); //初始变换
HRESULT InitText(); //字体接口
VOID Render( HWND ); //渲染
VOID RenderText( HWND ); //渲染提示信息
VOID UpdateScene( HWND ); //更新场景
VOID UpdateFrameMatrices( const LPD3DXFRAME, const LPD3DXMATRIX ); //为个骨骼更新变换矩阵，每帧
VOID InitLight(); //设置光源
VOID Cleanup(); 
HRESULT SetupBoneMatrixPointers( LPD3DXFRAME ); //设置骨骼链中的混合数组到container中，方便访问
HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER ); //
VOID DrawFrame( LPD3DXFRAME ); //绘制骨骼
VOID DrawMeshContainer( const LPD3DXMESHCONTAINER, const LPD3DXFRAME ); //绘制单个pMeshContainer



float								Get_FPS();															//计算帧速率函数

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE preInst, LPSTR lpCmdLine, int nCmdShow )
{
//设计并注册窗口类
	WNDCLASSEX wcEx = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
						GetModuleHandle( NULL ), NULL, NULL, NULL, NULL,
						wndName, NULL };
	RegisterClassEx( &wcEx );

	//创建窗口
	HWND hWnd = CreateWindow( wndName, wndTitle, WS_OVERLAPPEDWINDOW,
							  100, 100, wndWidth, wndHeight, GetDesktopWindow(),
							  NULL, hInst, NULL );

	if( SUCCEEDED( InitApp( hWnd, hInst ) ) )
	{
		if( SUCCEEDED( InitGeometry() ) )
		{
			//显示并更新窗口
			ShowWindow( hWnd, SW_SHOWDEFAULT );
			UpdateWindow( hWnd );

			//程序消息循环
			MSG msg;
			SecureZeroMemory( &msg, sizeof(msg) );
			while( msg.message != WM_QUIT )
			{
				if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
				else
					Render( hWnd );
			}
		}
	}

	UnregisterClass( wndName, wcEx.hInstance );
	return 0;
}

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
			DestroyWindow( hWnd );
		break;
	case WM_DESTROY:
		Cleanup();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return S_OK;
}

HRESULT InitApp( HWND hWnd , HINSTANCE hInst )
{
	//创建direct3d对象
	if( FAILED( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION )) )
		return E_FAIL;

	//填写Direct3dDevice配置参数
	D3DPRESENT_PARAMETERS d3dpp;
	SecureZeroMemory( &d3dpp, sizeof(d3dpp) );
	d3dpp.BackBufferWidth = wndWidth;
	d3dpp.BackBufferHeight = wndHeight;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.BackBufferCount = 1; //缓冲数
	d3dpp.hDeviceWindow = hWnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true; //开启深度缓冲
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//检测硬件
	D3DCAPS9 caps;
	g_pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps );
	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	//创建Direct3d设备
	if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
				hWnd, vp, &d3dpp, &g_pD3DDevice ) ) )
	{
		return E_FAIL;
	}

	InitText();

	g_startTime = (double)timeGetTime();

	return S_OK;
}

HRESULT InitGeometry()
{
	HRESULT hr;
	CAllocateHierarchy alloc;

	hr = D3DXLoadMeshHierarchyFromX( xFileName, D3DXMESH_MANAGED, g_pD3DDevice, &alloc, NULL, &g_pFrameRoot, &g_pAnimController );
	if( FAILED( hr ) )
		return E_FAIL;
	
	//设置骨骼链中的混合矩阵到Container中，方便访问
	hr = SetupBoneMatrixPointers( g_pFrameRoot );

	InitLight();
	
	InitTranformMatrix();
	g_pD3DDevice->SetRenderState( D3DRS_NORMALIZENORMALS, true ); //标准化顶点法线
	//g_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, true ); //开启镜面光

	//显卡信息
	wchar_t adapterTemp[200] = L"显卡型号：";
	D3DADAPTER_IDENTIFIER9 adapter; //获取显卡信息的结构体
	g_pAdapter = new wchar_t[200];
	g_pD3D->GetAdapterIdentifier( 0, 0, &adapter ); //获取显卡信息，保存在adapter中

	//将char*转化为wchar_t*
	int len = MultiByteToWideChar( CP_ACP, 0, adapter.Description, -1, NULL, 0 );
	MultiByteToWideChar( CP_ACP, 0, adapter.Description, -1, g_pAdapter, len );
	wcscat_s(adapterTemp,g_pAdapter ); //链接两个字符串
	memcpy( g_pAdapter, adapterTemp, sizeof(adapterTemp) ); //从新拷贝回g_pAdapter


	return S_OK;
}

VOID InitTranformMatrix()
{
	D3DXMATRIX matWorld;
	D3DXMatrixIdentity( &matWorld );
	g_pD3DDevice->SetTransform( D3DTS_WORLD, &matWorld );

	// 摄像机
	D3DXVECTOR3 pos(100.0f, 100.0f, -250.0f);
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	//g_camera.ReSetting( &pos, &target, &up );

	//D3DXVECTOR3 right( 1.0f, 0.0f, 0.0f);
	D3DXMATRIX V;
	//g_camera.GetViewMatrix( &V );
	D3DXMatrixLookAtLH(
		&V,
		&pos,
		&target,
		&up);
	g_pD3DDevice->SetTransform(D3DTS_VIEW, &V);
	
	// 设置投影变换矩阵.
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
			&proj,
			D3DX_PI * 0.5f, 
			(float)wndWidth/ (float)wndHeight,
			1.0f,
			1000.0f);
	g_pD3DDevice->SetTransform(D3DTS_PROJECTION, &proj);

	//视口剪切矩阵，实现视口变换
	D3DVIEWPORT9 vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = wndWidth;
	vp.Height = wndHeight;
	vp.MinZ = 0;
	vp.MaxZ = 1;
	g_pD3DDevice->SetViewport( &vp );
}

HRESULT InitText()
{
	HRESULT hr;
	//创建字体
	hr = D3DXCreateFont( g_pD3DDevice, 25, 0, 1000, 0, false,
					DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
					DEFAULT_QUALITY, 0, L"微软雅黑", &g_pText );

	return hr;
}

VOID Render( HWND hWnd )
{
	if( NULL == g_pD3DDevice )
		return ;
	//清屏
	g_pD3DDevice->Clear( 0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						 D3DCOLOR_XRGB( 28, 154, 207 ), 1.0f, 0 );

	if( SUCCEEDED( g_pD3DDevice->BeginScene() ) )
	{
		UpdateScene( hWnd );
		DrawFrame( g_pFrameRoot ); //绘制骨骼动画

		RenderText( hWnd );
		g_pD3DDevice->EndScene();
	}

	g_pD3DDevice->Present( NULL, NULL, NULL, NULL );
	//::Sleep(10000);
}

//递归绘制骨骼动画
VOID DrawFrame( LPD3DXFRAME pFrame )
{
	LPD3DXMESHCONTAINER pMeshContainer;

	pMeshContainer = pFrame->pMeshContainer;
	while( pMeshContainer != NULL )
	{
		DrawMeshContainer( pMeshContainer, pFrame );
		pMeshContainer = pMeshContainer->pNextMeshContainer;
	}

	if( pFrame->pFrameSibling != NULL )
	{
		DrawFrame( pFrame->pFrameSibling );
	}

	if( pFrame->pFrameFirstChild != NULL )
	{
		DrawFrame( pFrame->pFrameFirstChild );
	}
}

//绘制单个pMeshContainer
VOID DrawMeshContainer( const LPD3DXMESHCONTAINER pMeshContainerBase, const LPD3DXFRAME pFrameBase )
{
	D3DXMESHCONTAINER_EX* pMeshContainer = (D3DXMESHCONTAINER_EX*)pMeshContainerBase;
	D3DXFRAME_EX* pFrame = (D3DXFRAME_EX*)pFrameBase;
	unsigned int iMaterial;
	//unsigned int numBlend;
	unsigned int iAttrib;
	//DWORD attribIdPrev;
	//LPD3DXBONECOMBINATION pBoneComb;

	//unsigned int iMatrixIndex;
	//unsigned int iPaletteEntry;
	D3DXMATRIXA16 matTemp;

	
	if( pMeshContainer->pSkinInfo != NULL )
	{
		//只采用软件处理方式
		//D3DXMATRIX Identity;
		DWORD cBones = pMeshContainer->pSkinInfo->GetNumBones();
		DWORD iBone;
		PBYTE pbVerticesSrc;
		PBYTE pbVerticesDest;

		//D3DXMATRIX* pBoneMatrices = new D3DXMATRIX[cBones];
		for( iBone = 0; iBone < cBones; ++iBone )
		{
			D3DXMatrixMultiply(
				&g_pBoneMatrices[iBone],
				&pMeshContainer->pBoneOffsetMatrices[iBone],
				pMeshContainer->ppBoneMatrixPtrs[iBone] 
			);
		}

		pMeshContainer->pOrigMesh->LockVertexBuffer( D3DLOCK_READONLY, (LPVOID*)&pbVerticesSrc );
		pMeshContainer->MeshData.pMesh->LockVertexBuffer( 0, (LPVOID*)&pbVerticesDest );

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh( g_pBoneMatrices, NULL, pbVerticesSrc, pbVerticesDest );

		//delete[] pBoneMatrices;
		//pBoneMatrices = NULL;

		pMeshContainer->pOrigMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
		
		for( iAttrib = 0; iAttrib < pMeshContainer->numAttributeGroups; iAttrib++ )
		{
			g_pD3DDevice->SetMaterial( &( pMeshContainer->pMaterials[pMeshContainer->pAttributeTable[iAttrib].AttribId].MatD3D ) );
			g_pD3DDevice->SetTexture( 0, pMeshContainer->ppTextures[pMeshContainer->pAttributeTable[iAttrib].AttribId]);
			pMeshContainer->MeshData.pMesh->DrawSubset( pMeshContainer->pAttributeTable[iAttrib].AttribId);
		}
	}
	else
	{
		for( iMaterial = 0; iMaterial < pMeshContainer->NumMaterials; iMaterial++ )
		{
			g_pD3DDevice->SetMaterial( &pMeshContainer->pMaterials[iMaterial].MatD3D );
			g_pD3DDevice->SetTexture( 0, pMeshContainer->ppTextures[iMaterial] );
			pMeshContainer->MeshData.pMesh->DrawSubset( iMaterial );
		}
	}
}

VOID RenderText( HWND hWnd )
{
	RECT clientRect;
	GetClientRect( hWnd, &clientRect );

	//-绘制显卡信息
	g_pText->DrawText( NULL, g_pAdapter, -1, &clientRect,
					DT_TOP | DT_LEFT, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );

	//绘制FPS
	wchar_t strFPS[20];
	int len = swprintf_s(strFPS, 20, L"FPS：%.3f", Get_FPS());
	g_pText->DrawText( NULL, strFPS, len, &clientRect, DT_TOP | DT_RIGHT,
					D3DXCOLOR( 0.0f, 1.0f, 0.0f, 1.0f ) );

}


//-----------------------------------【Get_FPS( )函数】------------------------------------------
//	描述：获取帧速率
//--------------------------------------------------------------------------------------------------
float Get_FPS()
{

	static float  fps = 0;
	static int    frameCount = 0;//帧数
	static float  currentTime =0.0f;//当前时间
	static float  lastTime = 0.0f;//上次时间

	frameCount++;
	currentTime = timeGetTime()*0.001f;

	if(currentTime - lastTime > 1.0f) //将时间控制在1秒钟
	{
		fps = (float)frameCount /(currentTime - lastTime);//计算这1秒钟的FPS值
		lastTime = currentTime;
		frameCount    = 0;
	}

	return fps;
}


VOID UpdateScene( HWND hWnd )
{
	if( g_pAnimController != NULL )
	{
		double time = (double)timeGetTime();
		time = ( time - g_startTime ) * 0.001;
		g_startTime = (double)timeGetTime();
		g_pAnimController->AdvanceTime( time, NULL );
	}

	D3DXMATRIX matWorld;
	//D3DXMatrixIdentity( &matWorld );
	g_pD3DDevice->GetTransform( D3DTS_WORLD, &matWorld );
	UpdateFrameMatrices( g_pFrameRoot, &matWorld );
}
	
	

VOID UpdateFrameMatrices( const LPD3DXFRAME pFrameBase, const LPD3DXMATRIX pParentMatrix )
{
	D3DXFRAME_EX* pFrame = (D3DXFRAME_EX*)pFrameBase;

	if( pParentMatrix != NULL )
		D3DXMatrixMultiply( &pFrame->combinedTransformtionMatrix, &pFrame->TransformationMatrix, pParentMatrix );
	else
		pFrame->combinedTransformtionMatrix = pFrame->TransformationMatrix;
	
	if( pFrame->pFrameSibling != NULL )
	{
		UpdateFrameMatrices( pFrame->pFrameSibling, pParentMatrix );
	}

	if( pFrame->pFrameFirstChild != NULL )
	{
		UpdateFrameMatrices( pFrame->pFrameFirstChild, &pFrame->combinedTransformtionMatrix );
	}
}

VOID InitLight()
{
		//------------------------------
	//-1、填写D3DLIGHT9结构体
	//-2、SetLinght
	//-3、LightEnable
	//-----------------------------
	
	//填写光源结构体
	D3DLIGHT9 pointLight;
	SecureZeroMemory( &pointLight, sizeof(pointLight) );
	D3DXCOLOR white = D3DCOLOR_RGBA( 255, 255, 255, 255 );
	D3DXCOLOR black = D3DCOLOR_RGBA( 0, 0, 0, 0 );
	pointLight.Type = D3DLIGHT_POINT;
	pointLight.Ambient = white*0.4f;
	pointLight.Diffuse = white;
	pointLight.Specular = white*0.6f;
	pointLight.Position = D3DXVECTOR3( 200.0f, 200.0f, 0.0f ); //根据需要进行更改
	pointLight.Range = 500.0f;
	pointLight.Attenuation0 = 1.0f;
	pointLight.Attenuation1 = 0.0f;
	pointLight.Attenuation2 = 0.0f;

	g_pD3DDevice->SetLight( 0, &pointLight );
	g_pD3DDevice->LightEnable( 0, true );
}

VOID Cleanup()
{
	SAFE_DELETE( g_pAdapter );
	SAFE_RELEASE( g_pD3D );
	SAFE_RELEASE( g_pD3DDevice );
	SAFE_RELEASE( g_pText );
	SAFE_RELEASE( g_pAnimController );
	

	if( g_pFrameRoot != NULL )
	{
		CAllocateHierarchy alloc;
		D3DXFrameDestroy( g_pFrameRoot, &alloc );
		g_pFrameRoot = NULL;
	}
}

//递归设置骨骼链的混合数组
HRESULT SetupBoneMatrixPointers( LPD3DXFRAME pFrame )
{
	HRESULT hr;
	if( pFrame->pMeshContainer != NULL )
	{
		hr = SetupBoneMatrixPointersOnMesh( pFrame->pMeshContainer );
		if( FAILED( hr ) )
			return hr;
	}

	if( pFrame->pFrameSibling != NULL )
	{
		hr = SetupBoneMatrixPointers( pFrame->pFrameSibling );
		if( FAILED( hr ) )
			return hr;
	}

	if( pFrame->pFrameFirstChild != NULL )
	{
		hr = SetupBoneMatrixPointers( pFrame->pFrameFirstChild );
		if( FAILED( hr ) )
			return hr;
	}

	return S_OK;
}

HRESULT SetupBoneMatrixPointersOnMesh( LPD3DXMESHCONTAINER pMeshContainerBase )
{
	unsigned int iBone;
	unsigned int cBones;
	D3DXFRAME_EX* pFrame;

	D3DXMESHCONTAINER_EX* pMeshContainer = ( D3DXMESHCONTAINER_EX* )pMeshContainerBase;
	
	//如果是骨骼动画
	if( pMeshContainer->pSkinInfo != NULL )
	{
		cBones = pMeshContainer->pSkinInfo->GetNumBones();

		pMeshContainer->ppBoneMatrixPtrs = new D3DXMATRIX*[cBones];
		if( pMeshContainer->ppBoneMatrixPtrs == NULL )
			return E_OUTOFMEMORY;

		for( iBone = 0; iBone < cBones; iBone++ )
		{
			//根据骨骼名字寻找对应骨骼
			pFrame = (D3DXFRAME_EX*)D3DXFrameFind( g_pFrameRoot, pMeshContainer->pSkinInfo->GetBoneName( iBone ) );
			if( pFrame == NULL )
				return E_FAIL;

			pMeshContainer->ppBoneMatrixPtrs[iBone] = &pFrame->combinedTransformtionMatrix;
		}
	}

	//--------
	/*HRESULT hr = pMeshContainer->MeshData.pMesh->GetAttributeTable( NULL, &pMeshContainer->numAttributeGroups );
	if( FAILED( hr ) )
		return E_FAIL;
	pMeshContainer->pAttributeTable = new D3DXATTRIBUTERANGE[pMeshContainer->numAttributeGroups];
	if( pMeshContainer->pAttributeTable == NULL )
		return E_FAIL;
	hr = pMeshContainer->MeshData.pMesh->GetAttributeTable( pMeshContainer->pAttributeTable, NULL );
	if( FAILED( hr ) )
		return E_FAIL;*/


	return S_OK;
}

//在SkinMesh加载或者改变渲染方式时是调用
HRESULT GenerateSkinnedMesh( D3DXMESHCONTAINER_EX* pMeshContainer )
{
	HRESULT hr = S_OK;

	if( pMeshContainer->pSkinInfo == NULL )
		return hr;
	SAFE_RELEASE( pMeshContainer->MeshData.pMesh );
	SAFE_RELEASE( pMeshContainer->pBoneCombinatonBuf );

	hr = pMeshContainer->pOrigMesh->CloneMeshFVF( D3DXMESH_MANAGED, pMeshContainer->pOrigMesh->GetFVF(),
												g_pD3DDevice, &pMeshContainer->MeshData.pMesh );
	if( FAILED(hr) )
		return hr;

	hr = pMeshContainer->MeshData.pMesh->GetAttributeTable( NULL, &pMeshContainer->numAttributeGroups );
	if( FAILED(hr) )
		return hr;

	delete[] pMeshContainer->pAttributeTable;
	pMeshContainer->pAttributeTable = new D3DXATTRIBUTERANGE[pMeshContainer->numAttributeGroups];
	if( pMeshContainer->pAttributeTable == NULL )
		return E_OUTOFMEMORY;

	hr = pMeshContainer->MeshData.pMesh->GetAttributeTable( pMeshContainer->pAttributeTable, NULL );
	if( FAILED(hr) )
		return hr;

	if( g_numBoneMatricesMax < pMeshContainer->pSkinInfo->GetNumBones() )
	{
		g_numBoneMatricesMax = pMeshContainer->pSkinInfo->GetNumBones();

		//为材质分配空间
		g_pBoneMatrices = new D3DXMATRIXA16[g_numBoneMatricesMax];
		if( g_pBoneMatrices == NULL )
			return E_OUTOFMEMORY;
	}

	return hr;
}
