#pragma once

#include "BossTask.hpp"
#include "Counter.hpp"
#include "Effect.hpp"
#include "Vector2D.hpp"
#include <memory>


enum eBossA_state
{
	eBossA_Start,
	eBossA_Normal,
	eBossA_Weak,
	eBossA_Dead,
	eBossA_End,
};


class BossA : public BossTask
{
public:
	BossA();
	~BossA();

	virtual void Update() override;
	virtual void Draw() override;
	virtual const Vector2D& GetPos() const override {
		return pos;
	}
	virtual const bool IsDead() const override {
		return isDead;
	}

	// @brief		当たり判定
	// @param[in]	ColX, ColY	当たるモノの座標
	// @param[in]	DamagePoint	ダメージ値
	// @attention	円形と点の当たり判定方式
	bool HitCheck(const double& ColX, const double& ColY, const int& DamagePoint) override;
	
	void Start_Update();
	void Normal_Update();
	void Weak_Update();
	void Dead_Update();
	void End_Update();

	int GetTime();
	void Damage(const int& point);

	static const float	HIT_RANGE;

	bool		isExist;

private:
	// @brief		ボスの動きを変化させる
	// @param[in]	state_	動きの種類
	void ChangeState(eBossA_state state_);
	bool isFine();		// 元気ですかー！？
	void HitCheck();

	// @brief		画面外に出たか
	// @out			画面外ならtrue、画面内ならfalseを返す
	bool isOverLimit();	// 出てますかー！？
	
	static const float SC_LIMIT_XL, SC_LIMIT_XR;	// 画面内ボーダー
	static const float SC_LIMIT_YT, SC_LIMIT_YB;
	static const int MAX_HP;
	const float SPEED;

	std::unique_ptr<Counter>	c_starting;
	std::unique_ptr<Counter>	c_end;
	std::unique_ptr<Counter>	c_atk1;
	std::shared_ptr<Effect>		bomber;

	Vector2D pos;							// モデルのスクリーン座標
	VECTOR	mPos;							// モデルのワールド座標
	VECTOR	rota, rota_core;				// モデルの回転値
	VECTOR	startPos;						// スクリーンとワールド座標の線分
	int		hm;								// モデリングデータ用ハンドル
	int		hmWeaking;						// 死にかけ用モデル
	int hm_core;
	int		hg_shield;						// シールド
	int		hs_big;							// 巨弾
	int		hs_exp;							// 大爆発
	int		in_time, stop_time, out_time;	// 登場、停止、退場の時間
	int		big_time;
	int Screen;
	int c_dead;
	int c_start;
	float	targetX, targetY;				// 目的地
	float	angle;							// 体当たりの方向用
	bool	isMove;							// 攻撃開始しているか？
	bool	isEnd;							// 退場したか？
	bool	isDead;							// 死んだか？

	// static -----------------------------
	eBossA_state state;
	int		hp;			// 体力
	int		time;		// 生きている時間
	int		hs_break;						// シールド破損
	bool		isHit;
	bool		isWeak;
};
// EOF