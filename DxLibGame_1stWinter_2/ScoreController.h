#pragma once
#include "Player.h"

#include <string>

namespace ScoreData
{
	// �G 200 300
	// �^�C�� 30/1s
	// �cHP *0.1 (+1)
	// �G 8900
	// �^�C�� 2100/70s
	// HP 5  *1.5
	// �ő�X�R�A ��16500
	constexpr int kRankSSSBorder = 16500;
	constexpr int kRankSSBorder = 16000;
	constexpr int kRankSBorder = 15000;
	constexpr int kRankABorder = 13000;
	constexpr int kRankBBorder = 10000;
	constexpr int kRankCBorder = 5000;

	constexpr int kEnemy01Score = 200;
	constexpr int kEnemy02Score = 300;
}

/// <summary>
///	�X�R�A�Ǘ����s���V���O���g���N���X
/// </summary>
class ScoreController
{
private:

	// �V���O���g����
	ScoreController();
	ScoreController(const ScoreController&) = delete;
	void operator=(const ScoreController&) = delete;

	int _score;
	int _drawScore;
	float _time;
	int _playerHitpoint;

	// time�܂߂��X�R�A
	float _scoreIncludingTimeAndHitpointMul;

	std::wstring _rank;

	bool _isCount;
	bool _isTimeover;

	int _gameFontHandle;
	int _resultFontHandle;
	
public:

	/// <summary>
	/// �V���O���g���I�u�W�F�N�g��Ԃ�
	/// </summary>
	/// <returns>SceneController�V���O���g���I�u�W�F�N�g</returns>
	static ScoreController& GetInstance();

	void Init();

	void Update();

	void GameDraw();
	void DebugDraw();

	void ResultInit();
	void ResultDraw();
	
	void UpdateScore()
	{
		_scoreIncludingTimeAndHitpointMul =
			// ���Ԃ��X�R�A�ɕϊ�
			(_score + (_time * 30.0f)) *
			// �ϊ��������̂�playerHitpoint*0.1+1�{����
			((_playerHitpoint * 0.1f) + 1.0f);
	}

	// �Q�[���ɍs���O��
	void Reset();

	// �G��|���A�S�[�������㎞�ԉ��Z��
	void AddScore(int score)
	{
		_score += score;
	}

	// �J�E���g�̃I���I�t�؂�ւ�
	void SetTimeCount(bool isCount)
	{
		_isCount = isCount;
	}

	// Player����HP�󋵂��󂯎��
	void SetPlayerHitpoint(int hp)
	{
		_playerHitpoint = hp;
	}

	int GetScore() const { return _score; }

	bool IsTimeover() const { return _isTimeover; }
};