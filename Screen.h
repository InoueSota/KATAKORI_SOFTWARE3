#pragma once
#include <Novice.h>
#include "Vec2.h"

class Player;
class Screen
{
public:

	static constexpr int kWindowWidth = 1280;
	static constexpr int kWindowHeight = 720;
	static constexpr int kMiniMapSize = 100;

	Screen() {
		Init();
	}

private:

	Vec2 mWorldCenter;		//���[���h�̒��S
	Vec2 mScroll;			//�X�N���[���l

	bool mIsZoomEasing;		//�Y�[���t���O
	float mZoom;			//�Y�[���l
	float mZoomEasingt;		//�Y�[�������X�ɍs���C�[�W���O
	float mZoomStartValue;	//�Y�[���J�n�l
	float mZoomEndValue;	//�Y�[���I���l

	bool mIsShakeActive;	//�V�F�C�N�t���O
	Vec2 mScreenShake;		//�V�F�C�N�l
	float mShakeMag;		//�V�F�C�N�{��
	float mShakeEasingt;	//�V�F�C�N�����X�Ɏ��߂�C�[�W���O

	bool mIsHitStop;		//�q�b�g�X�g�b�v�V�F�C�N
	int mHitStopFrame;		//�q�b�g�X�g�b�v�t���[��

	Vec2 mMiniMapCenter;	//�~�j�}�b�v�̒��S
	float mMiniMapZoom;		//�~�j�}�b�v�̃Y�[���l

public:


	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// ��e���̃J�����V�F�C�N
	/// </summary>
	/// <param name="condition">�V�F�C�N�������</param>
	void ShakeUpdate(bool condition);

	/// <summary>
	/// �J�����V�F�C�N�i X�� �� Y�� �ǂ���������l �j
	/// </summary>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <param name="condition">�V�F�C�N�������</param>
	void ShakeUpdate(int min, int max, bool condition);

	/// <summary>
	/// �J�����V�F�C�N�i X�� �� Y �� �ʂ̒l �j
	/// </summary>
	/// <param name="minX">�ŏ��l�i X�� �j</param>
	/// <param name="maxX">�ő�l�i X�� �j</param>
	/// <param name="minY">�ŏ��l�i Y�� �j</param>
	/// <param name="maxY">�ő�l�i Y�� �j</param>
	/// <param name="condition">�V�F�C�N�������</param>
	void ShakeUpdate(int minX, int maxX, int minY, int maxY, bool condition);

	/// <summary>
	///	�q�b�g�X�g�b�v
	/// </summary>
	void HitStopUpdate();

	/// <summary>
	/// �q�b�g�X�g�b�v�t���O��true�ɂ���
	/// </summary>
	inline void SetHitStop() { mIsHitStop = true; }

	/// <summary>
	/// �q�b�g�X�g�b�v�t���O���擾����
	/// </summary>
	inline bool GetHitStop() { return mIsHitStop; }

	/// <summary>
	/// �X�N���[��
	/// </summary>
	void ScrollUpdate(Player& player);

	/// <summary>
	/// �X�N���[���l���擾����
	/// </summary>
	inline Vec2 GetScroll() { return mScroll; }

	/// <summary>
	/// �Y�[��
	/// </summary>
	void ZoomUpdate(bool isFever, bool isOldFever);

	/// <summary>
	/// �Y�[���l���擾����
	/// </summary>
	inline float GetZoom() { return mZoom; }


	//--------------------------------------------------------------------------------------------//


	/// <summary>
	/// ����`�悷��
	/// </summary>
	/// <param name="startposition">�`�悷����̎n�_���W</param>
	/// <param name="endposition">�`�悷����̏I�_���W</param>
	/// <param name="color">�`�悷��l�p�`�̐F</param>
	void DrawLine(Vec2 startposition, Vec2 endposition, unsigned int color = WHITE);

	/// <summary>
	/// �l�p�`��`�悷��
	/// </summary>
	/// <param name="position">�`�悷��l�p�`�̍�����W</param>
	/// <param name="width">�`�悷��l�p�`�̉���</param>
	/// <param name="height">�`�悷��l�p�`�̏c��</param>
	/// <param name="angle">�`�悷��l�p�`�̉�]�p</param>
	/// <param name="color">�`�悷��l�p�`�̐F</param>
	/// <param name="fillMode">�h��Ԃ����[�h</param>
	/// <param name="isScroll">�X�N���[�����邩</param>
	void DrawBox(Vec2 position, float width, float height, float angle = 0.0f, unsigned int color = WHITE, FillMode fillMode = kFillModeSolid, bool isScroll = true);

	/// <summary>
	/// �ȉ~��`�悷��
	/// </summary>
	/// <param name="position">�ȉ~�̒��S���W</param>
	/// <param name="radiusX">�ȉ~�̔��a (X����) </param>
	/// <param name="radiusY">�ȉ~�̔��a (Y����) </param>
	/// <param name="angle">�ȉ~�̉�]�p(rad)</param>
	/// <param name="color">�ȉ~�̐F</param>
	void DrawEllipse(Vec2 position, float radiusX, float radiusY, float angle = 0.0f, unsigned int color = WHITE, FillMode fillMode = kFillModeSolid);

	/// <summary>
	/// �~��`�悷��
	/// </summary>
	/// <param name="position">�~�̒��S���W</param>
	/// <param name="radius">�~�̔��a</param>
	/// <param name="color">�~�̐F</param>
	void DrawCircle(Vec2 position, float radius, unsigned int color = WHITE, FillMode fillMode = kFillModeSolid);

	//----- DrawBox �Ƃ̈Ⴂ �� Position �����S���W�ł���Ƃ��� -----//
	/// <summary>
	/// �����`��`�悷��
	/// </summary>
	/// <param name="position">���S���W</param>
	/// <param name="size">�傫��(= ���a)</param>
	/// <param name="color">�`�悷��X�v���C�g�̐F</param>
	/// <param name="fillMode">�h��Ԃ����[�h</param>
	/// <param name="isScroll">�X�N���[�����邩</param>
	void DrawSquare(Vec2 position, float size, unsigned int color = WHITE, FillMode fillMode = kFillModeSolid, bool isScroll = true);

	/// <summary>
	/// �����`��`�悷��i�X������j
	/// </summary>
	/// <param name="position">���S���W</param>
	/// <param name="width">����</param>
	/// <param name="height">�c��</param>
	/// <param name="angle">�X��</param>
	/// <param name="color">�`�悷��X�v���C�g�̐F</param>
	/// <param name="fillMode">�h��Ԃ����[�h</param>
	/// <param name="isScroll">�X�N���[�����邩</param>
	void DrawRectAngle(Vec2 position, float width, float height, float angle, unsigned int color = WHITE, FillMode fillMode = kFillModeSolid, bool isScroll = true);
	
	/// <summary>
	/// �����`��`�悷��i�X���Ȃ��j
	/// </summary>
	/// <param name="position">���S���W</param>
	/// <param name="width">����</param>
	/// <param name="height">�c��</param>
	/// <param name="color">�`�悷��X�v���C�g�̐F</param>
	/// <param name="fillMode">�h��Ԃ����[�h</param>
	/// <param name="isScroll">�X�N���[�����邩</param>
	void DrawRectAngle(Vec2 position, float width, float height, unsigned int color = WHITE, FillMode fillMode = kFillModeSolid, bool isScroll = true);

	/// <summary>
	/// �Î~��i�����`�j��`�悷��
	/// </summary>
	/// <param name="position">���S���W</param>
	/// <param name="size">�傫��(= ���a)</param>
	/// <param name="angle">�X��</param>
	/// <param name="srcW">�摜��̕`�悵�����͈͉���</param>
	/// <param name="srcH">�摜��̕`�悵�����͈͏c��</param>
	/// <param name="textureHandle">�e�N�X�`���̃n���h��</param>
	/// <param name="color">�`�悷��X�v���C�g�̐F</param>
	void DrawPicture(Vec2 position, float size, float angle, float srcW, float srcH, int textureHandle, unsigned int color = WHITE);

	/// <summary>
	/// �A�ԉ摜�i�����`�j��`�悷��
	/// </summary>
	/// <param name="position">���S���W</param>
	/// <param name="size">�傫��(= ���a)</param>
	/// <param name="srcX">�摜��̕`�悵�����͈͍�����W X�i�ϐ��j</param>
	/// <param name="srcW">�摜��̕`�悵�����͈͉���</param>
	/// <param name="srcH">�摜��̕`�悵�����͈͏c��</param>
	/// <param name="sheets">�摜����</param>
	/// <param name="frame">���t���[���Ő؂�ւ��邩</param>
	/// <param name="frameVariable">���Z���Ă���t���[���̕ϐ�</param>
	/// <param name="textureHandle">�e�N�X�`���̃n���h��</param>
	/// <param name="color">�`�悷��X�v���C�g�̐F</param>
	void DrawAnime(Vec2 position, float size, int srcX, int srcW, int srcH, int sheets, int frame, int& frameVariable, int textureHandle, unsigned int color = WHITE);

	/// <summary>
	/// �t�h�i�X�N���[�����Ȃ��j�i�����`�j
	/// </summary>
	/// <param name="position">���S���W</param>
	/// <param name="width">����</param>
	/// <param name="height">�c��</param>
	/// <param name="angle">�X��</param>
	/// <param name="srcX">�摜��̕`�悵�����͈͍�����W X�i�ϐ��j</param>
	/// <param name="srcW">�摜��̕`�悵�����͈͉���</param>
	/// <param name="srcH">�摜��̕`�悵�����͈͏c��</param>
	/// <param name="textureHandle">�e�N�X�`���̃n���h��</param>
	/// <param name="color">�`�悷��X�v���C�g�̐F</param>
	/// <param name="scale">�傫����ω�������</param>
	void DrawUI(Vec2 position, float width, float height, int srcX, int srcW, int srcH, int textureHandle, unsigned int color = WHITE, Vec2 scale = { 1.0f, 1.0f });

	/// <summary>
	/// �t�h�i�X�N���[�����Ȃ��j�i�����`�j
	/// </summary>
	/// <param name="position">���S���W</param>
	/// <param name="size">�傫��(= ���a)</param>
	/// <param name="srcX">�摜��̕`�悵�����͈͍�����W X�i�ϐ��j</param>
	/// <param name="srcW">�摜��̕`�悵�����͈͉���</param>
	/// <param name="srcH">�摜��̕`�悵�����͈͏c��</param>
	/// <param name="textureHandle">�e�N�X�`���̃n���h��</param>
	/// <param name="color">�`�悷��X�v���C�g�̐F</param>
	/// <param name="scale">�傫����ω�������</param>
	void DrawUI(Vec2 position, float size, int srcX, int srcW, int srcH, int textureHandle, unsigned int color = WHITE, Vec2 scale = { 1.0f, 1.0f });

	/// <summary>
	/// �~�j�}�b�v�̈ʒu���~�ŕ`�悷��
	/// </summary>
	/// <param name="position">���S���W</param>
	/// <param name="color">�`�悷��X�v���C�g�̐F</param>
	/// <param name="fillMode">�h��Ԃ����[�h</param>
	/// <param name="radius">���a</param>
	void DrawMiniMap(Vec2 position, unsigned int color = WHITE, FillMode fillMode = kFillModeSolid, float radius = 2.0f);


	//--------------------------------------------------------------------------------------------//


	/// <summary>
	/// ���[���h���W�ɕϊ�����
	/// </summary>
	Vec2 WorldTransform(Vec2 position);

	/// <summary>
	/// �~�j�}�b�v���W�ɕϊ�����
	/// </summary>
	Vec2 MiniMapTransform(Vec2 position);

	/// <summary>
	/// �X�N���[�����W�ɕϊ�����
	/// </summary>
	Vec2 ScreenTransform(Vec2 position);
};


