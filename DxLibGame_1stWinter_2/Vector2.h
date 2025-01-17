#pragma once
#include <cmath>

#include <DxLib.h>

// 2次元ベクトルクラス
class Vector2
{
public:
	float x;
	float y;

public:
	Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2(float _x, float _y) :
		x(_x),
		y(_y)
	{
	}

	// 演算子のオーバーロード
	// このクラス同士や一部の型と+-等の演算を行う場合の
	// 処理を書くことができる

	// 書式
	// operator[オーバーロードしたい演算子]
	// ↓の場合だとオーバーロードする演算子は"+="
	void operator+=(const Vector2& vec)
	{
		x += vec.x;
		y += vec.y;
	}
	Vector2 operator+(const Vector2& vec) const
	{
		Vector2 temp{ x + vec.x, y + vec.y };
		return temp;
	}

	// -
	void operator-=(const Vector2& vec)
	{
		x -= vec.x;
		y -= vec.y;
	}
	Vector2 operator-(const Vector2& vec) const
	{
		Vector2 temp{ x - vec.x, y - vec.y };
		return temp;
	}

	// *
	void operator*=(float scale)
	{
		x *= scale;
		y *= scale;
	}
	Vector2 operator*(float scale) const
	{
		Vector2 temp{ x * scale, y * scale };
		return temp;
	}

	// /
	void operator/=(float scale)
	{
		x /= scale;
		y /= scale;
	}
	Vector2 operator/(float scale) const
	{
		Vector2 temp{ x / scale, y / scale };
		return temp;
	}


	// 便利な関数群

	/// <summary>
	/// ベクトルの2乗の長さを返す
	/// </summary>
	/// <param name="a"></param>
	/// <returns></returns>
	float SqrMagnitude(Vector2 a)const
	{
		float temp = a.x * a.x + a.y * a.y;

		return temp;
	}

	/// <summary>
	/// ベクトルの2乗の長さを返す
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	/// <returns></returns>
	float SqrMagnitude(float _x, float _y)const
	{
		float temp = _x * _x + _y * _y;

		return temp;
	}

	/// <summary>
	/// このベクトルの2乗の長さを返す
	/// </summary>
	/// <returns></returns>
	float SqrMagnitude() const
	{
		float temp = this->x * this->x + this->y * this->y;

		return temp;
	}

	/// <summary>
	/// ベクトルの長さを返す
	/// </summary>
	/// <param name="a"></param>
	/// <returns></returns>
	float Magnitude(Vector2 a) const
	{
		float temp = SqrMagnitude(a);

		float abs = sqrtf(temp);

		return abs;
	}

	/// <summary>
	/// ベクトルの長さを返す
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	/// <returns></returns>
	float Magnitude(float _x, float _y) const
	{
		float temp = SqrMagnitude(_x, _y);

		float abs = sqrtf(temp);

		return abs;
	}

	/// <summary>
	/// このベクトルの長さを返す
	/// </summary>
	/// <returns></returns>
	float Magnitude() const
	{
		float temp = this->x * this->x + this->y * this->y;

		float abs = sqrtf(temp);

		return abs;
	}

	/// <summary>
	/// aとbの間の距離を返す
	/// (Distance(a, b) は (a-b).magnitudeと同じ)
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	float Distance(Vector2 a, Vector2 b) const
	{
		Vector2 vTemp;
		vTemp = a - b;

		float fTemp = SqrMagnitude(vTemp);

		float abs = sqrtf(fTemp);

		return abs;
	}

	/// <summary>
	/// 正規化(normalized)されたとき、方向を維持したままで
	/// 長さが 1.0 のベクトルを返す
	/// </summary>
	/// <param name="a"></param>
	/// <returns></returns>
	Vector2 Normalized(Vector2 a) const
	{
		// yに0が入ったときにおかしな挙動をしていた

		float temp = SqrMagnitude(a);

		float abs = sqrtf(temp);

		Vector2 ans;
		ans = a / abs;

		return ans;
	}

	/// <summary>
	/// 正規化(normalized)されたとき、方向を維持したままで
	/// 長さが 1.0 のベクトルを返す
	/// </summary>
	/// <param name="_x"></param>
	/// <param name="_y"></param>
	/// <returns></returns>
	Vector2 Normalized(float _x, float _y) const
	{
		float temp = SqrMagnitude(_x, _y);

		float abs = sqrtf(temp);

		Vector2 ans;
		ans.x = _x / abs;
		ans.y = _y / abs;

		return ans;
	}

	/// <summary>
	/// a と b by t ベクトルの間で線形補間する
	/// パラメーター t は[0, 1] の範囲で制限される
	/// t = 0 の場合、a を返す
	/// t = 1 の場合、b を返す
	/// t = 0.5 の場合、a と b の中点が返される
	/// t が 0 ~ 1 でない場合 a を返す(t = 0と同じ)
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <param name="t"></param>
	/// <returns></returns>
	Vector2 Lerp(Vector2 a, Vector2 b, float t) const
	{
		// 
		if (t < 0.0f) return a;
		if (t > 1.0f) return b;


		// バグってる処理
		Vector2 temp;
		temp = b - a;

		temp *= t;

		//temp += a;


		// バグってない処理
		//Vector2 temp;
		//temp.x = std::lerp(a.x, b.x, t);
		//temp.y = std::lerp(a.y, b.y, t);

		return temp;
	}


};