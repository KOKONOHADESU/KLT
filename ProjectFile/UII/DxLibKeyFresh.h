#pragma once

#include "SingletonBase.h"

#include <DxLib.h>

namespace
{
	// �L�[�o�b�t�@
	constexpr int kKeyStateBuf = 256;

	// �g���K�[�p�t���[���J�E���g
	constexpr int kKeyTriggerFrameCount = 1;
}

/// <summary>
/// �L�[�{�[�h�֘A
/// </summary>
namespace KEY
{
	/// <summary>
	/// �L�[�{�[�h����
	/// </summary>
	class DxLibKeyFresh : public SingletonBase<DxLibKeyFresh>
	{
	public:
		// SingletonBase�N���X�ł̂݃C���X�^���X������������
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
		/// �L�[�{�[�h�̓��͏�Ԏ擾
		/// </summary>
		void Update()
		{
			// �L�[�{�[�h�̓��͏�Ԃ��擾����
			GetHitKeyStateAll(m_keyBuf);
		}

		/// <summary>
		/// ������������
		/// </summary>
		/// <param name="keyNo">���͂���L�[                            </param>
		/// <returns           >true : �����Ă��� , false : �����Ă��Ȃ�</returns>
		bool IsPress(const int keyNo) const
		{
			// �w��̃L�[���͂̔��������
			if (m_keyBuf[keyNo])
			{
				return true;
			}

			return false;
		}

		/// <summary>
		/// �g���K�[����
		/// �{�^�����������ꍇ�ɔ�����Ƃ�
		/// </summary>
		/// <param name="keyNo"          >���͂���L�[                            </param>
		/// <param name="delayInputFrame">���͂Ƀf�B���C�t���[�����w�肷��        </param>
		/// <returns           >true : �����Ă��� , false : �����Ă��Ȃ�</returns>
		bool IsTrigger(const int keyNo , const int delayInputFrame = kKeyTriggerFrameCount)
		{			
			// �L�[��������Ă���ꍇ
			if (m_keyBuf[keyNo])
			{
				// �t���[���J�E���g��i�߂�
				m_pressFrameCount++;

				// �w�肵���t���[���̎��Ɉ�x�����g���K�[�𔭐�
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
		/// �����[�X
		/// �{�^���𗣂����ꍇ�ɓ��͂�����
		/// </summary>
		/// <param name="keyNo">���͂���L�[                            </param>
		/// <returns           >true : �����Ă��� , false : �����Ă��Ȃ�</returns>
		bool IsRelease(const int keyNo)
		{			
			// �w��̃L�[���͂̔��������
			if (m_keyBuf[keyNo])
			{
				// ����������
				m_isPress = true;
				// �����Ă��Ȃ��̊m�F
				m_isCheckOneNoPress = false;
			}
			else
			{
				// ���������Ă��Ȃ��ꍇ
				m_isPress = false;
			}

			// ��x���͂��Ă���ꍇ�A���������ĂȂ��ꍇ���m�F
			if (!m_isCheckOneNoPress && !m_isPress)
			{
				// ���Z�b�g
				ReleaseReset();

				// ���͏I��
				return true;
			}

			return false;
		}

		/// <summary>
		/// �����[�X�{�^���̃��Z�b�g
		/// </summary>
		void ReleaseReset()
		{
			// ���Z�b�g
			m_isPress           = false;
			m_isCheckOneNoPress = true;
		}

		/// <summary>
		/// ���ׂẴL�[���͊m�F
		/// </summary>
		/// <returns></returns>
		bool IsCheckHitKeyAll()
		{
			return CheckHitKeyAll();
		}

	private:
		// �L�[�p�o�b�t�@
		char m_keyBuf[kKeyStateBuf];

		// �����Ă���Ԃ̃t���[���J�E���g
		int m_pressFrameCount;

		// �����[�X����p
		// ���������Ă��邩
		bool m_isPress;

		// �����Ă��Ȃ����̊m�F����x����
		bool m_isCheckOneNoPress;
	};
}