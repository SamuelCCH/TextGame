#ifndef WEAPON_H_INCLUDED
#define WEAPON_H_INCLUDED

class Weapon {
protected:
    std::string name;
    std::string identifier;
    int damage;
    int range;
    //for heap allocation tracking
    int numPointers = 0;

public:
    virtual int dmgMultiplier (int) = 0;
    virtual ~Weapon() = default;
    void setName (std::string);
    void setDamage (int);
    void setRange (int);
    int getDamage ();
    int getNumPointers ();
    std::string getName();
    int getRange();
    std::string getIdentifier ();
    void addNumPointers ();
    void subtractNumPointers ();

};

class Sword : public Weapon {
protected:

public:
    //args: damage, range
    Sword(int, int);
    Sword();
    //args: strength
    int dmgMultiplier (int);
};

class Staff : public Weapon {
protected:

public:
    Staff(int, int);
    Staff();
    //args: intelligence
    int dmgMultiplier (int);

};

class Bow : public Weapon {
protected:

public:
    Bow(int, int);
    Bow();
    //args: dexterity
    int dmgMultiplier (int);

};


#endif // WEAPON_H_INCLUDED
