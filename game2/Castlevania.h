#include "Animation.h"
#include "Animations.h"
#include "AnimationFrame.h"
#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include "KeyEventHandler.h"
#include "MovableObject.h"
#include "Sprite.h"
#include "Sprites.h"
#include "Textures.h"

#include "Simon.h"
#include "Rope.h"
#include "BigCandle.h"
#include "Brick.h"
#include "DestroyingFlame.h"
#include "DestroyingFlames.h"
#include "ItemRope.h"


#define ID_TEX_SIMON		0
#define ID_TEX_ROPE			1
#define ID_TEX_GROUND		2
#define ID_TEX_MISC			3
#define ID_TEX_MONSTERS		4
#define ID_TEX_TILESET		5


CGame *game;
CSimon * simon;
CRope * rope;
CBigCandle * bigCandle;
CBrick * brick;
CItemRope * itemRope;
CDestroyingFlame * destroyingFlame;