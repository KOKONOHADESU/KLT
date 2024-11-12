#pragma once

#include <DxLib.h>

// 
class AddScreen
{
public:
	AddScreen():
		m_hScreen(-1)
	{};
	~AddScreen()
	{
		// 解放処理
		DeleteGraph(m_hScreen);
	};

	// グラフィックハンドルにする画面範囲を指定
	void SetScreen(int screenWidth , int screenHeight)
	{
		// スクリーンハンドルの作成
		m_hScreen = MakeScreen(screenWidth, screenHeight);
	}

	// 画面を作成する
	void CreateScreen()
	{
		// 画面の作成
		SetDrawScreen(m_hScreen);

		// 画面をクリアにする
		ClearDrawScreen();
	}	

	// 画面描画
	void RenderExtendScreen(float left , float top , float right , float bottom)
	{
		// 画面の切り替え
		SetDrawScreen(DX_SCREEN_BACK);

		DrawExtendGraphF(left, top, right, bottom, m_hScreen, true);
	}

	// 画面描画
	void RenderRotaScreen(float centerX, float centerY , float size , float angle)
	{
		// 画面の切り替え
		SetDrawScreen(DX_SCREEN_BACK);

		DrawRotaGraphF(centerX, centerY, size, angle, m_hScreen, true);
	}

private:

	// 画面ハンドル
	int m_hScreen;

};
