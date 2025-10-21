#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include "Weapon.h"

class Enemy {
protected:
    std::string name;
    int health;
    int damage;
    int distance;
    Weapon* loot;
public:
    //Have to intentionally create an enemy object with damage and health
    Enemy(int, int);
    virtual ~Enemy();
    void setLoot ();
    void setHealth (int);
    void setDamage(int);
    void setDistance (int);
    void subtractDistance (int);
    int getDistance();
    int getDamage();
    int getHealth();
    std::string getName();
    void takeDamage(int);
    int getLootDamage ();
    int getLootRange ();

    void attack();

};

class Goblin : public Enemy {
protected:
public:
    Goblin(int, int);

};

class ArcherGoblin : public Goblin {
protected:
public:
    //health, damage, range
    ArcherGoblin (int, int, int);

};

class MeleeGoblin : public Goblin {
protected:
public:
    MeleeGoblin (int, int, int);

};

class KingGoblin : public Goblin {
protected:
public:
    //No loot - once defeated, game ends
    KingGoblin(int, int, int);

};

#endif // ENEMY_H_INCLUDED
