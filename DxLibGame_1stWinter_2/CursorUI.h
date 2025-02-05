#pragma once
#include "Vector2.h"

namespace CursorUIData
{
	//constexpr float kCursorSizeMul = 3.0f;
	//constexpr float kEffectSizeMul = 3.0f;

	// グラフィックの幅と高さ
	constexpr int kUndecisionGraphWidth  = 48;
	constexpr int kUndecisionGraphHeight = 48;
	constexpr int kDecisionGraphWidth  = 48;
	constexpr int kDecisionGraphHeight = 48;
	constexpr int kEffectGraphWidth  = 16;
	constexpr int kEffectGraphHeight = 16;

	// 描画時の補正値
	constexpr int kUndecisionDrawPosOffsetX = 0;
	constexpr int kUndecisionDrawPosOffsetY = 0;
	constexpr int kDecisionDrawPosOffsetX = 0;
	constexpr int kDecisionDrawPosOffsetY = 0;
	constexpr int kEffectDrawPosOffsetX = 0;
	constexpr int kEffectDrawPosOffsetY = -6;
	const Vector2 kUndecisionDrawPosOffset =
	{ kUndecisionDrawPosOffsetX, kUndecisionDrawPosOffsetY };
	const Vector2 kDecisionDrawPosOffset =
	{ kDecisionDrawPosOffsetX, kDecisionDrawPosOffsetY };
	
	// エフェクトをカーソルからどのくらいずらして表示するか
	constexpr int kEffectPosOffsetX = 28;
	constexpr int kEffectPosOffsetY = 0 ;
	

	//アニメーション1コマのフレーム数
	constexpr int kUndecisionSingleAnimFrame = 15;
	constexpr int kDecisionSingleAnimFrame   = 6;
	constexpr int kEffectSingleAnimFrame = 5;
		
	// アニメーションのコマ数
	constexpr int kUndecisionAnimNum = 5;
	constexpr int kDecisionAnimNum   = 6;
	constexpr int kEffectAnimNum = 4;

	// アニメーション一周当たりのフレーム数
	constexpr int kUndecisionAnimTotalFrame = kUndecisionAnimNum * kUndecisionSingleAnimFrame;
	constexpr int kDecisionAnimTotalFrame   = kDecisionAnimNum   * kDecisionSingleAnimFrame;
	constexpr int kEffectAnimTotalFrame = kEffectAnimNum * kEffectSingleAnimFrame;
}

/// <summary>
/// どの選択肢を選んでいるか分かりやすくするためのもの
/// </summary>
class CursorUI
{
	int _useHandle;

	int _cursorUndecisionHandle;
	int _cursorDecisionHandle;
	int _effectHandle;

	//アニメーション1コマのフレーム数
	int _undecisionSingleAnimFrame;
	int _decisionSingleAnimFrame;
	int _effectSingleAnimFrame;
	int _undecisionAnimTotalFrame;
	int _decisionAnimTotalFrame;
	int _effectAnimTotalFrame;

	int _cursorAnimFrameCount;
	int _effectAnimFrameCount;
	int _decisionFrameCount;

	float _cursorSizeMul;
	float _effectSizeMul;

	// trueなら入力が効く
	bool _canInput;
	bool _isActiveEffect;

	// 現在選択中の場所
	// 0:上 1:下
	// (enumだとsceneが見るのが大変だった為)
	int _current;

	// _posは中心
	Vector2 _cursorPos;
	Vector2 _effectPos;
	Vector2 _cursorVel;
	Vector2 _effectVel;
	Vector2 _addPos;
	Vector2 _drawPos;
	Vector2 _cursorDrawPosOffset;

	// *_effectSizeMulする
	Vector2 _effectDrawPosOffset;
	Vector2 _effectPosOffset;

	using UpdateFunc_t = void(CursorUI::*)();
	UpdateFunc_t _nowUpdateState = nullptr;

	/// <summary>
	/// 未決定時のUpdate
	/// </summary>
	void UndecisionUpdate();

	/// <summary>
	/// 決定時のUpdate
	/// </summary>
	void DecisionUpdate();

	/// <summary>
	/// EffectGraphのみのUpdate
	/// 他のUpdateから呼んでくる
	/// 名前が誤解生みそう
	/// </summary>
	void EffectUpdate();

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CursorUI();

	/// <summary>
	/// 初期化
	/// </summary>
	void Init(Vector2 pos, Vector2 addPos, float cursorSizeMul, float effectSizeMul, int cursorUndecisionHandle, int cursorDecisionHandle, int effectHandle, bool canInput);

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画全般
	/// </summary>
	void Draw();

	/// <summary>
	/// 移動設定
	/// </summary>
	/// <param name="vel"></param>
	void SetCursorVel(Vector2 vel) { _cursorVel = vel; }

	/// <summary>
	/// 移動設定
	/// </summary>
	/// <param name="vel"></param>
	void SetEffectVel(Vector2 vel) { _effectVel = vel; }

	/// <summary>
	/// 現在のグラフィックを書き換える
	/// 結構強引...
	/// </summary>
	/// <param name="graphHandle"></param>
	void SetUseHandle(int graphHandle) { _useHandle = graphHandle; }

	/// <summary>
	/// 入力受付状態を切り替える
	/// </summary>
	/// <param name="canInput"></param>
	void SetCanInput(bool canInput) { _canInput = canInput; }

	/// <summary>
	/// _undecisionSingleAnimFrame
	/// _decisionSingleAnimFrame
	/// _effectSingleAnimFrame
	/// </summary>
	void SetSingleAnimFrame(int und, int d, int e)
	{
		_undecisionSingleAnimFrame = und;
		_decisionSingleAnimFrame = d;
		_effectSingleAnimFrame = e;
		_undecisionAnimTotalFrame = CursorUIData::kUndecisionAnimNum * _undecisionSingleAnimFrame;
		_decisionAnimTotalFrame = CursorUIData::kDecisionAnimNum * _decisionSingleAnimFrame;
		_effectAnimTotalFrame = CursorUIData::kEffectAnimNum * _effectSingleAnimFrame;
	}

	/// <summary>
	/// 決定が押されてからの経過フレーム
	/// </summary>
	/// <returns></returns>
	int GetDecisionFrame() const { return _decisionFrameCount; }

	/// <summary>
	/// 現在選択中の場所
	/// 0:上 1:下
	/// (enumだとsceneが見るのが大変だった為)
	/// </summary>
	/// <returns></returns>
	int GetCurrentPosition() const { return _current; }
};