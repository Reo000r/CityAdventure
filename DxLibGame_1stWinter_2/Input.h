#pragma once

namespace
{
	// 入力情報
	// PAD_INPUT_1 がA(下)
	// PAD_INPUT_2 がB(右)
	// PAD_INPUT_3 がX(左)
	// PAD_INPUT_4 がY(上)？
	// 今はPlayerしか置き換えられていないはず
#define INPUTRIGHT PAD_INPUT_RIGHT
#define INPUTLEFT PAD_INPUT_LEFT
#define INPUTJUMP PAD_INPUT_2
#define INPUTSHOT PAD_INPUT_1
}

/// <summary>
/// 入力系をコントロールするクラス
/// シングルトン化する
/// </summary>
class Input
{
private:
	Input() {}
	Input(const Input&) = delete;
	void operator=(const Input&) = delete;

	int _padInput = 0;
	int _lastInput = 0;

public:
	/// <summary>
	/// シングルトンオブジェクトを返す
	/// </summary>
	/// <returns>Applicationシングルトンオブジェクト</returns>
	static Input& GetInstance();

	/// <summary>
	/// 入力情報の更新
	/// </summary>
	void Update();

	/// <summary>
	/// 押されているかどうかの取得
	/// </summary>
	/// <param name="button">判定を行うボタン</param>
	/// <returns>押されていればtrue、でなければfalse</returns>
	bool IsPress(int button) const;

	/// <summary>
	/// 押された瞬間かどうかの取得
	/// </summary>
	/// <param name="button">判定を行うボタン</param>
	/// <returns>押されていればtrue、でなければfalse</returns>
	bool IsTrigger(int button) const;
};