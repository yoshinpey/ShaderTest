#include "Input.h"

namespace Input
{
	LPDIRECTINPUT8   pDInput = nullptr;			// DirectInput�I�u�W�F�N�g
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;	// �L�[�{�[�h�f�o�C�X�I�u�W�F�N�g
	BYTE keyState[256] = { 0 };					// ���݂̊e�L�[�̏��
	BYTE prevKeyState[256];						// �O�t���[���ł̊e�L�[�̏��

	HRESULT Initialize(HWND hWnd)
	{
		HRESULT hr = 0;
		//DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
		if (FAILED(hr))
		{
			MessageBox(NULL, "DirectInput�I�u�W�F�N�g�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
			return hr;
		}

		//�L�[�{�[�h�f�o�C�X�̍쐬
		hr = pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		if (FAILED(hr))
		{
			MessageBox(NULL, "�L�[�{�[�h�f�o�C�X�̍쐬�Ɏ��s���܂���", "�G���[", MB_OK);
			return hr;
		}

		//�f�[�^�t�H�[�}�b�g�̐ݒ�
		hr = pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		if (FAILED(hr))
		{
			MessageBox(NULL, "�f�[�^�t�H�[�}�b�g�̐ݒ�Ɏ��s���܂���", "�G���[", MB_OK);
			return hr;
		}

		//�������x���̐ݒ�
		hr = pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
		if (FAILED(hr))
		{
			MessageBox(NULL, "�������x���̐ݒ�Ɏ��s���܂���", "�G���[", MB_OK);
			return hr;
		}
		return S_OK;
	}

	void Update()
	{
		//�L�[�{�[�h�f�o�C�X�̎擾
		pKeyDevice->Acquire();

		//�O�t���[���̃L�[��Ԃ�ۑ�
		memcpy(prevKeyState, keyState, sizeof(keyState));

		//���݂̃L�[��Ԃ̎擾
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
	}

	bool IsKey(int keyCode)
	{
		//�w�肳�ꂽ�L�[�R�[�h�̃L�[��������Ă��邩�`�F�b�N
		if (keyState[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		//�L�[�����݉�����Ă���A�O��̃t���[���ł͉�����Ă��Ȃ����`�F�b�N
		if (IsKey(keyCode) && !(prevKeyState[keyCode] & 0x80))
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		//�L�[�����݉�����Ă��炸�A�O��̃t���[���ł͉�����Ă������`�F�b�N
		if (!IsKey(keyCode) && prevKeyState[keyCode] & 0x80)
		{
			return true;
		}
		return false;
	}

	void Release()
	{
		//���\�[�X�̉��
		SAFE_RELEASE(pDInput);
		SAFE_RELEASE(pKeyDevice);
	}
}

