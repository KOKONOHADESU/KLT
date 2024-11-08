#include <string>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <DxLib.h>

#include "DxLibKeyFresh.h"
#include "DxLibMouseFresh.h"
#include "Vec2.h"

namespace
{
    // �摜�����L�΂�����̃T�C�Y
    constexpr float kGraphChangeSize = 5.0f;

    // ��ʊO�ɉ摜���o���ꍇ
    constexpr float kGraphOutSideSize = 1.0f;

    // ���Z�b�g�{�^���̃T�C�Y
    constexpr float kResetBottonSize = 30.0f;
}

bool IsCheckSquare(float UpX, float UpY, float DownX, float DownY, float UpX2, float UpY2, float DownX2, float DownY2);

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
    bool isMain = false; // 
    Rect rect{ 0.0f,0.0f ,0.0f ,0.0f };      // �摜�T�C�Y
    Vec2<float> changeSize{ 0.0f,0.0f }; // �ύX�����T�C�Y
    float changeSizeXY = 0.0f;
    Vec2<float> tempClickPos{ 0.0f,0.0f }; // �ꎞ�I�Ɉʒu���L�^����
    Vec2<float> tempClickPosXY{ 0.0f,0.0f }; // �ꎞ�I�Ɉʒu���L�^����
    Vec2<float> tempChangeSize{ 0.0f,0.0f };
    Vec2<float> tempChangeSizeXY{ 0.0f,0.0f };
    Vec2<float> centerPos{ 0.0f,0.0f }; // �Z���^�[���W
    Vec2<float> size{ 0.0f,0.0f };      // �T�C�Y���L�^����
    Vec2<float> movePos{ 0.0f,0.0f };   // �ړ���
    Vec2<float> offset{ 0.0f,0.0f };    // �I�t�Z�b�g���L�^����ϐ�   
    Rect resetBotton{ 0.0f,0.0f ,0.0f ,0.0f }; // ���Z�b�g�{�^��
    bool isDragging = false;
    bool isChangeSizeDraggingX = false;
    bool isChangeSizeDraggingY = false;
    bool isChangeSizeDraggingXY = false;
    bool isGraphClick = false;
    bool isMoving = false;
    bool isChangeSizeClickX = false;
    bool isChangeSizeClickY = false;
    bool isChangeSizeClickXY = false;
    bool isChangeSizeY = false;
    Rect changeSizeCollX{ 0.0f,0.0f ,0.0f ,0.0f }; // �T�C�Y��ύX����ׂ̔���
    Rect changeSizeCollY{ 0.0f,0.0f ,0.0f ,0.0f }; // �T�C�Y��ύX����ׂ̔���
    Rect changeSizeCollXY{ 0.0f,0.0f ,0.0f ,0.0f }; // �T�C�Y��ύX����ׂ̔���
    bool isCheckMouse = false; // �}�E�X�J�[�\���ɓ������Ă��邩�ǂ���
    bool isCheckColl = false;
    bool isCheckSizeCollX = false;
    bool isCheckSizeCollY = false;
    bool isCheckSizeCollXY = false;
    bool isResetBottonColl = false;
    int hGraph = -1;     // �摜�n���h��
};

// �}�E�X�f�[�^
struct MouseData
{
    Rect rect{ 0.0f,0.0f, 0.0f, 0.0f };      // �T�C�Y
    Vec2<float> centerPos{ 0.0f,0.0f }; // �Z���^�[���W
};

namespace
{
    // �t�@�C���̖��O�擾
    char filepath[256];

    // �摜�n���h��
    std::vector<GraphicData> graphData;

    // �}�E�X�f�[�^
    MouseData mouseRectPos{};

    // �f�[�^��`�悷�邩�ǂ���1
    bool isUIDrawer = true;

    bool isScreenLeftHit   = false;
    bool isScreenTopHit    = false;
    bool isScreenRightHit  = false;
    bool isScreenBottomHit = false;
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
        Vec2<float>
        {
            static_cast<float>(x),
            static_cast<float>(y)
        }
    };

    if (!GetDragFilePath(filepath))
    {
        // �摜�f�[�^�̒ǉ�
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

        // �h���b�O�t�@�C���f�[�^�̃N���A
        DragFileInfoClear();
    }    

    for (auto& graph : graphData)
    {
        // ���W�f�[�^�̍X�V
        {
            // ���S���W����Ƃ��ĉ摜�̃T�C�Y���v�Z����
            graph.rect.left   = graph.centerPos.x - (graph.size.x / 2.0f);
            graph.rect.top    = graph.centerPos.y - (graph.size.y / 2.0f);
            graph.rect.right  = graph.centerPos.x + (graph.size.x / 2.0f) + graph.changeSize.x;
            graph.rect.bottom = graph.centerPos.y + (graph.size.y / 2.0f) + graph.changeSize.y;

            // X �T�C�Y��ύX����ׂ̔�����W
            graph.changeSizeCollX.left   = graph.rect.right - kGraphChangeSize;
            graph.changeSizeCollX.top    = graph.rect.top;
            graph.changeSizeCollX.right  = graph.rect.right;
            graph.changeSizeCollX.bottom = graph.rect.bottom;

            // Y �T�C�Y��ύX����ׂ̔�����W
            graph.changeSizeCollY.left    = graph.rect.left;
            graph.changeSizeCollY.top     = graph.rect.bottom - kGraphChangeSize;
            graph.changeSizeCollY.right   = graph.rect.right;
            graph.changeSizeCollY.bottom  = graph.rect.bottom;

            // XY �T�C�Y��ύX����ׂ̔�����W
            graph.changeSizeCollXY.left   = graph.rect.right - kGraphChangeSize;
            graph.changeSizeCollXY.top    = graph.rect.bottom - kGraphChangeSize;
            graph.changeSizeCollXY.right  = graph.rect.right;
            graph.changeSizeCollXY.bottom = graph.rect.bottom;

            graph.resetBotton.left   = graph.rect.right      - kGraphChangeSize - kGraphChangeSize - kResetBottonSize;
            graph.resetBotton.top    = graph.rect.top        + kGraphChangeSize + kGraphChangeSize;
            graph.resetBotton.right  = graph.rect.right      - kGraphChangeSize - kGraphChangeSize;
            graph.resetBotton.bottom = graph.resetBotton.top + kResetBottonSize;
        }

        // �摜�ƃ}�E�X�̔���
        if (IsCheckSquare(
            mouseRectPos.rect.left, mouseRectPos.rect.top, mouseRectPos.rect.right, mouseRectPos.rect.bottom,
            graph.rect.left, graph.rect.top, graph.rect.right, graph.rect.bottom))
        {
            graph.isCheckColl = true;
            // �}�E�X�ɐG��Ă���摜
            graph.isCheckMouse = true;
        }
        else
        {
            graph.isCheckMouse = false;
        }

        // �T�C�Y��ύX����ׂ�
        if (IsCheckSquare(
            mouseRectPos.rect.left, mouseRectPos.rect.top, mouseRectPos.rect.right, mouseRectPos.rect.bottom,
            graph.changeSizeCollX.left, graph.changeSizeCollX.top, graph.changeSizeCollX.right, graph.changeSizeCollX.bottom))
        {            
            // �����蔻��m�F
            graph.isCheckSizeCollX = true;  
        }
        
        if (IsCheckSquare(
            mouseRectPos.rect.left, mouseRectPos.rect.top, mouseRectPos.rect.right, mouseRectPos.rect.bottom,
            graph.changeSizeCollY.left, graph.changeSizeCollY.top, graph.changeSizeCollY.right, graph.changeSizeCollY.bottom))
        {
            // �����蔻��m�F
            graph.isCheckSizeCollY = true;            
        }

        if (IsCheckSquare(
            mouseRectPos.rect.left, mouseRectPos.rect.top, mouseRectPos.rect.right, mouseRectPos.rect.bottom,
            graph.changeSizeCollXY.left, graph.changeSizeCollXY.top, graph.changeSizeCollXY.right, graph.changeSizeCollXY.bottom))
        {
            // �����蔻��m�F
            graph.isCheckSizeCollXY = true;
        }

        // ���Z�b�g�{�^���̊m�F
        if (IsCheckSquare(
            mouseRectPos.rect.left, mouseRectPos.rect.top, mouseRectPos.rect.right, mouseRectPos.rect.bottom,
            graph.resetBotton.left, graph.resetBotton.top, graph.resetBotton.right, graph.resetBotton.bottom))
        {
            graph.isResetBottonColl = true;
        }
        
        // �}�E�X�N���b�N
        {
            // �}�E�X�N���b�N�J�n���ɃI�t�Z�b�g���v�Z
            if (MOUSE::DxLibMouseFresh::GetInstance()->IsPress(MOUSE_INPUT_LEFT) &&
                graph.isCheckColl)
            {
                // �N���b�N���Ă���
                graph.isGraphClick = true;

                // �摜�𓮂���
                graph.isMoving = true;
            }
            else
            {
                // �}�E�X�{�^���𗣂�����h���b�O�I��
                graph.isDragging = false;

                // �N���b�N���Ă��Ȃ�
                graph.isGraphClick = false;

                // �G��Ă��Ȃ��摜
                graph.isCheckColl = false;
            }

            // �T�C�Y�ύX������ꍇ
            if (MOUSE::DxLibMouseFresh::GetInstance()->IsPress(MOUSE_INPUT_LEFT) &&
                graph.isCheckSizeCollX &&
                !graph.isCheckSizeCollY)
            {
                // �摜�𓮂����Ȃ�
                graph.isMoving = false;

                graph.isChangeSizeClickX = true;

                // �}�E�X�ɐG��Ă���摜
                graph.isCheckMouse = true;
            }
            else
            {
                graph.isChangeSizeClickX = false;
                graph.isChangeSizeDraggingX = false;
                graph.tempClickPos.x = 0.0f;
                graph.tempChangeSize.x = 0.0f;

                // �����蔻��m�F
                graph.isCheckSizeCollX = false;
            }

            // �T�C�Y�ύX������ꍇ
            if (MOUSE::DxLibMouseFresh::GetInstance()->IsPress(MOUSE_INPUT_LEFT) &&
                graph.isCheckSizeCollY &&
                !graph.isCheckSizeCollX)
            {
                // �摜�𓮂����Ȃ�
                graph.isMoving = false;

                graph.isChangeSizeClickY = true;

                // �}�E�X�ɐG��Ă���摜
                graph.isCheckMouse = true;
            }
            else
            {
                graph.isChangeSizeClickY = false;
                graph.isChangeSizeDraggingY = false;
                graph.tempClickPos.y = 0.0f;
                graph.tempChangeSize.y = 0.0f;

                // �����蔻��m�F
                graph.isCheckSizeCollY = false;
            }


            // �T�C�Y�ύX������ꍇ
            if (MOUSE::DxLibMouseFresh::GetInstance()->IsPress(MOUSE_INPUT_LEFT) &&
                graph.isCheckSizeCollXY)
            {
                // �摜�𓮂����Ȃ�
                graph.isMoving = false;

                graph.isChangeSizeClickXY = true;

                // �}�E�X�ɐG��Ă���摜
                graph.isCheckMouse = true;
            }
            else
            {
                graph.isChangeSizeClickXY = false;
                graph.isChangeSizeDraggingXY = false;
                graph.tempClickPosXY.x = 0.0f;
                graph.tempClickPosXY.y = 0.0f;
                graph.tempChangeSizeXY.x = 0.0f;
                graph.tempChangeSizeXY.y = 0.0f;

                // �����蔻��m�F
                graph.isCheckSizeCollXY = false;
            }

            // ���Z�b�g����ꍇ
            if (MOUSE::DxLibMouseFresh::GetInstance()->IsPress(MOUSE_INPUT_LEFT) &&
                graph.isResetBottonColl)
            {
                graph.changeSize.x = 0.0f;
                graph.changeSize.y = 0.0f;

                graph.isResetBottonColl = false;
            }
        }
    }{}

    // �z��̏��Ԃ�ς����������̂����͌�������邢
    // �C���������璼��()
    {
        // �z�񂪂Ȃ��ꍇ�͏��������Ȃ�
        if (graphData.size() != 0)
        {
            // �z��̍ő吔����v�f�̊m�F
            for (int i = static_cast<int>(graphData.size()) - 1; i >= 0; i--)
            {
                // �N���b�N���ꂽ�v�f��T��
                if (graphData[i].isCheckColl)
                {
                    graphData[i].isMain = true;
                    break;
                }
                else
                {
                    graphData[i].isMain = false;
                }
            }

            GraphicData tempData;
            for (int i = 0; i < graphData.size(); i++)
            {
                // �N���b�N����Ă��Ȃ��v�f���m�F
                if (!graphData[i].isMain)
                {
                    graphData[i].isMoving = false;
                    graphData[i].isCheckColl = false;
                }
                else
                {
                    // �N���b�N����Ă���v�f�̃R�s�[
                    tempData = graphData[i];
                    graphData[i] = {};
                }
            }
            // �R�s�[�����v�f���Ō�̗v�f�ɂ���
            graphData.push_back(tempData);
        }
    }
    
    for (auto& graph : graphData)
    {
        // �摜�ʒu�̕ύX
        {
            // �摜���N���b�N���Ă���ꍇ
            // �N���b�N�������߂ɉ摜�ƃ}�E�X�̈ʒu�̌v�Z������
            if (graph.isGraphClick && !graph.isDragging)
            {
                graph.isDragging = true;
                graph.offset.x = mouseRectPos.centerPos.x - graph.centerPos.x;
                graph.offset.y = mouseRectPos.centerPos.y - graph.centerPos.y;
            }

            // �ʒu��ύX����
            // �}�E�X���h���b�O���Ă��邩
            // ���������肪�Ƃ�Ă��邩
            if (graph.isDragging && graph.isMoving)
            {
                graph.centerPos.x = mouseRectPos.centerPos.x - graph.offset.x;
                graph.centerPos.y = mouseRectPos.centerPos.y - graph.offset.y;
            }
        }

        if (graph.isMain && isUIDrawer)
        {
            // �摜�T�C�Y�̕ύX
            {
                // �T�C�Y�ύX���邽�߂ɃN���b�N���Ă����ꍇ
                if (graph.isChangeSizeClickX)
                {
                    // �h���b�O���Ă��Ȃ��ꍇ
                    if (!graph.isChangeSizeDraggingX)
                    {
                        graph.isChangeSizeDraggingX = true;

                        // �}�E�X�̃N���b�N���W���L�^����
                        graph.tempClickPos.x = mouseRectPos.centerPos.x;

                        graph.tempChangeSize.x = graph.changeSize.x;
                    }

                    // �h���b�O���Ă���ꍇ
                    if (graph.isChangeSizeDraggingX)
                    {
                        // �}�E�X��X���W�ړ��ʂ��v�Z
                        const float range = graph.tempClickPos.x - mouseRectPos.centerPos.x;

                        // �ړ��ʂ����̂܂ܓK�p���邱�ƂŁA�g��E�k�����\��
                        graph.changeSize.x = graph.tempChangeSize.x - range;
                    }
                }

                // �T�C�Y�ύX���邽�߂ɃN���b�N���Ă����ꍇ
                if (graph.isChangeSizeClickY)
                {
                    // �h���b�O���Ă��Ȃ��ꍇ
                    if (!graph.isChangeSizeDraggingY)
                    {
                        graph.isChangeSizeDraggingY = true;

                        // �}�E�X�̃N���b�N���W���L�^����
                        graph.tempClickPos.y = mouseRectPos.centerPos.y;

                        graph.tempChangeSize.y = graph.changeSize.y;
                    }

                    // �h���b�O���Ă���ꍇ
                    if (graph.isChangeSizeDraggingY)
                    {
                        // �}�E�X��X���W�ړ��ʂ��v�Z
                        const float range = graph.tempClickPos.y - mouseRectPos.centerPos.y;

                        // �ړ��ʂ����̂܂ܓK�p���邱�ƂŁA�g��E�k�����\��
                        graph.changeSize.y = graph.tempChangeSize.y - range;
                    }
                }

                // �T�C�Y�ύX���邽�߂ɃN���b�N���Ă����ꍇ
                if (graph.isChangeSizeClickXY)
                {
                    // �h���b�O���Ă��Ȃ��ꍇ
                    if (!graph.isChangeSizeDraggingXY)
                    {
                        graph.isChangeSizeDraggingXY = true;

                        // �}�E�X�̃N���b�N���W���L�^����
                        graph.tempClickPosXY.x = mouseRectPos.centerPos.x;
                        graph.tempClickPosXY.y = mouseRectPos.centerPos.y;

                        graph.tempChangeSizeXY.x = graph.changeSize.x;
                        graph.tempChangeSizeXY.y = graph.changeSize.y;
                    }

                    // �h���b�O���Ă���ꍇ
                    if (graph.isChangeSizeDraggingXY)
                    {
                        // �}�E�X��X���W�ړ��ʂ��v�Z
                        const float rangeX = graph.tempClickPosXY.x - mouseRectPos.centerPos.x;
                        const float rangeY = graph.tempClickPosXY.y - mouseRectPos.centerPos.y;
                        const float range = (rangeX + rangeY) / 2.0f;
                        // �ړ��ʂ����̂܂ܓK�p���邱�ƂŁA�g��E�k�����\��
                        graph.changeSize.x = graph.tempChangeSizeXY.x - range;
                        graph.changeSize.y = graph.tempChangeSizeXY.y - range;
                    }
                }
            }
        }

        // ��ʊO����
        {
            isScreenLeftHit = false;
            isScreenTopHit = false;
            isScreenRightHit = false;
            isScreenBottomHit = false;
            if (graph.rect.left < 0)
            {
                isScreenLeftHit = true;
            }
            if (graph.rect.top < 0)
            {
                isScreenTopHit = true;
            }
            if (graph.rect.right > 1920)
            {
                isScreenRightHit = true;
            }
            if (graph.rect.bottom > 1080)
            {
                isScreenBottomHit = true;
            }
        }
    }

    KEY::DxLibKeyFresh::GetInstance()->Update();
    // �`��f�[�^�̐؂�ւ�
    if (KEY::DxLibKeyFresh::GetInstance()->IsTrigger(KEY_INPUT_U))
    {
        isUIDrawer = !isUIDrawer;
    }
}

void Draw()
{
    // �}�E�X�̔���
    DrawBoxAA(
        mouseRectPos.rect.left,
        mouseRectPos.rect.top,
        mouseRectPos.rect.right,
        mouseRectPos.rect.bottom,
        0xffffff,false);

    // �摜�`��
    for (auto& graph : graphData)
    {
        // �摜
        DrawExtendGraphF(
            graph.rect.left,
            graph.rect.top,
            graph.rect.right,
            graph.rect.bottom,
            graph.hGraph,true);

        if (isUIDrawer)
        {
            // �摜�̘g
            DrawBoxAA(
                graph.rect.left,
                graph.rect.top,
                graph.rect.right,
                graph.rect.bottom,
                0xffffff, false);
        }

        // �}�E�X�ɉ摜���G��Ă����ꍇ
        if (graph.isCheckMouse)
        {
            // �f�[�^��`�悷�邩�ǂ���
            if (isUIDrawer)
            {
                // X���W
                DrawBoxAA(
                    graph.changeSizeCollX.left,
                    graph.changeSizeCollX.top,
                    graph.changeSizeCollX.right,
                    graph.changeSizeCollX.bottom,
                    0xff0000, true);

                // Y���W
                DrawBoxAA(
                    graph.changeSizeCollY.left,
                    graph.changeSizeCollY.top,
                    graph.changeSizeCollY.right,
                    graph.changeSizeCollY.bottom,
                    0x00ff00, true);

                // X Y
                DrawBoxAA(
                    graph.changeSizeCollXY.left,
                    graph.changeSizeCollXY.top,
                    graph.changeSizeCollXY.right,
                    graph.changeSizeCollXY.bottom,
                    0xffffff, true);

                // ���Z�b�g�{�^��
                DrawBoxAA(
                    graph.resetBotton.left,
                    graph.resetBotton.top,
                    graph.resetBotton.right,
                    graph.resetBotton.bottom,
                    0xffff00, true);

                // ���Z�b�g�{�^��
                DrawBoxAA(
                    graph.resetBotton.left - 1,
                    graph.resetBotton.top - 1,
                    graph.resetBotton.right + 1,
                    graph.resetBotton.bottom + 1,
                    0xffffff, false);

                // ���W
                DrawFormatStringF(
                    graph.rect.left, graph.rect.top,
                    0xffffff,
                    "���S���W       [x : %f , y : %f]", graph.centerPos.x, graph.centerPos.y);
                // �l�p�`���W
                DrawFormatStringF(
                    graph.rect.left, graph.rect.top + 16,
                    0xffffff,
                    "�l�p�`���W     [left : %f , top : %f , right : %f , bottom : %f]", graph.rect.left, graph.rect.top, graph.rect.right, graph.rect.bottom);
                // �g��T�C�Y
                DrawFormatStringF(
                    graph.rect.left, graph.rect.top + 16 + 16,
                    0xffffff,
                    "�g��T�C�Y     [x : %f , y : %f]", graph.changeSize.x, graph.changeSize.y);
                // �f�t�H���g�摜�T�C�Y
                DrawFormatStringF(
                    graph.rect.left, graph.rect.top + 16 + 16 + 16,
                    0xffffff,
                    "�����摜�T�C�Y [x : %f , y : %f]", graph.size.x, graph.size.y);
            }
        }
    }

    // �f�[�^��`�悷�邩�ǂ���
    if (isUIDrawer)
    {
        // ��ʊO�ɉ摜�ŏo���ꍇ
        if (isScreenLeftHit)
        {
            DrawBoxAA(0.0f, 0.0f, kGraphOutSideSize, 1080.0f, 0xff0000, true);
        }
        if (isScreenTopHit)
        {
            DrawBoxAA(0.0f, 0.0f, 1920.0f, kGraphOutSideSize, 0x00ff00, true);
        }
        if (isScreenRightHit)
        {
            DrawBoxAA(1920.0f - kGraphOutSideSize, 0.0f, 1920.0f, 1080.0f, 0xff0000, true);
        }
        if (isScreenBottomHit)
        {
            DrawBoxAA(0.0f, 1080.0f - kGraphOutSideSize, 1920.0f, 1080.0f, 0x00ff00, true);
        }    
    }

    // �摜���܂�����ǂݍ��܂�Ă��Ȃ��ꍇ
    if (graphData.size() == 0)
    {
        DrawString(0, 0, "�摜���h���b�O&�h���b�v���Ă�", 0xffffff);
    }
}

void End()
{
    // ���������
    for (int i = 0; i < graphData.size() ; i++)
    {
        DeleteGraph(graphData[i].hGraph);
    }

    MOUSE::DxLibMouseFresh::Destroy();
    KEY::DxLibKeyFresh::Destroy();
}

bool IsCheckSquare(float UpX, float UpY, float DownX, float DownY,
    float UpX2, float UpY2, float DownX2, float DownY2)
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

    // �������
    End();

    // �c�w���C�u�����g�p�̏I������
    DxLib_End();

    // �\�t�g�̏I�� 
    return 0;
}