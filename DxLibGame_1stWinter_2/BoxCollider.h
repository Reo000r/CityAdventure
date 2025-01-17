#pragma once
#include "Vector2.h"
#include "Game.h"
#include "GameSceneCamera.h"

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
	BoxCollider(Vector2 pos, Vector2 offset, Game::Size size);

	/// <summary>
	/// �����蔻��̕\����
	/// �I�u�W�F�N�g�̒��S�ɉ~�̕\��
	/// </summary>
	/// <param name="camera"></param>
	void DispCol(GameSceneCamera camera);

	/// <summary>
	/// �傫���̔{�����܂񂾔���̕��ƍ�������Ԃ�
	/// </summary>
	/// <returns>�傫���̔{�����܂񂾔���̕��ƍ������</returns>
	Game::Size GetBoxSize() { return _colSize; }

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
	Game::Rect AddCameraOffset(Game::Rect rect, const GameSceneCamera camera)
	{
		rect.top    += static_cast<int>(camera.GetDrawOffset().y);
		rect.bottom += static_cast<int>(camera.GetDrawOffset().y);
		rect.left   += static_cast<int>(camera.GetDrawOffset().x);
		rect.right  += static_cast<int>(camera.GetDrawOffset().x);
		
		return rect;
	}
};

