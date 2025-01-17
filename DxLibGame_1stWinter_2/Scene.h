#pragma once

class SceneController;

/// <summary>
/// シーンの基底クラス
/// 実体は持たない
/// </summary>
class Scene
{
public:
	Scene() {};

	/// <summary>
	/// 内部変数の更新
	/// </summary>
	virtual void Update() abstract;

	/// <summary>
	/// 描画全般
	/// </summary>
	virtual void Draw() abstract;
};

