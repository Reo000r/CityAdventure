#include "CursorUI.h"
#include "Input.h"

#include <cassert>

void CursorUI::UndecisionUpdate()
{
	_cursorAnimFrameCount++;

	if (_cursorAnimFrameCount >= _undecisionAnimTotalFrame)
	{
		_cursorAnimFrameCount = 0;
	}

	// エフェクトが出ていたら入力を受け付けない
	if (_isActiveEffect) return;

	// 決定
	// (決定っぽいボタンを割り当てて)
	if (_canInput &&
		Input::GetInstance().IsTrigger(INPUTJUMP))
	{
		_nowUpdateState = &CursorUI::DecisionUpdate;
		_useHandle = _cursorDecisionHandle;
		_cursorAnimFrameCount = 0;
		_isActiveEffect = true;
		_cursorDrawPosOffset = CursorUIData::kDecisionDrawPosOffset;
		return;
	}

	// 同時入力は弾く
	if (_canInput &&
		Input::GetInstance().IsTrigger(PAD_INPUT_DOWN) &&
		Input::GetInstance().IsTrigger(PAD_INPUT_UP))
	{
		//return;
	}
	else if (_canInput &&
			 (_current != 1) &&
		 	 (Input::GetInstance().IsTrigger(PAD_INPUT_DOWN)))
	{
		_current = 1;
		_drawPos = _cursorPos + _addPos;
	}
	else if (_canInput &&
			 (_current != 0) &&
			 (Input::GetInstance().IsTrigger(PAD_INPUT_UP)))
	{
		_current = 0;
		_drawPos = _cursorPos;
	}

	_cursorPos += _cursorVel;

	// 上
	if (_current == 0)
	{
		_drawPos = _cursorPos;
	}
	// 下
	else if (_current == 1)
	{
		_drawPos = _cursorPos + _addPos;
	}
	else
	{
		assert(false);
	}
}

void CursorUI::DecisionUpdate()
{
	_decisionFrameCount++;
	_cursorAnimFrameCount++;

	if (_cursorAnimFrameCount >= _decisionAnimTotalFrame)
	{
		_cursorAnimFrameCount = 0;
	}

	_cursorPos += _cursorVel;

	// 上
	if (_current == 0)
	{
		_drawPos = _cursorPos;
	}
	// 下
	else if (_current == 1)
	{
		_drawPos = _cursorPos + _addPos;
	}
	else
	{
		assert(false);
	}

	EffectUpdate();
}

void CursorUI::EffectUpdate()
{
	_effectAnimFrameCount++;

	if (_effectAnimFrameCount >= _effectAnimTotalFrame)
	{
		_effectAnimFrameCount = 0;
	}

	_effectPos += _effectVel;
}

CursorUI::CursorUI() :
	_nowUpdateState(&CursorUI::UndecisionUpdate),
	_useHandle(0),
	_cursorUndecisionHandle(0),
	_cursorDecisionHandle(0),
	_effectHandle(0),
	_undecisionSingleAnimFrame(CursorUIData::kUndecisionSingleAnimFrame),
	_decisionSingleAnimFrame(CursorUIData::kDecisionSingleAnimFrame),
	_effectSingleAnimFrame(CursorUIData::kEffectSingleAnimFrame),
	_undecisionAnimTotalFrame(CursorUIData::kUndecisionAnimNum* CursorUIData::kUndecisionSingleAnimFrame),
	_decisionAnimTotalFrame(CursorUIData::kDecisionAnimNum* CursorUIData::kDecisionSingleAnimFrame),
	_effectAnimTotalFrame(CursorUIData::kEffectAnimNum* CursorUIData::kEffectSingleAnimFrame),
	_cursorAnimFrameCount(0),
	_effectAnimFrameCount(0),
	_decisionFrameCount(0),
	_cursorSizeMul(1.0f),
	_effectSizeMul(1.0f),
	_canInput(false),
	_isActiveEffect(false),
	_current(0),
	_cursorPos({}),
	_effectPos({}),
	_cursorVel({}),
	_effectVel({}),
	_addPos({}),
	_drawPos({}),
	_cursorDrawPosOffset(CursorUIData::kUndecisionDrawPosOffset),
	_effectDrawPosOffset({ CursorUIData::kEffectDrawPosOffsetX, CursorUIData::kEffectDrawPosOffsetY }),
	_effectPosOffset({ CursorUIData::kEffectPosOffsetX, CursorUIData::kEffectPosOffsetY })
{
}

void CursorUI::Init(Vector2 pos, Vector2 addPos, float cursorSizeMul, float effectSizeMul, int cursorUndecisionHandle, int cursorDecisionHandle, int effectHandle, bool canInput)
{
	_cursorPos = pos;
	_addPos = addPos;
	_drawPos = _cursorPos;

	_cursorSizeMul = cursorSizeMul;
	_effectSizeMul = effectSizeMul;
	_effectDrawPosOffset *= _effectSizeMul;
	_effectPosOffset     *= _effectSizeMul;

	_cursorUndecisionHandle = cursorUndecisionHandle;
	_cursorDecisionHandle   = cursorDecisionHandle;
	_effectHandle = effectHandle;
	_useHandle = _cursorUndecisionHandle;

	_canInput = canInput;
}

void CursorUI::Update()
{
	(this->*_nowUpdateState)();
}

void CursorUI::Draw()
{
	// グラフィックの切り出し位置(X座標)を計算する
	int animNo = _cursorAnimFrameCount / _undecisionSingleAnimFrame;

	// 表示位置(中心)
	int x = static_cast<int>(_drawPos.x + _cursorDrawPosOffset.x);
	int y = static_cast<int>(_drawPos.y + _cursorDrawPosOffset.y);

	// 切り出し開始位置
	int cutX = animNo * CursorUIData::kUndecisionGraphWidth;
	int cutY = 0;

	// 切り出し量
	int width  = CursorUIData::kUndecisionGraphWidth;
	int height = CursorUIData::kUndecisionGraphHeight;

	// 雑処理
	if (_useHandle == _cursorDecisionHandle)
	{
		int animNo = _cursorAnimFrameCount / _decisionSingleAnimFrame;

		int cutX = animNo * CursorUIData::kDecisionGraphWidth;
		int cutY = 0;

		int width  = CursorUIData::kDecisionGraphWidth;
		int height = CursorUIData::kDecisionGraphHeight;
	}

	DrawRectRotaGraph(
		x, y,
		cutX, cutY,
		width, height,
		_cursorSizeMul, 0.0,
		_useHandle, true, false, false);


	// エフェクト表示
	if (_isActiveEffect)
	{
		// グラフィックの切り出し位置(X座標)を計算する
		animNo = _effectAnimFrameCount / _effectSingleAnimFrame;

		// 表示位置(中心)
		x = static_cast<int>(_effectPos.x + _drawPos.x + _effectPosOffset.x + _effectDrawPosOffset.x);
		y = static_cast<int>(_effectPos.y + _drawPos.y + _effectPosOffset.y + _effectDrawPosOffset.y);

		// 切り出し開始位置
		cutX = animNo * CursorUIData::kEffectGraphWidth;
		cutY = 0;

		// 切り出し量
		width  = CursorUIData::kEffectGraphWidth;
		height = CursorUIData::kEffectGraphHeight;

		// エフェクト
		DrawRectRotaGraph(
			x, y,
			cutX, cutY,
			width, height,
			_effectSizeMul, 0.0,
			_effectHandle, true, false, false);
	}
	
}
