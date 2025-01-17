#pragma once

class Application
{
private:
	Application() {}
	// なぜか通ってしまうので=deleteを付けた
	Application(const Application&) = delete;
	void operator=(const Application&) = delete;

	int _frame = 0;

public:
	/// <summary>
	/// シングルトンオブジェクトを返す
	/// </summary>
	/// <returns>Applicationシングルトンオブジェクト</returns>
	static Application& GetInstance();

	/// <summary>
	/// アプリケーションの初期化
	/// </summary>
	/// <returns>true:初期化成功 / false:初期化失敗</returns>
	bool Init();

	/// <summary>
	/// メインループを起動
	/// </summary>
	void Run();

	/// <summary>
	/// アプリケーションの後処理
	/// </summary>
	void Terminate();
};

