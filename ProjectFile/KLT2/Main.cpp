#include <DxLib.h>

#include <math.h>
#include <algorithm>

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
}

/// <summary>
/// �v���C���[�N���X
/// </summary>
class Player final
{
public:
    Player() :
        m_playerPos({ 1920.0f / 2.0f, 1080.0f / 2.0f }),
        m_top(0.0f),
        m_bottom(0.0f),
        m_left(0.0f),
        m_right(0.0f)
    {}
    ~Player() {}

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
            m_playerPos.x += vec.x * kPlayerSpeed;
            m_playerPos.y += vec.y * kPlayerSpeed;
        }

        // �ړ��͈͂𐧌�
        {
            // ��
            m_playerPos.x = (std::max)(m_playerPos.x, m_left + kPlayerSize);

            // �E
            m_playerPos.x = (std::min)(m_playerPos.x, m_right - kPlayerSize);

            // ��
            m_playerPos.y = (std::max)(m_playerPos.y, m_top + kPlayerSize);

            // ��
            m_playerPos.y = (std::min)(m_playerPos.y, m_bottom - kPlayerSize);
        }
    }

    void Draw()
    {
        // �v���C���[�`��        
        DrawCircleAA(m_playerPos.x, m_playerPos.y, kPlayerSize, 64, 0xff0000);

    }

    // �s���͈͂̃Z�b�g
    void SetMovementRange(float top, float bottom, float left, float right)
    {
        m_top     = top;
        m_bottom  = bottom;
        m_left    = left;
        m_right   = right;
    }

private:
    // �v���C���[���W
    Vec2 m_playerPos;

    float m_top;
    float m_bottom;
    float m_right;
    float m_left;
};

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

    // �ړ��͈�
    float top    = 1080 / 2 - 300;
    float bottom = 1080 / 2 + 300;
    float left   = 1920 / 2 - 300;
    float right  = 1920 / 2 + 300;

    bool isChange = false;
    int frameCount = 0;

    // ���̂̍쐬
    Player* pPlayer = new Player();

    while (ProcessMessage() == 0)
    {
        LONGLONG  time = GetNowHiPerformanceCount();

        // ��ʂ̃N���A
        ClearDrawScreen();

        // �v���C���[����
        {
            pPlayer->Update();

            // �s���͈͂̍��W���󂯎��
            pPlayer->SetMovementRange(top, bottom, left, right);
        }

        // �v���C���[�̕`��
        {
            pPlayer->Draw();
        }   

        // �ړ��͈͂̍X�V
        {
            frameCount++;
            if (frameCount > 60)
            {
                frameCount = 0;
                isChange = !isChange;
            }

            if (!isChange)
            {
                left  -= 3.0f;
                right += 3.0f;
            }
            else
            {
                left  += 3.0f;
                right -= 3.0f;
            }
        }

        // �s���͈͂̕`��
        {
            // �g�̌���
            for (int i = 0; i < kThick; i++)
            {
                // �ړ��͈͕`��
                DrawBox(
                    left - i,
                    top - i,
                    right + i,
                    bottom + i,
                    0xffffff,
                    false);
            }
        }

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

    // �������
    delete pPlayer;

    // �c�w���C�u�����g�p�̏I������
    DxLib_End();

    // �\�t�g�̏I�� 
    return 0;
}