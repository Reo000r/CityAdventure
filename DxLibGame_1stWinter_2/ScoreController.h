#pragma once
#include "Player.h"
#include "Game.h"

#include <string>

namespace ScoreData
{
	// ランクボーダー
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
///	スコア管理を行うシングルトンクラス
/// </summary>
class ScoreController
{
private:

	// シングルトン化
	ScoreController();
	ScoreController(const ScoreController&) = delete;
	void operator=(const ScoreController&) = delete;

	int _score;
	int _drawScore;
	float _time;
	int _playerHitpoint;

	int _addScore;

	// time含めたスコア
	float _scoreIncludingTimeAndHitpointMul;

	std::wstring _rank;

	bool _isCount;
	bool _isTimeover;

	int _gameFontHandle;
	int _resultFontHandle;
	
public:

	/// <summary>
	/// シングルトンオブジェクトを返す
	/// </summary>
	/// <returns>SceneControllerシングルトンオブジェクト</returns>
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
			// 時間をスコアに変換
			(_score + (_time * 30.0f)) *
			// 変換したものをplayerHitpoint*0.1+1倍する
			((_playerHitpoint * 0.1f) + 1.0f);
	}

	// ゲームに行く前等
	void Reset();

	// 敵を倒す、ゴールした後時間加算等
	void AddScore(int score)
	{
		_score += score;
		_addScore = (_score - _drawScore) * (ScoreData::kAddScoreSpeed / 60.0f);
		if (_addScore > ScoreData::kMaxAddScore) _addScore = ScoreData::kMaxAddScore;
	}

	// カウントのオンオフ切り替え
	void SetTimeCount(bool isCount)
	{
		_isCount = isCount;
	}

	// PlayerからHP状況を受け取る
	void SetPlayerHitpoint(int hp)
	{
		_playerHitpoint = hp;
	}

	int GetScore() const { return _score; }

	bool IsTimeover() const { return _isTimeover; }
};