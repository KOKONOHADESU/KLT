#pragma once

/// <summary>
/// シングルトンベース
/// </summary>
/// <typeparam name="T">継承する型</typeparam>
template<class T>
class SingletonBase
{
public:
	/// <summary>
	/// インスタンスの初期化とポインターの取得
	/// </summary>
	/// <returns>ポインターと渡す</returns>
	static T* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new T();
		}

		return m_pInstance;
	}

	/// <summary>
	/// インスタンスの削除
	/// </summary>
	static void Destroy()
	{
		if (m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = nullptr;
		}
	}

protected:
	SingletonBase() {};
	virtual ~SingletonBase() {};

private:
	// コピーコンストラクタの禁止
	SingletonBase(const SingletonBase&) = delete;
	SingletonBase& operator=(const SingletonBase&) = delete;

	// シングルトン用ポインタ
	static T* m_pInstance;

};
// インスタンスの実体化
template <typename T> T* SingletonBase<T>::m_pInstance;
