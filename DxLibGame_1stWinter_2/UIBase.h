#pragma once
#include "vector2.h"

#include <memory>

/// <summary>
/// UIの基底クラス
/// </summary>
class UIBase
{
protected:

	int _useHandle;

	int _animFrameCount;

	// 左ならtrue、右ならfalse
	bool _isReverseGraphX;

	// trueなら活性化
	// UpdateとDrawとDisActiveを受け付ける
	// falseなら非活性化
	// Activeを受け付ける
	// 初期false
	bool _isActive;

	// 透明度
	// 0~1
	// 1が不透明
	float _alpha;

	// _posは中心
	Vector2 _pos;
	
	Vector2 _vel;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	UIBase(Vector2 pos);

	// (引数同じじゃないので)
	/// <summary>
	/// 初期化
	/// </summary>
	//virtual void Init(int graphHandle) abstract;

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	virtual void Update() abstract;

	/// <summary>
	/// 描画全般
	/// </summary>
	virtual void Draw() abstract;

	/// <summary>
	/// 活性化
	/// </summary>
	virtual bool Active(Vector2 pos, bool isReverse) abstract;

	/// <summary>
	/// 非活性化
	/// </summary>
	virtual void DisActive() abstract;

	/// <summary>
	/// 活性化状態を取得
	/// </summary>
	bool GetActiveStats() const { return _isActive; }
};