#pragma once

#include "Keyboard.hpp"
#include "Graphic.hpp"
#include "Range.hpp"
#include "Counter.hpp"
#include "Vector2D.hpp"
#include "PieceEffect.hpp"
#include "Bomb.hpp"
#include <memory>


// 入力したキーの方向
enum class eInputDir
{
	Neutral,
	Right,
	Left
};


enum class ePlayerState
{
	Start,
	Game,
	Dead,
};


class Player
{
public:
	Player();
	~Player();
	void Update();
	void Draw();
	void setup(std::shared_ptr<Bomb>);
	void Update_Start();
	void Update_Game();
	void Update_Dead();
	void Draw_Start();
	void Draw_Game();
	void Draw_Dead();
	void SetStart();
	void InputMng();	// 連射データ管理
	void GetPos(double* x, double* y) {
		*x = this->pos.x;
		*y = this->pos.y;
	}
	const Vector2D& GetPos() const {
		return pos;
	}
	void AddBomb() {
		bombNum++;
	}
	void ShiftReset() {
		powlv = 0;
	}
	void DownBombNum();
	
	// @brief	アームをセットする
	// @attention	アーム装備中なら得点
	void SetArm();

	void Shift(const bool isUp);
	const int GetShiftLevel() const {
		return powlv;
	}
	const int GetBombNum() const {
		return bombNum;
	}
	const int GetLife() const {
		return life;
	}
	const int GetRensha() const {
		return rensha;
	}
	const bool IsDead() const {
		return isDead;
	}
	const bool IsMuteki() const {
		return isMuteki;
	}
	const bool IsStart() const {
		return isStart;
	}

	// @brief	自機の円と他の点の当たり判定
	bool HitCheckCircle(const double& ColX, const double& ColY);

	// @brief	自機の円と他の円の当たり判定
	bool HitCheckCircle(const double & Range1, const double & Range2, const double & X1, const double & Y1, const double & X2, const double & Y2);

	static const int HIT_RANGE;
	
private:
	void Move();			// プレイヤーの動き
	void Rensha_Update();

	static int const MaxPowLevel;				// シフトの限界値
	static int const MAX_RENSHA;				// 連射ゲージの範囲
	static int const MIN_RENSHA;
	double const SpeedForce;					// 移動速度
	double const LimitScreenXL, LimitScreenXR;	// 画面制限
	double const LimitScreenYT, LimitScreenYB;
	double const Y_START;						// スタート地点

	int	rensha;		// 連射ゲージ
	bool isStart;		// スタート地点に到達したか？
	bool isHit;			// 被弾したか？
	bool isArm;			// アーム展開中か？
	// ---------------------------

	std::unique_ptr<Graphic> graphic;	// 文字画像
	std::unique_ptr<Counter> c_start;	// スタート準備用カウンター
	std::unique_ptr<Counter> c_dead;
	std::shared_ptr<Bomb> mBomber;

	eInputDir keydir;		// キー入力方向
	ePlayerState state;		// プレイヤーの状態
	eSpread_t dead_ef;		// やられ演出
	Vector2D pos;			// 自機の座標
	Vector2D vec;			// 自機の移動ベクトル
	int	hs_shiftUp,			// 音ハンドル
		hs_shiftDown,
		hs_dead;
	int	bombNum;			// 現在のボム数
	int	hg[3];				// 画像用ハンドル
	int	hg_arm[12];			// シールド画像ハンドル
	int	life;				// 残機
	int	elapsedTime;		// 経過時間
	int powlv;				// パワーレベル(シフトレベル)
	bool isDead;			// 死んでいるか？
	bool isMuteki;			// 無敵か？
};