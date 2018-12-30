//#include <algorithm>

#include "Simon.h"
#include "Camera.h"
#include "Animations.h"
#include "BigCandle.h"
#include "ItemRope.h"
#include "BigHeart.h"
#include "InvisibleWall.h"
#include "Heart.h"
#include "ItemDagger.h"
#include "Flame.h"
#include "Dagger.h"
#include "Timer.h"
#include "Zombie.h"
#include "Candle.h"
#include "StairsUp.h"
#include "StairsDown.h"
#include "Panther.h"
#include "PinkBat.h"
#include "Board.h"
#include "Portals.h"
#include "Fish.h"
#include "FireBall.h"
#include "InvisibleWater.h"
#include "Bubbles.h"
#include "ItemMeat.h"
#include "AutoMoveZone.h"
#include "Gate.h"
#include "SecretBrick.h"
#include "MoneyBag.h"
#include "Axe.h"
#include "ItemAxe.h"
#include "ItemHolyWater.h"
#include "Cross.h"
#include "BossBat.h"
#include "CameraEvent.h"
#include "ItemSuperPotion.h"
#include "Watch.h"
#include "HolyWater.h"
#include "DoubleShot.h"
#include "MagicBall.h"

#include "debug.h"

CSimon * CSimon::__instance = NULL;

CSimon::CSimon()
{
	this->controllable = true;
	this->jumping = false;
	this->crouching = false;
	this->autoMove = false;
	this->dying = false;
	this->secondaryWeapon = Weapon::NONE;

	this->auto_crouch_start = 0;
	this->attack_start = 0;
	this->flicker_start = 0;
	this->auto_start = 0;
	this->onStairs = 0;

	this->health = PLAYER_HEALTH_DEFAULT;
	this->numberOfHearts = HEART_DEFAULT;

	// ready to be used
	rope = CRope::GetInstance();
	weaponsInstance = CWeapons::GetInstance();
}

void CSimon::Render()
{	
	// Flicker by getting damaged.
	if (untouchable_start != 0)
	{
		if (this->argb.alpha != SIMON_UNTOUCHABLE_ALPHA_VALUE)
			this->argb.alpha = SIMON_UNTOUCHABLE_ALPHA_VALUE;
		else
			this->argb = ARGB();
	}


	// Flicker by touching item-rope
	else if (flicker_start != 0)
	{
		// Animation frame will be rendered with
		// these 3 colors defined by change the argb value
		if (this->argb.blue != 0)
			this->argb.blue = 0;

		else if (argb.green != 0)
			this->argb.green = 0;

		else
			this->argb = ARGB();	
	}


	// Render the background with flashing effect
	if (flash_start != TIMER_IDLE )
		ProceedBackgroundColor();


	CGameObject::Render();
}

void CSimon::ProceedBackgroundColor()
{
	if (whiteBackground == true)
	{
		CGame * game = CGame::GetInstance();
		LPDIRECT3DDEVICE9 d3ddv = game->GetDirect3DDevice();
		LPDIRECT3DSURFACE9 bb = game->GetBackBuffer();
		LPD3DXSPRITE spriteHandler = game->GetSpriteHandler();

		// Fill the background with white screen
		d3ddv->ColorFill(bb, NULL, COLOR_WHITE(ARGB_ALPHA_MAX_VALUE));


		// To prevent the background being white next frame.
		whiteBackground = false;
	}
	else
		whiteBackground = true;
}

void CSimon::PickAnimation()
{
	if (attack_start != 0)
	{
		if (crouching)
			currentAniID = (nx > 0) ?
				(int)SimonAniID::CROUCH_ATTACK_RIGHT :
				(int)SimonAniID::CROUCH_ATTACK_LEFT;

		else if (onStairs == 1)
			currentAniID = (nx > 0) ?
				(int)SimonAniID::UP_STAIR_ATTACK_RIGHT :
				(int)SimonAniID::UP_STAIR_ATTACK_LEFT;

		else if (onStairs == -1)
			currentAniID = (nx > 0) ?
				(int)SimonAniID::DOWN_STAIR_ATTACK_RIGHT :
				(int)SimonAniID::DOWN_STAIR_ATTACK_LEFT;

		else
			currentAniID = (nx > 0) ?
				(int)SimonAniID::ATTACK_RIGHT :
				(int)SimonAniID::ATTACK_LEFT;
	}

	else if (onStairs == 1)
	{
		if (vx != 0)
			currentAniID = (nx > 0) ?
				(int)SimonAniID::WALK_UPSTAIRS_RIGHT :
				(int)SimonAniID::WALK_UPSTAIRS_LEFT;

		else
			currentAniID = (nx > 0) ?
				(int)SimonAniID::IDLE_UPSTAIRS_RIGHT :
				(int)SimonAniID::IDLE_UPSTAIRS_LEFT;

	}

	else if (onStairs == -1)
	{
		if (vx != 0)
			currentAniID = (nx > 0) ?
				(int)SimonAniID::WALK_DOWNSTAIRS_RIGHT :
				(int)SimonAniID::WALK_DOWNSTAIRS_LEFT;
		else
			currentAniID = (nx > 0) ?
				(int)SimonAniID::IDLE_DOWNSTAIRS_RIGHT :
				(int)SimonAniID::IDLE_DOWNSTAIRS_LEFT;
	}

	else if (untouchable_start == -1)
	{
		currentAniID = (nx > 0) ?
			(int)SimonAniID::DAMAGING_RIGHT :
			(int)SimonAniID::DAMAGING_LEFT;
	}		

	// these two action use the same animation
	else if (crouching || jumping)
	{
		currentAniID = (nx > 0) ?
			(int)SimonAniID::CROUCH_RIGHT :
			(int)SimonAniID::CROUCH_LEFT;

		if (jumping && !crouching)
			currentAniID = (nx > 0) ?
			(int)SimonAniID::IDLE_RIGHT :
			(int)SimonAniID::IDLE_LEFT;
	}

	//else if (health <= 0)
	else if (dying)
	{
		currentAniID = (nx > 0) ?
			(int)SimonAniID::DIE_RIGHT :
			(int)SimonAniID::DIE_LEFT;
	}

	else if (vx > 0)
	{
		currentAniID = (int)SimonAniID::WALK_RIGHT;
	}

	else if (vx < 0)
	{
		currentAniID = (int)SimonAniID::WALK_LEFT;
	}

	else if (vx == 0)
	{
		currentAniID = (nx > 0) ?
			(int)SimonAniID::IDLE_RIGHT :
			(int)SimonAniID::IDLE_LEFT;
	}
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy
	CActiveObject::Update(dt);


	// Save the coObjects
	this->coObjects = coObjects;
	

	// While jumping and falling
	if (jumping)
		ProceedJumping();
	else
		if (onStairs == 0)
			ProceedGravity();


	// Keep vy not to be too high
	if (vy > SIMON_MAX_SPEED_Y)
		vy = SIMON_MAX_SPEED_Y;


	// Auto Crouch
	if (auto_crouch_start != TIMER_IDLE)
		ProceedAutoCrouching();


	// Attacking
	if (attack_start != TIMER_IDLE)
		ProceedAttacking();


	// Being Untouchable
	if (untouchable_start != TIMER_IDLE &&
		untouchable_start != TIMER_ETERNAL)
		ProceedBeingUntouchable();


	// Flickering
	if (flicker_start != TIMER_IDLE)
		ProceedFlickering();


	// Screen flashing
	if (flash_start != TIMER_IDLE)
		ProceedFlashingScreen();


	// Being On Stairs
	if (onStairs != 0)
		ProceedOnStairs();


	// Simon's death
	if (health <= 0 && !jumping)	// Simon has landed on the ground
	{
		vx = vy = 0;
		controllable = false;
		dying = true;
	}


	// Choose matched animation
	PickAnimation();


	// Overlapping
	ovObjects.clear();
	for (UINT i = 0; i < coObjects->size(); ++i)
		if (this->IsOverlapping(coObjects->at(i)))
			ovObjects.push_back(coObjects->at(i));

	if (ovObjects.size() != 0)
		ProceedOverlapping();


	// Collisions
	vector<LPCOLLISIONEVENT> coEvents;
	coEvents.clear();

	if (dying == false)
		CalcPotentialCollisions(coObjects, coEvents);
	else
	{
		// Prevent the Simon reaction of being get damage ( by overlapping )
		untouchable_start = GetTickCount();
	}


	// Auto Move: this will re-calculate the dx, dy
	if (autoMove)
		ProceedAutoMove();

	
	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
		ProceedCollisions(coEvents);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	

	CalibrateCameraPosition();
}

void CSimon::ProceedFlickering()
{
	if (GetTickCount() - flicker_start >= SIMON_FLICKERING_TIME)
	{
		this->argb = ARGB();
		flicker_start = TIMER_IDLE;
	}
}

void CSimon::ProceedBeingUntouchable()
{
	if (GetTickCount() - untouchable_start > untouchable_time)
	{
		// Stop flickering effect
		this->argb = ARGB();
		flicker_start = TIMER_IDLE;

		untouchable_start = TIMER_IDLE;
		untouchable_time = 0;
	}
}

void CSimon::ProceedAttacking()
{
	if (GetTickCount() - attack_start > SIMON_ATTACK_TIME)
	{
		// If attacking by rope
		if (this->rope->GetState() == STATE_VISIBLE) 
			this->rope->SetState(STATE_INVISIBLE);
		else
			weaponsInstance->UseWeapon(secondaryWeapon, this);

		// Stop timer
		attack_start = TIMER_IDLE;
	}
	else 
		// While attacking, Simon can't move horizontally, except finish jumping
		if (!jumping)
			vx = 0;
}

void CSimon::ProceedAutoCrouching()
{
	if (GetTickCount() - auto_crouch_start >= SIMON_AUTO_CROUCH_TIME)
	{
		this->StandUp();

		// Stop counting
		auto_crouch_start = TIMER_IDLE;
	}
}

void CSimon::ProceedJumping()
{
	// When reach this speed, Simon will stand up and fall faster.
	if (vy > SIMON_MAX_SPEED_WITH_JUMP_GRAVITY)
	{
		vy += SIMON_FALL_GRAVITY * this->dt;

		if (crouching == true)
			this->StandUp();
	}
	else
		vy += SIMON_JUMP_GRAVITY * this->dt;


	// NOTE: JUMPING FLAG WILL BE ASSIGNED TO FALSE 
	// WHEN REACHES THE GROUND ( void ProceedCollision(...) );
}

void CSimon::ProceedGravity()
{
	// If falling
	if (vy != 0)
		controllable = false;

	vy += SIMON_FALL_GRAVITY * this->dt;
}

/*
	Help get out of the stairs
*/
void CSimon::ProceedOnStairs()
{
	// Try getting out the stairs
	LPGAMEOBJECT stairs = NULL;
	
	if (onStairs == 1)
	{
		for (UINT i = 0; i < ovObjects.size(); ++i)
			if (dynamic_cast<CStairsDown *>(ovObjects[i]))
			{
				stairs = ovObjects[i];
				break;
			}

		if (stairs == NULL) return;

		float xS, yS;
		stairs->GetPosition(xS, yS);

		// Has reached the stairs exit
		if (y < yS)
		{
			y = yS - DEFLECTION_AVOID_OVERLAPPING;
			onStairs = 0;

			autoMove = false;
			vx = vy = 0;
		}
	}
	else if (onStairs == -1)
	{
		for (UINT i = 0; i < ovObjects.size(); ++i)
			if (dynamic_cast<CStairsUp *>(ovObjects[i]))
			{
				stairs = ovObjects[i];
				break;
			}

		if (stairs == NULL) return;

		float xS, yS;
		stairs->GetPosition(xS, yS);

		// Has reached the stairs exit
		if (y > yS)
		{
			y = yS - DEFLECTION_AVOID_OVERLAPPING;
			onStairs = 0;

			autoMove = false;
			vx = vy = 0;
		}
	}
}

void CSimon::ProceedOverlapping()
{
	LPGAMEOBJECT obj;
	for (UINT i = 0; i < ovObjects.size(); i++)
	{
		obj = ovObjects[i];
		
		// To use the portals effectly by avoid Simon overlapping on a Portal object
		if (dynamic_cast<CAutoMoveZone *>(obj))
		{
			if (this->attack_start != TIMER_IDLE ||
				this->untouchable_start != TIMER_IDLE ||
				this->crouching == true ||
				this->jumping == true)
				continue;
		
			if (onStairs == 1)
				this->Upstairs();
			else if (onStairs == -1)
				this->Downstairs();
			else if (onStairs == 0)
				this->StartAutoMove(this->nx * SIMON_AUTO_MOVE_SPEED_X, 0, SIMON_AUTO_MOVE_TIME);
		}

		else if (dynamic_cast<CZombie *>(obj) ||
			dynamic_cast<CPanther *>(obj) ||
			dynamic_cast<CPinkBat *>(obj) ||
			dynamic_cast<CFish *>(obj) ||
			dynamic_cast<CFireBall *>(obj) ||
			dynamic_cast<CBossBat *>(obj))
		{
			if (untouchable_start == TIMER_IDLE)
			{
				this->OnGetDamaged(obj);
				return;
			}

			if (dynamic_cast<CPinkBat *>(obj))
				obj->Destroy();
		}

		else if (dynamic_cast<CItemRope *>(obj))
		{
			this->rope->LevelUp();
			this->Flicker();
			CTimer::GetInstance()->Freeze(SIMON_TOUCH_ITEM_FREEZING_TIME);
			obj->SetState(STATE_INVISIBLE);
		}

		else if (dynamic_cast<CBigHeart *>(obj))
		{
			obj->SetState(STATE_INVISIBLE);

			numberOfHearts += obj->GetPoint();
			CBoard::GetInstance()->SetHeart(numberOfHearts);
		}

		else if (dynamic_cast<CHeart *>(obj))
		{
			obj->SetState(STATE_INVISIBLE);

			numberOfHearts += obj->GetPoint();
			CBoard::GetInstance()->SetHeart(numberOfHearts);
		}

		else if (dynamic_cast<CItemDagger *>(obj))
		{
			obj->SetState(STATE_INVISIBLE);
			secondaryWeapon = Weapon::DAGGER;
		}

		else if (dynamic_cast<CItemAxe *>(obj))
		{
			obj->SetState(STATE_INVISIBLE);
			secondaryWeapon = Weapon::AXE;
		}

		else if (dynamic_cast<CItemHolyWater *>(obj))
		{
			obj->SetState(STATE_INVISIBLE);
			secondaryWeapon = Weapon::HOLYWATER;
		}

		else if (dynamic_cast<CItemMeat *>(obj))
		{
			obj->SetState(STATE_INVISIBLE);

			this->health += obj->GetPoint();
			CBoard::GetInstance()->AddPlayerLife(obj->GetPoint());
		}

		else if (dynamic_cast<CMagicBall *>(obj))
		{
			obj->SetState(STATE_INVISIBLE);

			this->health += obj->GetPoint();
			CBoard::GetInstance()->AddPlayerLife(obj->GetPoint());
		}

		else if (dynamic_cast<CItemSuperPotion *>(obj))
		{
			DebugOut(L"\n[INFO] Touch Item Super Potion");
			obj->SetState(STATE_INVISIBLE);

			this->BeUntouchableByItem();
		}
		else if (dynamic_cast<CWatch *>(obj))
		{
			obj->SetState(STATE_INVISIBLE);
			secondaryWeapon = Weapon::WATCH;
		}
		else if (dynamic_cast<CDoubleShot *>(obj))
		{
			obj->SetState(STATE_INVISIBLE);
			weaponsInstance->UpgradeToDoubleShot();
		}
		else if (dynamic_cast<CMoneyBag *>(obj))
		{
			obj->SetState(STATE_INVISIBLE);

			CBoard::GetInstance()->AddScore(obj->GetPoint());
		}

		else if (dynamic_cast<CCross *>(obj))
		{
			obj->SetState(STATE_INVISIBLE);

			// Start the flashing screen effect
			flash_start = GetTickCount();

			// Kill all the monsters
			KillAllMonsters();
		}

		else if (dynamic_cast<CCameraEvent *>(obj))
		{
			DebugOut(L"\n[INFO] Touch Camera Event");
			CCameraEvent * cameraEvent = dynamic_cast<CCameraEvent *>(obj);

			float xEvent, yEvent;
			obj->GetPosition(xEvent, yEvent);

			if (cameraEvent->GetCameraEvent() == CameraEvent::SLOWLY_MOVE)
				cameraInstance->StartSlowlyMoveViewport(xEvent, yEvent);

			else if (cameraEvent->GetCameraEvent() == CameraEvent::LOCK)
				cameraInstance->SetLockEffect(true);
		}

		else if (dynamic_cast<CPortal *>(obj))
		{
			CPortals::GetInstance()->Teleport((CPortal *)obj, this->x, this->y);
		}

		else if (dynamic_cast<CInvisibleWater *>(obj))
		{
			// Falling into water effect
			float pX = this->x + SIMON_IDLE_BBOX_WIDTH / 2;
			float pY = this->y + SIMON_IDLE_BBOX_HEIGHT;
			CBubbles::GetInstance()->ShowSomeBubblesForFish(this->x, this->y);

			// Simon's death
			this->SetState(STATE_INVISIBLE);
			this->dying = true;
		}
	}
}

void CSimon::ProceedFlashingScreen()
{
	if (GetTickCount() - flash_start > SCREEN_FLASH_TIME)
		flash_start = TIMER_IDLE;
	else KillAllMonsters();
}

void CSimon::ProceedAutoMove()
{
	// Auto move and stop when reach destination
	if (auto_start == TIMER_IDLE)
	{
		if (autoInfo.vx != 0 &&
			autoInfo.xDes != x)
		{
			ReDirect(autoInfo.xDes);
			vx = nx * autoInfo.vx;
			dx = vx * dt;

			// Calibrate the dx if Simon has passed and is moving far away from the destination
			if (dx > 0)
			{
				if (x + dx > autoInfo.xDes)
					dx = autoInfo.xDes - x;
			}
			else
			{
				if (x + dx < autoInfo.xDes)
					dx = autoInfo.xDes - x;
			}
		}
		else
		{
			autoMove = false;
			controllable = true;
			dx = dy = vx = vy = 0;
		}
	}

	// Auto move and stop when run out of automoving time
	else
	{
		if (GetTickCount() - auto_start > autoInfo.autoTimeLast)
		{
			auto_start = TIMER_IDLE;
			autoMove = false;
			controllable = true;
			dx = dy = vx = vy = 0;
		}
		else
		{
			// Calculate the dx, dy while moving automatically
			vx = autoInfo.vx;
			vy = autoInfo.vy;
			dx = vx * dt;
			dy = vy * dt;
		}
	}
}

void CSimon::ProceedCollisions(vector<LPCOLLISIONEVENT> &coEvents)
{
	vector<LPCOLLISIONEVENT> coEventsResult;
	float min_tx, min_ty, nx, ny;
	FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

	// Set the object's position right to the point occured collision
	x += min_tx * dx;
	y += min_ty * dy;

	for (UINT i = 0; i < coEventsResult.size(); i++)
	{
		LPCOLLISIONEVENT e = coEventsResult[i];

		if (dynamic_cast<CBigCandle *>(e->obj) ||
			dynamic_cast<CFlame *>(e->obj) ||
			dynamic_cast<CDagger *>(e->obj) ||
			dynamic_cast<CCandle *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch something but do nothing !!");

			// Ignore other collisions by completing the rest of dx / dy without blocked.
			if (e->nx != 0)	x += (1 - min_tx) * dx;
			if (e->ny != 0)	y += (1 - min_ty) * dy;
		}

		else if (dynamic_cast<CItemRope *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Item Rope");

			this->rope->LevelUp();
			this->Flicker();
			CTimer::GetInstance()->Freeze(SIMON_TOUCH_ITEM_FREEZING_TIME);
			e->obj->SetState(STATE_INVISIBLE);
		}

		else if (dynamic_cast<CBigHeart *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Big Heart");
			e->obj->SetState(STATE_INVISIBLE);

			numberOfHearts += e->obj->GetPoint();
			CBoard::GetInstance()->SetHeart(numberOfHearts);
		}

		else if (dynamic_cast<CHeart *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Heart");
			e->obj->SetState(STATE_INVISIBLE);

			numberOfHearts += e->obj->GetPoint();
			CBoard::GetInstance()->SetHeart(numberOfHearts);
		}

		else if (dynamic_cast<CItemDagger *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Item Dagger");

			e->obj->SetState(STATE_INVISIBLE);
			secondaryWeapon = Weapon::DAGGER;
		}

		else if (dynamic_cast<CItemAxe *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Item Axe");

			e->obj->SetState(STATE_INVISIBLE);
			secondaryWeapon = Weapon::AXE;
		}

		else if (dynamic_cast<CItemHolyWater *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Item Holy Water");

			e->obj->SetState(STATE_INVISIBLE);
			secondaryWeapon = Weapon::HOLYWATER;
		}

		else if (dynamic_cast<CItemMeat *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Meat");
			e->obj->SetState(STATE_INVISIBLE);

			this->health += e->obj->GetPoint();
			CBoard::GetInstance()->AddPlayerLife(e->obj->GetPoint());
		}

		else if (dynamic_cast<CMagicBall *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Magic Ball");
			e->obj->SetState(STATE_INVISIBLE);

			this->health += e->obj->GetPoint();
			CBoard::GetInstance()->AddPlayerLife(e->obj->GetPoint());
		}

		else if (dynamic_cast<CItemSuperPotion *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Item Super Potion");
			e->obj->SetState(STATE_INVISIBLE);

			this->BeUntouchableByItem();
		}
		else if (dynamic_cast<CWatch *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Item Watch");

			e->obj->SetState(STATE_INVISIBLE);
			secondaryWeapon = Weapon::WATCH;
		}
		else if (dynamic_cast<CDoubleShot *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch DoubleShot Upgrade ");
			e->obj->SetState(STATE_INVISIBLE);

			weaponsInstance->UpgradeToDoubleShot();
		}
		else if (dynamic_cast<CMoneyBag *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch MoneyBag");
			e->obj->SetState(STATE_INVISIBLE);

			CBoard::GetInstance()->AddScore(e->obj->GetPoint());
		}

		else if (dynamic_cast<CCross *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Cross");
			e->obj->SetState(STATE_INVISIBLE);

			// Start the flashing screen effect
			flash_start = GetTickCount();

			// Kill all the monsters
			KillAllMonsters();
		}

		else if (dynamic_cast<CCameraEvent *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Camera Event");
			CCameraEvent * cameraEvent = dynamic_cast<CCameraEvent *>(e->obj);

			float xEvent, yEvent;
			e->obj->GetPosition(xEvent, yEvent);

			if (cameraEvent->GetCameraEvent() == CameraEvent::SLOWLY_MOVE)
				cameraInstance->StartSlowlyMoveViewport(xEvent, yEvent);

			else if (cameraEvent->GetCameraEvent() == CameraEvent::LOCK)
				cameraInstance->SetLockEffect(true);
		}

		else if (dynamic_cast<CGate *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch Gate");

			// Block
			if (e->nx < 0)
			{
				dynamic_cast<CGate *>(e->obj)->SetOpenning(true);
			}
			else if (e->nx > 0)
			{
				x += nx * DEFLECTION_AVOID_OVERLAPPING;
				vx = 0;
			}
		}

		else if (dynamic_cast<CZombie *>(e->obj) ||
				dynamic_cast<CPanther *>(e->obj) ||
				dynamic_cast<CPinkBat *>(e->obj) ||
				dynamic_cast<CFish *>(e->obj) ||
				dynamic_cast<CFireBall *>(e->obj) ||
				dynamic_cast<CBossBat *>(e->obj))
		{
			DebugOut(L"\n[INFO] Touch a monster !!");

			if (untouchable_start == TIMER_IDLE)
			{
				this->OnGetDamaged(e);
				return;
			}
			
			else
			{
				// Ignore the collision by completing the rest of dx/dy
				if (e->nx != 0) x += (1 - min_tx) * dx;
				if (e->ny != 0) y += (1 - min_ty) * dy;
			}


			if (dynamic_cast<CPinkBat *>(e->obj))
				e->obj->Destroy();
		}

		else if (dynamic_cast<CPortal *>(e->obj))
		{
			CPortals::GetInstance()->Teleport((CPortal *)e->obj, this->x, this->y);
		}

		else if (dynamic_cast<CInvisibleWater *>(e->obj))
		{
			// Death's effect
			float pX = this->x + SIMON_IDLE_BBOX_WIDTH / 2;
			float pY = this->y + SIMON_IDLE_BBOX_HEIGHT;
			CBubbles::GetInstance()->ShowSomeBubblesForFish(this->x, this->y);

			// Simon's death
			this->SetState(STATE_INVISIBLE);
			this->dying = true;
		}

		// block with ground objects
		else if (dynamic_cast<CSecretBrick *>(e->obj))
		{
			if (e->nx != 0)
			{
				x += nx * DEFLECTION_AVOID_OVERLAPPING;
				vx = 0;
			}

			if (e->ny < 0)
			{
				y += ny * DEFLECTION_AVOID_OVERLAPPING;
				vy = 0;
			}
		}
		else if (dynamic_cast<CInvisibleWall *>(e->obj))
		{
			// Simon ignores the ground while on stairs
			if (onStairs != 0)
				continue;

			if (e->nx != 0)
			{
				// Prevent overlapping next frame
				x += nx * DEFLECTION_AVOID_OVERLAPPING;
			}

			if (e->ny < 0)
			{
				// Regain the control
				// In case Simon is not "auto move" and alive
				if (!autoMove && health > 0)
					controllable = true;


				if (jumping)	
				{
					this->StandUp();
					jumping = false;
				}


				// When falls from too high place
				if (vy == SIMON_MAX_SPEED_Y)
					AutoCrouch();
					 

				// When getting damage and landing on the ground
				if (untouchable_start == TIMER_ETERNAL)
				{
					AutoCrouch();
					BeUntouchable();

					if (health > 0) Flicker();
				}


				// Prevent overlapping next frame
				y += ny * DEFLECTION_AVOID_OVERLAPPING;
				vy = 0;
			}
		}
	}
}

void CSimon::Flicker()
{
	if (flicker_start == TIMER_IDLE)
		flicker_start = GetTickCount();
}

/*
	The function helps Simon stand up without overlapping 
	objects by re-locating Simon position
*/
void CSimon::StandUp()
{
	if (jumping && crouching)
	{
		crouching = false;
		y += SIMON_JUMPING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT;
	}
	else if (crouching && attack_start == TIMER_IDLE)
	{
		crouching = false;
		y += SIMON_CROUCHING_BBOX_HEIGHT - SIMON_IDLE_BBOX_HEIGHT;
	}		
}

void CSimon::AutoCrouch()
{
	SetAction(Action::CROUCH);

	// If setting action succeeded
	if (crouching)
		auto_crouch_start = GetTickCount();
}

/*
	Use this to start the untouchable_start timer
*/
void CSimon::BeUntouchable()
{
	if (untouchable_start == TIMER_IDLE ||
		untouchable_start == TIMER_ETERNAL)
	{
		untouchable_start = GetTickCount();
		untouchable_time = SIMON_UNTOUCHABLE_TIME;
	}
}

void CSimon::BeUntouchableByItem()
{
	this->BeUntouchable();
	this->Flicker();

	// With different untouchable time
	untouchable_time = SIMON_UNTOUCHABLE_BY_ITEM_TIME;
}

void CSimon::OnGetDamaged(LPCOLLISIONEVENT e)
{
	if (onStairs == 0)
	{
		this->nx = (e->nx != 0) ?
			-(e->nx) :
			-(e->obj->GetDirection());

		vx = vy = dx = dy = 0;

		this->vx = (-this->nx) * SIMON_DAMAGED_DEFLECT_X;
		this->vy = SIMON_DAMAGED_DEFLECT_Y;

		// Set jumping = true to re-use the SIMON_JUMP_GRAVITY
		jumping = true;
		controllable = false;

		// Simon is being untouchable but 
		// not going to stop being untouchable ( because of being on-air )
		untouchable_start = TIMER_ETERNAL;
	}
	else
	{
		BeUntouchable();
		Flicker();
	}	

	// Update Simon's health
	CBoard * board = CBoard::GetInstance();
	int objDamage = e->obj->GetDamage();
	board->AddPlayerLife(-objDamage);
	health -= objDamage;
}

void CSimon::OnGetDamaged(LPGAMEOBJECT monster)
{
	if (onStairs == 0)
	{
		vx = vy = dx = dy = 0;

		this->vx = (-this->nx) * SIMON_DAMAGED_DEFLECT_X;
		this->vy = SIMON_DAMAGED_DEFLECT_Y;

		// Set jumping = true to re-use the SIMON_JUMP_GRAVITY
		jumping = true;
		controllable = false;

		// Simon is being untouchable but 
		// not going to stop being untouchable ( because of being on-air )
		untouchable_start = TIMER_ETERNAL;
	}
	else
	{
		BeUntouchable();
		Flicker();
	}

	// Update Simon's health
	CBoard * board = CBoard::GetInstance();
	int objDamage = monster->GetDamage();
	board->AddPlayerLife(-objDamage);
	health -= objDamage;
}

void CSimon::MoveRight()
{
	if (onStairs == 0)
	{
		if (attack_start == 0 &&	// While attacking, Simon cannot move
			!jumping)				// While jumping, Simon cannot move
		{
			this->nx = 1;
			if (!crouching)
				this->vx = SIMON_WALKING_SPEED * this->nx;
		}
	}	
	else
	{
		if (onStairs == 1)
			if (nx == 1)
				Upstairs();
			else
				Downstairs();

		else if (onStairs == -1)
			if (nx == 1)
				Downstairs();
			else
				Upstairs();
	}
}

void CSimon::MoveLeft()
{
	if (onStairs == 0)
	{
		if (attack_start == TIMER_IDLE &&	// While attacking, Simon cannot move
			!jumping)						// While jumping, Simon cannot move
		{
			this->nx = -1;

			// Can only change direction while crouching
			if (!crouching)
				this->vx = SIMON_WALKING_SPEED * this->nx;
		}
	}
	else
	{
		if (onStairs == 1)
			if (nx == 1)
				Downstairs();
			else
				Upstairs();

		else if (onStairs == -1)
			if (nx == 1)
				Upstairs();
			else
				Downstairs();
	}	
}

void CSimon::Crouch()
{
	if (!crouching && 
		onStairs == 0 &&
		attack_start == 0)
	{
		this->crouching = true;
		this->vx = 0;

		// Re-locate Simon to avoid overlapping
		this->y += SIMON_IDLE_BBOX_HEIGHT - SIMON_CROUCHING_BBOX_HEIGHT;
	}	
}

void CSimon::Idle()
{
	if (!jumping)
	{
		this->vx = 0;

		if (crouching && auto_crouch_start == TIMER_IDLE)
			this->StandUp();
	}
}

/*
	Use the weapon or item, if has
*/
void CSimon::UseWeapon()
{
	if (secondaryWeapon == Weapon::NONE)
		this->Attack(SIMON_ATTACK_BY_ROPE);
	else
	{
		// Check if Simon can do an use-weapon-action
		if (!IsAbleToUseWeapon())
			return;

		if (secondaryWeapon == Weapon::WATCH)
			CTimer::GetInstance()->Freeze(SIMON_DO_FREEZE_EVERYTHING_TIME, this);
		else
			this->Attack(SIMON_ATTACK_BY_ITEM);		
	}
}

void CSimon::Upstairs()
{
	if (jumping || attack_start != TIMER_IDLE)
		return;

	// Check if Simon is on stairs
	// If not, try to get into the stair
	if (onStairs == 0)
	{
		LPGAMEOBJECT stairs = NULL;

		// Check if Simon is on a stairs-start
		for (UINT i = 0; i < ovObjects.size(); ++i)
			if (dynamic_cast<CStairsUp *>(ovObjects[i]))
			{
				stairs = ovObjects[i];
				break;
			}

		// Do nothing if there is no stairs overlapped
		if (stairs == NULL) return;

		// Try to reach the right point of stairs
		float xS, yS;
		stairs->GetPosition(xS, yS);
		if (x != xS)
			StartAutoMove(SIMON_WALKING_SPEED, xS);
		else
		{
			nx = stairs->GetDirection();
			StartAutoMove(nx * SIMON_STAIR_SPEED_X, -SIMON_STAIR_SPEED_Y, SIMON_AUTO_STAIR_TIME);
			onStairs = 1;
		}
	}
	
	else
	{
		// Simon is going downstairs and wanting to climb back
		if (onStairs == -1)
		{
			this->nx = -nx;
			onStairs = 1;
		}

		StartAutoMove(nx * SIMON_STAIR_SPEED_X, -SIMON_STAIR_SPEED_Y, SIMON_AUTO_STAIR_TIME);
	}
}

void CSimon::Downstairs()
{
	if (jumping || attack_start != TIMER_IDLE)
		return;

	// Check if Simon is onStairs
	// If not, try to get into the stairs
	if (onStairs == 0)
	{
		// Check if Simon is on a stairs-start
		LPGAMEOBJECT stairs = NULL;
		for (UINT i = 0; i < ovObjects.size(); ++i)
			if (dynamic_cast<CStairsDown *>(ovObjects[i]))
			{
				stairs = ovObjects[i];
				break;
			}

		// Do nothing if there is no stairs overlapped
		if (stairs == NULL) return;

		// Try to reach the right point of stairs
		float xS, yS;
		stairs->GetPosition(xS, yS);
		if (x != xS)
			StartAutoMove(SIMON_WALKING_SPEED, xS);
		else
		{
			nx = stairs->GetDirection();
			StartAutoMove(nx * SIMON_STAIR_SPEED_X, SIMON_STAIR_SPEED_Y, SIMON_AUTO_STAIR_TIME);
			onStairs = -1;
		}
	}

	else
	{
		// Simon is going upstairs and wanting to go back
		if (onStairs == 1)
		{
			nx = -nx;
			onStairs = -1;
		}

		StartAutoMove(nx * SIMON_STAIR_SPEED_X, SIMON_STAIR_SPEED_Y, SIMON_AUTO_STAIR_TIME);
	}

}

void CSimon::Revive()
{
	this->health = PLAYER_HEALTH_DEFAULT;
	CBoard::GetInstance()->SetPlayerLife(this->health);
	dying = false;
	this->state = STATE_VISIBLE;
	this->controllable = true;
}

/*
	Move automatically to the given position (x-axis) with the given speed.
	Move horizontally only.
*/
void CSimon::StartAutoMove(float vx, float xDestination)
{
	if (!autoMove)
	{
		autoInfo.xDes = xDestination;
		autoInfo.vx = vx;

		autoMove = true;
		controllable = false;
	}
}

void CSimon::StartAutoMove(float vx, float vy, DWORD time)
{
	// Can only perform one auto-move at a time
	// NOTE: this autoMove flag will be turned off in Update() -> ProceedAutoMove()
	if (!autoMove)
	{
		autoInfo.vx = vx;
		autoInfo.vy = vy;
		autoInfo.autoTimeLast = time;

		autoMove = true;
		auto_start = GetTickCount();
		controllable = false;
	}
}

bool CSimon::IsAbleToUseWeapon()
{
	int heartsCost = 0;
	switch (secondaryWeapon)
	{
	case Weapon::DAGGER:
		heartsCost = DAGGER_HEART_COST;
		break;
	case Weapon::AXE:
		heartsCost = AXE_HEART_COST;
		break;
	case Weapon::HOLYWATER:
		heartsCost = HOLYWATER_HEART_COST;
		break;
	case Weapon::WATCH:
		heartsCost = WATCH_HEART_COST;
		break;
	default:
		break;
	}

	if (numberOfHearts - heartsCost >= 0)
	{
		if (secondaryWeapon == Weapon::WATCH)
		{
			numberOfHearts -= heartsCost;
			CBoard::GetInstance()->SetHeart(numberOfHearts);
			return true;
		}

		else if (CWeapons::GetInstance()->CheckQuantity(secondaryWeapon) &&
			attack_start == TIMER_IDLE)
		{
			numberOfHearts -= heartsCost;
			CBoard::GetInstance()->SetHeart(numberOfHearts);
			return true;
		}		
	}
	
	return false;
}

void CSimon::KillAllMonsters()
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CZombie *>(coObjects->at(i)) ||
			dynamic_cast<CPinkBat *>(coObjects->at(i)) ||
			dynamic_cast<CFish *>(coObjects->at(i)))
			coObjects->at(i)->Destroy();
	}
}

void CSimon::Jump()
{
	if (!jumping && !crouching
		&& attack_start == TIMER_IDLE 
		&& onStairs == 0)
	{
		this->jumping = true;
		this->crouching = true;
		this->vy = -SIMON_JUMP_SPEED;		
	}
}

void CSimon::Attack(int choice)
{
	if (attack_start == 0)
	{
		attack_start = GetTickCount();

		// Simon behaviors
		if (jumping)
			this->StandUp();

		if (choice == SIMON_ATTACK_BY_ROPE)
		{
			// Set up Simon's rope
			rope->SetState(STATE_VISIBLE);
			rope->SetDirection(nx);
		}
	}
}

void CSimon::CalibrateCameraPosition()
{
	// Get simon's central point
	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);
	float xCentral = (l + r) / 2;
	float yCentral = (t + b) / 2;


	cameraInstance->FocusOnPoint(xCentral, yCentral);
}

void CSimon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;

	switch (currentAniID)
	{
	case (int)SimonAniID::CROUCH_RIGHT:
	case (int)SimonAniID::CROUCH_LEFT:
	case (int)SimonAniID::CROUCH_ATTACK_LEFT:
	case (int)SimonAniID::CROUCH_ATTACK_RIGHT:
		if (jumping)
		{
			// While jumping, Simon will have different bounding box
			// Even though the animation is the same
			right = x + SIMON_JUMPING_BBOX_WIDTH;
			bottom = y + SIMON_JUMPING_BBOX_HEIGHT;
		}
		else
		{
			right = x + SIMON_CROUCHING_BBOX_WIDTH;
			bottom = y + SIMON_CROUCHING_BBOX_HEIGHT;
		}
		break;		
	default:
		right = x + SIMON_IDLE_BBOX_WIDTH;
		bottom = y + SIMON_IDLE_BBOX_HEIGHT;
		break;
	}
}


/*
	Tell Simon to do something, if in control
*/
void CSimon::SetAction(Action action)
{
	if (controllable && freezing == false)
	{
		switch (action)
		{
		case Action::WALK_RIGHT:
			this->MoveRight();
			break;
		case Action::WALK_LEFT:
			this->MoveLeft();
			break;
		case Action::CROUCH:
			this->Crouch();
			break;
		case Action::JUMP:
			this->Jump();
			break;
		case Action::ATTACK:
			this->Attack();
			break;
		case Action::USE_ITEM:
			this->UseWeapon();
			break;
		case Action::UPSTAIRS:
			this->Upstairs();
			break;
		case Action::DOWNSTAIRS:
			this->Downstairs();
			break;
		case Action::IDLE:
			this->Idle();
			break;
		default:
			break;
		}
	}
}

void CSimon::SetFreezing(bool freezing)
{
	CGameObject::SetFreezing(freezing);
	rope->SetFreezing(freezing);
}

CSimon * CSimon::GetInstance()
{
	if (__instance == NULL)
		__instance = new CSimon();

	return __instance;
}

