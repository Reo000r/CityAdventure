#pragma once
#include "Player.h"

#include <string>

namespace ScoreData
{
	// 敵 200 300
	// タイム 30/1s
	// 残HP *0.1 (+1)
	// 敵 8900
	// タイム 2100/70s
	// HP 5  *1.5
	// 最大スコア 約16500
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