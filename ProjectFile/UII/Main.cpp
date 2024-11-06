#include <string>
#include <stdio.h>
#include <vector>

#include <DxLib.h>

namespace
{
    // 画像引き伸ばし判定のサイズ
    constexpr float kGraphChangeSize = 5.0f;
}

bool IsCheckSquare(int UpX, int UpY, int DownX, int DownY, int UpX2, int UpY2, int DownX2, int DownY2);

// ２次元座標
struct Vec2
{
    float x;
    float y;
};

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
    Rect rect;      // 画像サイズ
    Vec2 changeSize; // 変更したサイズ
    float changeSizeXY = 0.0f;
    Vec2 tempClickPos; // 一時的に位置を記録する
    Vec2 tempChangeSize;
    Vec2 centerPos; // センター座標
    Vec2 size;      // サイズを記録する
    Vec2 movePos;   // 移動先
    Vec2 offset;    // オフセットを記録する変数    
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
    Rect changeSizeCollX; // サイズを変更する為の判定
    Rect changeSizeCollY; // サイズを変更する為の判定
    Rect changeSizeCollXY; // サイズを変更する為の判定
    bool isCheckMouse = false; // マウスカーソルに当たっているかどうか
    bool isCheckColl = false;
    bool isCheckSizeCollX = false;
    bool isCheckSizeCollY = false;
    bool isCheckSizeCollXY = false;
    int hGraph;     // 画像ハンドル
};

// マウスデータ
struct MouseData
{
    Rect rect;      // サイズ
    Vec2 centerPos; // センター座標
};

namespace
{
    // ファイルの名前取得
    char filepath[256];

    // 画像ハンドル
    std::vector<GraphicData> graphData;

    // マウスデータ
    MouseData mouseRectPos{};
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
        Vec2
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
        
        // マウスクリック
        {
            // マウスクリック開始時にオフセットを計算
            if ((GetMouseInput() & MOUSE_INPUT_LEFT) &&
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
            if ((GetMouseInput() & MOUSE_INPUT_LEFT) &&
                graph.isCheckSizeCollX)
            {
                printfDx("X\n");

                // 画像を動かさない
                graph.isMoving = false;

                graph.isChangeSizeClickX = true;
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
            if ((GetMouseInput() & MOUSE_INPUT_LEFT) &&
                graph.isCheckSizeCollY)
            {
                printfDx("Y\n");

                // 画像を動かさない
                graph.isMoving = false;

                graph.isChangeSizeClickY = true;
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

            /*
            // サイズ変更をする場合
            if ((GetMouseInput() & MOUSE_INPUT_LEFT) &&
                graph.isCheckSizeCollXY)
            {
                printfDx("XY\n");

                // 画像を動かさない
                graph.isMoving = false;

                graph.isChangeSizeClickXY = true;
            }
            else
            {
                graph.isChangeSizeClickXY = false;
                graph.isChangeSizeDraggingXY = false;
                graph.tempClickPos.x = 0.0f;
                graph.tempClickPos.y = 0.0f;
                graph.tempChangeSize.x = 0.0f;
                graph.tempChangeSize.y = 0.0f;

                // 当たり判定確認
                graph.isCheckSizeCollXY = false;
            }
            */
        }
       
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
            /*
            // サイズ変更するためにクリックしていた場合
            if (graph.isChangeSizeClickXY)
            {
                // ドラッグしていない場合
                if (!graph.isChangeSizeDraggingXY)
                {
                    graph.isChangeSizeDraggingXY = true;

                    // マウスのクリック座標を記録する
                    graph.tempClickPos.x = mouseRectPos.centerPos.x;
                    graph.tempClickPos.y = mouseRectPos.centerPos.y;

                    graph.tempChangeSize.x = graph.changeSize.x;
                    graph.tempChangeSize.y = graph.changeSize.y;
                }

                // ドラッグしている場合
                if (graph.isChangeSizeDraggingXY)
                {
                    // マウスのX座標移動量を計算
                    const float rangeX = graph.tempClickPos.x - mouseRectPos.centerPos.x;
                    const float rangeY = graph.tempClickPos.y - mouseRectPos.centerPos.y;

                    // 移動量をそのまま適用することで、拡大・縮小が可能に
                    graph.changeSize.x = graph.tempChangeSize.x - rangeX;
                    graph.changeSize.y = graph.tempChangeSize.y - rangeY;

                    
                }                
            }
            */
        }
    }
}

void Draw()
{
    // マウスの判定
    DrawBox(
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

        // 画像の枠
        DrawBox(
            graph.rect.left,
            graph.rect.top,
            graph.rect.right,
            graph.rect.bottom,
            0xffffff, false);

        // マウスに画像が触れていた場合
        if (graph.isCheckMouse)
        {
            // X座標
            DrawBox(
                graph.changeSizeCollX.left,
                graph.changeSizeCollX.top,
                graph.changeSizeCollX.right,
                graph.changeSizeCollX.bottom,
                0xff0000, true);

            // Y座標
            DrawBox(
                graph.changeSizeCollY.left,
                graph.changeSizeCollY.top,
                graph.changeSizeCollY.right,
                graph.changeSizeCollY.bottom,
                0x00ff00, true);

            // X Y
            DrawBox(
                graph.changeSizeCollXY.left,
                graph.changeSizeCollXY.top,
                graph.changeSizeCollXY.right,
                graph.changeSizeCollXY.bottom,
                0xffffff, true);

            // 座標
            DrawFormatString(graph.rect.left, graph.rect.top, 0xffffff, "中心座標　x : %f , y : %f", graph.centerPos.x, graph.centerPos.y);
            // 四角形座標
            DrawFormatString(graph.rect.left, graph.rect.top + 16, 0xffffff, "四角形座標　left : %f , top : %f , right : %f , bottom : %f", graph.rect.left, graph.rect.top, graph.rect.right, graph.rect.bottom);
        }
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
    }

    // ＤＸライブラリ使用の終了処理
    DxLib_End();

    // ソフトの終了 
    return 0;
}