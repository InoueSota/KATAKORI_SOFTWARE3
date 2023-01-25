#include "Function.h"
#include "ControllerInput.h"



/*�@��r�@*/
void Comparison(int& num, int num1) {
	if (num < num1) {
		num = num1;
	}
};

/*�@���`��ԁ@*/
float Lerp(float easing, int end) {
	return (1.0f - easing) * 0 + easing * end;
}

/*�@���l�����߂�֐��@*/
float Clamp(float num, float min, float max) {
	if (num >= max) {
		return max;
	}
	if (num <= min) {
		return min;
	}
	return num;
};

/*�@�C�[�W���O�g�p���ɑ����ʂƃN�����v���y�ɍs�����߂̊֐��@*/
float EasingClamp(float increase, float t) {
	if (t >= 1.0f) {
		return 1.0f;
	}
	return t + increase;
}

/*�@�x��������ƃ��W�A���ɕϊ�����֐��@*/
float Degree(float degree) {
	return degree * (M_PI / 180);
}
/*�@���W�A��������Ɠx���ɕϊ�����֐��@*/
float Radian(float radian) {
	return radian * (180 / M_PI);
}

/*�@�����_���@*/
int RAND(int min, int max) {
	return min + (int)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}
float RAND(float min, float max) {
	return min + (float)(rand() * (max - min + 1.0) / (1.0 + RAND_MAX));
}
void SRAND() {
	srand((unsigned)time(NULL));
}

//�Q���̂Ȃ��p�����߂�
float BetweenTheta(Vec2 direction) {

	//����ƌ����x�N�g��
	Vec2 base = { 1,0 };
	direction.Normalized();

	//�Ȃ��p�����߂�
	float dp = direction.Dot(base);
	float cp = direction.Cross(base);
	float tmpTheta = atan2(cp, dp);
	if (tmpTheta < 0) {
		return Degree(360) + tmpTheta;
	}
	return tmpTheta;
}


//�R���g���[���[�̍��X�e�B�b�N�ňړ�����Ƃ����̊֐�
Vec2 LeftStickVelocity(float speed) {

	int tmpX, tmpY;
	Controller::GetLeftStick(0, tmpX, tmpY);
	Vec2 tmpVelocity = { (float)tmpX, (float)tmpY };
	tmpVelocity = tmpVelocity.Normalized();
	tmpVelocity *= speed;
	return tmpVelocity;
}