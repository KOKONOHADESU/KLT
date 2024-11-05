#include <DxLib.h>

#include <math.h>
#include <algorithm>

//////////////////////
// エネミーパターン //
//////////////////////
// 0 突進
// 1 追跡
#define ENEMYPATTERN 1;

// 二次元ベクトル
struct Vec2
{
    float x = 0.0f;
    float y = 0.0f;
};

namespace
{
    // プレイヤー速度
    constexpr float kPlayerSpeed = 10.0f;

    // プレイヤーサイズ
    constexpr float kPlayerSize = 10.0f;

    // 枠の大きさ
    constexpr int kThick = 30;


    // エネミー速度
    constexpr float kEnemySpeed = 5.0f;
}

namespace
{
    // プレイヤー座標
    Vec2 playerPos{ 1920.0f / 2.0f, 1080.0f / 2.0f };

    // エネミー座標
    Vec2 enemyPos{ 0.0f,0.0f };

    Vec2 enemyComponents{ 0.0f,0.0f };
    Vec2 enemyVec{ 0.0f,0.0f };

#if !ENEMYPATTERN

    bool isOneFrameVec = true;

#endif

}

// 更新
void Update();
// 描画
void Draw();

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

    // ＤＸライブラリ使用の終了処理
    DxLib_End();

    // ソフトの終了 
    return 0;
}

// 更新
void Update()
{
    // プレイヤー処理
    {
        // ベクトルの初期化
        Vec2 vec{ 0.0f,0.0f };

        // 移動ベクトルを生成
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

        // ベクトルの長さを計算
        const float length = sqrtf(vec.x * vec.x + vec.y * vec.y);

        // 長さが0でない場合のみノーマライズ
        if (length > 0.0f)
        {
            vec.x /= length;
            vec.y /= length;
        }

        // 移動ベクトルに速度をかける
        playerPos.x += vec.x * kPlayerSpeed;
        playerPos.y += vec.y * kPlayerSpeed;
    }

    // 移動範囲を制限
    {
        // 左
        playerPos.x = (std::max)(playerPos.x, (1920.0f / 2.0f - 300.0f) + kPlayerSize);

        // 右
        playerPos.x = (std::min)(playerPos.x, (1920.0f / 2.0f + 300.0f) - kPlayerSize);

        // 上
        playerPos.y = (std::max)(playerPos.y, (1080.0f / 2.0f - 300.0f) + kPlayerSize);

        // 下
        playerPos.y = (std::min)(playerPos.y, (1080.0f / 2.0f + 300.0f) - kPlayerSize);
    }

    // エネミー処理
    {

#if ENEMYPATTERN
        // 追跡
        {
            Vec2 components{ 0.0f,0.0f };
            Vec2 vec{ 0.0f,0.0f };

            //成分計算
            components.x = playerPos.x - enemyPos.x;
            components.y = playerPos.y - enemyPos.y;

            //大きさ計算
            const float length = sqrtf(components.x * components.x + components.y * components.y);

            // 長さが0でない場合のみノーマライズ
            if (length > 0.0f)
            {
                //方向計算（正規化）
                vec.x = components.x / length;
                vec.y = components.y / length;
            }

            //移動後の位置を計算
            enemyPos.x += vec.x * kEnemySpeed;
            enemyPos.y += vec.y * kEnemySpeed;
        }

#else
        // 突撃
        {
            // 1フレームだけプレイヤーの座標を計算
            if (isOneFrameVec)
            {

                isOneFrameVec = false;

                //成分計算
                enemyComponents.x = playerPos.x - enemyPos.x;
                enemyComponents.y = playerPos.y - enemyPos.y;

                //大きさ計算
                const float length = sqrtf(enemyComponents.x * enemyComponents.x + enemyComponents.y * enemyComponents.y);

                // 長さが0でない場合のみノーマライズ
                if (length > 0.0f)
                {
                    //方向計算（正規化）
                    enemyVec.x = enemyComponents.x / length;
                    enemyVec.y = enemyComponents.y / length;
                }
            }

            //移動後の位置を計算
            enemyPos.x += enemyVec.x * kEnemySpeed;
            enemyPos.y += enemyVec.y * kEnemySpeed;
        }

#endif
    }
}

// 描画
void Draw()
{
    // プレイヤー処理
    {
        // プレイヤー描画
        {
            DrawCircleAA(playerPos.x, playerPos.y, kPlayerSize, 64, 0xff0000);
        }

        // 行動範囲の描画
        {
            // 枠の厚み
            for (int i = 0; i < kThick; i++)
            {
                // 移動範囲描画
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

    // エネミー処理
    {
        // エネミーの描画
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