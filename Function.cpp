#include "Function.h"

/*　比較　*/
void Comparison(int& num, int num1) {
	if (num < num1) {
		num = num1;
	}
};

/*　線形補間　*/
float Lerp(float easing, int end) {
	return (1.0f - easing) * 0 + easing * end;
}

/*　数値を収める関数　*/
float Clamp(float num, float min, float max) {
	if (num >= max) {
		return max;
	}
	if (num <= min) {
		return min;
	}
	return num;
};

/*　イージング使用時に増加量とクランプを楽に行うための関数　*/
float EasingClamp(float increase, float t) {
	if (t >= 1.0f) {
		return 1.0f;
	}
	return t + increase;
}

/*　度数を入れるとラジアンに変換する関数　*/
float Degree(float degree) {
	return degree * (M_PI / 180);
}
/*　ラジアンを入れると度数に変換する関数　*/
float Radian(float radian) {
	return radian * (180 / M_PI);
}

/*　ランダム　*/
int RAND(int min, int max) {
	return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}
float RAND(float min, float max) {
	return min + (float)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}
void SRAND() {
	srand((unsigned)time(NULL));
}

//２線のなす角を求める
float BetweenTheta(Vec2 direction) {

	//基準線と向きベクトル
	Vec2 base = { 1,0 };
	direction.Normalized();

	//なす角を求める
	float dp = direction.Dot(base);
	float cp = direction.Cross(base);
	float tmpTheta = atan2(cp, dp);
	if (tmpTheta < 0) {
		return Degree(360) + tmpTheta;
	}
	return tmpTheta;
}