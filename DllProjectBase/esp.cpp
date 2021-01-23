#include "esp.h"
#include "mem.h"

#define PI 3.14159265358979323846

bool WorldToScreen(Vector3 pos, Vector2& screen, float matrix[16], int windowWidth, int windowHeight)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	Vector4 clipCoords;
	clipCoords.x = pos.x * matrix[0] + pos.y * matrix[4] + pos.z * matrix[8] + matrix[12];
	clipCoords.y = pos.x * matrix[1] + pos.y * matrix[5] + pos.z * matrix[9] + matrix[13];
	clipCoords.z = pos.x * matrix[2] + pos.y * matrix[6] + pos.z * matrix[10] + matrix[14];
	clipCoords.w = pos.x * matrix[3] + pos.y * matrix[7] + pos.z * matrix[11] + matrix[15];

	if (clipCoords.w < 0.1f)
		return false;

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	Vector3 NDC;
	NDC.x = clipCoords.x / clipCoords.w;
	NDC.y = clipCoords.y / clipCoords.w;
	NDC.z = clipCoords.z / clipCoords.w;

	screen.x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	screen.y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);
	return true;
}

float getDistanceBetweenEntities(Entity* firstEntity, Entity* secondEntity)
{
	return sqrt(pow(firstEntity->getPosition().x - secondEntity->getPosition().x, 2) + pow(firstEntity->getPosition().y - secondEntity->getPosition().y, 2));
}

Entity* getClosestEnemy(Entity* player, EntityList* entList, bool teamGame, int playerCount)
{
	float closestEntityDist = INFINITY;
	Entity* closestEntity = nullptr;
	float tmpDistance = 0;

	for (int i = 1; i < playerCount + 1; i++) {
		if (entList->entities[i] != nullptr && isEntityValid(entList->entities[i])) {
			if (teamGame) {
				if (strcmp(player->getTeam(), entList->entities[i]->getTeam()) != 0) {
					tmpDistance = getDistanceBetweenEntities(player, entList->entities[i]);
					if (tmpDistance < closestEntityDist) {
						closestEntityDist = tmpDistance;
						closestEntity = entList->entities[i];
					}
				}
			}
			else {
				tmpDistance = getDistanceBetweenEntities(player, entList->entities[i]);
				if (tmpDistance < closestEntityDist) {
					closestEntityDist = tmpDistance;
					closestEntity = entList->entities[i];
				}
			}
		}
	}
	return closestEntity;
}

float calculateDifferenceBetweenAngles(float firstAngle, float secondAngle)
{
	float difference = secondAngle - firstAngle;
	while (difference < -180) difference += 360;
	while (difference > 180) difference -= 360;
	return difference;
}

bool isEntityValid(Entity* entity) {
	return entity->getLife() > 0 && entity->getLife() < 1000 && entity->getIsDead() == false;
}

Entity* getClosestEnemyToCrosshair(Entity* player, EntityList* entList, bool teamGame, int playerCount)
{
	Entity* closestEntity = nullptr;

	float tmpAngleDiff = 0;
	float closestEntityAngleDiff = INFINITY;

	float currentXAngle = 0;
	float currentYAngle = 0;

	for (int i = 1; i < playerCount + 1; i++) {
		if (entList->entities[i] != nullptr) {

			if (isEntityValid(entList->entities[i])) {

				Vector3 src = player->getPosition();
				Vector3 dst = entList->entities[i]->getPosition();

				// calculate angles for target
				Vector2 angle;
				angle.x = (atan2f(dst.x - src.x, dst.y - src.y) * -1.0) / PI * 180.0;
				angle.y = asinf((dst.z - src.z) / src.Distance(dst)) * (180.0 / PI);

				// read current angles
				currentXAngle = player->getviewAngleX();
				currentYAngle = player->getviewAngleY();

				// calculate absolute angle diff
				tmpAngleDiff = std::abs(calculateDifferenceBetweenAngles(currentXAngle, angle.x));
				tmpAngleDiff += std::abs(currentYAngle - angle.y);

				if (teamGame) {
					if (strcmp(player->getTeam(), entList->entities[i]->getTeam()) != 0) {
						if (tmpAngleDiff < closestEntityAngleDiff) {
							closestEntityAngleDiff = tmpAngleDiff;
							closestEntity = entList->entities[i];
						}
					}
				}
				else {
					if (tmpAngleDiff < closestEntityAngleDiff) {
						closestEntityAngleDiff = tmpAngleDiff;
						closestEntity = entList->entities[i];
					}
				}
			}
		}
	}
	return closestEntity;
}

void clickMouseLeft() {

	INPUT    Input = { 0 };
	// left down 
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &Input, sizeof(INPUT));
	Sleep(25);
	// left up
	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &Input, sizeof(INPUT));
}

void shootIfEnemyCloseToCrosshair(Entity* player, EntityList* entList, bool teamGame, int playerCount)
{
	// make variable depending on distance
	if (getClosestEnemyToCrosshairFOVDistance(player, entList, teamGame, playerCount) < 2.f) {
		clickMouseLeft();
	}
}

float getClosestEnemyToCrosshairFOVDistance(Entity* player, EntityList* entList, bool teamGame, int playerCount)
{
	Entity* closestEntity = nullptr;

	float tmpAngleDiff = 0;
	float closestEntityAngleDiff = INFINITY;

	float currentXAngle = 0;
	float currentYAngle = 0;

	for (int i = 1; i < playerCount + 1; i++) {
		if (entList->entities[i] != nullptr &&
			entList->entities[i]->getLife() > 0) {

			if (isEntityValid(entList->entities[i])) {

				Vector3 src = player->getPosition();
				Vector3 dst = entList->entities[i]->getPosition();

				// calculate angles for target
				Vector2 angle;
				angle.x = (atan2f(dst.x - src.x, dst.y - src.y) * -1.0) / PI * 180.0;
				angle.y = asinf((dst.z - src.z) / src.Distance(dst)) * (180.0 / PI);

				// read current angles
				currentXAngle = player->getviewAngleX();
				currentYAngle = player->getviewAngleY();

				// calculate absolute angle diff
				tmpAngleDiff = std::abs(calculateDifferenceBetweenAngles(currentXAngle, angle.x));
				tmpAngleDiff += std::abs(currentYAngle - angle.y);

				if (teamGame) {
					if (strcmp(player->getTeam(), entList->entities[i]->getTeam()) != 0) {
						if (tmpAngleDiff < closestEntityAngleDiff) {
							closestEntityAngleDiff = tmpAngleDiff;
							closestEntity = entList->entities[i];
						}
					}
				}
				else {
					if (tmpAngleDiff < closestEntityAngleDiff) {
						closestEntityAngleDiff = tmpAngleDiff;
						closestEntity = entList->entities[i];
					}
				}
			}
		}
	}

	return closestEntityAngleDiff;
}


bool isTeamGame(uintptr_t baseAddressMainMod)
{
	BYTE gameMode = *(BYTE*)(baseAddressMainMod + gameModePointer);
	return std::find(teamModes.begin(), teamModes.end(), gameMode) != teamModes.end();
}
