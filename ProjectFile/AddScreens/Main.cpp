
#include "AddScreen.h"

#include <DxLib.h>

// �v���O������WinMain����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// window���[�h�ݒ�
	ChangeWindowMode(true);
	// �E�C���h�E���ݒ�
	SetMainWindowText("AddScreens");
	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(1920, 1080, 32);
	// window���[�h����window�T�C�Y��ύX���邩�ǂ���
	SetWindowSizeChangeEnableFlag(true);
	// ���O�e�L�X�g���쐬���邩�ǂ���
	SetOutApplicationLogValidFlag(false);

	// �_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	// �c�w���C�u��������������
	if (DxLib_Init() == -1)
	{
		// �G���[���N�����璼���ɏI��
		return -1;
	}

	//�Q�[�����ɃE�B���h�E���[�h��؂�ւ��Ă��O���t�B�b�N�X�n���h����
	//���Z�b�g���Ȃ�
	// �t���X�N���[���E�C���h�E�̐؂�ւ��Ń��\�[�X��������̂�h���B
	// Effekseer���g�p����ꍇ�͕K���ݒ肷��B
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	AddScreen* screen = new AddScreen();

	screen->SetScreen(1920, 1080);

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();

		// ��ʂ̃N���A
		ClearDrawScreen();

		static float angle = 0.0f;

		angle++;

		screen->CreateScreen();
		DrawBox(300, 300, 600, 600, 0xffffff, false);
		DrawString(1920 / 2, 1080 / 2, "hogehoge", 0xffffff);
		screen->RenderExtendScreen(0, 0, 1000, 1000);
	//	screen->RenderRotaScreen(1920 / 2, 1080 / 2, 1, angle);

		// ����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	delete screen;

	// �c�w���C�u�����g�p�̏I������
	DxLib_End();

	// �\�t�g�̏I�� 
	return 0;
}