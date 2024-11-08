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
    // 画像引き伸ばし判定のサイズ
    constexpr float kGraphChangeSize = 5.0f;

    // 画面外に画像が出た場合
    constexpr float kGraphOutSideSize = 1.0f;

    // リセットボタンのサイズ
    constexpr float kResetBottonSize = 30.0f;
}

bool IsCheckSquare(float UpX, float UpY, float DownX, float DownY, float UpX2, float UpY2, float DownX2, float DownY2);

// 四角形
struct Rect
{
    float top;
    float bottom;
    float left;
    float right;
};

// 画像保存データ
struct GraphicData
{
    bool isMain = false; // 
    Rect rect{ 0.0f,0.0f ,0.0f ,0.0f };      // 画像サイズ
    Vec2<float> changeSize{ 0.0f,0.0f }; // 変更したサイズ
    float changeSizeXY = 0.0f;
    Vec2<float> tempClickPos{ 0.0f,0.0f }; // 一時的に位置を記録する
    Vec2<float> tempClickPosXY{ 0.0f,0.0f }; // 一時的に位置を記録する
    Vec2<float> tempChangeSize{ 0.0f,0.0f };
    Vec2<float> tempChangeSizeXY{ 0.0f,0.0f };
    Vec2<float> centerPos{ 0.0f,0.0f }; // センター座標
    Vec2<float> size{ 0.0f,0.0f };      // サイズを記録する
    Vec2<float> movePos{ 0.0f,0.0f };   // 移動先
    Vec2<float> offset{ 0.0f,0.0f };    // オフセットを記録する変数   
    Rect resetBotton{ 0.0f,0.0f ,0.0f ,0.0f }; // リセットボタン
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
    Rect changeSizeCollX{ 0.0f,0.0f ,0.0f ,0.0f }; // サイズを変更する為の判定
    Rect changeSizeCollY{ 0.0f,0.0f ,0.0f ,0.0f }; // サイズを変更する為の判定
    Rect changeSizeCollXY{ 0.0f,0.0f ,0.0f ,0.0f }; // サイズを変更する為の判定
    bool isCheckMouse = false; // マウスカーソルに当たっているかどうか
    bool isCheckColl = false;
    bool isCheckSizeCollX = false;
    bool isCheckSizeCollY = false;
    bool isCheckSizeCollXY = false;
    bool isResetBottonColl = false;
    int hGraph = -1;     // 画像ハンドル
};

// マウスデータ
struct MouseData
{
    Rect rect{ 0.0f,0.0f, 0.0f, 0.0f };      // サイズ
    Vec2<float> centerPos{ 0.0f,0.0f }; // センター座標
};

namespace
{
    // ファイルの名前取得
    char filepath[256];

    // 画像ハンドル
    std::vector<GraphicData> graphData;

    // マウスデータ
    MouseData mouseRectPos{};

    // データを描画するかどうか1
    bool isUIDrawer = true;

    bool isScreenLeftHit   = false;
    bool isScreenTopHit    = false;
    bool isScreenRightHit  = false;
    bool isScreenBottomHit = false;
}

void Update()
{
    // マウスの座標
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
        // 画像データの追加
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

        // ドラッグファイルデータのクリア
        DragFileInfoClear();
    }    

    for (auto& graph : graphData)
    {
        // 座標データの更新
        {
            // 中心座標を基準として画像のサイズを計算する
            graph.rect.left   = graph.centerPos.x - (graph.size.x / 2.0f);
            graph.rect.top    = graph.centerPos.y - (graph.size.y / 2.0f);
            graph.rect.right  = graph.centerPos.x + (graph.size.x / 2.0f) + graph.changeSize.x;
            graph.rect.bottom = graph.centerPos.y + (graph.size.y / 2.0f) + graph.changeSize.y;

            // X サイズを変更する為の判定座標
            graph.changeSizeCollX.left   = graph.rect.right - kGraphChangeSize;
            graph.changeSizeCollX.top    = graph.rect.top;
            graph.changeSizeCollX.right  = graph.rect.right;
            graph.changeSizeCollX.bottom = graph.rect.bottom;

            // Y サイズを変更する為の判定座標
            graph.changeSizeCollY.left    = graph.rect.left;
            graph.changeSizeCollY.top     = graph.rect.bottom - kGraphChangeSize;
            graph.changeSizeCollY.right   = graph.rect.right;
            graph.changeSizeCollY.bottom  = graph.rect.bottom;

            // XY サイズを変更する為の判定座標
            graph.changeSizeCollXY.left   = graph.rect.right - kGraphChangeSize;
            graph.changeSizeCollXY.top    = graph.rect.bottom - kGraphChangeSize;
            graph.changeSizeCollXY.right  = graph.rect.right;
            graph.changeSizeCollXY.bottom = graph.rect.bottom;

            graph.resetBotton.left   = graph.rect.right      - kGraphChangeSize - kGraphChangeSize - kResetBottonSize;
            graph.resetBotton.top    = graph.rect.top        + kGraphChangeSize + kGraphChangeSize;
            graph.resetBotton.right  = graph.rect.right      - kGraphChangeSize - kGraphChangeSize;
            graph.resetBotton.bottom = graph.resetBotton.top + kResetBottonSize;
        }

        // 画像とマウスの判定
        if (IsCheckSquare(
            mouseRectPos.rect.left, mouseRectPos.rect.top, mouseRectPos.rect.right, mouseRectPos.rect.bottom,
            graph.rect.left, graph.rect.top, graph.rect.right, graph.rect.bottom))
        {
            graph.isCheckColl = true;
            // マウスに触れている画像
            graph.isCheckMouse = true;
        }
        else
        {
            graph.isCheckMouse = false;
        }

        // サイズを変更する為の
        if (IsCheckSquare(
            mouseRectPos.rect.left, mouseRectPos.rect.top, mouseRectPos.rect.right, mouseRectPos.rect.bottom,
            graph.changeSizeCollX.left, graph.changeSizeCollX.top, graph.changeSizeCollX.right, graph.changeSizeCollX.bottom))
        {            
            // 当たり判定確認
            graph.isCheckSizeCollX = true;  
        }
        
        if (IsCheckSquare(
            mouseRectPos.rect.left, mouseRectPos.rect.top, mouseRectPos.rect.right, mouseRectPos.rect.bottom,
            graph.changeSizeCollY.left, graph.changeSizeCollY.top, graph.changeSizeCollY.right, graph.changeSizeCollY.bottom))
        {
            // 当たり判定確認
            graph.isCheckSizeCollY = true;            
        }

        if (IsCheckSquare(
            mouseRectPos.rect.left, mouseRectPos.rect.top, mouseRectPos.rect.right, mouseRectPos.rect.bottom,
            graph.changeSizeCollXY.left, graph.changeSizeCollXY.top, graph.changeSizeCollXY.right, graph.changeSizeCollXY.bottom))
        {
            // 当たり判定確認
            graph.isCheckSizeCollXY = true;
        }

        // リセットボタンの確認
        if (IsCheckSquare(
            mouseRectPos.rect.left, mouseRectPos.rect.top, mouseRectPos.rect.right, mouseRectPos.rect.bottom,
            graph.resetBotton.left, graph.resetBotton.top, graph.resetBotton.right, graph.resetBotton.bottom))
        {
            graph.isResetBottonColl = true;
        }
        
        // マウスクリック
        {
            // マウスクリック開始時にオフセットを計算
            if (MOUSE::DxLibMouseFresh::GetInstance()->IsPress(MOUSE_INPUT_LEFT) &&
                graph.isCheckColl)
            {
                // クリックしている
                graph.isGraphClick = true;

                // 画像を動かす
                graph.isMoving = true;
            }
            else
            {
                // マウスボタンを離したらドラッグ終了
                graph.isDragging = false;

                // クリックしていない
                graph.isGraphClick = false;

                // 触れていない画像
                graph.isCheckColl = false;
            }

            // サイズ変更をする場合
            if (MOUSE::DxLibMouseFresh::GetInstance()->IsPress(MOUSE_INPUT_LEFT) &&
                graph.isCheckSizeCollX &&
                !graph.isCheckSizeCollY)
            {
                // 画像を動かさない
                graph.isMoving = false;

                graph.isChangeSizeClickX = true;

                // マウスに触れている画像
                graph.isCheckMouse = true;
            }
            else
            {
                graph.isChangeSizeClickX = false;
                graph.isChangeSizeDraggingX = false;
                graph.tempClickPos.x = 0.0f;
                graph.tempChangeSize.x = 0.0f;

                // 当たり判定確認
                graph.isCheckSizeCollX = false;
            }

            // サイズ変更をする場合
            if (MOUSE::DxLibMouseFresh::GetInstance()->IsPress(MOUSE_INPUT_LEFT) &&
                graph.isCheckSizeCollY &&
                !graph.isCheckSizeCollX)
            {
                // 画像を動かさない
                graph.isMoving = false;

                graph.isChangeSizeClickY = true;

                // マウスに触れている画像
                graph.isCheckMouse = true;
            }
            else
            {
                graph.isChangeSizeClickY = false;
                graph.isChangeSizeDraggingY = false;
                graph.tempClickPos.y = 0.0f;
                graph.tempChangeSize.y = 0.0f;

                // 当たり判定確認
                graph.isCheckSizeCollY = false;
            }


            // サイズ変更をする場合
            if (MOUSE::DxLibMouseFresh::GetInstance()->IsPress(MOUSE_INPUT_LEFT) &&
                graph.isCheckSizeCollXY)
            {
                // 画像を動かさない
                graph.isMoving = false;

                graph.isChangeSizeClickXY = true;

                // マウスに触れている画像
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

                // 当たり判定確認
                graph.isCheckSizeCollXY = false;
            }

            // リセットする場合
            if (MOUSE::DxLibMouseFresh::GetInstance()->IsPress(MOUSE_INPUT_LEFT) &&
                graph.isResetBottonColl)
            {
                graph.changeSize.x = 0.0f;
                graph.changeSize.y = 0.0f;

                graph.isResetBottonColl = false;
            }
        }
    }{}

    // 配列の順番を変えたいがこのやり方は効率がわるい
    // 気が向いたら直す()
    {
        // 配列がない場合は処理をしない
        if (graphData.size() != 0)
        {
            // 配列の最大数から要素の確認
            for (int i = static_cast<int>(graphData.size()) - 1; i >= 0; i--)
            {
                // クリックされた要素を探す
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
                // クリックされていない要素を確認
                if (!graphData[i].isMain)
                {
                    graphData[i].isMoving = false;
                    graphData[i].isCheckColl = false;
                }
                else
                {
                    // クリックされている要素のコピー
                    tempData = graphData[i];
                    graphData[i] = {};
                }
            }
            // コピーした要素を最後の要素にする
            graphData.push_back(tempData);
        }
    }
    
    for (auto& graph : graphData)
    {
        // 画像位置の変更
        {
            // 画像をクリックしている場合
            // クリックした初めに画像とマウスの位置の計算をする
            if (graph.isGraphClick && !graph.isDragging)
            {
                graph.isDragging = true;
                graph.offset.x = mouseRectPos.centerPos.x - graph.centerPos.x;
                graph.offset.y = mouseRectPos.centerPos.y - graph.centerPos.y;
            }

            // 位置を変更する
            // マウスをドラッグしているか
            // 動かす判定がとれているか
            if (graph.isDragging && graph.isMoving)
            {
                graph.centerPos.x = mouseRectPos.centerPos.x - graph.offset.x;
                graph.centerPos.y = mouseRectPos.centerPos.y - graph.offset.y;
            }
        }

        if (graph.isMain && isUIDrawer)
        {
            // 画像サイズの変更
            {
                // サイズ変更するためにクリックしていた場合
                if (graph.isChangeSizeClickX)
                {
                    // ドラッグしていない場合
                    if (!graph.isChangeSizeDraggingX)
                    {
                        graph.isChangeSizeDraggingX = true;

                        // マウスのクリック座標を記録する
                        graph.tempClickPos.x = mouseRectPos.centerPos.x;

                        graph.tempChangeSize.x = graph.changeSize.x;
                    }

                    // ドラッグしている場合
                    if (graph.isChangeSizeDraggingX)
                    {
                        // マウスのX座標移動量を計算
                        const float range = graph.tempClickPos.x - mouseRectPos.centerPos.x;

                        // 移動量をそのまま適用することで、拡大・縮小が可能に
                        graph.changeSize.x = graph.tempChangeSize.x - range;
                    }
                }

                // サイズ変更するためにクリックしていた場合
                if (graph.isChangeSizeClickY)
                {
                    // ドラッグしていない場合
                    if (!graph.isChangeSizeDraggingY)
                    {
                        graph.isChangeSizeDraggingY = true;

                        // マウスのクリック座標を記録する
                        graph.tempClickPos.y = mouseRectPos.centerPos.y;

                        graph.tempChangeSize.y = graph.changeSize.y;
                    }

                    // ドラッグしている場合
                    if (graph.isChangeSizeDraggingY)
                    {
                        // マウスのX座標移動量を計算
                        const float range = graph.tempClickPos.y - mouseRectPos.centerPos.y;

                        // 移動量をそのまま適用することで、拡大・縮小が可能に
                        graph.changeSize.y = graph.tempChangeSize.y - range;
                    }
                }

                // サイズ変更するためにクリックしていた場合
                if (graph.isChangeSizeClickXY)
                {
                    // ドラッグしていない場合
                    if (!graph.isChangeSizeDraggingXY)
                    {
                        graph.isChangeSizeDraggingXY = true;

                        // マウスのクリック座標を記録する
                        graph.tempClickPosXY.x = mouseRectPos.centerPos.x;
                        graph.tempClickPosXY.y = mouseRectPos.centerPos.y;

                        graph.tempChangeSizeXY.x = graph.changeSize.x;
                        graph.tempChangeSizeXY.y = graph.changeSize.y;
                    }

                    // ドラッグしている場合
                    if (graph.isChangeSizeDraggingXY)
                    {
                        // マウスのX座標移動量を計算
                        const float rangeX = graph.tempClickPosXY.x - mouseRectPos.centerPos.x;
                        const float rangeY = graph.tempClickPosXY.y - mouseRectPos.centerPos.y;
                        const float range = (rangeX + rangeY) / 2.0f;
                        // 移動量をそのまま適用することで、拡大・縮小が可能に
                        graph.changeSize.x = graph.tempChangeSizeXY.x - range;
                        graph.changeSize.y = graph.tempChangeSizeXY.y - range;
                    }
                }
            }
        }

        // 画面外処理
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
    // 描画データの切り替え
    if (KEY::DxLibKeyFresh::GetInstance()->IsTrigger(KEY_INPUT_U))
    {
        isUIDrawer = !isUIDrawer;
    }
}

void Draw()
{
    // マウスの判定
    DrawBoxAA(
        mouseRectPos.rect.left,
        mouseRectPos.rect.top,
        mouseRectPos.rect.right,
        mouseRectPos.rect.bottom,
        0xffffff,false);

    // 画像描画
    for (auto& graph : graphData)
    {
        // 画像
        DrawExtendGraphF(
            graph.rect.left,
            graph.rect.top,
            graph.rect.right,
            graph.rect.bottom,
            graph.hGraph,true);

        if (isUIDrawer)
        {
            // 画像の枠
            DrawBoxAA(
                graph.rect.left,
                graph.rect.top,
                graph.rect.right,
                graph.rect.bottom,
                0xffffff, false);
        }

        // マウスに画像が触れていた場合
        if (graph.isCheckMouse)
        {
            // データを描画するかどうか
            if (isUIDrawer)
            {
                // X座標
                DrawBoxAA(
                    graph.changeSizeCollX.left,
                    graph.changeSizeCollX.top,
                    graph.changeSizeCollX.right,
                    graph.changeSizeCollX.bottom,
                    0xff0000, true);

                // Y座標
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

                // リセットボタン
                DrawBoxAA(
                    graph.resetBotton.left,
                    graph.resetBotton.top,
                    graph.resetBotton.right,
                    graph.resetBotton.bottom,
                    0xffff00, true);

                // リセットボタン
                DrawBoxAA(
                    graph.resetBotton.left - 1,
                    graph.resetBotton.top - 1,
                    graph.resetBotton.right + 1,
                    graph.resetBotton.bottom + 1,
                    0xffffff, false);

                // 座標
                DrawFormatStringF(
                    graph.rect.left, graph.rect.top,
                    0xffffff,
                    "中心座標       [x : %f , y : %f]", graph.centerPos.x, graph.centerPos.y);
                // 四角形座標
                DrawFormatStringF(
                    graph.rect.left, graph.rect.top + 16,
                    0xffffff,
                    "四角形座標     [left : %f , top : %f , right : %f , bottom : %f]", graph.rect.left, graph.rect.top, graph.rect.right, graph.rect.bottom);
                // 拡大サイズ
                DrawFormatStringF(
                    graph.rect.left, graph.rect.top + 16 + 16,
                    0xffffff,
                    "拡大サイズ     [x : %f , y : %f]", graph.changeSize.x, graph.changeSize.y);
                // デフォルト画像サイズ
                DrawFormatStringF(
                    graph.rect.left, graph.rect.top + 16 + 16 + 16,
                    0xffffff,
                    "初期画像サイズ [x : %f , y : %f]", graph.size.x, graph.size.y);
            }
        }
    }

    // データを描画するかどうか
    if (isUIDrawer)
    {
        // 画面外に画像で出た場合
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

    // 画像がまだ一つも読み込まれていない場合
    if (graphData.size() == 0)
    {
        DrawString(0, 0, "画像をドラッグ&ドロップしてね", 0xffffff);
    }
}

void End()
{
    // メモリ解放
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

// プログラムはWinMainから始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
    // windowモード設定
    ChangeWindowMode(true);

    // 画面サイズの設定
    SetGraphMode(1920, 1080, 32);

    // ログ出力を行うか否か
    SetOutApplicationLogValidFlag(false);

    // ダブルバッファモード
    SetDrawScreen(DX_SCREEN_BACK);

    // ＤＸライブラリ初期化処理
    if (DxLib_Init() == -1)
    {
        // エラーが起きたら直ちに終了
        return -1;
    }
    
    //ドラッグ＆ドロップを許可
    SetDragFileValidFlag(true);

    while (ProcessMessage() == 0)
    {
        LONGLONG  time = GetNowHiPerformanceCount();

        // 画面のクリア
        ClearDrawScreen();

        // 更新
        Update();

        // 描画
        Draw();

        // 裏画面を表画面を入れ替える
        ScreenFlip();

        // escキーを押したら終了する
        if (CheckHitKey(KEY_INPUT_ESCAPE))
        {
            break;
        }

        // fpsを60に固定
        while (GetNowHiPerformanceCount() - time < 16667)
        {
        }
    }

    // 解放処理
    End();

    // ＤＸライブラリ使用の終了処理
    DxLib_End();

    // ソフトの終了 
    return 0;
}