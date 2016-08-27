//=============================================================================
// Name: InputClass.h
//	Des: ��װ��DirectInput����������봦�����ͷ�ļ�
//=============================================================================
#pragma once
#include "D3DUtil.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

//Input�ඨ�忪ʼ
class Input
{
private:
	IDirectInput8					* m_pDirectInput;				//IDirectInput8�ӿڶ���
	IDirectInputDevice8				* m_KeyboardDevice;				//�����豸�ӿڶ���
	char							m_keyBuffer[256];				//���ڼ��̼�ֵ�洢������

	IDirectInputDevice8				*m_MouseDevice;					//����豸�ӿڶ���
	DIMOUSESTATE					m_MouseState;					//��������ֵ�洢��һ���ṹ��

public:
	HRESULT		Init( HWND hWnd,HINSTANCE hInstance,DWORD keyboardCoopFlags, DWORD mouseCoopFlags ); //��ʼ��DirectInput���̼���������豸
	void				GetKeyboardInput();							//���ڻ�ȡ������Ϣ
	void				GetMouseInput();							//���ڻ�ȡ�����Ϣ
	void				GetInput();									//���ڻ�ȡ������Ϣ�ĺ���
	bool				IsKeyDown(int iKey);						//�жϼ�����ĳ���Ƿ���
	bool				IsMouseButtonDown(int button);				//�ж���갴���Ƿ񱻰���
	float				MouseDX();									//��������X������ֵ
	float				MouseDY();									//��������Y������ֵ
	float				MouseDZ();									//��������Z������ֵ


public:
	Input(void);												//���캯��
	~Input(void);												//��������
};