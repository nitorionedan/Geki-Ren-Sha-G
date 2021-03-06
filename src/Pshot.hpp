#pragma once

#include "Vector2D.hpp"
#include <array>
#include <memory>


class Counter;
class Bullet;


enum eShot_Color
{
	eShot_Blue,
	eShot_Orange,
	eShot_Red,
};


class Pshot
{
public:
	Pshot::Pshot();
	Pshot::~Pshot();
	void Update();
	void Draw();
	void SetParam(Vector2D& pos, int& shiftLevel);
	void ShiftReset();
	const int GetShiftLevel() const {
		return shiftLevel;
	}

private:
	void Input();													// 入力管理
	void ShotCount();												// セミオート連射のためのカウント管理
	void SetFirePosition(const double X_POS, const double Y_POS);	// 発射位置を設定
	void Fire();													// f_existを立てる
	void Move();													// 動きを計算
	void HitCheck();
	void Reset();													// 弾をリセット
	void SetAtk(const int& Aatk, const int& Batk, const int& Catk);	// 攻撃力を変える
	void CopyStaticMem();

	const int SCREEN_LIMIT_XR, SCREEN_LIMIT_XL;		// 画面の範囲
	const int SCREEN_LIMIT_YT, SCREEN_LIMIT_YB;

	const double A_SPEED, B_SPEED, C_SPEED;
	const double BASE_ANGLE;
	const double ADD_ANGLE1;
	const double ADD_ANGLE2;
	const double ADD_ANGLE3;
	const double ADD_ANGLE4;
	const double ADD_ANGLE5;
	const double ADD_ANGLE6;

	const double ANGLE_L_1;
	const double ANGLE_L_2;
	const double ANGLE_L_3;
	const double ANGLE_L_4;
	const double ANGLE_L_5;
	const double ANGLE_L_6;

	const double ANGLE_R_1;
	const double ANGLE_R_2;
	const double ANGLE_R_3;
	const double ANGLE_R_4;
	const double ANGLE_R_5;
	const double ANGLE_R_6;
	
	const double ADD_FIRE_POS;
	const double ADD_FIRE_POS_Y;

	std::unique_ptr<Counter> c_shot;				// 発射時間用カウンター
	std::array<Bullet*, 20>	Ashot;					// 青
	std::array<Bullet*, 30>	Bshot;					// 橙
	std::array<Bullet*, 30>	Cshot;					// 赤

	std::array<int, 3> hg;							// 赤、青、橙
	std::array<int, 3> hg2;							// 赤、青、橙(強化ver)
	int hg_fire;
	int hs_shot;									// 効果音ハンドル
	int hs_hit;
	int shiftLevel;									// シフトレベル
	double mPlayer_x, mPlayer_y;					// 自機の座標
	std::array<double, 3> x_pos, y_pos, angle;		// 座標、回転値
	std::array<double, 2> x_fire;
	double y_fire;
	bool isShot;									// 発射中か？
	bool isShotNext;								// 次も発射するか？

	static std::array<Vector2D, 20>	s_Apos;
	static std::array<Vector2D, 30>	s_Bpos;
	static std::array<Vector2D, 30>	s_Cpos;
};


class IPshot
{
public:
	~IPshot(){}
	static void set(std::shared_ptr<Pshot> pshot);
	static void reset() {
		mPshot.reset();
	}
	static void SetParam(Vector2D& pos, int& shiftLevel) {
		mPshot->SetParam(pos, shiftLevel);
	}
	static void ShiftReset() {
		mPshot->ShiftReset();
	}

private:
	IPshot() { mPshot = nullptr; }
	static std::shared_ptr<Pshot> mPshot;
};