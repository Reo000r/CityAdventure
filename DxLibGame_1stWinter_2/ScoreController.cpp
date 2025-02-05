#include "ScoreController.h"
#include <DxLib.h>
#include "Vector2.h"
#include "Game.h"

#include <cassert>

ScoreController::ScoreController() :
	_score(0),
	_drawScore(0),
	_time(0.0f),
	_playerHitpoint(0),
	_scoreIncludingTimeAndHitpointMul(0.0f),
	_rank(L"NAN"),
	_isCount(false),
	_isTimeover(false),
	_gameFontHandle(0),
	_resultFontHandle(0)
{
	int gameFontSize = 48;
	int gameFontWidth = -1; // 0-9
	int resultFontSize = 48;
	int resultFontWidth = -1; // 0-9
	_gameFontHandle = CreateFontToHandle(L"Algerian", gameFontSize, gameFontWidth, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	assert(_gameFontHandle >= 0);
	_resultFontHandle = CreateFontToHandle(L"Algerian", resultFontSize, resultFontWidth, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	assert(_resultFontHandle >= 0);
}

ScoreController& ScoreController::GetInstance()
{
	// この宣言の時点でメモリが確保されてアプリ終了まで残る
	static ScoreController controller;
	return controller;
}

void ScoreController::Init()
{
	Reset();
}

void ScoreController::Update()
{
	if (_isCount) _time -= 1 / 60.0f;

	UpdateScore();

	if (_score > _drawScore) _drawScore += 7;
	if (_score < _drawScore) _drawScore = _score;

	if (_time <= 0.0f && !_isTimeover)
	{
		_isTimeover = true;
	}
}

void ScoreController::GameDraw()
{
	//Vector2 _drawScorePos = { 256+512, 512-64 };
	//Vector2 _drawRankPos  = { 256+512, 512-0 };
	Vector2 _drawTimePos  = { Game::kScreenWidth - 300-120, 0 };
	Vector2 _drawScorePos = { Game::kScreenWidth - 300-120, 0+32+16 };
	float temp = _time;
	int time = static_cast<int>(floor(temp));

	DrawFormatStringToHandle(_drawTimePos.x, _drawTimePos.y, 0xeeeeee, _gameFontHandle, L"Time   :    %03d", time);
	DrawFormatStringToHandle(_drawScorePos.x, _drawScorePos.y, 0xeeeeee, _gameFontHandle, L"Score:%05d", _drawScore);

#ifdef _DEBUG

	DebugDraw();

#endif // _DEBUG
}

void ScoreController::DebugDraw()
{
#ifdef _DEBUG

	DrawFormatString(0, 256, 0xffffff, L"_time : %f", _time);
	DrawFormatString(0, 256 + 16 * 1, 0xffffff, L"_score : %d", _score);
	DrawFormatString(0, 256 + 16 * 2, 0xffffff, L"_scoreIncludingTime : %f", _scoreIncludingTimeAndHitpointMul);
	DrawFormatString(0, 256 + 16 * 3, 0xffffff, L"_playerHitpoint : %d", _playerHitpoint);

#endif // _DEBUG
}

void ScoreController::ResultInit()
{
	UpdateScore();
	float temp = _scoreIncludingTimeAndHitpointMul;
	int score = static_cast<int>(floor(temp));

	std::wstring str;

	if (score >= ScoreData::kRankSSSBorder) { str = L"SSS"; }
	else if (score >= ScoreData::kRankSSBorder) { str = L"SS"; }
	else if (score >= ScoreData::kRankSBorder) { str = L"S"; }
	else if (score >= ScoreData::kRankABorder) { str = L"A"; }
	else if (score >= ScoreData::kRankBBorder) { str = L"B"; }
	else if (score >= ScoreData::kRankCBorder) { str = L"C"; }
	else { str = L"D"; }
	
	_rank = str;
}

void ScoreController::ResultDraw()
{
	//Vector2 _drawScorePos = { 256+512, 512-64 };
	//Vector2 _drawRankPos  = { 256+512, 512-0 };
	Vector2 _drawScorePos = { Game::kScreenCenterWidth-300-100, Game::kScreenCenterHeight-50 };
	Vector2 _drawRankPos  = { Game::kScreenCenterWidth+300-200, Game::kScreenCenterHeight-50 };
	float temp = _scoreIncludingTimeAndHitpointMul;
	int score = static_cast<int>(floor(temp));

	DrawFormatStringToHandle(_drawScorePos.x, _drawScorePos.y, 0xeeeeee, _resultFontHandle, L"Score : %d", score);
	DrawFormatStringToHandle(_drawRankPos.x, _drawRankPos.y, 0xeeeeee, _resultFontHandle, L"Rank : %s", _rank.c_str());

#ifdef _DEBUG

	DebugDraw();

#endif // _DEBUG

}

void ScoreController::Reset()
{
	_score = 0;
	_drawScore = 0;
	_time = 120.0f;
	_playerHitpoint = 0;
	_scoreIncludingTimeAndHitpointMul = 0.0f;
	_rank = L"NAN";
	_isCount = false;
	_isTimeover = false;
}