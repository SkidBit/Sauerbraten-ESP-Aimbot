#pragma once
#include "includes.h"

#define CHECK_BAD_PTR(x) if(IsBadReadPtr(this,sizeof(x))) return nullptr
#define CHECK_BAD(x) if(IsBadReadPtr(this, sizeof(x))) return
#define CHECK_BAD_NUM(x) if(IsBadReadPtr(this, sizeof(x))) return 0
#define CHECK_BAD_VEC3(x) if(IsBadReadPtr(this, sizeof(x))) return Vector3{0,0,0}
#define CHECK_BAD_VEC2(x) if(IsBadReadPtr(this, sizeof(x))) return Vector2{0,0}

struct Vector2 { float x, y; };

class Vector3
{
public:

	float x, y, z;

	Vector3() {};
	Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}
	Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
	Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
	Vector3 operator * (const float& rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
	Vector3 operator / (const float& rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }
	Vector3& operator += (const Vector3& rhs) { return *this = *this + rhs; }
	Vector3& operator -= (const Vector3& rhs) { return *this = *this - rhs; }
	Vector3& operator *= (const float& rhs) { return *this = *this * rhs; }
	Vector3& operator /= (const float& rhs) { return *this = *this / rhs; }
	float Length() const { return sqrtf(x * x + y * y + z * z); }
	Vector3 Normalize() const { return *this * (1 / Length()); }
	float Distance(const Vector3& rhs) const { return (*this - rhs).Length(); }
};

struct Vector4
{
	float x, y, z, w;
};

class Entity
{
public:
	char pad_0000[48]; //0x0000
	Vector3 position; //0x0030
	float viewAngleX; //0x003C
	float viewAngleY; //0x0040
	char pad_0044[51]; //0x0044
	bool isDead; //0x0077 int8_t before, but bool should work fine
	char pad_0078[256]; //0x0078
	int32_t life; //0x0178
	char pad_017C[4]; //0x017C
	int32_t armor; //0x0180
	char pad_0184[240]; //0x0184
	char name[20]; //0x0274
	char pad_0288[240]; //0x0288
	char team[10]; //0x0378
	char pad_0382[3104]; //0x0382

	Vector3 getPosition() { CHECK_BAD_VEC3(Entity); return position; };
	bool getIsDead() { CHECK_BAD_NUM(Entity); return isDead; };
	float getviewAngleX() { CHECK_BAD_NUM(Entity); return viewAngleX; };
	float getviewAngleY() { CHECK_BAD_NUM(Entity); return viewAngleY; };
	int32_t getLife() { CHECK_BAD_NUM(Entity); return life; };
	int32_t getArmor() { CHECK_BAD_NUM(Entity); return armor; };
	void setViewAngleX(float newAngle) { CHECK_BAD(Entity); viewAngleX = newAngle; };
	void setViewAngleY(float newAngle) { CHECK_BAD(Entity); viewAngleY = newAngle; };
	char* getName() { CHECK_BAD_PTR(Entity); return name; };
	char* getTeam() { CHECK_BAD_PTR(Entity); return team; };

}; //Size: 0x0FA2

class EntityList
{
public:
	Entity* entities[64]; //0x0000 start at entry 1!
}; //Size: 0x0008


inline std::vector<BYTE> teamModes = { 2, 4, 6, 8, 10, 11, 12, 17, 13, 14, 18, 15, 16, 19, 20, 21, 22 };

const uintptr_t entityListPointer = 0x346C90;
const uintptr_t localPlayerPointer = 0x2A5730;
const uintptr_t viewMatrixPointer = 0x32D040;
const uintptr_t gameModePointer = 0x26f6c0;
const uintptr_t playerCountPointer = 0x346C9C;

// globals
extern Entity* localPlayer;
extern EntityList* entityList;
extern float* viewMatrix;
extern int playerCount;
extern uintptr_t baseAddressMainMod;

extern bool shutdown;

