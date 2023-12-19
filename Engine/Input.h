#pragma once

#include <dInput.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dInput8.lib")

#define SAFE_RELEASE(p) if(p != nullptr){ p->Release(); p = nullptr;}

namespace Input
{
	//������
	HRESULT Initialize(HWND hWnd);

	//�X�V
	void Update();

	//�L�[��������Ă��邩���ׂ�
	bool IsKey(int keyCode);

	//�L�[���������������ׂ�i�������ςȂ��͖����j
	bool IsKeyDown(int keyCode);

	//�L�[���������������ׂ�
	bool IsKeyUp(int keyCode);

	//�������
	void Release();
};