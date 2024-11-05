#include <string>
#include <stdio.h>
#include <vector>

#include <DxLib.h>

bool IsCheckSquare(int UpX, int UpY, int DownX, int DownY, int UpX2, int UpY2, int DownX2, int DownY2);

// �Q�������W
struct Vec2
{
    float x;
    float y;
};

// �l�p�`
struct Rect
{
    float top;
    float bottom;
    float left;
    float right;
};

// �摜�ۑ��f�[�^
struct GraphicData
{
    Rect rect;      // �摜�T�C�Y
    Vec2 centerPos; // �Z���^�[���W
    Vec2 size;      // �T�C�Y���L�^����
    Vec2 movePos;   // �ړ���
    int hGraph;     // �摜�n���h��
};

struct MouseData
{
    Rect rect;      // �T�C�Y
    Vec2 centerPos; // �Z���^�[���W
};

namespace
{
    // �t�@�C���̖��O�擾
    char filepath[256];

    // �摜�n���h��
    std::vector<GraphicData> graphData;

    // �}�E�X�f�[�^
    MouseData mouseRectPos{};

    // �I�t�Z�b�g���L�^����ϐ�
    Vec2 offset;
    bool isDragging = false;
}

void Update()
{
    // �}�E�X�̍��W
    int x = 0;
    int y = 0;
    GetMousePoint(&x, &y);
    mouseRectPos =
    {
        Rect
        {
            static_cast<float>(y) - 1,
            static_cast<float>(y) + 1,
            static_cast<float>(x) - 1,
            static_cast<float>(x) + 1
        },
        Vec2
        {
            static_cast<float>(x),
            static_cast<float>(y)
        }
    };

    if (!GetDragFilePath(filepath))
    {
        GraphicData data{};
        data.hGraph = LoadGraph(filepath);
        data.centerPos.x = 1920.0f / 2.0f;
        data.centerPos.y = 1080.f  / 2.0f;
        float x = 0.0f;
        float y = 0.0f;
        GetGraphSizeF(data.hGraph, &x, &y);
        data.size.x = x;
        data.size.y = y;
        data.rect.left   = data.centerPos.x - (x / 2.0f);
        data.rect.top    = data.centerPos.y - (y / 2.0f);
        data.rect.right  = data.centerPos.x + (x / 2.0f);
        data.rect.bottom = data.centerPos.y + (y / 2.0f);
        graphData.push_back(data);
    }

    for (auto& graph : graphData)
    {
        graph.rect.left   = graph.centerPos.x - (graph.size.x / 2);
        graph.rect.top    = graph.centerPos.y - (graph.size.y / 2);
        graph.rect.right  = graph.centerPos.x + (graph.size.x / 2);
        graph.rect.bottom = graph.centerPos.y + (graph.size.y / 2);

        // �摜�ƃ}�E�X�̔���
        if (IsCheckSquare(
            mouseRectPos.rect.left, mouseRectPos.rect.top, mouseRectPos.rect.right, mouseRectPos.rect.bottom,
            graph.rect.left, graph.rect.top, graph.rect.right, graph.rect.bottom))
        {
            // �}�E�X�N���b�N�J�n���ɃI�t�Z�b�g���v�Z
            if ((GetMouseInput() & MOUSE_INPUT_LEFT))
            {
                // �N���b�N�������߂ɉ摜�ƃ}�E�X�̈ʒu�̌v�Z������
                if (!isDragging)
                {
                    isDragging = true;
                    offset.x = mouseRectPos.centerPos.x - graph.centerPos.x;
                    offset.y = mouseRectPos.centerPos.y - graph.centerPos.y;
                }
                else
                {
                    // �ʒu��ύX����
                    graph.centerPos.x = mouseRectPos.centerPos.x - offset.x;
                    graph.centerPos.y = mouseRectPos.centerPos.y - offset.y;
                }
            }
            else
            {
                // �}�E�X�{�^���𗣂�����h���b�O�I��
                isDragging = false;
            }
        }
    }
}

void Draw()
{
    // �}�E�X�̔���
    DrawBox(
        mouseRectPos.rect.left,
        mouseRectPos.rect.top,
        mouseRectPos.rect.right,
        mouseRectPos.rect.bottom,
        0xffffff,false);

    // �摜�`��
    for (auto& graph : graphData)
    {
        DrawExtendGraphF(
            graph.rect.left,
            graph.rect.top,
            graph.rect.right,
            graph.rect.bottom,
            graph.hGraph,true);
    }
}

bool IsCheckSquare(int UpX, int UpY, int DownX, int DownY,
    int UpX2, int UpY2, int DownX2, int DownY2)
{
    if (DownX < UpX2) return false;
    if (UpX > DownX2) return false;
    if (DownY < UpY2) return false;
    if (UpY > DownY2) return false;

    return true;
}

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
    
    //�h���b�O���h���b�v������
    SetDragFileValidFlag(true);

    while (ProcessMessage() == 0)
    {
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
    }

    // �c�w���C�u�����g�p�̏I������
    DxLib_End();

    // �\�t�g�̏I�� 
    return 0;
}