#pragma once
#include "Vector2.h"
#include "Game.h"
#include "GameSceneCamera.h"

#include <memory>

// ����Collider�N���X���p�����Ă������A
// CircleCollider�̎����\�肪�����Ȃ����̂�
// Collider�Ɏ������Ă������̂𒼐ڎ��悤�ɂȂ���
class BoxCollider
{
protected:

	// �I�u�W�F�N�g�̒��S�ʒu
	Vector2 _pos;

	// �����蔻��쐬���̈ʒu�����p
	Vector2 _colPosOffset;

	// ����̕��ƍ���
	// (�\���{���͊܂�)
	Game::Size _colSize;

	// ���̃t���[���Ńq�b�g���Ă������ǂ���
	// �eUpdate�֐����̍ŏ���false�ɂ��Ă���
	// (����Ȃ�����)
	bool _isHitFrame;

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="pos">���S�ʒu</param>
	/// <param name="offset">�����蔻��쐬���̈ʒu�␳�l</param>
	/// <param name="size">�\���{�����܂ޔ���̕��ƍ���</param>
	BoxCollider(Vector2 pos, Vector2 offset, Game::Size size);

	/// <summary>
	/// �����蔻��̕\����
	/// �I�u�W�F�N�g�̒��S�ɉ~�̕\��
	/// </summary>
	/// <param name="camera"></param>
	void DispCol(std::weak_ptr<GameSceneCamera> camera) const;

	/// <summary>
	/// �I�u�W�F�N�g�̒��S�ʒu��Ԃ�
	/// </summary>
	/// <returns></returns>
	Vector2 GetPos() const { return _pos; }

	/// <summary>
	/// �傫���̔{�����܂񂾔���̕��ƍ�������Ԃ�
	/// </summary>
	/// <returns>�傫���̔{�����܂񂾔���̕��ƍ������</returns>
	Game::Size GetBoxSize() const { return _colSize; }

	/// <summary>
	/// �ʒu������������`����Ԃ�
	/// �傫���̔{���͊܂�
	/// �����蔻��̈ʒu�␳�͍l�����Ȃ�
	/// (�ʒu���������Ȃ��ق����g���₷���H)
	/// </summary>
	/// <returns>�ʒu������������`���</returns>
	Game::Rect GetRect() const
	{
		Game::Rect rect;

		rect.top    = static_cast<int>(_pos.y - _colSize.height * 0.5f);
		rect.bottom = static_cast<int>(_pos.y + _colSize.height * 0.5f);
		rect.left   = static_cast<int>(_pos.x - _colSize.width  * 0.5f);
		rect.right  = static_cast<int>(_pos.x + _colSize.width  * 0.5f);
		
		return rect;
	}

	/// <summary>
	/// �ʒu�Ɠ����蔻�������������`����Ԃ�
	/// �傫���̔{���͊܂�
	/// colPosOffset���v�Z�ɒǉ�
	/// </summary>
	/// <returns>�ʒu�Ɠ����蔻�������������`���</returns>
	Game::Rect GetColRect() const
	{
		Game::Rect rect;

		rect.top    = static_cast<int>(_pos.y - _colSize.height * 0.5f + _colPosOffset.y);
		rect.bottom = static_cast<int>(_pos.y + _colSize.height * 0.5f + _colPosOffset.y);
		rect.left   = static_cast<int>(_pos.x - _colSize.width  * 0.5f + _colPosOffset.x);
		rect.right  = static_cast<int>(_pos.x + _colSize.width  * 0.5f + _colPosOffset.x);
		
		return rect;
	}

	/// <summary>
	/// ��`���ɃJ�����̕␳�����������l��Ԃ�
	/// �傫���̔{���͊܂�
	/// </summary>
	/// <returns>�ʒu�Ɠ����蔻�������������`���</returns>
	Game::Rect AddCameraOffset(Game::Rect rect, std::weak_ptr<GameSceneCamera> camera) const
	{
		Vector2 offset = camera.lock()->GetDrawOffset();
		rect.top    += static_cast<int>(offset.y);
		rect.bottom += static_cast<int>(offset.y);
		rect.left   += static_cast<int>(offset.x);
		rect.right  += static_cast<int>(offset.x);
		
		return rect;
	}
};

