#pragma once
#include "includes.h"
#include "constants.h"


bool WorldToScreen(Vector3 pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight);
float getDistanceBetweenEntities(Entity* firstEntity, Entity* secondEntity);
Entity* getClosestEnemy(Entity* player, EntityList* entList, bool teamgame, int playerCount);
bool isTeamGame(uintptr_t baseAddressMainMod);
Entity* getClosestEnemyToCrosshair(Entity* player, EntityList* entList, bool teamGame, int playerCount);
float calculateDifferenceBetweenAngles(float firstAngle, float secondAngle);