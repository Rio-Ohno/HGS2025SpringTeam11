#ifndef KEY_H_
#define KEY_H_

#define MAX_KEY (10)
#define MAX_MOTION (5)
#define MAX_ACTIONDIRECTION (3)

#include "main.h"
#include "model.h"

//モーションの種類
typedef enum
{
	MOTIONTYPE_NEUTRAL = 0,
	MOTIONTYPE_MOVE,
	MOTIONTYPE_ACTION,
	MOTIONTYPE_JUMP,
	MOTIONTYPE_LANDING,

	MOTIONTYPE_LOCKON_R_MOVE,	// ロックオン時の移動　右
	MOTIONTYPE_LOCKON_L_MOVE,	// ロックオン時の移動　左
	MOTIONTYPE_LOCKON_F_MOVE,	// ロックオン時の移動　前後

	MOTIONTYPE_DOWN,			//ダウン
	MOTIONTYPE_ACTION_EXPLOSION,//攻撃（近距離）
	MOTIONTYPE_ACTION_HORMING,	//攻撃（追尾）

	MOTIONTYPE_KAIHI_MAE,		//前回避
	MOTIONTYPE_KAIHI_USIRO,		//後ろ回避
	MOTIONTYPE_KAIHI_MIGI,		//右回避
	MOTIONTYPE_KAIHI_HIDARI,	//左回避

	MOTIONTYPE_MAX
}MOTIONTYPE;

typedef struct
{
	float fPosX;
	float fPosY;
	float fPosZ;
	float fRotX;
	float fRotY;
	float fRotZ;
}KEY;

typedef struct
{
	int nFrame;
	KEY aKey[MAX_PARTS];
}KEYINFO;

typedef struct
{
	int nStartKey, nEndKey;
	int nStartFrame, nEndFrame;
	bool bActionStart,bFirst;
}ACTIONFRAME;

typedef struct
{
	bool bLoop;
	int nNumKey;
	KEYINFO aKeyInfo[MAX_KEY];
	ACTIONFRAME ActionFrameInfo[MAX_ACTIONDIRECTION];
}MOTIONINFO;

//プレイヤーの構造体
typedef struct
{
	//モデル
	MODELINFO aModel[MAX_PARTS];
	int nNumModel;
	//モーション
	MOTIONINFO aMotionInfo[MOTIONTYPE_MAX];	// モーション情報
	int nNumMotion;							// モーションの総数
	MOTIONTYPE motionType;					// モーションの種類
	bool bLoopMotion;						// ループするかどうか
	int nNumKey;							// キーの総数
	int nKey;								// 現在のキーNo
	int NextKey;							// 次のキー
	int nCntMotion;							// モーションのカウンター

	bool bFinish;							// 現在のモーションが終了しているかどうか
	bool bBlendMotion;						// ブレンドモーションがあるかどうか
	MOTIONTYPE motionTypeBlend;				// ブレンドモーションの種類
	bool bLoopBlend;						// ループするかどうか
	int nNumKeyBlend;						// ブレンドモーションのキーの総数
	int nKeyBlend;							// ブレンドモーションののキーNo
	int nNextKeyBlend;						// ブレンドモーションの次のキー
	int nFrameBlend;						// ブレンドフレーム
	int nCntBlend;							// ブレンドフレームカウント
	int nCntMotionBlend;					// ブレンドモーションのカウンター
}OBJECTINFO;

#endif // !KEY_H_