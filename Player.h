#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED

#include <vector>
class Weapon;
class Enemy;

class Player {
protected:
    std::vector<Weapon*> inventory; //weapon inventory
    Weapon* equippedWeapon = nullptr;
    int skillPoints; //levels up the below
    int health;
    int currentHealth;
    int strength; //increases damage for swords and broadswords
    int defense;
    int evasion;
    int intelligence; //staffs
    int dexterity; //daggers and bows

public:
    //sp, h, s, d, e, i, d, cH
    Player(int, int, int, int, int, int, int, int);
    virtual ~Player();
    void addSkillPoints (int sp);
    void addHealth (int h);
    void addCurrentHealth (int cH);
    void addStrength (int s);
    void addDefense (int d);
    void addEvasion (int e);
    void addIntelligence (int i);
    void addDexterity (int dex);
    void takeDamage (int);
    void setCurrentHealth(int h);
    void viewStats();

    //Inventory methods
    void setInventorySize ();
    void viewInventory ();
    void equipWeapon (int);
    void deleteWeapon (int);
    void addSword (std::string, int, int);
    void addBow (std::string, int, int);
    void addStaff (std::string, int, int);
    std::string getEquippedWeapon ();
    int getCurrentHealth ();
    int getHealth ();
    int getWeaponDamage();
    int getWeaponRange();
    bool checkInventoryFull ();
    friend void addToInventory(Player*& p, Weapon*& loot);
    friend void equipOrThrowFromInventory (Player*& p);

};

class Mage : public Player {
protected:

public:
    Mage(int, int, int, int, int, int, int, int);

};

class Knight : public Player {
protected:

public:
    Knight(int, int, int, int, int, int, int, int);
};

class Thief : public Player {
protected:

public:
    Thief(int, int, int, int, int, int, int, int);
};


#endif // PLAYER_H_INCLUDED
