#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "main.h"
#include "input.h"
#include "seiza.h"

#define MAX_MOUSE			(2)

// マウスの構造体定義
typedef struct {
	ZODIAC					tag;			// 今選択している星座を記録
	D3DXVECTOR3				pos;			// 位置
	D3DXCOLOR				col;			// 色
	bool					use;
}MOUSE;


HRESULT InitMouse();
void UninitMouse2();
void UpdateMouse();
void DrawMouse();

MOUSE* GetMouse(void);
SEIZA** GetSeiza(void);


#endif
