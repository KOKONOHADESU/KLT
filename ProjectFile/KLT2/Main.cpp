#include <DxLib.h>

#include <math.h>
#include <algorithm>

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
}

/// <summary>
/// プレイヤークラス
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
            m_playerPos.x += vec.x * kPlayerSpeed;
            m_playerPos.y += vec.y * kPlayerSpeed;
        }

        // 移動範囲を制限
        {
            // 左
            m_playerPos.x = (std::max)(m_playerPos.x, m_left + kPlayerSize);

            // 右
            m_playerPos.x = (std::min)(m_playerPos.x, m_right - kPlayerSize);

            // 上
            m_playerPos.y = (std::max)(m_playerPos.y, m_top + kPlayerSize);

            // 下
            m_playerPos.y = (std::min)(m_playerPos.y, m_bottom - kPlayerSize);
        }
    }

    void Draw()
    {
        // プレイヤー描画        
        DrawCircleAA(m_playerPos.x, m_playerPos.y, kPlayerSize, 64, 0xff0000);

    }

    // 行動範囲のセット
    void SetMovementRange(float top, float bottom, float left, float right)
    {
        m_top     = top;
        m_bottom  = bottom;
        m_left    = left;
        m_right   = right;
    }

private:
    // プレイヤー座標
    Vec2 m_playerPos;

    float m_top;
    float m_bottom;
    float m_right;
    float m_left;
};

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

    // 移動範囲
    float top    = 1080 / 2 - 300;
    float bottom = 1080 / 2 + 300;
    float left   = 1920 / 2 - 300;
    float right  = 1920 / 2 + 300;

    bool isChange = false;
    int frameCount = 0;

    // 実体の作成
    Player* pPlayer = new Player();

    while (ProcessMessage() == 0)
    {
        LONGLONG  time = GetNowHiPerformanceCount();

        // 画面のクリア
        ClearDrawScreen();

        // プレイヤー処理
        {
            pPlayer->Update();

            // 行動範囲の座標を受け取る
            pPlayer->SetMovementRange(top, bottom, left, right);
        }

        // プレイヤーの描画
        {
            pPlayer->Draw();
        }   

        // 移動範囲の更新
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

        // 行動範囲の描画
        {
            // 枠の厚み
            for (int i = 0; i < kThick; i++)
            {
                // 移動範囲描画
                DrawBox(
                    left - i,
                    top - i,
                    right + i,
                    bottom + i,
                    0xffffff,
                    false);
            }
        }

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
    delete pPlayer;

    // ＤＸライブラリ使用の終了処理
    DxLib_End();

    // ソフトの終了 
    return 0;
}