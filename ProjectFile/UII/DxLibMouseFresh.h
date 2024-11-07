#pragma once

#include "SingletonBase.h"
#include "Vec2.h"

#include <DxLib.h>

namespace
{
	// トリガー用フレームカウント
	constexpr int kMouseTriggerFrameCount = 1;
}

/// <summary>
/// マウス関連
/// </summary>
namespace MOUSE
{
	/// <summary>
	/// パッド処理
	/// </summary>
	class DxLibMouseFresh : public SingletonBase<DxLibMouseFresh>
	{
	public:
		// SingletonBaseクラスでのみインスタンス生成を許可する
		friend class SingletonBase<DxLibMouseFresh>;

		~DxLibMouseFresh() {};

	private:
		DxLibMouseFresh() :
			m_pos({ 0.0f,0.0f }),
			m_isTrigger(false),
			m_pressFrameCount(0)
		{

		}

	public:
		/// <summary>
		/// マウスの状態取得
		/// </summary>
		void Update()
		{
			int x = 0, y = 0;

			// マウスの座標更新
			GetMousePoint(&x, &y);

			// 座標をコピー
			m_pos.x = static_cast<float>(x);
			m_pos.y = static_cast<float> (y);
		}

		/// <summary>
		/// 押し下げ判定
		/// </summary>
		/// <param name="keyNo">入力するキー                            </param>
		/// <returns           >true : 押している , false : 押していない</returns>
		bool IsPress(int mouseNo) const
		{
			// 入力しているかどうか
			if ((GetMouseInput() & mouseNo))
			{
				return true;
			}
			else
			{
				return false;
			}			
		}

		/// <summary>
		/// トリガー判定
		/// </summary>
		/// <param name="keyNo">入力するキー                            </param>
		/// <returns           >true : 押している , false : 押していない</returns>
		bool IsTrigger(int mouseNo)
		{
			// 指定のキー入力の判定をする
			if ((GetMouseInput() & mouseNo))
			{
				// フレームカウント
				m_pressFrameCount++;

				// 指定したフレームの時に処理をする
				if (m_pressFrameCount == kMouseTriggerFrameCount)
				{
					return true;
				}
			}
			else
			{
				// 押していない場合はフレームカウントをリセット
				m_pressFrameCount = 0;
			}

			return false;
		}
		
		/// <summary>
		/// マウスの座標を渡す
		/// </summary>
		/// <typeparam name="T">座標の型を決める</typeparam>
		/// <returns           >マウスの座標    </returns>
		template <typename T>
		Vec2<T> GetPos()
		{
			int x = 0, y = 0;

			// マウスの座標更新
			GetMousePoint(&x, &y);

			// キャストする
		//	Vec2<T> pos{ static_cast<T>(m_pos.x),static_cast<T>(m_pos.y) };

			// T型にキャストした座標を渡す
			return Vec2<T>(x,y);
		}

	private:
		// 座標
		Vec2<float> m_pos;

		// トリガー用判定を取る
		bool m_isTrigger = false;

		// 押している間のフレームカウント
		int m_pressFrameCount = 0;
	};
}