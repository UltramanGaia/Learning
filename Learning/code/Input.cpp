//=============================================================================
// Desc: DirectInput键盘鼠标输入处理类源文件
//=============================================================================
#include "Input.h"


//-----------------------------------------------------------------------------
// Desc: 构造函数
//-----------------------------------------------------------------------------
Input::Input()
{
	m_pDirectInput = NULL;
	m_KeyboardDevice = NULL;
	ZeroMemory(m_keyBuffer,sizeof(char)*256);
	m_MouseDevice= NULL;
	ZeroMemory(&m_MouseState, sizeof(m_MouseState));
}


//-----------------------------------------------------------------------------
// Name：Input::Init(）
// Desc: 初始化DirectInput键盘及鼠标输入设备
//-----------------------------------------------------------------------------
HRESULT Input::Init( HWND hWnd,HINSTANCE hInstance,DWORD keyboardCoopFlags, DWORD mouseCoopFlags )
{
	HRESULT hr;
	//初始化一个IDirectInput8接口对象
	HR(DirectInput8Create( hInstance, DIRECTINPUT_VERSION, 
		IID_IDirectInput8,(void**)&m_pDirectInput,NULL ));

	//进行键盘设备的初始化
	HR( m_pDirectInput->CreateDevice( GUID_SysKeyboard, &m_KeyboardDevice, NULL ));
	HR( m_KeyboardDevice->SetCooperativeLevel( hWnd, keyboardCoopFlags));
	HR( m_KeyboardDevice->SetDataFormat( &c_dfDIKeyboard ));
	HR( m_KeyboardDevice->Acquire( ));
	HR( m_KeyboardDevice->Poll( ));

	//进行鼠标设备的初始化
	HR( m_pDirectInput->CreateDevice( GUID_SysMouse, &m_MouseDevice, NULL ));
	HR( m_MouseDevice->SetCooperativeLevel( hWnd,mouseCoopFlags));
	HR( m_MouseDevice->SetDataFormat( &c_dfDIMouse ));
	HR( m_MouseDevice->Acquire( ));
	HR( m_KeyboardDevice->Poll( ));

	return S_OK;
}

void Input::GetKeyboardInput()
{
	HRESULT hr = m_KeyboardDevice->GetDeviceState(sizeof(m_keyBuffer), (void**)&m_keyBuffer); 
	//获取键盘输入消息
	if(hr)
	{
		m_KeyboardDevice->Acquire();  
		m_KeyboardDevice->GetDeviceState( sizeof(m_keyBuffer),(LPVOID)m_keyBuffer );
	}

}

void Input::GetMouseInput()
{
	HRESULT hr = m_MouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), (void**)&m_MouseState); 
	//获取鼠标输入消息
	if(hr)
	{
		m_MouseDevice->Acquire();
		m_MouseDevice->GetDeviceState( sizeof(DIMOUSESTATE), (void**)&m_MouseState);
	}
}

//-----------------------------------------------------------------------------
// Name：Input::GetInput()
// Desc: 用于获取输入信息的函数
//-----------------------------------------------------------------------------
void Input::GetInput()
{
	GetKeyboardInput();
	GetMouseInput();
}

//-----------------------------------------------------------------------------
// Name：Input::IsKeyDown()
// Desc: 判断键盘上某个键是否按下
//-----------------------------------------------------------------------------
bool Input::IsKeyDown(int iKey)
{
	if(m_keyBuffer[iKey] & 0x80)
		return true;
	else
		return false;
}


//-----------------------------------------------------------------------------
// Name：Input::IsMouseButtonDown()
// Desc: 判断鼠标上某键是否按下
//-----------------------------------------------------------------------------
bool Input::IsMouseButtonDown(int button)
{
	return (m_MouseState.rgbButtons[button] & 0x80) != 0;
}

//-----------------------------------------------------------------------------
// Name：Input::MouseDX
// Desc: 返回鼠标指针的X轴坐标值
//-----------------------------------------------------------------------------
float Input::MouseDX()
{
	return (float)m_MouseState.lX;
}

//-----------------------------------------------------------------------------
// Name：Input::MouseDY
// Desc: 返回鼠标指针的Y轴坐标值
//-----------------------------------------------------------------------------
float Input::MouseDY()
{
	return (float)m_MouseState.lY;
}

//-----------------------------------------------------------------------------
// Name：Input::MouseDZ
// Desc: 返回鼠标指针的Z轴坐标值（滚轮）
//-----------------------------------------------------------------------------
float Input::MouseDZ()
{
	return (float)m_MouseState.lZ;
}




//-----------------------------------------------------------------------------
// Desc: 析构函数
//-----------------------------------------------------------------------------
Input::~Input(void)
{
	if(m_KeyboardDevice != NULL)
		m_KeyboardDevice->Unacquire();
	if(m_MouseDevice != NULL)
		m_MouseDevice->Unacquire();
	SAFE_RELEASE(m_KeyboardDevice);
	SAFE_RELEASE(m_MouseDevice);
	SAFE_RELEASE(m_pDirectInput);
}
