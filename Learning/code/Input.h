//=============================================================================
// Name: InputClass.h
// Des: 封装了DirectInput键盘鼠标输入处理类的头文件
//=============================================================================
#pragma once
#include "D3DUtil.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

//Input类定义开始
class Input
{
private:
	IDirectInput8					* m_pDirectInput;				//IDirectInput8接口对象
	IDirectInputDevice8				* m_KeyboardDevice;				//键盘设备接口对象
	char							m_keyBuffer[256];				//用于键盘键值存储的数组

	IDirectInputDevice8				*m_MouseDevice;					//鼠标设备接口对象
	DIMOUSESTATE					m_MouseState;					//用于鼠标键值存储的一个结构体

public:
	HRESULT		Init( HWND hWnd,HINSTANCE hInstance,DWORD keyboardCoopFlags, DWORD mouseCoopFlags ); //初始化DirectInput键盘及鼠标输入设备
	void				GetKeyboardInput();							//用于获取键盘信息
	void				GetMouseInput();							//用于获取鼠标信息
	void				GetInput();									//用于获取输入信息的函数
	bool				IsKeyDown(int iKey);						//判断键盘上某键是否按下
	bool				IsMouseButtonDown(int button);				//判断鼠标按键是否被按下
	float				MouseDX();									//返回鼠标的X轴坐标值
	float				MouseDY();									//返回鼠标的Y轴坐标值
	float				MouseDZ();									//返回鼠标的Z轴坐标值


public:
	Input(void);													//构造函数
	~Input(void);													//析构函数
};
