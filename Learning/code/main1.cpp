
//-----------------------------------������˵����----------------------------------------------
// С��Ϸ  GAIA
// VS2012��
//���ߣ����Ľ�  UltramanGaia
//ʱ�䣺2016��8��
//------------------------------------------------------------------------------------------------

//-----------------------------------���궨�岿�֡�--------------------------------------------
// ������������
//------------------------------------------------------------------------------------------------
#define WINDOW_TITLE	 _T("GAIA   By UltramanGaia ")  //���ڱ���

// GUI����궨��
#define 	GUI_MENU							0
#define 	GUI_MENU_LOCAL_LOAD					1
#define 	GUI_MENU_CONNECT					2
#define 	GUI_MENU_ONLINE_LOAD				3
#define 	GUI_MENU_OPTION						4

// GUI�еĿؼ���ID
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

// ���������ʾ��Ϸ״̬
#define 	GAME_MENU							1
#define 	GAME_LOCAL							2
/*
#define 	GAME_ONLINE							3
#define		GAME_WIN							7
#define		GAME_FAIL							8
*/
//-----------------------------------��ͷ�ļ��������֡�---------------------------------------
//	����������ͷ�ļ�
//------------------------------------------------------------------------------------------------                                                                                      
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
#include <time.h> 
#include <vector>
#include "DirectInputClass.h"											//��ȡ������Ϣ����
#include "CameraClass.h"												//�������������
#include "TerrainClass.h"												//���ε���
#include "SkyBoxClass.h"												//��յ���
#include "SnowParticleClass.h"											//ѩ�����ӵ���
#include "XFileModelClass.h"											//����.x�ļ�����
#include "AllocateHierarchyClass.h"										//������������
#include "D3DGUIClass.h"												//GUI�����
#include "D3DUtil.h"
#include "Character.h"
#include "FireBall.h"
#include "Hero.h"
#include "Tower.h"
#include "Soldier.h"
#include "Monster.h"

using namespace std;
//-----------------------------------�����ļ��������֡�---------------------------------------
//	�������������ļ�
//------------------------------------------------------------------------------------------------  
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib, "dinput8.lib")  
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "winmm.lib") 


/*
// �ذ�Ķ���ṹ
struct CUSTOMVERTEX
{
float _x, _y, _z;
float _u, _v ;
CUSTOMVERTEX(float x, float y, float z, float u, float v)
: _x(x), _y(y), _z(z), _u(u), _v(v) {}
};
#define D3DFVF_CUSTOMVERTEX  (D3DFVF_XYZ | D3DFVF_TEX1)
*/

//-----------------------------------��ȫ�ֱ����������֡�-------------------------------------
//	������ȫ�ֱ�������
//------------------------------------------------------------------------------------------------D
fstream									g_fOption;
bool									g_isFullscreen			= false;			//�Ƿ�ȫ��
int										WINDOW_WIDTH			= 1366;				//���ڿ��
int										WINDOW_HEIGHT			= 768;				//���ڸ߶�



vector <Character*>						g_vCharacter;
vector <FireBall * >					g_vFireBall;
DWORD									g_currentTrack			= DWORD(0);
bool									g_isGuiInit				= false;
bool									g_is3DInit				= false;
int										g_userModal				= MOSHOU;
int										g_userIndex				= 0;

int										g_currentGamestate		= GAME_MENU;		//��Ϸ��ǰ״̬
LPDIRECT3DDEVICE9						g_pd3dDevice			= NULL;				//Direct3D�豸����
LPD3DXFONT								g_pTextFPS				= NULL;				//����COM�ӿ�
LPD3DXFONT								g_pTextAdaperName		= NULL;				//�Կ���Ϣ��2D�ı�
LPD3DXFONT								g_pTextHelper			= NULL;				//������Ϣ��2D�ı�
LPD3DXFONT								g_pTextInfor			= NULL;				//������Ϣ��2D�ı�
float									g_FPS					= 0.0f;								//֡����
wchar_t									g_strFPS[50]			={0};				//������ʾ֡���ʵ��ַ�����
wchar_t									g_strAdapterName[60]	={0};				//������ʾ�Կ����Ƶ��ַ�����
D3DXMATRIX								g_matWorld;									//�������
D3DLIGHT9								g_Light;									//ȫ�ֹ���
DInputClass*							g_pDInput				= NULL;				//DInputClass���ָ��
CameraClass*							g_pCamera				= NULL;				//��������ָ��
TerrainClass*							g_pTerrain				= NULL;				//�������ָ��
SkyBoxClass*							g_pSkyBox=NULL;								//��պ����ָ��
SnowParticleClass*						g_pSnowParticles		= NULL;				//ѩ������ϵͳ��ָ��
XFileModelClass*						g_pXFileModel1			= NULL;				//ģ�������
XFileModelClass*						g_pXFileModel2			= NULL;			
XFileModelClass*	     				g_pXFileModel3			= NULL;		
XFileModelClass*						g_pXFileModel4			= NULL;	

//�ĸ��͹���������ص�ȫ�ֱ���
LPD3DXFRAME								g_pFrameRoot			= NULL;
//D3DXMATRIX*								g_pBoneMatrices			= NULL;
CAllocateHierarchy*						g_pAllocateHier			= NULL;
LPD3DXANIMATIONCONTROLLER				g_pAnimController		= NULL;

//LPDIRECT3DVERTEXBUFFER9					g_pHPVBuffer			= NULL;    // HP���㻺�����
//LPDIRECT3DTEXTURE9						g_pHPTexture			= NULL;    // HP�������


bool									g_LMBDown				= false;			// GUI�У��������Ƿ���
int										g_MouseX				= 0;
int										g_MouseY				= 0;				//�������

//����ȫ��GUI�����
D3DGUIClass	*							g_MenuGUI				= NULL;				//�˵�����
D3DGUIClass	*							g_LocalLoadGUI			= NULL;				//������Ϸ���ؽ���
D3DGUIClass	*							g_ConnectGUI			= NULL;				//������Ϸ���ӽ���
D3DGUIClass	*							g_OnlineLoadGUI			= NULL;				//������Ϸ���ؽ���
D3DGUIClass	*							g_OptionGUI				= NULL;				//��Ϸѡ�����

int										g_MenuGUIFontID			= -1;				//GUI����������ID
int										g_LocalLoadGUIFontID	= -1;
int										g_ConnectGUIFontID		= -1;
int										g_OnlineLoadGUIFontID	= -1;
int										g_OptionGUIFontID		= -1;

int										g_currentGUI			= GUI_MENU;			//��ǰ��GUI


//-----------------------------------��ȫ�ֺ����������֡�-------------------------------------
//	������ȫ�ֺ�������
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK					WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam );	//���ڹ��̴�����
HRESULT								Direct3D_Init(HWND hwnd,HINSTANCE hInstance);						//Direct3D��ʼ������
bool								ObjectsForGui_Init();												//GUI��Ⱦ��Դ��ʼ������
bool								ObjectsFor3D_Init();												//3D��Ϸ��Ⱦ��Դ��ʼ������
void								Direct3D_GUI_Render( HWND hwnd,float fTimeDelta);					//Direct3D_GUI��Ⱦ����
void								Direct3D_GUI_Update( HWND hwnd,float fTimeDelta);					//Direct3D_GUI���º���
void								Direct3D_Render( HWND hwnd,float fTimeDelta);						//Direct3D��Ⱦ����
void								Direct3D_Update( HWND hwnd,float fTimeDelta);						//Direct3D���º���
void								Direct3D_CleanUp( );												//����COM�ӿڶ���
float								Get_FPS();															//����֡���ʺ���
void								HelpText_Render(HWND hwnd);											//��ʾ������Ϣ����
void								GUICallback(int id, int state);										//GUI����ص�����
void								MainMsgLoop(HWND hwnd);												//����Ϣѭ��

void								judge(FireBall * ball);															//�жϻ���ײ������


//-----------------------------------��WinMain( )������--------------------------------------
//	������WindowsӦ�ó�����ں���
//------------------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd)
{
	int winPosX = 300;
	int winPosY = 150;

	g_fOption.open("GameMedia\\option" , ios::in);
	char temp[256];
	g_fOption>>temp>>g_isFullscreen>>temp>>WINDOW_WIDTH>>temp>>WINDOW_HEIGHT>>temp>>winPosX>>temp>>winPosY;
	g_fOption.close();

	//��ƴ�����
	WNDCLASSEX wndClass={0} ;					
	wndClass.cbSize = sizeof( WNDCLASSEX ) ;	
	wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ڵ���ʽ
	wndClass.lpfnWndProc = WndProc;				//���ô��ڹ��̺���
	wndClass.cbClsExtra		= 0;
	wndClass.cbWndExtra		= 0;
	wndClass.hInstance = hInstance;			
	wndClass.hIcon=(HICON)::LoadImage(NULL,_T("GameMedia\\icon.ico"),IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE); //�ӱ��ؼ���icoͼ��
	wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );			//������Ĺ������
	wndClass.hbrBackground=(HBRUSH)GetStockObject(GRAY_BRUSH);  
	wndClass.lpszMenuName = NULL;								//�˵���Դ�����֡�
	wndClass.lpszClassName = _T("WindowsClassForGAIA");			//����������֡�

	//ע�ᴰ����
	if( !RegisterClassEx( &wndClass ) )							
		return -1;		

	//��������
	HWND hwnd = CreateWindow( _T("WindowsClassForGAIA"),WINDOW_TITLE,			
		WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT, WINDOW_WIDTH,					//WS_POPUPû�б�����
		WINDOW_HEIGHT, NULL, NULL, hInstance, NULL );



	//Direct3D��Դ�ĳ�ʼ��
	if (!(S_OK==Direct3D_Init (hwnd,hInstance)))
	{
		MessageBox(hwnd, _T("Direct3D��Դ��ʼ��ʧ��~��"), _T("������Ϣ����"), 0);
	}
	if(!ObjectsForGui_Init()) 
	{
		MessageBox(hwnd, _T("GUI��Դ��ʼ��ʧ��~��"), _T("������Ϣ����"), 0);
	}
	g_isGuiInit = true;

//	PlaySound(L"GameMedia\\AssassinsCreedTheme�̿�����.wav", NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);   //ѭ�����ű�������
	mciSendString(L"play Background repeat", NULL, 0, NULL);
	
	if(!g_isFullscreen)
	{
		MoveWindow(hwnd,winPosX,winPosY,WINDOW_WIDTH,WINDOW_HEIGHT,true);   //��������λ��
	}
	ShowWindow( hwnd, nShowCmd );									//ShowWindow������ʾ����
	UpdateWindow(hwnd);												//�Դ��ڽ��и���

	//����DirectInput��ĳ�ʼ��
	g_pDInput = new DInputClass();
	g_pDInput->Init(hwnd,hInstance,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE,DISCL_FOREGROUND | DISCL_NONEXCLUSIVE); //ǰ̨���Ƕ�ռģʽ


	//��Ϣѭ������
	MainMsgLoop(hwnd);

	//ע��������
	UnregisterClass(_T("WindowsClassForGAIA"), wndClass.hInstance);
	return 0;  
}


//-----------------------------------��MainMsgLoop( )������----------------------------------
//	����������Ϣѭ������
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
				if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //�鿴��Ϣ���У���������Ϣ
				{
					TranslateMessage( &msg );					//���������Ϣת��Ϊ�ַ���Ϣ
					DispatchMessage( &msg );					//������Ϣ�����ڳ���
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
				if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //�鿴��Ϣ���У���������Ϣ
				{
					TranslateMessage( &msg );					//���������Ϣת��Ϊ�ַ���Ϣ
					DispatchMessage( &msg );					//������Ϣ�����ڳ���
				}
				Direct3D_Update(hwnd,fTimeDelta);			//����ĸ���
				Direct3D_Render(hwnd,fTimeDelta);			//�������Ⱦ			
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

//-----------------------------------��Direct3D_Init( )������----------------------------------
//	������Direct3D��ʼ������
//------------------------------------------------------------------------------------------------
HRESULT Direct3D_Init(HWND hwnd,HINSTANCE hInstance)
{
	//����������Դ
	 mciSendString(L"open GameMedia\\AssassinsCreedTheme�̿�����.wav alias Background", NULL, 0, NULL);
	 mciSendString(L"open GameMedia\\����.mp3 alias Fight", NULL, 0, NULL);
	 mciSendString(L"open GameMedia\\ը����ը.wav alias Hit", NULL, 0, NULL);

	LPDIRECT3D9  pD3D = NULL; //Direct3D�ӿڶ���Ĵ���
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) ) //��ʼ��Direct3D�ӿڶ��󣬲�����DirectX�汾Э��
		return E_FAIL;

	//��ȡӲ���豸��Ϣ
	D3DCAPS9 caps; int vp = 0;
	if( FAILED( pD3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps ) ) )
	{
		return E_FAIL;
	}
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //֧��Ӳ���������㣬����Ӳ����������
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //��֧�֣��������������

	//���D3DPRESENT_PARAMETERS�ṹ��
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

	//����Direct3D�豸�ӿ�
	if(FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, 
		hwnd, vp, &d3dpp, &g_pd3dDevice)))
		return E_FAIL;


	//��ȡ�Կ���Ϣ��g_strAdapterName�У������Կ�����֮ǰ���ϡ���ǰ�Կ��ͺţ����ַ���
	wchar_t TempName[60]=L"��ǰ�Կ��ͺţ�"; 
	D3DADAPTER_IDENTIFIER9 Adapter;  
	pD3D->GetAdapterIdentifier(0,0,&Adapter);//��ȡ�Կ���Ϣ
	int len = MultiByteToWideChar(CP_ACP,0, Adapter.Description, -1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, Adapter.Description, -1, g_strAdapterName, len);
	wcscat_s(TempName,g_strAdapterName);
	wcscpy_s(g_strAdapterName,TempName);


	SAFE_RELEASE(pD3D) //LPDIRECT3D9�ӿڶ����ͷ�

		return S_OK;
}


//-----------------------------------��ObjectsForGui_Init( )������--------------------------------------
//	��������Ⱦ��Դ��ʼ������
//--------------------------------------------------------------------------------------------------
bool ObjectsForGui_Init()
{
	//��������
	D3DXCreateFont(g_pd3dDevice, 36, 0, 0, 1000, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, _T("Calibri"), &g_pTextFPS);
	D3DXCreateFont(g_pd3dDevice, 20, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"��������", &g_pTextAdaperName); 
	D3DXCreateFont(g_pd3dDevice, 23, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"΢���ź�", &g_pTextHelper); 
	D3DXCreateFont(g_pd3dDevice, 26, 0, 1000, 0, false, DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, 0, L"����", &g_pTextInfor); 


	//���������������
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_NONE);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_NONE);
	g_pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);


	//--------------------------------------------------��GUIϵͳ��ش��롿-------------------------------------------------------

	// ����GUIϵͳ
	g_MenuGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT);			//�˵�����
	g_LocalLoadGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT);	//������Ϸ���ؽ���
	g_ConnectGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT);		//������Ϸ���ӽ���
	g_OnlineLoadGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT);	//������Ϸ���ؽ���
	g_OptionGUI = new D3DGUIClass(g_pd3dDevice, WINDOW_WIDTH, WINDOW_HEIGHT);		//��Ϸѡ�����

	// �ֱ���ӱ���ͼ
	if(!g_MenuGUI->AddBackground(L"GameMedia/GameMenu.jpg")) return false;  
	if(!g_LocalLoadGUI->AddBackground(L"GameMedia/GameLocalLoad.jpg")) return false;  
	if(!g_ConnectGUI->AddBackground(L"GameMedia/GameConnect.jpg")) return false;
	if(!g_OnlineLoadGUI->AddBackground(L"GameMedia/GameOnlineLoad.jpg")) return false;
	if(!g_OptionGUI->AddBackground(L"GameMedia/GameOption.jpg")) return false;

	// �ֱ�������������
	if(!g_MenuGUI->CreateTextFont(L"΢���ź�", 28, &g_MenuGUIFontID)) return false;
	if(!g_LocalLoadGUI->CreateTextFont(L"΢���ź�", 38, &g_LocalLoadGUIFontID)) return false;
	if(!g_ConnectGUI->CreateTextFont(L"΢���ź�", 38, &g_ConnectGUIFontID)) return false;
	if(!g_OnlineLoadGUI->CreateTextFont(L"΢���ź�", 38, &g_OnlineLoadGUIFontID)) return false;
	if(!g_OptionGUI->CreateTextFont(L"΢���ź�", 38, &g_OptionGUIFontID)) return false;




	//---------------------------------��MenuGUI���沼�֡�---------------------------------
	// ��Ӿ�̬�ı�
	if(!g_MenuGUI->AddStaticText(STATIC_TEXT_ID, L"By UltramanGaia",
		WINDOW_WIDTH - 200, WINDOW_HEIGHT - 60, D3DCOLOR_XRGB(55,155,255), g_MenuGUIFontID)) return false;

	// ��Ӱ�ť
	if(!g_MenuGUI->AddButton(BUTTON_LOCAL_ID, WINDOW_WIDTH * 7 / 12 , WINDOW_HEIGHT / 2 -70, L"GameMedia\\localUp.png",
		L"GameMedia\\localOver.png", L"GameMedia\\localDown.png")) return false;

	if(!g_MenuGUI->AddButton(BUTTON_ONLINE_ID, WINDOW_WIDTH * 7 / 12,WINDOW_HEIGHT / 2     , L"GameMedia\\onlineUp.png",
		L"GameMedia\\onlineOver.png", L"GameMedia\\onlineDown.png")) return false;

	if(!g_MenuGUI->AddButton(BUTTON_OPTION_ID, WINDOW_WIDTH *7 / 12, WINDOW_HEIGHT / 2 + 70, L"GameMedia\\optionUp.png",
		L"GameMedia\\optionOver.png", L"GameMedia\\optionDown.png")) return false;

	if(!g_MenuGUI->AddButton(BUTTON_QUIT_ID, WINDOW_WIDTH * 7 / 12 , WINDOW_HEIGHT / 2 + 140, L"GameMedia\\quitUp.png",
		L"GameMedia\\quitOver.png", L"GameMedia\\quitDown.png")) return false;


	//------------------------��LocalLoad���沼�֡�------------------------
	// ��Ӿ�̬�ı�
	if(!g_LocalLoadGUI->AddStaticText(STATIC_TEXT_ID, L"ѡ��Ӣ��",
		200,  WINDOW_HEIGHT / 2, D3DCOLOR_XRGB(55,155,255), g_LocalLoadGUIFontID)) return false;
	// ��Ӱ�ť
	if(!g_LocalLoadGUI->AddButton(BUTTON_SELECT_MOSHOU_ID, 100,  WINDOW_HEIGHT - 300 , L"GameMedia\\moshouUp.png", L"GameMedia\\moshouOver.png",
		L"GameMedia\\moshouDown.png")) return false;
	if(!g_LocalLoadGUI->AddButton(BUTTON_SELECT_TINY_ID,   400,  WINDOW_HEIGHT - 300 , L"GameMedia\\tinyUp.png", L"GameMedia\\tinyOver.png",
		L"GameMedia\\tinyDown.png")) return false;

	if(!g_LocalLoadGUI->AddButton(BUTTON_START_ID, WINDOW_WIDTH - 300, WINDOW_HEIGHT - 100 , L"GameMedia\\startUp.png", L"GameMedia\\startOver.png",
		L"GameMedia\\startDown.png")) return false;


	//------------------------��Connect���沼�֡�------------------------
	// ��Ӿ�̬�ı�
	if(!g_ConnectGUI->AddStaticText(STATIC_TEXT_ID, L"Loading...",
		WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, D3DCOLOR_XRGB(55,155,255), g_ConnectGUIFontID)) return false;
	// ��Ӱ�ť
	if(!g_ConnectGUI->AddButton(BUTTON_CONNECT_ID, WINDOW_WIDTH - 300, WINDOW_HEIGHT - 170 , L"GameMedia\\connectUp.png", L"GameMedia\\connectOver.png",
		L"GameMedia\\connectDown.png")) return false;
	if(!g_ConnectGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH - 300, WINDOW_HEIGHT - 100 , L"GameMedia\\backUp.png", L"GameMedia\\backOver.png",
		L"GameMedia\\backDown.png")) return false;

	//------------------------��OnlineLoad���沼�֡�------------------------
	// ��Ӿ�̬�ı�
	if(!g_OnlineLoadGUI->AddStaticText(STATIC_TEXT_ID, L"Loading...",
		WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, D3DCOLOR_XRGB(55,155,255), g_OnlineLoadGUIFontID)) return false;
	// ��Ӱ�ť
	if(!g_OnlineLoadGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH - 300, WINDOW_HEIGHT - 100 , L"GameMedia\\backUp.png", L"GameMedia\\backOver.png",
		L"GameMedia\\backDown.png")) return false;

	//------------------------��Option���沼�֡�------------------------
	// ��Ӿ�̬�ı�
	if(!g_OptionGUI->AddStaticText(STATIC_TEXT_ID, L"Option...",
		WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2, D3DCOLOR_XRGB(55,155,255), g_OptionGUIFontID)) return false;
	// ��Ӱ�ť
	if(!g_OptionGUI->AddButton(BUTTON_BACK_ID, WINDOW_WIDTH - 300, WINDOW_HEIGHT - 100 , L"GameMedia\\backUp.png", L"GameMedia\\backOver.png",
		L"GameMedia\\backDown.png")) return false;


	return true;
}

//-----------------------------------��ObjectsFor3D_Init( )������--------------------------------------
//	��������Ⱦ��Դ��ʼ������
//--------------------------------------------------------------------------------------------------
bool ObjectsFor3D_Init()
{

	//--------------------------------------------------����Ϸ��ش��롿-------------------------------------------------------

	//���� ��ʼ������
	g_pTerrain = new TerrainClass(g_pd3dDevice);
	g_pTerrain->LoadTerrainFromFile(L"GameMedia\\hightMap.raw", L"GameMedia\\��ͼ.png");//���ļ����ظ߶�ͼ������
	g_pTerrain->InitTerrain(200, 200, 100.0f, 10.0f, 1.0f,1.0f);  //�ĸ�ֵ�ֱ��Ƕ�������������������������࣬�߶�ͼ����ϵ��,�ᣬ���ö���������ͼ��ͼ

	// ���ù���  
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

	// ��������ʼ�����������
	g_pCamera = new CameraClass(g_pd3dDevice);
	g_pCamera->SetCameraPosition(&D3DXVECTOR3(0.0f, 300.0f, -800.0f));  //������������ڵ�λ��
	g_pCamera->SetTargetPosition(&D3DXVECTOR3(0.0f, 400.0f, 0.0f));  //����Ŀ��۲�����ڵ�λ��
	g_pCamera->SetViewMatrix();  //����ȡ���任����
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, 1.0f, 1.0f, 200000.0f);
	g_pCamera->SetProjMatrix(&matProj);

	//��������ʼ����ն���
	g_pSkyBox = new SkyBoxClass( g_pd3dDevice );
	g_pSkyBox->LoadSkyTextureFromFile(L"GameMedia\\SunSetFront2048.png",L"GameMedia\\SunSetBack2048.png",L"GameMedia\\SunSetRight2048.png",L"GameMedia\\SunSetLeft2048.png", L"GameMedia\\SunSetUp2048.png");//���ļ�����ǰ�������ҡ�����5���������ͼ
	g_pSkyBox->InitSkyBox(50000);  //������պеı߳�

	//��������ʼ��ѩ������ϵͳ  
	g_pSnowParticles = new SnowParticleClass(g_pd3dDevice);  
	g_pSnowParticles->InitSnowParticle();  


	// ������������
	//��
	ifstream infileForTower("GameMedia\\towerInfo.txt");
	if(!infileForTower)
	{

		MessageBox(NULL,L"towerInfo.txt �ļ���ʧ��",L"�ļ���ʧ��",0);
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

	//���Ӣ��
	Hero * TempChar = new Hero(g_userModal);
	g_vCharacter.push_back(TempChar);
	g_vCharacter[g_vCharacter.size() - 1]->id = g_vCharacter.size() - 1;
	g_vCharacter[g_vCharacter.size() - 1]->species = 0;
	g_vCharacter[g_vCharacter.size() - 1]->team = Red;
	g_vCharacter[g_vCharacter.size() - 1]->Init(g_pd3dDevice,g_pDInput , g_pTerrain, g_pCamera,&g_vCharacter);
	g_vCharacter[g_vCharacter.size() - 1]->setControlByUser(true);
	g_userIndex = g_vCharacter.size() - 1;
	g_vCharacter[g_vCharacter.size() - 1]->setPosition(D3DXVECTOR3(-7565.6,0.0,-7190.5));

	//�о�Ӣ��
	TempChar = new Hero(rand()%2==1?MOSHOU:TINY);
	g_vCharacter.push_back(TempChar);
	g_vCharacter[g_vCharacter.size() - 1]->id = g_vCharacter.size() - 1;
	g_vCharacter[g_vCharacter.size() - 1]->species = 0;
	g_vCharacter[g_vCharacter.size() - 1]->team = Blue;
	g_vCharacter[g_vCharacter.size() - 1]->Init(g_pd3dDevice,g_pDInput , g_pTerrain, g_pCamera,&g_vCharacter);
	g_vCharacter[g_vCharacter.size() - 1]->setControlByUser(false);
	g_vCharacter[g_vCharacter.size() - 1]->setPosition(D3DXVECTOR3(3516.8,0.0,3393.6));


	//�ҷ�С��
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

	//Ұ��
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


//-----------------------------------��WndProc( )������--------------------------------------
//	���������ڹ��̺���WndProc,�Դ�����Ϣ���д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK WndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	if(g_currentGamestate == GAME_MENU)			//�ڲ˵�״̬
	{
		switch( message )		
		{
		case WM_LBUTTONDOWN:					//����������
			g_LMBDown = true;
			break;

		case WM_LBUTTONUP:						//����������
			g_LMBDown = false;
			break;

		case WM_MOUSEMOVE:						//����ƶ�
			g_MouseX = LOWORD (lParam);
			g_MouseY = HIWORD (lParam);
			break;

		case WM_KEYDOWN:						//���̰�����Ϣ
			if (wParam == VK_ESCAPE)			//ESC��
			{
				if(g_currentGUI == GUI_MENU)
					DestroyWindow(hwnd);		// ���ٴ���, ������һ��WM_DESTROY��Ϣ
				else
				{
					g_currentGUI = GUI_MENU;
				}
			}
			break;

		case WM_PAINT:							//�ͻ����ػ���Ϣ
			Direct3D_GUI_Render(hwnd,0.0f);		//Direct3D_GUI_Render���������л���Ļ���
			ValidateRect(hwnd, NULL);			//���¿ͻ�������ʾ
			break;			

		case WM_DESTROY:						//����������Ϣ
			Direct3D_CleanUp();					//Direct3D_CleanUp����������COM�ӿڶ���
			PostQuitMessage( 0 );				//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
			break;		

		default:					
			return DefWindowProc( hwnd, message, wParam, lParam );		//��������ΪĬ��
		}
	}
	else if(g_currentGamestate == GAME_LOCAL )
	{
		switch( message )		
		{
		case WM_KEYUP:							//���̰�����Ϣ
			if (wParam == VK_ESCAPE)			//ESC��
			{
				g_currentGamestate = GAME_MENU;
				g_currentGUI = GUI_MENU;
			}
			break;

		case WM_PAINT:							//�ͻ����ػ���Ϣ
			Direct3D_Render(hwnd,0.0f);			//Direct3D_Render���������л���Ļ���
			ValidateRect(hwnd, NULL);			//���¿ͻ�������ʾ
			break;			

		case WM_DESTROY:						//����������Ϣ
			Direct3D_CleanUp();					//Direct3D_CleanUp����������COM�ӿڶ���
			PostQuitMessage( 0 );				//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
			break;		

		default:					
			return DefWindowProc( hwnd, message, wParam, lParam );		//��������Ĭ��
		}
	}

	return 0;		
}


//-----------------------------------��GUICallback( )������---------------------------------------
//	������GUIϵͳ�Ļص���������д��ť���º����ش������
//--------------------------------------------------------------------------------------------------
void GUICallback(int id, int state)
{
	switch(id)
	{
	case BUTTON_LOCAL_ID:   //������Ϸ��ť
		if(state == UGP_BUTTON_DOWN)
		{
			g_currentGUI = GUI_MENU_LOCAL_LOAD;
			g_LMBDown = false;
			g_MouseX  = 0;
			g_MouseY = 0;
		}
		break;

	case BUTTON_ONLINE_ID:  //������Ϸ��ť
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_MENU_CONNECT;
		break;

	case BUTTON_START_ID:	//��ʼ��Ϸ��ť
		if(state == UGP_BUTTON_DOWN)
		{
			if(!g_is3DInit)
			{
				if(!ObjectsFor3D_Init()) 
				{
					MessageBox(NULL, _T("3D��Ϸ��Դ��ʼ��ʧ��~��"), _T("������Ϣ����"), 0);
				}
				g_is3DInit = true;
			}
			g_currentGamestate = GAME_LOCAL;
		}
		break;

	case BUTTON_CONNECT_ID:  //���Ӱ�ť
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_MENU_ONLINE_LOAD;
		break;

	case BUTTON_OPTION_ID: //��Ϸѡ�ť
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_MENU_OPTION;
		break;

	case BUTTON_BACK_ID: //���ذ�ť
		if(state == UGP_BUTTON_DOWN)
			g_currentGUI = GUI_MENU;
		break;

	case BUTTON_QUIT_ID://�˳���Ϸ��ť
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

//-----------------------------------��Direct3D_Update( )������--------------------------------
//	�������������ݵĸ��£����Ǽ�ʱ��Ⱦ���뵫����Ҫ��ʱ���õģ��簴���������ĸ���
//--------------------------------------------------------------------------------------------------
void	Direct3D_GUI_Update( HWND hwnd,float fTimeDelta)
{
	while(ShowCursor(true) < 0);


}

//-----------------------------------��Direct3D_Update( )������--------------------------------
//	�������������ݵĸ��£����Ǽ�ʱ��Ⱦ���뵫����Ҫ��ʱ���õģ��簴���������ĸ���
//--------------------------------------------------------------------------------------------------
void	Direct3D_Update( HWND hwnd,float fTimeDelta)
{
	//ʹ��DirectInput���ȡ����  
	g_pDInput->GetInput();  


	// ��������������ƶ��ӽ�  
	if (g_pDInput->IsKeyDown(DIK_A))  g_pCamera->MoveAlongRightVec(-20.0f);  
	if (g_pDInput->IsKeyDown(DIK_D))  g_pCamera->MoveAlongRightVec( 20.0f);  
	if (g_pDInput->IsKeyDown(DIK_W))  g_pCamera->MoveAlongLookVec( 20.0f);  
	if (g_pDInput->IsKeyDown(DIK_S))  g_pCamera->MoveAlongLookVec(-20.0f);  
	if (g_pDInput->IsKeyDown(DIK_R))  g_pCamera->MoveAlongUpVec( 10.0f);  
	if (g_pDInput->IsKeyDown(DIK_F))  g_pCamera->MoveAlongUpVec(-10.0f);  

	//���������������ת�ӽ�  
	if (g_pDInput->IsKeyDown(DIK_LEFT)) g_pCamera->RotationUpVec(-0.003f);  
	if (g_pDInput->IsKeyDown(DIK_RIGHT))g_pCamera->RotationUpVec( 0.003f);  
	if (g_pDInput->IsKeyDown(DIK_UP))	g_pCamera->RotationRightVec(-0.003f);  
	if (g_pDInput->IsKeyDown(DIK_DOWN)) g_pCamera->RotationRightVec( 0.003f);  
	if (g_pDInput->IsKeyDown(DIK_Q))	g_pCamera->RotationLookVec(0.001f);  
	if (g_pDInput->IsKeyDown(DIK_E))	g_pCamera->RotationLookVec( -0.001f);  

	//������������������������ת  
	g_pCamera->RotationUpVec(g_pDInput->MouseDX()* 0.0003f);  
	g_pCamera->RotationRightVec(g_pDInput->MouseDY() * 0.0003f);  

	//�����ֿ��ƹ۲����������  
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

	//���㲢����ȡ���任����  
	D3DXMATRIX matView;  
	g_pCamera->CalculateViewMatrix(&matView);  
	g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView);  

	//����ȷ������任����浽g_matWorld��  
	D3DXMatrixTranslation(&g_matWorld, 0.0f, 0.0f, fPosZ);  

	/*	//����������ƶ�����
	POINT lt,rb;
	RECT rect;
	GetClientRect(hwnd,&rect);  //ȡ�ô����ڲ�����
	lt.x = rect.left;
	lt.y = rect.top;
	rb.x = rect.right;
	rb.y = rect.bottom;
	//��lt��rb�Ĵ�������ת��Ϊ��Ļ����
	ClientToScreen(hwnd,&lt);
	ClientToScreen(hwnd,&rb);
	//����Ļ���������趨��������
	rect.left = lt.x;
	rect.top = lt.y;
	rect.right = rb.x;
	rect.bottom = rb.y;
	//����������ƶ�����
	ClipCursor(&rect);
	*/
	while( ShowCursor(false) >= 0);		//���������

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

//�жϻ���ײ������
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

//-----------------------------------��Direct3D_GUI_Render( )������-------------------------------
//	������ʹ��Direct3D_GUI������Ⱦ
//--------------------------------------------------------------------------------------------------
void Direct3D_GUI_Render(HWND hwnd,float fTimeDelta)
{
	//��������
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB(100, 155, 255), 1.0f, 0);

	//��ʼ����
	g_pd3dDevice->BeginScene(); 

	//��ʽ����


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

	//-----------------------------������������Ϣ��-----------------------------
	//	HelpText_Render(hwnd);


	//��������
	g_pd3dDevice->EndScene();

	//��ʾ��ת
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);  



}

//-----------------------------------��Direct3D_Render( )������-------------------------------
//	������ʹ��Direct3D������Ⱦ
//--------------------------------------------------------------------------------------------------
void Direct3D_Render(HWND hwnd,float fTimeDelta)
{
	//��������
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER|D3DCLEAR_STENCIL, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

	//��ʼ����
	g_pd3dDevice->BeginScene();

	//��ʽ����


	//-----------------------------�����Ƶ��Ρ�-----------------------------
	g_pTerrain->RenderTerrain(&g_matWorld, false);  //��Ⱦ���Σ��ҵڶ���������Ϊfalse����ʾ����Ⱦ�����ε��߿�


	//-----------------------------�����ƹ���������------------------------
	vector<Character * > :: iterator it;
	for(it = g_vCharacter.begin();it != g_vCharacter.end();it++)
	{
		(*it)->Draw();
	}

	//���ƻ���
	vector <FireBall * >:: iterator it1;
	for(it1 = g_vFireBall.begin(); it1 != g_vFireBall.end() ;it1++)
	{
		(*it1)->Render();
	}

	/*
	DrawFrame(g_pd3dDevice, g_pFrameRoot);
	*/
	//-----------------------------�����Ƶذ塿-----------------------------
	//	D3DXMATRIX matFloor;
	///	D3DXMatrixTranslation(&matFloor, 0.0f, 0.0f, 0.0f);
	//	g_pd3dDevice->SetTransform(D3DTS_WORLD, &matFloor);
	//	g_pd3dDevice->SetStreamSource(0, g_pFloorVBuffer, 0, sizeof(CUSTOMVERTEX));
	//	g_pd3dDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
	//	g_pd3dDevice->SetTexture(0, g_pFloorTexture);
	//	g_pd3dDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);




	//-----------------------------��������ա�-----------------------------
	D3DXMATRIX matSky,matTransSky,matRotSky;
	D3DXMatrixTranslation(&matTransSky,0.0f,-13000.0f,0.0f);
	D3DXMatrixRotationY(&matRotSky, -0.00002f*timeGetTime());   //��ת�������, ��ģ���Ʋ��˶�Ч��
	matSky=matTransSky*matRotSky;
	g_pSkyBox->RenderSkyBox(&matSky, false);

	//-----------------------------������ѩ������ϵͳ��------------------------
//	g_pSnowParticles->UpdateSnowParticle(fTimeDelta);
//	g_pSnowParticles->RenderSnowParticle();

	//-----------------------------������������Ϣ��-----------------------------
	HelpText_Render(hwnd);


	//��������
	g_pd3dDevice->EndScene();   

	//��ʾ��ת
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL); 

}

//-----------------------------------��HelpText_Render( )������-------------------------------
//	��������ʾ������Ϣ�ĺ���
//--------------------------------------------------------------------------------------------------
void HelpText_Render(HWND hwnd)
{
	//����һ�����Σ����ڻ�ȡ�����ھ���
	RECT formatRect;
	GetClientRect(hwnd, &formatRect);

	//��ʾÿ��֡��
	formatRect.top = 5;
	int charCount = swprintf_s(g_strFPS, 20, _T("FPS:%0.3f"), Get_FPS() );
	g_pTextFPS->DrawText(NULL, g_strFPS, charCount , &formatRect, DT_TOP | DT_RIGHT, D3DCOLOR_RGBA(0,239,136,255));

	//��ʾ�Կ�������
	g_pTextAdaperName->DrawText(NULL,g_strAdapterName, -1, &formatRect, 
		DT_TOP | DT_LEFT, D3DXCOLOR(1.0f, 0.5f, 0.0f, 1.0f));

	//��ʾ�������Ϣ
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

	//���������Ϣ
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

//-----------------------------------��Get_FPS( )������------------------------------------------
//	��������ȡ֡����
//--------------------------------------------------------------------------------------------------
float Get_FPS()
{
	static float  fps = 0;
	static int    frameCount = 0;//֡��
	static float  currentTime =0.0f;//��ǰʱ��
	static float  lastTime = 0.0f;//�ϴ�ʱ��
	frameCount++;
	currentTime = timeGetTime()*0.001f;

	if(currentTime - lastTime > 1.0f) //��ʱ�������1����
	{
		fps = (float)frameCount /(currentTime - lastTime);//������1���ӵ�FPSֵ
		lastTime = currentTime;
		frameCount    = 0;
	}

	return fps;
}



//-----------------------------------��Direct3D_CleanUp( )������--------------------------------
//	��������Direct3D����Դ���������ͷ�COM�ӿڶ���
//---------------------------------------------------------------------------------------------------
void Direct3D_CleanUp()
{
	//�ͷ�COM�ӿڶ���
	SAFE_RELEASE(g_pd3dDevice);
	SAFE_RELEASE(g_pTextFPS)
		SAFE_RELEASE(g_pd3dDevice)
		SAFE_DELETE(g_MenuGUI)
		SAFE_DELETE(g_LocalLoadGUI)
		SAFE_DELETE(g_OnlineLoadGUI)
		SAFE_DELETE(g_ConnectGUI)
		SAFE_DELETE(g_OptionGUI)
}



