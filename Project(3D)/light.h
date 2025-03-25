//==========================
//
// ライト処理 [light.h]
// Author: RioOhno
//
//==========================

#ifndef _LIGHT_H_
#define _LIGHT_H_

// インクルード
#include "main.h"

// マクロ定義
#define MAX_LIGHT (3)			// ライトの最大数

//==========================
// プロトタイプ宣言
//==========================
void InitLight(void);		// ライトの初期化
void UninitLight(void);		// ライトの終了
void UpdateLight(void);		// ライトの更新

#endif