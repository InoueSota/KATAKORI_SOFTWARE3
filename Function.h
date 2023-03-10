#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include<cstdlib>
#include<ctime>
#include "Vec2.h"

/*　比較　*/
void Comparison(int& num, int num1);

/*　線形補間　*/
float Lerp(float easing, int end);

/*　数値を収める関数　*/
float Clamp(float num, float min, float max); 

/// <summary>
/// イージング使用時に増加量とクランプを楽に行うための関数
/// </summary>
/// <param name="increase">増加量</param>
/// <param name="t">変数</param>
float EasingClamp(float increase, float t);

/*　度数を入れるとラジアンに変換する関数　*/
float Degree(float degree);
/*　ラジアンを入れると度数に変換する関数　*/
float Radian(float radian);

/*　ランダム　*/
int RAND(int min, int max);
float RAND(float min, float max);
void SRAND();

//２線のなす角を求める
float BetweenTheta(Vec2 direction);

//コントローラーの左スティックで移動するとき等の関数
Vec2 LeftStickVelocity(float speed);