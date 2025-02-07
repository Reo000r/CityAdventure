#pragma once
#include "Player.h"
#include "Game.h"

#include <string>

namespace ScoreData
{
	// �����N�{�[�_�[
#ifdef LOAD_DEBUG_STAGE
	constexpr int kRankSSSBorder = 66666;
	constexpr int kRankSSBorder = 60000;
	constexpr int kRankSBorder = 50000;
	constexpr int kRankABorder = 40000;
	constexpr int kRankBBorder = 30000;
	constexpr int kRankCBorder = 15000;

	constexpr int kEnemy01Score = 200;
	constexpr int kEnemy02Score = 300;
	constexpr int kBossScore = 5000;

	constexpr float kTimeToScoreMul = 30.0f;

	constexpr float kMaxAddScore = 30.0f - 0.01f;
	constexpr float kAddScoreSpeed = 3.0f - 0.01f;

	constexpr float kStartTime = 300.0f - 0.01f;
#else
	constexpr int kRankSSSBorder = 34000;
	constexpr int kRankSSBorder  = 32000;
	constexpr int kRankSBorder   = 29000;
	constexpr int kRankABorder   = 25000;
	constexpr int kRankBBorder   = 15000;
	constexpr int kRankCBorder   = 10000;

	constexpr int kEnemy01Score = 200;
	constexpr int kEnemy02Score = 300;
	constexpr int kBossScore = 5000;

	constexpr float kTimeToScoreMul = 30.0f;

	constexpr float kMaxAddScore = 30.0f - 0.01f;
	constexpr float kAddScoreSpeed = 3.0f - 0.01f;

	constexpr float kStartTime = 150.0f - 0.01f;
#endif // LOAD_DEBUG_STAGE
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

	int _addScore;

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
		_addScore = (_score - _drawScore) * (ScoreData::kAddScoreSpeed / 60.0f);
		if (_addScore > ScoreData::kMaxAddScore) _addScore = ScoreData::kMaxAddScore;
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