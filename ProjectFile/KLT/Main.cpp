#include <DxLib.h>

#include <math.h>
#include <algorithm>

//////////////////////
// �G�l�~�[�p�^�[�� //
//////////////////////
// 0 �ːi
// 1 �ǐ�
#define ENEMYPATTERN 1;

// �񎟌��x�N�g��
struct Vec2
{
    float x = 0.0f;
    float y = 0.0f;
};

namespace
{
    // �v���C���[���x
    constexpr float kPlayerSpeed = 10.0f;

    // �v���C���[�T�C�Y
    constexpr float kPlayerSize = 10.0f;

    // �g�̑傫��
    constexpr int kThick = 30;


    // �G�l�~�[���x
    constexpr float kEnemySpeed = 5.0f;
}

namespace
{
    // �v���C���[���W
    Vec2 playerPos{ 1920.0f / 2.0f, 1080.0f / 2.0f };

    // �G�l�~�[���W
    Vec2 enemyPos{ 0.0f,0.0f };

    Vec2 enemyComponents{ 0.0f,0.0f };
    Vec2 enemyVec{ 0.0f,0.0f };

#if !ENEMYPATTERN

    bool isOneFrameVec = true;

#endif

}

// �X�V
void Update();
// �`��
void Draw();

// �v���O������WinMain����n�܂�܂�
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    // window���[�h�ݒ�
    ChangeWindowMode(true);

    // ��ʃT�C�Y�̐ݒ�
    SetGraphMode(1920, 1080, 32);

    // ���O�o�͂��s�����ۂ�
    SetOutApplicationLogValidFlag(false);

    // �_�u���o�b�t�@���[�h
    SetDrawScreen(DX_SCREEN_BACK);

    // �c�w���C�u��������������
    if (DxLib_Init() == -1)
    {
        // �G���[���N�����璼���ɏI��
        return -1;
    }
  

    while (ProcessMessage() == 0)
    {
        LONGLONG  time = GetNowHiPerformanceCount();

        // ��ʂ̃N���A
        ClearDrawScreen();

        // �X�V
        Update();

        // �`��
        Draw();

        // ����ʂ�\��ʂ����ւ���
        ScreenFlip();

        // esc�L�[����������I������
        if (CheckHitKey(KEY_INPUT_ESCAPE))
        {
            break;
        }

        // fps��60�ɌŒ�
        while (GetNowHiPerformanceCount() - time < 16667)
        {
        }
    }

    // �c�w���C�u�����g�p�̏I������
    DxLib_End();

    // �\�t�g�̏I�� 
    return 0;
}

// �X�V
void Update()
{
    // �v���C���[����
    {
        // �x�N�g���̏�����
        Vec2 vec{ 0.0f,0.0f };

        // �ړ��x�N�g���𐶐�
        if (CheckHitKey(KEY_INPUT_W))
        {
            vec.y -= 1.0f;
        }
        if (CheckHitKey(KEY_INPUT_S))
        {
            vec.y += 1.0f;
        }
        if (CheckHitKey(KEY_INPUT_A))
        {
            vec.x -= 1.0f;                    
        }
        if (CheckHitKey(KEY_INPUT_D))
        {
            vec.x += 1.0f;
        }

        // �x�N�g���̒������v�Z
        const float length = sqrtf(vec.x * vec.x + vec.y * vec.y);

        // ������0�łȂ��ꍇ�̂݃m�[�}���C�Y
        if (length > 0.0f)
        {
            vec.x /= length;
            vec.y /= length;
        }

        // �ړ��x�N�g���ɑ��x��������
        playerPos.x += vec.x * kPlayerSpeed;
        playerPos.y += vec.y * kPlayerSpeed;
    }

    // �ړ��͈͂𐧌�
    {
        // ��
        playerPos.x = (std::max)(playerPos.x, (1920.0f / 2.0f - 300.0f) + kPlayerSize);

        // �E
        playerPos.x = (std::min)(playerPos.x, (1920.0f / 2.0f + 300.0f) - kPlayerSize);

        // ��
        playerPos.y = (std::max)(playerPos.y, (1080.0f / 2.0f - 300.0f) + kPlayerSize);

        // ��
        playerPos.y = (std::min)(playerPos.y, (1080.0f / 2.0f + 300.0f) - kPlayerSize);
    }

    // �G�l�~�[����
    {

#if ENEMYPATTERN
        // �ǐ�
        {
            Vec2 components{ 0.0f,0.0f };
            Vec2 vec{ 0.0f,0.0f };

            //�����v�Z
            components.x = playerPos.x - enemyPos.x;
            components.y = playerPos.y - enemyPos.y;

            //�傫���v�Z
            const float length = sqrtf(components.x * components.x + components.y * components.y);

            // ������0�łȂ��ꍇ�̂݃m�[�}���C�Y
            if (length > 0.0f)
            {
                //�����v�Z�i���K���j
                vec.x = components.x / length;
                vec.y = components.y / length;
            }

            //�ړ���̈ʒu���v�Z
            enemyPos.x += vec.x * kEnemySpeed;
            enemyPos.y += vec.y * kEnemySpeed;
        }

#else
        // �ˌ�
        {
            // 1�t���[�������v���C���[�̍��W���v�Z
            if (isOneFrameVec)
            {

                isOneFrameVec = false;

                //�����v�Z
                enemyComponents.x = playerPos.x - enemyPos.x;
                enemyComponents.y = playerPos.y - enemyPos.y;

                //�傫���v�Z
                const float length = sqrtf(enemyComponents.x * enemyComponents.x + enemyComponents.y * enemyComponents.y);

                // ������0�łȂ��ꍇ�̂݃m�[�}���C�Y
                if (length > 0.0f)
                {
                    //�����v�Z�i���K���j
                    enemyVec.x = enemyComponents.x / length;
                    enemyVec.y = enemyComponents.y / length;
                }
            }

            //�ړ���̈ʒu���v�Z
            enemyPos.x += enemyVec.x * kEnemySpeed;
            enemyPos.y += enemyVec.y * kEnemySpeed;
        }

#endif
    }
}

// �`��
void Draw()
{
    // �v���C���[����
    {
        // �v���C���[�`��
        {
            DrawCircleAA(playerPos.x, playerPos.y, kPlayerSize, 64, 0xff0000);
        }

        // �s���͈͂̕`��
        {
            // �g�̌���
            for (int i = 0; i < kThick; i++)
            {
                // �ړ��͈͕`��
                DrawBox(
                    1920 / 2 - 300 - i,
                    1080 / 2 - 300 - i,
                    1920 / 2 + 300 + i,
                    1080 / 2 + 300 + i,
                    0xffffff,
                    false);
            }
        }
    }

    // �G�l�~�[����
    {
        // �G�l�~�[�̕`��
        {
            DrawBox(
                enemyPos.x - 50.0f,
                enemyPos.y - 50.0f,
                enemyPos.x + 50.0f,
                enemyPos.y + 50.0f,
                0xffffff,
                true);
        }
    }
}