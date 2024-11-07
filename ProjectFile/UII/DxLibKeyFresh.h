#pragma once

#include "SingletonBase.h"

#include <DxLib.h>

namespace
{
	// キーバッファ
	constexpr int kKeyStateBuf = 256;

	// トリガー用フレームカウント
	constexpr int kKeyTriggerFrameCount = 1;
}

/// <summary>
/// キーボード関連
/// </summary>
namespace KEY
{
	/// <summary>
	/// キーボード処理
	/// </summary>
	class DxLibKeyFresh : public SingletonBase<DxLibKeyFresh>
	{
	public:
		// SingletonBaseクラスでのみインスタンス生成を許可する
		friend class SingletonBase<DxLibKeyFresh>;

		~DxLibKeyFresh() {};

	private:
		DxLibKeyFresh() :
			m_keyBuf(),
			m_pressFrameCount(0),
			m_isPress(false),
			m_isCheckOneNoPress(true)
		{

		}

	public:
		/// <summary>
		/// キーボードの入力状態取得
		/// </summary>
		void Update()
		{
			// キーボードの入力状態を取得する
			GetHitKeyStateAll(m_keyBuf);
		}

		/// <summary>
		/// 押し下げ判定
		/// </summary>
		/// <param name="keyNo">入力するキー                            </param>
		/// <returns           >true : 押している , false : 押していない</returns>
		bool IsPress(const int keyNo) const
		{
			// 指定のキー入力の判定をする
			if (m_keyBuf[keyNo])
			{
				return true;
			}

			return false;
		}

		/// <summary>
		/// トリガー判定
		/// ボタンを押した場合に判定をとる
		/// </summary>
		/// <param name="keyNo"          >入力するキー                            </param>
		/// <param name="delayInputFrame">入力にディレイフレームを指定する        </param>
		/// <returns           >true : 押している , false : 押していない</returns>
		bool IsTrigger(const int keyNo , const int delayInputFrame = kKeyTriggerFrameCount)
		{			
			// キーが押されている場合
			if (m_keyBuf[keyNo])
			{
				// フレームカウントを進める
				m_pressFrameCount++;

				// 指定したフレームの時に一度だけトリガーを発生
				if (m_pressFrameCount == delayInputFrame)
				{								
					return true;
				}
			}
			else
			{
				m_pressFrameCount = 0;
			}

			return false;
		}



		/// <summary>
		/// リリース
		/// ボタンを離した場合に入力をする
		/// </summary>
		/// <param name="keyNo">入力するキー                            </param>
		/// <returns           >true : 押している , false : 押していない</returns>
		bool IsRelease(const int keyNo)
		{			
			// 指定のキー入力の判定をする
			if (m_keyBuf[keyNo])
			{
				// 長押し判定
				m_isPress = true;
				// 押していなかの確認
				m_isCheckOneNoPress = false;
			}
			else
			{
				// 長押ししていない場合
				m_isPress = false;
			}

			// 一度入力している場合、長押ししてない場合を確認
			if (!m_isCheckOneNoPress && !m_isPress)
			{
				// リセット
				ReleaseReset();

				// 入力終了
				return true;
			}

			return false;
		}

		/// <summary>
		/// リリースボタンのリセット
		/// </summary>
		void ReleaseReset()
		{
			// リセット
			m_isPress           = false;
			m_isCheckOneNoPress = true;
		}

		/// <summary>
		/// すべてのキー入力確認
		/// </summary>
		/// <returns></returns>
		bool IsCheckHitKeyAll()
		{
			return CheckHitKeyAll();
		}

	private:
		// キー用バッファ
		char m_keyBuf[kKeyStateBuf];

		// 押している間のフレームカウント
		int m_pressFrameCount;

		// リリース判定用
		// 長押ししているか
		bool m_isPress;

		// 押していないかの確認を一度する
		bool m_isCheckOneNoPress;
	};
}