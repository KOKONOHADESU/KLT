#pragma once

#include <DxLib.h>

// 
class AddScreen
{
public:
	AddScreen():
		m_hScreen(-1)
	{};
	~AddScreen()
	{
		// �������
		DeleteGraph(m_hScreen);
	};

	// �O���t�B�b�N�n���h���ɂ����ʔ͈͂��w��
	void SetScreen(int screenWidth , int screenHeight)
	{
		// �X�N���[���n���h���̍쐬
		m_hScreen = MakeScreen(screenWidth, screenHeight);
	}

	// ��ʂ��쐬����
	void CreateScreen()
	{
		// ��ʂ̍쐬
		SetDrawScreen(m_hScreen);

		// ��ʂ��N���A�ɂ���
		ClearDrawScreen();
	}	

	// ��ʕ`��
	void RenderExtendScreen(float left , float top , float right , float bottom)
	{
		// ��ʂ̐؂�ւ�
		SetDrawScreen(DX_SCREEN_BACK);

		DrawExtendGraphF(left, top, right, bottom, m_hScreen, true);
	}

	// ��ʕ`��
	void RenderRotaScreen(float centerX, float centerY , float size , float angle)
	{
		// ��ʂ̐؂�ւ�
		SetDrawScreen(DX_SCREEN_BACK);

		DrawRotaGraphF(centerX, centerY, size, angle, m_hScreen, true);
	}

private:

	// ��ʃn���h��
	int m_hScreen;

};
