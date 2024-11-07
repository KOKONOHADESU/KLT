#pragma once

/// <summary>
/// �V���O���g���x�[�X
/// </summary>
/// <typeparam name="T">�p������^</typeparam>
template<class T>
class SingletonBase
{
public:
	/// <summary>
	/// �C���X�^���X�̏������ƃ|�C���^�[�̎擾
	/// </summary>
	/// <returns>�|�C���^�[�Ɠn��</returns>
	static T* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new T();
		}

		return m_pInstance;
	}

	/// <summary>
	/// �C���X�^���X�̍폜
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
	// �R�s�[�R���X�g���N�^�̋֎~
	SingletonBase(const SingletonBase&) = delete;
	SingletonBase& operator=(const SingletonBase&) = delete;

	// �V���O���g���p�|�C���^
	static T* m_pInstance;

};
// �C���X�^���X�̎��̉�
template <typename T> T* SingletonBase<T>::m_pInstance;
