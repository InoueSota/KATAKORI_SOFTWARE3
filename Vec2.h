#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
struct Vec2
{
	/// ƒƒ“ƒo•Ï”

	float x; //¬•ªx
	float y; //¬•ªy



/// ƒƒ“ƒoŠÖ”

	/// <summary>
	/// ƒfƒtƒHƒ‹ƒgƒRƒ“ƒXƒgƒ‰ƒNƒ^
	/// </summary>
	Vec2();


	/// <summary>
	/// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	/// </summary>
	/// <param name="nx"> x¬•ª</param>
	/// <param name="ny"> y¬•ª</param>
	 Vec2(float nx, float ny);

	/// <summary>
	/// ’·‚³‚ğ‹‚ß‚éŠÖ”
	/// </summary>
	/// <returns>floatŒ^</returns>
	float Length() const;

	/// <summary>
	/// ’·‚³‚Ì2æ‚ğ‹‚ß‚éŠÖ”
	/// </summary>
	/// <returns>floatŒ^</returns>
	float LengthSquare() const;

	/// <summary>
	/// ˆø”‚ÌƒxƒNƒgƒ‹‚Æ‚Ì“àÏ
	/// </summary>
	/// <param name="other"> Vec2</param>
	/// <returns>floatŒ^</returns>
	 float Dot(const Vec2& other) const;

	/// <summary>
	/// ˆø”‚ÌƒxƒNƒgƒ‹‚Æ‚ÌŠOÏ
	/// </summary>
	/// <param name="other"> Vec2</param>
	/// <returns>floatŒ^</returns>
	 float Cross(const Vec2& other) const;

	/// <summary>
	/// ˆø”‚ÌƒxƒNƒgƒ‹‚Æ‚Ì‹——£
	/// </summary>
	/// <param name="other"> Vec2</param>
	/// <returns>floatŒ^</returns>
	float DistanceFrom(const Vec2& other) const;

	/// <summary>
	/// ³‹K‰»
	/// </summary>
	/// <returns>Vec2Œ^</returns>
	Vec2 Normalized() const;

	/// <summary>
	/// ƒ[ƒƒxƒNƒgƒ‹‚Å‚ ‚é‚©
	/// </summary>
	/// <returns>true: 0‚Å‚ ‚é   false: 0‚Å‚È‚¢</returns>
	 bool isZero() const;

	/// <summary>
	/// ƒ[ƒƒxƒNƒgƒ‹‚É‚·‚é
	/// </summary>
	void setZero();

	/// <summary>
	/// ƒxƒNƒgƒ‹‚Ì‰ñ“]
	/// </summary>
	/// <param name="t"> ‰ñ“]Šp“xƒ‰ƒWƒAƒ“</param>
	/// <returns>Vec2Œ^</returns>
	Vec2 Rotation(float t) const;

	/// <summary>
	/// ’P€ + 
	/// </summary>
	/// <returns>Vec2Œ^</returns>
	Vec2 operator +() const;

	/// <summary>
	/// ’P€ - 
	/// </summary>
	/// <returns>Vec2Œ^</returns>
	 Vec2 operator -() const;

	/// <summary>
	/// 2€ + 
	/// </summary>
	/// <param name="other"> Vec2</param>
	/// <returns>Vec2Œ^</returns>
	 Vec2 operator +(const Vec2& other) const;

	/// <summary>
	/// 2€ - 
	/// </summary>
	/// <param name="other"> Vec2</param>
	/// <returns>Vec2Œ^</returns>
	 Vec2 operator -(const Vec2& other) const;

	/// <summary>
	/// 2€ * 
	/// </summary>
	/// <param name="s"> s”{</param>
	/// <returns>Vec2Œ^</returns>
	Vec2 operator *(float s) const;

	/// <summary>
	/// 2€ / 
	/// </summary>
	/// <param name="s">  1 / s”{</param>
	/// <returns>Vec2Œ^</returns>
	 Vec2 operator /(float s) const;

	/// <summary>
	/// •¡‡‘ã“ü‰‰Z +=
	/// </summary>
	/// <param name="other"> Vec2</param>
	/// <returns>Vec2Œ^</returns>
	Vec2& operator +=(const Vec2& other);

	/// <summary>
	/// •¡‡‘ã“ü‰‰Z -=
	/// </summary>
	/// <param name="other"> Vec2</param>
	/// <returns>Vec2Œ^</returns>
	Vec2& operator -=(const Vec2& other);

	/// <summary>
	/// •¡‡‘ã“ü‰‰Z *= 
	/// </summary>
	/// <param name="s"> s”{</param>
	/// <returns>Vec2Œ^</returns>
	Vec2& operator *=(float s);

	/// <summary>
	/// •¡‡‘ã“ü‰‰Z /= 
	/// </summary>
	/// <param name="s"> 1 / s”{</param>
	/// <returns>Vec2Œ^</returns>
	Vec2& operator /=(float s);

};

/// <summary>
/// Vec2 ‚ªŒã‚É‚­‚é 2€ * 
/// </summary>
/// <param name="s"> s”{</param>
/// <param name="v"> Vec2</param>
/// <returns>Vec2Œ^</returns>
 Vec2 operator *(float s, const Vec2& v);

