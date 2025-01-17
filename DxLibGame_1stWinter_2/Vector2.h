#pragma once
#include <cmath>

#include <DxLib.h>

// 2�����x�N�g���N���X
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

	// ���Z�q�̃I�[�o�[���[�h
	// ���̃N���X���m��ꕔ�̌^��+-���̉��Z���s���ꍇ��
	// �������������Ƃ��ł���

	// ����
	// operator[�I�[�o�[���[�h���������Z�q]
	// ���̏ꍇ���ƃI�[�o�[���[�h���鉉�Z�q��"+="
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


	// �֗��Ȋ֐��Q

	/// <summary>
	/// �x�N�g����2��̒�����Ԃ�
	/// </summary>
	/// <param name="a"></param>
	/// <returns></returns>
	float SqrMagnitude(Vector2 a)const
	{
		float temp = a.x * a.x + a.y * a.y;

		return temp;
	}

	/// <summary>
	/// �x�N�g����2��̒�����Ԃ�
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
	/// ���̃x�N�g����2��̒�����Ԃ�
	/// </summary>
	/// <returns></returns>
	float SqrMagnitude() const
	{
		float temp = this->x * this->x + this->y * this->y;

		return temp;
	}

	/// <summary>
	/// �x�N�g���̒�����Ԃ�
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
	/// �x�N�g���̒�����Ԃ�
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
	/// ���̃x�N�g���̒�����Ԃ�
	/// </summary>
	/// <returns></returns>
	float Magnitude() const
	{
		float temp = this->x * this->x + this->y * this->y;

		float abs = sqrtf(temp);

		return abs;
	}

	/// <summary>
	/// a��b�̊Ԃ̋�����Ԃ�
	/// (Distance(a, b) �� (a-b).magnitude�Ɠ���)
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
	/// ���K��(normalized)���ꂽ�Ƃ��A�������ێ������܂܂�
	/// ������ 1.0 �̃x�N�g����Ԃ�
	/// </summary>
	/// <param name="a"></param>
	/// <returns></returns>
	Vector2 Normalized(Vector2 a) const
	{
		// y��0���������Ƃ��ɂ������ȋ��������Ă���

		float temp = SqrMagnitude(a);

		float abs = sqrtf(temp);

		Vector2 ans;
		ans = a / abs;

		return ans;
	}

	/// <summary>
	/// ���K��(normalized)���ꂽ�Ƃ��A�������ێ������܂܂�
	/// ������ 1.0 �̃x�N�g����Ԃ�
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
	/// a �� b by t �x�N�g���̊ԂŐ��`��Ԃ���
	/// �p�����[�^�[ t ��[0, 1] �͈̔͂Ő��������
	/// t = 0 �̏ꍇ�Aa ��Ԃ�
	/// t = 1 �̏ꍇ�Ab ��Ԃ�
	/// t = 0.5 �̏ꍇ�Aa �� b �̒��_���Ԃ����
	/// t �� 0 ~ 1 �łȂ��ꍇ a ��Ԃ�(t = 0�Ɠ���)
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


		// �o�O���Ă鏈��
		Vector2 temp;
		temp = b - a;

		temp *= t;

		//temp += a;


		// �o�O���ĂȂ�����
		//Vector2 temp;
		//temp.x = std::lerp(a.x, b.x, t);
		//temp.y = std::lerp(a.y, b.y, t);

		return temp;
	}


};