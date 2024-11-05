#include <string>
#include <stdio.h>
#include <vector>

#include <DxLib.h>

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
    Vec2 centerPos; // センター座標
    Vec2 size;      // サイズを記録する
    Vec2 movePos;   // 移動先
    int hGraph;     // 画像ハンドル
};

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

    // オフセットを記録する変数
    Vec2 offset;
    bool isDragging = false;
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

        // 画像とマウスの判定
        if (IsCheckSquare(
            mouseRectPos.rect.left, mouseRectPos.rect.top, mouseRectPos.rect.right, mouseRectPos.rect.bottom,
            graph.rect.left, graph.rect.top, graph.rect.right, graph.rect.bottom))
        {
            // マウスクリック開始時にオフセットを計算
            if ((GetMouseInput() & MOUSE_INPUT_LEFT))
            {
                // クリックした初めに画像とマウスの位置の計算をする
                if (!isDragging)
                {
                    isDragging = true;
                    offset.x = mouseRectPos.centerPos.x - graph.centerPos.x;
                    offset.y = mouseRectPos.centerPos.y - graph.centerPos.y;
                }
                else
                {
                    // 位置を変更する
                    graph.centerPos.x = mouseRectPos.centerPos.x - offset.x;
                    graph.centerPos.y = mouseRectPos.centerPos.y - offset.y;
                }
            }
            else
            {
                // マウスボタンを離したらドラッグ終了
                isDragging = false;
            }
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