
//-----------------------------------【程序说明】----------------------------------------------
// 小游戏  GAIA
// VS2012版
//作者：潘文杰  UltramanGaia
//时间：2016年8月
//------------------------------------------------------------------------------------------------

//-----------------------------------【宏定义部分】--------------------------------------------
// 描述：辅助宏
//------------------------------------------------------------------------------------------------
#define WINDOW_TITLE	 _T("GAIA   By UltramanGaia ")  //窗口标题

// GUI界面宏定义
#define 	GUI_MENU							0
#define 	GUI_MENU_LOCAL_LOAD					1
#define 	GUI_MENU_CONNECT					2
#define 	GUI_MENU_ONLINE_LOAD				3
#define 	GUI_MENU_OPTION						4

// GUI中的控件的ID
#define BUTTON_START_ID							0
#define BUTTON_LOCAL_ID							1
#define BUTTON_ONLINE_ID						2
#define BUTTON_CONNECT_ID						3
#define BUTTON_OPTION_ID						4
#define BUTTON_QUIT_ID							5
#define BUTTON_BACK_ID							7
#define STATIC_TEXT_ID							8
#define BUTTON_SELECT_TINY_ID					9
#define BUTTON_SELECT_MOSHOU_ID					10

// 定义宏来表示游戏状态
#define 	GAME_MENU							1
#define 	GAME_LOCAL							2
/*
#define 	GAME_ONLINE							3
#define		GAME_WIN							7
#define		GAME_FAIL							8
*/
//-----------------------------------【头文件包含部分】---------------------------------------
//	描述：包含头文件
//------------------------------------------------------------------------------------------------                                                                                      
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <time.h> 
#include <vector>
#include "DirectInputClass.h"											//获取输入信息的类
#include "CameraClass.h"												//虚拟摄像机的类
#include "TerrainClass.h"												//地形的类
#include "SkyBoxClass.h"												//天空的类
#include "SnowParticleClass.h"											//雪花粒子的类
#include "XFileModelClass.h"											//载入.x文件的类
#include "AllocateHierarchyClass.h"										//骨骼动画的类
#include "D3DGUIClass.h"												//GUI界面的
#include "D3DUtil.h"
#include "Character.h"
#include "FireBall.h"
#include "Hero.h"
#include "Tower.h"
#include "Soldier.h"
#include "Monster.h"

using namespace std;
//-----------------------------------【库文件包含部分】---------------------------------------
//	描述：包含库文件
//------------------------------------------------------------------------------------------------  
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "dinput8.lib")  
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "winmm.lib") 


/*
// 地板的顶点结构
struct CUSTOMVERTEX
{
float _x, _y, _z;
float _u, _v ;
CUSTOMVERTEX(float x, float y, float z, float u, float v)
: _x(x), _y(y), _z(z), _u(u), _v(v) {}
};
#define D3DFVF_CUSTOMVERTEX  (D3DFVF_XYZ | D3DFVF_TEX1)
*/

//-----------------------------------【全局变量声明部分】-------------------------------------
//	描述：全局变量声明
//------------------------------------------------------------------------------------------------D
fstream									g_fOption;
bool									g_isFullscreen			= false;			//是否全屏
int										WINDOW_WIDTH			= 1366;				//窗口宽度
int										WINDOW_HEIGHT			= 768;				//窗口高度



vector <Character*>						g_vCharacter;
vector <FireBall * >					g_vFireBall;
DWORD									g_currentTrack			= DWORD(0);
bool									g_isGuiInit				= false;
bool									g_is3DInit				= false;
int										g_userModal				= MOSHOU;
int										g_userIndex				= 0;

int										g_currentGamestate		= GAME_MENU;		//游戏当前状态
LPDIRECT3DDEVICE9						g_pd3dDevice			= NULL;				//Direct3D设备对象
LPD3DXFONT								g_pTextFPS				= NULL;				//字体COM接口
LPD3DXFONT								g_pTextAdaperName		= NULL;				//显卡信息的2D文本
LPD3DXFONT								g_pTextHelper			= NULL;				//帮助信息的2D文本
LPD3DXFONT								g_pTextInfor			= NULL;				//绘制信息的2D文本
float									g_FPS					= 0.0f;								//帧速率
wchar_t									g_strFPS[50]			={0};				//储存显示帧速率的字符数组
wchar_t									g_strAdapterName[60]	={0};				//储存显示显卡名称的字符数组
D3DXMATRIX								g_matWorld;									//世界矩阵
D3DLIGHT9								g_Light;									//全局光照
DInputClass*							g_pDInput				= NULL;				//DInputClass类的指针
CameraClass*							g_pCamera				= NULL;				//摄像机类的指针
TerrainClass*							g_pTerrain				= NULL;				//地形类的指针
SkyBoxClass*							g_pSkyBox=NULL;								//天空盒类的指针
SnowParticleClass*						g_pSnowParticles		= NULL;				//雪花粒子系统的指针
XFileModelClass*						g_pXFileModel1			= NULL;				//模型类对象
XFileModelClass*						g_pXFileModel2			= NULL;			
XFileModelClass*	     				g_pXFileModel3			= NULL;		
XFileModelClass*						g_pXFileModel4			= NULL;	

//四个和骨骼动画相关的全局变量
LPD3DXFRAME								g_pFrameRoot			= NULL;
//D3DXMATRIX*								g_pBoneMatrices			= NULL;
CAllocateHierarchy*						g_pAllocateHier			= NULL;
LPD3DXANIMATIONCONTROLLER				g_pAnimController		= NULL;

//LPDIRECT3DVERTEXBUFFER9					g_pHPVBuffer			= NULL;    // HP顶点缓存对象
//LPDIRECT3DTEXTURE9						g_pHPTexture			= NULL;    // HP纹理对象


bool									g_LMBDown				= false;			// GUI中，鼠标左键是否按下
int										g_MouseX				= 0;
int										g_MouseY				= 0;				//鼠标坐标

//创建全局GUI类对象
D3DGUIClass	*							g_MenuGUI				= NULL;				//菜单界面
D3DGUIClass	*							g_LocalLoadGUI			= NULL;				//单人游戏加载界面
D3DGUIClass	*							g_ConnectGUI			= NULL;				//多人游戏连接界面
D3DGUIClass	*							g_OnlineLoadGUI			= NULL;				//多人游戏加载界面
D3DGUIClass	*							g_OptionGUI				= NULL;				//游戏选项界面

int										g_MenuGUIFontID			= -1;				//GUI中字体对象的ID
int										g_LocalLoadGUIFontID	= -1;
int										g_ConnectGUIFontID		= -1;
int										g_OnlineLoadGUIFontID	= -1;
int										g_OptionGUIFontID		= -1;

int										g_currentGUI			= GUI_MENU;			//当前的GUI


//-----------------------------------【全局函数声明部分】-------------------------------------
//	描述：全局函数声明
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK					WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );	//窗口过程处理函数
HRESULT								Direct3D_Init(HWND hwnd,HINSTANCE hInstance);						//Direct3D初始化函数
bool								ObjectsForGui_Init();												//GUI渲染资源初始化函数
bool								ObjectsFor3D_Init();												//3D游戏渲染资源初始化函数
void								Direct3D_GUI_Render( HWND hwnd,float fTimeDelta);					//Direct3D_GUI渲染函数
void								Direct3D_GUI_Update( HWND hwnd,float fTimeDelta);					//Direct3D_GUI更新函数
void								Direct3D_Render( HWND hwnd,float fTimeDelta);						//Direct3D渲染函数
void								Direct3D_Update( HWND hwnd,float fTimeDelta);						//Direct3D更新函数
void								Direct3D_CleanUp( );												//清理COM接口对象
float								Get_FPS();															//计算帧速率函数
void								HelpText_Render(HWND hwnd);											//显示帮助信息函数
void								GUICallback(int id, int state);										//GUI界面回调函数
void								MainMsgLoop(HWND hwnd);												//主消息循环

void								judge(FireBall * ball);															//判断火球撞击物体


//-----------------------------------【WinMain( )函数】--------------------------------------
//	描述：Windows应用程序入口函数
//------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{
	int winPosX = 300;
	int winPosY = 150;

	g_fOption.open("GameMedia\\option" , ios::in);
	char temp[256];
	g_fOption>>temp>>g_isFullscreen>>temp>>WINDOW_WIDTH>>temp>>WINDOW_HEIGHT>>temp>>winPosX>>temp>>winPosY;
	g_fOption.close();

	//设计窗口类
	WNDCLASSEX wndClass={0} ;					
	wndClass.cbSize = sizeof( WNDCLASSEX ) ;	
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//窗口的样式
	wndClass.lpfnWndProc = WndProc;				//设置窗口过程函数
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance = hInstance;			
	wndClass.hIcon=(HICON)::LoadImage(NULL,_T("GameMedia\\icon.ico"),IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE); //从本地加载ico图标
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );			//窗口类的光标句柄。
	wndClass.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH);  
	wndClass.lpszMenuName = NULL;								//菜单资源的名字。
	wndClass.lpszClassName = _T("WindowsClassForGAIA");			//窗口类的名字。

	//注册窗口类
	if( !RegisterClassEx( &wndClass ) )							
		return -1;		

	//创建窗口
	HWND hwnd = CreateWindow( _T("WindowsClassForGAIA"),WINDOW_TITLE,			
		WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,					//WS_POPUP没有标题栏
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL );



	//Direct3D资源的初始化
	if (!(S_OK==Direct3D_Init (hwnd,hInstance)))
	{
		MessageBox(hwnd, _T("Direct3D资源初始化失败~！"), _T("错误信息窗口"), 0);
	}
	if(!ObjectsForGui_Init()) 
	{
		MessageBox(hwnd, _T("GUI资源初始化失败~！"), _T("错误信息窗口"), 0);
	}
	g_isGuiInit = true;

//	PlaySound(L"GameMedia\\AssassinsCreedTheme刺客信条.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);   //循环播放背景音乐
	mciSendString(L"play Background repeat", NULL, 0, NULL);
	
	if(!g_isFullscreen)
	{
		MoveWindow(hwnd,winPosX,winPosY,WINDOW_WIDTH,WINDOW_HEIGHT,true);   //调整窗口位置
	}
	ShowWindow( hwnd, nShowCmd );									//ShowWindow函数显示窗口
	UpdateWindow(hwnd);												//对窗口进行更新

	//进行DirectInput类的初始化
	g_pDInput = new DInputClass();
	g_pDInput->Init(hwnd,hInstance,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); //前台，非独占模式


	//消息循环过程
	MainMsgLoop(hwnd);

	//注销窗口类
	UnregisterClass(_T("WindowsClassForGAIA"), wndClass.hInstance);
	return 0;  
}


//-----------------------------------【MainMsgLoop( )函数】----------------------------------
//	描述：主消息循环函数
//-----------------------------------------------------------------------------------------------	
void MainMsgLoop(HWND hwnd)
{
	MSG msg = { 0 }; 
	while( msg.message != WM_QUIT )	
	{

		static float fLastTime  = (float)::timeGetTime();
		static float fCurrTime  = (float)::timeGetTime();
		static float fTimeDelta = 0.0f;
		fCurrTime  = (float)::timeGetTime();
		fTimeDelta = (fCurrTime - fLastTime) / 1000.0f;
		fLastTime  = fCurrTime;

		switch(g_currentGamestate)
		{
		case GAME_MENU:
			{
				if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //查看消息队列，有则发送消息
				{
					TranslateMessage( &msg );					//将虚拟键消息转换为字符消息
					DispatchMessage( &msg );					//发送消息给窗口程序。
				}
				else
				{
					Direct3D_GUI_Update(hwnd,fTimeDelta);
					Direct3D_GUI_Render(hwnd,fTimeDelta);
				}
			}
			break;
		case GAME_LOCAL:
			{
				if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //查看消息队列，有则发送消息
				{
					TranslateMessage( &msg );					//将虚拟键消息转换为字符消息
					DispatchMessage( &msg );					//发送消息给窗口程序。
				}
				Direct3D_Update(hwnd,fTimeDelta);			//画面的更新
				Direct3D_Render(hwnd,fTimeDelta);			//画面的渲染			
			}
			break;
		default:
			{
				Direct3D_GUI_Update(hwnd,fTimeDelta);
				Direct3D_GUI_Render(hwnd,fTimeDelta);
			}
		}
	}

}

//-----------------------------------【Direct3D_Init( )函数】----------------------------------
//	描述：Direct3D初始化函数
//------------------------------------------------------------------------------------------------
HRESULT Direct3D_Init(HWND hwnd,HINSTANCE hInstance)
{
	//加载音乐资源
	 mciSendString(L"open GameMedia\\AssassinsCreedTheme刺客信条.wav alias Background", NULL, 0, NULL);
	 mciSendString(L"open GameMedia\\火球.mp3 alias Fight", NULL, 0, NULL);
	 mciSendString(L"open GameMedia\\炸弹爆炸.wav alias Hit", NULL, 0, NULL);

	LPDIRECT3D9  pD3D = NULL; //Direct3D接口对象的创建
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ) //初始化Direct3D接口对象，并进行DirectX版本协调
		return E_FAIL;

	//获取硬件设备信息
	D3DCAPS9 caps; int vp = 0;
	if( FAILED( pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps ) ) )
	{
		return E_FAIL;
	}
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //支持硬件顶点运算，就用硬件顶点运算
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //不支持，用软件顶点运算

	//填充D3DPRESENT_PARAMETERS结构体
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth            = WINDOW_WIDTH;
	d3dpp.BackBufferHeight           = WINDOW_HEIGHT;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 2;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = !g_isFullscreen;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	//创建Direct3D设备接口
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;


	//获取显卡信息到g_strAdapterName中，并在显卡名称之前加上“当前显卡型号：”字符串
	wchar_t TempName[60]=L"当前显卡型号："; 
	D3DADAPTER_IDENTIFIER9 Adapter;  
	pD3D->GetAdapterIdentifier(0,0,&Adapter);//获取显卡信息
	int len = MultiByteToWideChar(CP_ACP,0, Adapter.Description, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, g_strAdapterName, len);
	wcscat_s(TempName,g_strAdapterName);
	wcscpy_s(g_strAdapterName,TempName);


	SAFE_RELEASE(pD3D) //LPDIRECT3D9接口对象释放

		return S_OK;
}


//-----------------------------------【ObjectsForGui_Init( )函数】--------------------------------------
//	描述：渲染资源初始化函数
//--------------------------------------------------------------------------------------------------
bool ObjectsForGui_Init()
{
	//创建字体
	D3DXCreateFont(g_pd3dDevice, 36, 0, 0, 1000, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"), &g_pTextFPS);
	D3DXCreateFont(g_pd3dDevice, 20, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"华文中宋", &g_pTextAdaperName); 
	D3DXCreateFont(g_pd3dDevice, 23, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"微软雅黑", &g_pTextHelper); 
	D3DXCreateFont(g_pd3dDevice, 26, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"黑体", &g_pTextInfor); 


	//设置纹理采样参数
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);


	//--------------------------------------------------【GUI系统相关代码】-------------------------------------------------------

	// 创建GUI系统
	g_MenuGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT);			//菜单界面
	g_LocalLoadGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT);	//单人游戏加载界面
	g_ConnectGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT);		//多人游戏连接界面
	g_OnlineLoadGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT);	//多人游戏加载界面
	g_OptionGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT);		//游戏选项界面

	// 分别添加背景图
	if(!g_MenuGUI->AddBackground(L"GameMedia/GameMenu.jpg")) return false;  
	if(!g_LocalLoadGUI->AddBackground(L"GameMedia/GameLocalLoad.jpg")) return false;  
	if(!g_ConnectGUI->AddBackground(L"GameMedia/GameConnect.jpg")) return false;
	if(!g_OnlineLoadGUI->AddBackground(L"GameMedia/GameOnlineLoad.jpg")) return false;
	if(!g_OptionGUI->AddBackground(L"GameMedia/GameOption.jpg")) return false;

	// 分别给界面添加字体
	if(!g_MenuGUI->CreateTextFont(L"微软雅黑", 28, &g_MenuGUIFontID)) return false;
	if(!g_LocalLoadGUI->CreateTextFont(L"微软雅黑", 38, &g_LocalLoadGUIFontID)) return false;
	if(!g_ConnectGUI->CreateTextFont(L"微软雅黑", 38, &g_ConnectGUIFontID)) return false;
	if(!g_OnlineLoadGUI->CreateTextFont(L"微软雅黑", 38, &g_OnlineLoadGUIFontID)) return false;
	if(!g_OptionGUI->CreateTextFont(L"微软雅黑", 38, &g_OptionGUIFontID)) return false;




	//---------------------------------【MenuGUI界面布局】---------------------------------
	// 添加静态文本
	if(!g_MenuGUI->AddStaticText(STATIC_TEXT_ID, L"By UltramanGaia",
		WINDOW_WIDTH - 200, WINDOW_HEIGHT - 60, D3DCOLOR_XRGB(55,155,255), g_MenuGUIFontID)) return false;

	// 添加按钮
	if(!g_MenuGUI->AddButton(BUTTON_LOCAL_ID, WINDOW_WIDTH * 7 / 12 , WINDOW_HEIGHT / 2 -70, L"GameMedia\\localUp.png",
		L"GameMedia\\localOver.png", L"GameMedia\\localDown.png")) return false;

	if(!g_MenuGUI->AddButton(BUTTON_ONLINE_ID, WINDOW_WIDTH * 7 / 12,WINDOW_HEIGHT / 2     , L"GameMedia\\onlineUp.png",
		L"GameMedia\\onlineOver.png", L"GameMedia\\onlineDown.png")) return false;

	if(!g_MenuGUI->AddButton(BUTTON_OPTION_ID, WINDOW_WIDTH *7 / 12, WINDOW_HEIGHT / 2 + 70, L"GameMedia\\optionUp.png",
		L"GameMedia\\optionOver.png", L"GameMedia\\optionDown.png")) return false;

	if(!g_MenuGUI->AddButton(BUTTON_QUIT_ID, WINDOW_WIDTH * 7 / 12 , WINDOW_HEIGHT / 2 + 140, L"GameMedia\\quitUp.png",
		L"GameMedia\\quitOver.png", L"GameMedia\\quitDown.png")) return false;


	//------------------------【LocalLoad界面布局】------------------------
	// 添加静态文本
	if(!g_LocalLoadGUI->AddStaticText(STATIC_TEXT_ID, L"选择英雄",
		200,  WINDOW_HEIGHT / 2, D3DCOLOR_XRGB(55,155,255), g_LocalLoadGUIFontID)) return false;
	// 添加按钮
	if(!g_LocalLoadGUI->AddButton(BUTTON_SELECT_MOSHOU_ID, 100,  WINDOW_HEIGHT - 300 , L"GameMedia\\moshouUp.png", L"GameMedia\\moshouOver.png",
		L"GameMedia\\moshouDown.png")) return false;
	if(!g_LocalLoadGUI->AddButton(BUTTON_SELECT_TINY_ID,   400,  WINDOW_HEIGHT - 300 , L"GameMedia\\tinyUp.png", L"GameMedia\\tinyOver.png",
		L"GameMedia\\tinyDown.png")) return false;

	if(!g_LocalLoadGUI->AddButton(BUTTON_START_ID, WINDOW_WIDTH - 300, WINDOW_HEIGHT - 100 , L"GameMedia\\startUp.png", L"GameMedia\\startOver.png",
		L"GameMedia\\startDown.png")) return false;


	//------------------------【Connect界面布局】------------------------
	// 添加静态文本
	if(!g_ConnectGUI->AddStaticText(STATIC_TEXT_ID, L"Loading...",
		WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, D3DCOLOR_XRGB(55,155,255), g_ConnectGUIFontID)) return false;
	// 添加按钮
	if(!g_ConnectGUI->AddButton(BUTTON_CONNECT_ID, WINDOW_WIDTH - 300, WINDOW_HEIGHT - 170 , L"GameMedia\\connectUp.png", L"GameMedia\\connectOver.png",
		L"GameMedia\\connectDown.png")) return false;
	if(!g_ConnectGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH - 300, WINDOW_HEIGHT - 100 , L"GameMedia\\backUp.png", L"GameMedia\\backOver.png",
		L"GameMedia\\backDown.png")) return false;

	//------------------------【OnlineLoad界面布局】------------------------
	// 添加静态文本
	if(!g_OnlineLoadGUI->AddStaticText(STATIC_TEXT_ID, L"Loading...",
		WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, D3DCOLOR_XRGB(55,155,255), g_OnlineLoadGUIFontID)) return false;
	// 添加按钮
	if(!g_OnlineLoadGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH - 300, WINDOW_HEIGHT - 100 , L"GameMedia\\backUp.png", L"GameMedia\\backOver.png",
		L"GameMedia\\backDown.png")) return false;

	//------------------------【Option界面布局】------------------------
	// 添加静态文本
	if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"Option...",
		WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, D3DCOLOR_XRGB(55,155,255), g_OptionGUIFontID)) return false;
	// 添加按钮
	if(!g_OptionGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH - 300, WINDOW_HEIGHT - 100 , L"GameMedia\\backUp.png", L"GameMedia\\backOver.png",
		L"GameMedia\\backDown.png")) return false;


	return true;
}

//-----------------------------------【ObjectsFor3D_Init( )函数】--------------------------------------
//	描述：渲染资源初始化函数
//--------------------------------------------------------------------------------------------------
bool ObjectsFor3D_Init()
{

	//--------------------------------------------------【游戏相关代码】-------------------------------------------------------

	//创建 初始化地形
	g_pTerrain = new TerrainClass(g_pd3dDevice);
	g_pTerrain->LoadTerrainFromFile(L"GameMedia\\hightMap.raw", L"GameMedia\\贴图.png");//从文件加载高度图和纹理
	g_pTerrain->InitTerrain(200, 200, 100.0f, 10.0f, 1.0f,1.0f);  //四个值分别是顶点行数，顶点列数，顶点间间距，高度图缩放系数,横，竖用多少张纹理图贴图

	// 设置光照  
	::ZeroMemory(&g_Light, sizeof(g_Light));  
	g_Light.Type          = D3DLIGHT_DIRECTIONAL;  
	g_Light.Ambient       = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);  
	g_Light.Diffuse       = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);  
	g_Light.Specular      = D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f);  
	g_Light.Direction     = D3DXVECTOR3(1.0f, 1.0f, 1.0f);  
	g_pd3dDevice->SetLight(0, &g_Light);  
	g_pd3dDevice->LightEnable(0, true);  
	g_pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);  
	g_pd3dDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

	// 创建并初始化虚拟摄像机
	g_pCamera = new CameraClass(g_pd3dDevice);
	g_pCamera->SetCameraPosition(&D3DXVECTOR3(0.0f, 300.0f, -800.0f));  //设置摄像机所在的位置
	g_pCamera->SetTargetPosition(&D3DXVECTOR3(0.0f, 400.0f, 0.0f));  //设置目标观察点所在的位置
	g_pCamera->SetViewMatrix();  //设置取景变换矩阵
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, 1.0f, 1.0f, 200000.0f);
	g_pCamera->SetProjMatrix(&matProj);

	//创建并初始化天空对象
	g_pSkyBox = new SkyBoxClass( g_pd3dDevice );
	g_pSkyBox->LoadSkyTextureFromFile(L"GameMedia\\SunSetFront2048.png",L"GameMedia\\SunSetBack2048.png",L"GameMedia\\SunSetRight2048.png",L"GameMedia\\SunSetLeft2048.png", L"GameMedia\\SunSetUp2048.png");//从文件加载前、后、左、右、顶面5个面的纹理图
	g_pSkyBox->InitSkyBox(50000);  //设置天空盒的边长

	//创建并初始化雪花粒子系统  
	g_pSnowParticles = new SnowParticleClass(g_pd3dDevice);  
	g_pSnowParticles->InitSnowParticle();  


	// 创建骨骼动画
	//塔
	ifstream infileForTower("GameMedia\\towerInfo.txt");
	if(!infileForTower)
	{

		MessageBox(NULL,L"towerInfo.txt 文件打开失败",L"文件打开失败",0);
	}
	for(int i = 0 ; i < 8 ; i ++)
	{
		Tower * TempChar = new Tower();
		g_vCharacter.push_back(TempChar);
		D3DXVECTOR3 tower;
		float towerFacing;
		int team;
		infileForTower>>tower.x>>tower.y>>tower.z>>towerFacing>>team;
		g_vCharacter[g_vCharacter.size() - 1]->id = g_vCharacter.size() - 1;
		g_vCharacter[g_vCharacter.size() - 1]->team = (Team)team;
		g_vCharacter[g_vCharacter.size() - 1]->species = 1;
		g_vCharacter[g_vCharacter.size() - 1]->Init(g_pd3dDevice,g_pDInput , g_pTerrain, g_pCamera,&g_vCharacter);
		g_vCharacter[g_vCharacter.size() - 1]->setControlByUser(false);
		g_vCharacter[g_vCharacter.size() - 1]->setPosition(tower );
		g_vCharacter[g_vCharacter.size() - 1]->setFacing(towerFacing);
	}

	//玩家英雄
	Hero * TempChar = new Hero(g_userModal);
	g_vCharacter.push_back(TempChar);
	g_vCharacter[g_vCharacter.size() - 1]->id = g_vCharacter.size() - 1;
	g_vCharacter[g_vCharacter.size() - 1]->species = 0;
	g_vCharacter[g_vCharacter.size() - 1]->team = Red;
	g_vCharacter[g_vCharacter.size() - 1]->Init(g_pd3dDevice,g_pDInput , g_pTerrain, g_pCamera,&g_vCharacter);
	g_vCharacter[g_vCharacter.size() - 1]->setControlByUser(true);
	g_userIndex = g_vCharacter.size() - 1;
	g_vCharacter[g_vCharacter.size() - 1]->setPosition(D3DXVECTOR3(-7565.6,0.0,-7190.5));

	//敌军英雄
	TempChar = new Hero(rand()%2==1?MOSHOU:TINY);
	g_vCharacter.push_back(TempChar);
	g_vCharacter[g_vCharacter.size() - 1]->id = g_vCharacter.size() - 1;
	g_vCharacter[g_vCharacter.size() - 1]->species = 0;
	g_vCharacter[g_vCharacter.size() - 1]->team = Blue;
	g_vCharacter[g_vCharacter.size() - 1]->Init(g_pd3dDevice,g_pDInput , g_pTerrain, g_pCamera,&g_vCharacter);
	g_vCharacter[g_vCharacter.size() - 1]->setControlByUser(false);
	g_vCharacter[g_vCharacter.size() - 1]->setPosition(D3DXVECTOR3(3516.8,0.0,3393.6));


	//我方小兵
	for(int i = 0 ; i < 3 ;i++)
	{
		Soldier * TempChar = new Soldier(i);
		g_vCharacter.push_back(TempChar);
		g_vCharacter[g_vCharacter.size() - 1]->setControlByUser(false);
		g_vCharacter[g_vCharacter.size() - 1]->id = g_vCharacter.size() - 1;
		g_vCharacter[g_vCharacter.size() - 1]->species = 2;
		g_vCharacter[g_vCharacter.size() - 1]->team = Red;
		g_vCharacter[g_vCharacter.size() - 1]->Init(g_pd3dDevice,g_pDInput , g_pTerrain, g_pCamera,&g_vCharacter);
	}

	for(int i = 0 ; i < 3 ;i++)
	{
		Soldier * TempChar = new Soldier(i);
		g_vCharacter.push_back(TempChar);
		g_vCharacter[g_vCharacter.size() - 1]->setControlByUser(false);
		g_vCharacter[g_vCharacter.size() - 1]->id = g_vCharacter.size() - 1;
		g_vCharacter[g_vCharacter.size() - 1]->species = 2;
		g_vCharacter[g_vCharacter.size() - 1]->team = Blue;
		g_vCharacter[g_vCharacter.size() - 1]->Init(g_pd3dDevice,g_pDInput , g_pTerrain, g_pCamera,&g_vCharacter);
	}

	//野怪
	//1
	Monster * TempChar1 = new Monster(D3DXVECTOR3(-2243.8,0.0,614.4));
	g_vCharacter.push_back(TempChar1);
	g_vCharacter[g_vCharacter.size() - 1]->setControlByUser(false);
	g_vCharacter[g_vCharacter.size() - 1]->id = g_vCharacter.size() - 1;
	g_vCharacter[g_vCharacter.size() - 1]->species = 3;
	g_vCharacter[g_vCharacter.size() - 1]->team = Other;
	g_vCharacter[g_vCharacter.size() - 1]->Init(g_pd3dDevice,g_pDInput , g_pTerrain, g_pCamera,&g_vCharacter);

	//2
	TempChar1 = new Monster(D3DXVECTOR3(95.2,0.0,-4813.9));
	g_vCharacter.push_back(TempChar1);
	g_vCharacter[g_vCharacter.size() - 1]->setControlByUser(false);
	g_vCharacter[g_vCharacter.size() - 1]->id = g_vCharacter.size() - 1;
	g_vCharacter[g_vCharacter.size() - 1]->species = 3;
	g_vCharacter[g_vCharacter.size() - 1]->team = Other;
	g_vCharacter[g_vCharacter.size() - 1]->Init(g_pd3dDevice,g_pDInput , g_pTerrain, g_pCamera,&g_vCharacter);


	return true;
}


//-----------------------------------【WndProc( )函数】--------------------------------------
//	描述：窗口过程函数WndProc,对窗口消息进行处理
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if(g_currentGamestate == GAME_MENU)			//在菜单状态
	{
		switch( message )		
		{
		case WM_LBUTTONDOWN:					//鼠标左键按下
			g_LMBDown = true;
			break;

		case WM_LBUTTONUP:						//鼠标左键弹起
			g_LMBDown = false;
			break;

		case WM_MOUSEMOVE:						//鼠标移动
			g_MouseX = LOWORD (lParam);
			g_MouseY = HIWORD (lParam);
			break;

		case WM_KEYDOWN:						//键盘按下消息
			if (wParam == VK_ESCAPE)			//ESC键
			{
				if(g_currentGUI == GUI_MENU)
					DestroyWindow(hwnd);		// 销毁窗口, 并发送一条WM_DESTROY消息
				else
				{
					g_currentGUI = GUI_MENU;
				}
			}
			break;

		case WM_PAINT:							//客户区重绘消息
			Direct3D_GUI_Render(hwnd,0.0f);		//Direct3D_GUI_Render函数，进行画面的绘制
			ValidateRect(hwnd, NULL);			//更新客户区的显示
			break;			

		case WM_DESTROY:						//窗口销毁消息
			Direct3D_CleanUp();					//Direct3D_CleanUp函数，清理COM接口对象
			PostQuitMessage( 0 );				//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
			break;		

		default:					
			return DefWindowProc( hwnd, message, wParam, lParam );		//其他设置为默认
		}
	}
	else if(g_currentGamestate == GAME_LOCAL )
	{
		switch( message )		
		{
		case WM_KEYUP:							//键盘按下消息
			if (wParam == VK_ESCAPE)			//ESC键
			{
				g_currentGamestate = GAME_MENU;
				g_currentGUI = GUI_MENU;
			}
			break;

		case WM_PAINT:							//客户区重绘消息
			Direct3D_Render(hwnd,0.0f);			//Direct3D_Render函数，进行画面的绘制
			ValidateRect(hwnd, NULL);			//更新客户区的显示
			break;			

		case WM_DESTROY:						//窗口销毁消息
			Direct3D_CleanUp();					//Direct3D_CleanUp函数，清理COM接口对象
			PostQuitMessage( 0 );				//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
			break;		

		default:					
			return DefWindowProc( hwnd, message, wParam, lParam );		//其他调用默认
		}
	}

	return 0;		
}


//-----------------------------------【GUICallback( )函数】---------------------------------------
//	描述：GUI系统的回调函数，填写按钮按下后的相关处理代码
//--------------------------------------------------------------------------------------------------
void GUICallback(int id, int state)
{
	switch(id)
	{
	case BUTTON_LOCAL_ID:   //单人游戏按钮
		if(state == UGP_BUTTON_DOWN)
		{
			g_currentGUI = GUI_MENU_LOCAL_LOAD;
			g_LMBDown = false;
			g_MouseX  = 0;
			g_MouseY = 0;
		}
		break;

	case BUTTON_ONLINE_ID:  //多人游戏按钮
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_MENU_CONNECT;
		break;

	case BUTTON_START_ID:	//开始游戏按钮
		if(state == UGP_BUTTON_DOWN)
		{
			if(!g_is3DInit)
			{
				if(!ObjectsFor3D_Init()) 
				{
					MessageBox(NULL, _T("3D游戏资源初始化失败~！"), _T("错误信息窗口"), 0);
				}
				g_is3DInit = true;
			}
			g_currentGamestate = GAME_LOCAL;
		}
		break;

	case BUTTON_CONNECT_ID:  //连接按钮
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_MENU_ONLINE_LOAD;
		break;

	case BUTTON_OPTION_ID: //游戏选项按钮
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_MENU_OPTION;
		break;

	case BUTTON_BACK_ID: //返回按钮
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_MENU;
		break;

	case BUTTON_QUIT_ID://退出游戏按钮
		if(state == UGP_BUTTON_DOWN)
			PostQuitMessage(0);
		break;

	case BUTTON_SELECT_MOSHOU_ID:
		if(state == UGP_BUTTON_DOWN)
			g_userModal = MOSHOU;
		break;
	case BUTTON_SELECT_TINY_ID:
		if(state == UGP_BUTTON_DOWN)
			g_userModal = TINY;
		break;
	}
	g_LMBDown = false;


}

//-----------------------------------【Direct3D_Update( )函数】--------------------------------
//	描述：各种数据的更新，不是即时渲染代码但是需要即时调用的，如按键后的坐标的更改
//--------------------------------------------------------------------------------------------------
void	Direct3D_GUI_Update( HWND hwnd,float fTimeDelta)
{
	while(ShowCursor(true) < 0);


}

//-----------------------------------【Direct3D_Update( )函数】--------------------------------
//	描述：各种数据的更新，不是即时渲染代码但是需要即时调用的，如按键后的坐标的更改
//--------------------------------------------------------------------------------------------------
void	Direct3D_Update( HWND hwnd,float fTimeDelta)
{
	//使用DirectInput类读取数据  
	g_pDInput->GetInput();  


	// 沿摄像机各分量移动视角  
	if (g_pDInput->IsKeyDown(DIK_A))  g_pCamera->MoveAlongRightVec(-20.0f);  
	if (g_pDInput->IsKeyDown(DIK_D))  g_pCamera->MoveAlongRightVec( 20.0f);  
	if (g_pDInput->IsKeyDown(DIK_W))  g_pCamera->MoveAlongLookVec( 20.0f);  
	if (g_pDInput->IsKeyDown(DIK_S))  g_pCamera->MoveAlongLookVec(-20.0f);  
	if (g_pDInput->IsKeyDown(DIK_R))  g_pCamera->MoveAlongUpVec( 10.0f);  
	if (g_pDInput->IsKeyDown(DIK_F))  g_pCamera->MoveAlongUpVec(-10.0f);  

	//沿摄像机各分量旋转视角  
	if (g_pDInput->IsKeyDown(DIK_LEFT)) g_pCamera->RotationUpVec(-0.003f);  
	if (g_pDInput->IsKeyDown(DIK_RIGHT))g_pCamera->RotationUpVec( 0.003f);  
	if (g_pDInput->IsKeyDown(DIK_UP))	g_pCamera->RotationRightVec(-0.003f);  
	if (g_pDInput->IsKeyDown(DIK_DOWN)) g_pCamera->RotationRightVec( 0.003f);  
	if (g_pDInput->IsKeyDown(DIK_Q))	g_pCamera->RotationLookVec(0.001f);  
	if (g_pDInput->IsKeyDown(DIK_E))	g_pCamera->RotationLookVec( -0.001f);  

	//鼠标控制右向量和上向量的旋转  
	g_pCamera->RotationUpVec(g_pDInput->MouseDX()* 0.0003f);  
	g_pCamera->RotationRightVec(g_pDInput->MouseDY() * 0.0003f);  

	//鼠标滚轮控制观察点收缩操作  
	static float fPosZ=0.0f;  
	fPosZ += g_pDInput->MouseDZ()*0.03f;  


	if(g_pDInput->IsKeyDown(DIK_C))
	{
		g_vCharacter[g_userIndex]->setControlByUser(false);
		g_userIndex++;
		if(g_userIndex > g_vCharacter.size() - 1)
			g_userIndex = 0;
		g_vCharacter[g_userIndex]->setControlByUser(true);
		Sleep(1000);

	}

	if(g_pDInput->IsKeyDown(DIK_B))
	{
		D3DXVECTOR3 pos;
		g_vCharacter[g_userIndex]->getPosition(&pos);

		bool ok = false;
		for(int i = 0 ;  i < g_vFireBall.size() ; i ++)
		{
			if(g_vFireBall[i]->isDestroy)
			{
				g_vFireBall[i]->Init(g_vCharacter[g_userIndex]->team , g_userIndex , 500.0f,pos ,g_vCharacter[g_userIndex]->facing);
				ok = true;
				break;
			}
		}
		if(!ok)
		{
			FireBall * temp = new FireBall(g_pd3dDevice);
			g_vFireBall.push_back(temp);

			g_vFireBall[g_vFireBall.size() - 1]->Init(g_vCharacter[g_userIndex]->team , g_userIndex , 500.0f,pos ,g_vCharacter[g_userIndex]->facing);


		}

	}

	if(!g_pDInput->IsKeyDown(DIK_SPACE))
	{
		D3DXVECTOR3 vCharPos;
		//	g_vCharacter[0]->getPosition(&vCharPos);
		g_vCharacter[g_userIndex]->getPosition(&vCharPos);
		g_pCamera->FollowPosition(&vCharPos);
	}

	//计算并设置取景变换矩阵  
	D3DXMATRIX matView;  
	g_pCamera->CalculateViewMatrix(&matView);  
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);  

	//把正确的世界变换矩阵存到g_matWorld中  
	D3DXMatrixTranslation(&g_matWorld, 0.0f, 0.0f, fPosZ);  

	/*	//限制鼠标光标移动区域
	POINT lt,rb;
	RECT rect;
	GetClientRect(hwnd,&rect);  //取得窗口内部矩形
	lt.x = rect.left;
	lt.y = rect.top;
	rb.x = rect.right;
	rb.y = rect.bottom;
	//将lt和rb的窗口坐标转换为屏幕坐标
	ClientToScreen(hwnd,&lt);
	ClientToScreen(hwnd,&rb);
	//以屏幕坐标重新设定矩形区域
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	//限制鼠标光标移动区域
	ClipCursor(&rect);
	*/
	while( ShowCursor(false) >= 0);		//隐藏鼠标光标

	srand((unsigned int)timeGetTime());
	vector<Character * > :: iterator it;
	for(it = g_vCharacter.begin();it != g_vCharacter.end();it++)
	{
		(*it)->Control(fTimeDelta );
		(*it)->Update();
	}

	vector <FireBall * >:: iterator it1;
	for(it1 = g_vFireBall.begin(); it1 != g_vFireBall.end() ;it1++)
	{
		judge(*it1);
		(*it1)->Update(fTimeDelta);
	}


}

//判断火球撞击物体
void  judge(FireBall * ball)
{
	if(ball->isDestroy)
		return;

	bool hit = false ;
	for(int i = 0 ; i < g_vCharacter.size(); i++)
	{
		if(ball->index  == g_vCharacter[i]->id)
			continue;
		if(ball->m_team  == g_vCharacter[i]->team)
			continue;
		if(D3DXVec3Length(&(ball->position - g_vCharacter[i]->position)) < ball->BallRadius + g_vCharacter[i]->modalRadius )
		{
			hit = true;
			if(g_vCharacter[ball->index]->atk  > g_vCharacter[i]->def)
				g_vCharacter[i]->hp -= g_vCharacter[ball->index]->atk  - g_vCharacter[i]->def;
			if(g_vCharacter[i]->hp <= 0)
			{
				g_vCharacter[i]->alive = false;
				g_vCharacter[i]->timeForComeBackToLife = g_vCharacter[i]->level * 5.0f;
				g_vCharacter[ball->index]->exp += totalExp[g_vCharacter[i]->level - 1] / 2;
				if(g_vCharacter[ball->index]->exp > totalExp[g_vCharacter[ball->index]->level - 1])
				{
					g_vCharacter[ball->index]->exp -= totalExp[g_vCharacter[ball->index]->level - 1];
					g_vCharacter[ball->index]->level += 1;
					if(g_vCharacter[ball->index]->level > 15)
						g_vCharacter[ball->index]->level = 15;
				}

			}

		}
		if(hit)
		{
			if(ball->index == g_userIndex)
				mciSendString(L"play Hit from 0", NULL, 0, NULL);
			ball->isDestroy = true;
			break;
		}

	}

}

//-----------------------------------【Direct3D_GUI_Render( )函数】-------------------------------
//	描述：使用Direct3D_GUI进行渲染
//--------------------------------------------------------------------------------------------------
void Direct3D_GUI_Render(HWND hwnd,float fTimeDelta)
{
	//清屏操作
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100, 155, 255), 1.0f, 0);

	//开始绘制
	g_pd3dDevice->BeginScene(); 

	//正式绘制


	if(g_currentGUI == GUI_MENU)
	{
		ProcessGUI(g_MenuGUI, g_LMBDown, g_MouseX,
			g_MouseY, GUICallback);
	}
	else if(g_currentGUI == GUI_MENU_LOCAL_LOAD)
	{
		ProcessGUI(g_LocalLoadGUI, g_LMBDown, g_MouseX,
			g_MouseY, GUICallback);
	}
	else if(g_currentGUI == GUI_MENU_ONLINE_LOAD)
	{
		ProcessGUI(g_OnlineLoadGUI, g_LMBDown, g_MouseX,
			g_MouseY, GUICallback);
	}
	else if(g_currentGUI == GUI_MENU_CONNECT)
	{
		ProcessGUI(g_ConnectGUI, g_LMBDown, g_MouseX,
			g_MouseY, GUICallback);
	}
	else if(g_currentGUI == GUI_MENU_OPTION)
	{
		ProcessGUI(g_OptionGUI, g_LMBDown, g_MouseX,
			g_MouseY, GUICallback);
	}
	else
	{
		ProcessGUI(g_MenuGUI, g_LMBDown, g_MouseX,
			g_MouseY, GUICallback);
	}

	//-----------------------------【绘制文字信息】-----------------------------
	//	HelpText_Render(hwnd);


	//结束绘制
	g_pd3dDevice->EndScene();

	//显示翻转
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);  



}

//-----------------------------------【Direct3D_Render( )函数】-------------------------------
//	描述：使用Direct3D进行渲染
//--------------------------------------------------------------------------------------------------
void Direct3D_Render(HWND hwnd,float fTimeDelta)
{
	//清屏操作
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//开始绘制
	g_pd3dDevice->BeginScene();

	//正式绘制


	//-----------------------------【绘制地形】-----------------------------
	g_pTerrain->RenderTerrain(&g_matWorld, false);  //渲染地形，且第二个参数设为false，表示不渲染出地形的线框


	//-----------------------------【绘制骨骼动画】------------------------
	vector<Character * > :: iterator it;
	for(it = g_vCharacter.begin();it != g_vCharacter.end();it++)
	{
		(*it)->Draw();
	}

	//绘制火球
	vector <FireBall * >:: iterator it1;
	for(it1 = g_vFireBall.begin(); it1 != g_vFireBall.end() ;it1++)
	{
		(*it1)->Render();
	}

	/*
	DrawFrame(g_pd3dDevice, g_pFrameRoot);
	*/
	//-----------------------------【绘制地板】-----------------------------
	//	D3DXMATRIX matFloor;
	///	D3DXMatrixTranslation(&matFloor, 0.0f, 0.0f, 0.0f);
	//	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matFloor);
	//	g_pd3dDevice->SetStreamSource(0, g_pFloorVBuffer, 0, sizeof(CUSTOMVERTEX));
	//	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	//	g_pd3dDevice->SetTexture(0, g_pFloorTexture);
	//	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);




	//-----------------------------【绘制天空】-----------------------------
	D3DXMATRIX matSky,matTransSky,matRotSky;
	D3DXMatrixTranslation(&matTransSky,0.0f,-13000.0f,0.0f);
	D3DXMatrixRotationY(&matRotSky, -0.00002f*timeGetTime());   //旋转天空网格, 简单模拟云彩运动效果
	matSky=matTransSky*matRotSky;
	g_pSkyBox->RenderSkyBox(&matSky, false);

	//-----------------------------【绘制雪花粒子系统】------------------------
//	g_pSnowParticles->UpdateSnowParticle(fTimeDelta);
//	g_pSnowParticles->RenderSnowParticle();

	//-----------------------------【绘制文字信息】-----------------------------
	HelpText_Render(hwnd);


	//结束绘制
	g_pd3dDevice->EndScene();   

	//显示翻转
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL); 

}

//-----------------------------------【HelpText_Render( )函数】-------------------------------
//	描述：显示帮助信息的函数
//--------------------------------------------------------------------------------------------------
void HelpText_Render(HWND hwnd)
{
	//定义一个矩形，用于获取主窗口矩形
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);

	//显示每秒帧数
	formatRect.top = 5;
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS() );
	g_pTextFPS->DrawText(NULL, g_strFPS, charCount , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_RGBA(0,239,136,255));

	//显示显卡类型名
	g_pTextAdaperName->DrawText(NULL,g_strAdapterName, -1, &formatRect, 
		DT_TOP | DT_LEFT, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));

	//显示摄像机信息
	formatRect.bottom -= 100;
	//	LPCSTR info = "info\nhaha";
	char info[500];

	D3DXVECTOR3 vPos , vTar , vRig , vUp ,vLook;
	if(g_pCamera)
	{
		g_pCamera->GetCameraPosition(&vPos);
		g_pCamera->GetTargetPosition(&vTar);
		g_pCamera->GetRightVrctor(&vRig);
		g_pCamera->GetUpVector(&vUp);
		g_pCamera->GetLookVector(&vLook);

	}

	sprintf(info , "vPos:    %.1f    %.1f    %.1f\n\
				   vTar:    %.1f    %.1f    %.1f \n\
				   vRig:    %.1f    %.1f    %.1f \n\
				   vUp:     %.1f    %.1f    %.1f \n\
				   vLook:    %.1f    %.1f    %.1f \n",
				   vPos.x , vPos.y , vPos.z ,
				   vTar.x , vTar.y ,vTar.z ,
				   vRig.x , vRig.y , vRig.z,
				   vUp.x , vUp.y ,vUp.z,
				   vLook.x , vLook.y , vLook.z);

	g_pTextInfor->DrawTextA(NULL,info,-1,&formatRect , DT_BOTTOM|DT_LEFT ,D3DXCOLOR(1.0f,0.5f,0.0f,1.0f));

	//动画轨道信息
	D3DXVECTOR3 heroPos , tarPos;
	g_vCharacter[g_userIndex]->getPosition(&heroPos);
	g_vCharacter[g_userIndex]->getTargetPosition(&tarPos);
	char cTrack[500];
	if(g_vCharacter.size() > 0)
		sprintf(cTrack , "walk : 3\n\
						 run : 0\n\
						 idle : 2\n\
						 attack : 1\n\
						 currentTrack: %d \n\
						 currentAnim: %d \n\
						 heroHeight:  %.1f\n\
						 heroPos:    %.1f    %.1f    %.1f \n\
						 tarPos:     %.1f    %.1f    %.1f \n\
						 facing:      %.3f \n\
						 facingTarget:      %.3f", 
						 g_vCharacter[g_userIndex]->getCurrentTrack(),
						 g_vCharacter[g_userIndex]->getCurrentAnim(),
						 g_pTerrain ->getPositionHeight(heroPos / 100),
						 heroPos.x , heroPos.y , heroPos.z,
						 tarPos.x,tarPos.y,tarPos.z,
						 g_vCharacter[g_userIndex]->facing, 
						 g_vCharacter[g_userIndex]->facingTarget 
						 );

	g_pTextHelper ->DrawTextA(NULL,cTrack,-1,&formatRect , DT_BOTTOM|DT_RIGHT ,D3DXCOLOR(1.0f,0.5f,0.0f,1.0f));

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



//-----------------------------------【Direct3D_CleanUp( )函数】--------------------------------
//	描述：对Direct3D的资源进行清理，释放COM接口对象
//---------------------------------------------------------------------------------------------------
void Direct3D_CleanUp()
{
	//释放COM接口对象
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pTextFPS)
		SAFE_RELEASE(g_pd3dDevice)
		SAFE_DELETE(g_MenuGUI)
		SAFE_DELETE(g_LocalLoadGUI)
		SAFE_DELETE(g_OnlineLoadGUI)
		SAFE_DELETE(g_ConnectGUI)
		SAFE_DELETE(g_OptionGUI)
}



