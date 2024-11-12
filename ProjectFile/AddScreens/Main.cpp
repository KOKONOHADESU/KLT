
#include "AddScreen.h"

#include <DxLib.h>

// プログラムはWinMainから始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// windowモード設定
	ChangeWindowMode(true);
	// ウインドウ名設定
	SetMainWindowText("AddScreens");
	// 画面サイズの設定
	SetGraphMode(1920, 1080, 32);
	// windowモード時にwindowサイズを変更するかどうか
	SetWindowSizeChangeEnableFlag(true);
	// ログテキストを作成するかどうか
	SetOutApplicationLogValidFlag(false);

	// ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)
	{
		// エラーが起きたら直ちに終了
		return -1;
	}

	//ゲーム中にウィンドウモードを切り替えてもグラフィックスハンドルを
	//リセットしない
	// フルスクリーンウインドウの切り替えでリソースが消えるのを防ぐ。
	// Effekseerを使用する場合は必ず設定する。
	SetChangeScreenModeGraphicsSystemResetFlag(false);

	AddScreen* screen = new AddScreen();

	screen->SetScreen(1920, 1080);

	while (ProcessMessage() == 0)
	{
		LONGLONG  time = GetNowHiPerformanceCount();

		// 画面のクリア
		ClearDrawScreen();

		static float angle = 0.0f;

		angle++;

		screen->CreateScreen();
		DrawBox(300, 300, 600, 600, 0xffffff, false);
		DrawString(1920 / 2, 1080 / 2, "hogehoge", 0xffffff);
		screen->RenderExtendScreen(0, 0, 1000, 1000);
	//	screen->RenderRotaScreen(1920 / 2, 1080 / 2, 1, angle);

		// 裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{
		}
	}

	delete screen;

	// ＤＸライブラリ使用の終了処理
	DxLib_End();

	// ソフトの終了 
	return 0;
}