#include <iostream>
#include <vector>
#include <cstring>
#include <limits>
#include <ctime>
#include <bits/stdc++.h>
#include <iomanip>
//#include <coino.h>
#include "GameManager.h"
#include "Weapon.h"
#include "Player.h"
#include "Enemy.h"
#define INVENTORY_SIZE 5

using namespace std;

//fwd decl
int getValidIntInput (int low, int high);
char getValidCharInput (char c1, char c2, char c3);
char getFleeOrMove ();
char getAttackOrFlee ();
void getEnter ();
void displayNonCombatUI (Player*& pl);
void CombatLoop (Player*& p, Enemy*& enemy, GameManager& gm, int& combatCounter, std::string enemyType);
bool validPlayerAttack (Player*& pl, Enemy*& en, GameManager& g);
void playerMove (Enemy*& en, GameManager& g);
void enemyTurn (Player*& pl, Enemy*& en, GameManager& g);
void checkPlayerStatus (Player*& pl, GameManager& g);
bool checkEnemyDead (Enemy*& en);
bool checkMinDistance (Enemy*& en);
Weapon* lootDrop (std::string name);

//Useful functions
void clearsrc() {
    std::cout << "\033[2J\033[1;1H";
}
int getValidIntInput (int low, int high) {
    int userInp;
    std::cin >> userInp;
    while (std::cin.fail() || userInp < low || userInp > high) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\nEnter a valid integer: ";
        std::cin >> userInp;
        std::cin.ignore();
        std::cout << std::endl;
    }
    return userInp;

}

char getValidCharInput (char c1, char c2, char c3) {
    char input;
    std::cin >> input;
    while (std::cin.fail() || (input != c1 && input != c2 && input != c3) ) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\nEnter a valid character: ";
        std::cin >> input;
        std::cin.ignore();
        std::cout << std::endl;
    }
    return input;

}

char getFleeOrMove () {
    char input;
    std::cin >> input;
    while (std::cin.fail() || (input != 'f' && input != 'm')) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\nEnter a valid character: ";
        std::cin >> input;
        std::cin.ignore();
        std::cout << std::endl;
    }
    return input;
}

char getAttackOrFlee () {
    char input;
    std::cin >> input;
    while (std::cin.fail() || (input != 'a' && input != 'f')) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\nEnter a valid character: ";
        std::cin >> input;
        std::cin.ignore();
        std::cout << std::endl;
    }
    return input;

}

void getEnter () {
    std::cout << "\n\nPress Enter to Continue...\n\n";
    std::cin.clear();
    std::cin.get();
    std::cin.ignore();
}
void displayNonCombatUI (Player*& pl) {
    std::cout << "\nMenu                   Equipped Weapon: " <<  pl->getEquippedWeapon()  << " \n  1. Move Forward               Health: " << pl->getCurrentHealth()  << "/" << pl->getHealth()
              << "\n  2. Rest                                                |\n  3. Inventory                                           |\n  4. Player Stats                                        |\n"
              << "---------------------------------------------------------+\n\n";
}
void displayCombatUI (Player*& pl, Enemy*& en) {
    //Enemy display
    std::cout << std::right << std::setw(80) << "|  " << en->getName() << "\n";
    std::cout << std::right << std::setw(79) << "|\n";
    std::cout << std::right << std::setw(88) << "|  Health: " << en->getHealth() << "   Distance: " << en->getDistance() << "\n";
    std::cout << std::right << std::setw(79) << "|\n";
    std::cout << std::right << std::setw(120) << "---------------------------------------";

    //Player display
    std::cout << "\nYou" << std::right << std::setw(50) << "|\n";
    std::cout << "Health: " << pl->getCurrentHealth() << "/" << pl->getHealth() << std::right << std::setw(38) << "|\n";
    std::cout << "(a)ttack -- Deal " << pl->getWeaponDamage() << " damage" << std::right << std::setw(27) << "|\n";
    std::cout << "(f)lee -- Run away from fight."  << std::right << std::setw(23) << "|\n";
    std::cout << "(m)move -- Move 5 units towards enemy."  << std::right << std::setw(15) << "|\n";
    std::cout << "--------------------------------------------------";
}

void equipOrThrowFromInventory (Player*& p) {
    int userInp, userInp2;
    userInp = getValidIntInput(1,3);
    switch (userInp) {
    //Equip item from inventory
    case 1:
        clearsrc();
        p->viewInventory();
        std::cout << "\n\nChoose item to equip: (1-5)\n";
        userInp2 = getValidIntInput(1,5);
        if (p->inventory[userInp2-1] == nullptr) {
            std::cout << "\nNo item to equip\n";
            getEnter();
            break;
        }
        p->equipWeapon(userInp2);
        clearsrc();
        p->viewInventory();
        getEnter();
        break;
    //Delete item from inventory
    case 2:
        std::cout << "\n\nChoose item to throw: (1-5)\n";
        userInp2 = getValidIntInput(1,5);
        clearsrc();
        p->deleteWeapon(userInp2);
        p->viewInventory();
        getEnter();
    //Return to main menu
    case 3:
        break;

    }
}

//when player chooses to move forward
void CombatLoop (Player*& p, Enemy*& enemy, GameManager& gm, int& combatCounter, std::string enemyType) {
    //melee case
    char cInp;
    bool successfulAttack = true;

    //create enemy depending on type specified and iteration of combat
    if (enemyType == "melee" && combatCounter == 0) {
        enemy = new MeleeGoblin(20,20,1);
        enemy->setDistance(10);
    }
    else if (enemyType == "archer" && combatCounter == 0) {
        enemy = new ArcherGoblin(20, 20,10);
        enemy->setDistance(25);
    }
    else if (enemyType == "melee" && combatCounter == 1){
        enemy = new MeleeGoblin(40,20,1);
        enemy->setDistance(15);
    }
    else if (enemyType == "archer" && combatCounter == 1) {
        enemy = new ArcherGoblin(30, 20,10);
        enemy->setDistance(25);
    }
    else if (combatCounter == 2) {
        enemy = new KingGoblin(100, 10, 5);
        enemy->setDistance(2);
    }

    std::cout << "\nEnemy Encountered!\n";
    while (enemy->getHealth() > 0 || cInp != 'f') {
        clearsrc();
        gm.setPlayerTurn(true);
        //combat UI
        displayCombatUI(p, enemy);
        std::cout << "\nPress 'a' to attack\n"
                  << "Press 'f' to flee combat\n"
                  << "Press 'm' to move towards combat\n";

        cInp = getValidCharInput('a', 'f', 'm');

        //Case 1 - Player attempts an attack
        if (cInp == 'a') {
            successfulAttack = validPlayerAttack(p, enemy, gm);
            if ( successfulAttack == true) {
                //check if enemy died
                if (checkEnemyDead(enemy)) {
                    clearsrc();
                    displayCombatUI(p,enemy);
                    std::cout << "\nEnemy has been slain!\n";
                    if (combatCounter >= 2) {
                        std::cout << "\nYou've won the war!\n";
                        gm.endGame();
                    }
                    getEnter();
                    break;
                }
                //case where enemy is still alive after attack
                std::cout << "\n" << p->getWeaponDamage() << " Damage dealt to enemy!\n";
            }
            if (successfulAttack == false) {
                std::cout << "\nToo far from enemy!\n";
                getEnter();
                continue;
            }

        }

        //Case 2 - Player attempts to move
        if (cInp == 'm') {
            //if player tries to move when he is already 1 unit away
            if (checkMinDistance(enemy) ) {
                std::cout << "\nCan't get any closer!\n";
                getEnter();
                continue;
            }
            playerMove (enemy, gm);
            getEnter();
        }

        //Case 3 - Player flees combat
        if (cInp == 'f') {
            std::cout << "\nSafely fled combat.\n";
            if (combatCounter >= 2) {
                //if he flees at king goblin, end the game as via the coward end route - given by both player and king goblin being alive
                std::cout << "\nYou coward\n";
                gm.endGame();
            }
            getEnter();
            break;

        }

        //Now that we've dealt with all the Player turn cases, move onto enemy turn
        enemyTurn(p, enemy, gm);
        //check if player died after enemy turn
        if (gm.getEnd() == true) {
            std::cout << "\nYou died\n";
            getEnter();
            break;
        }
        getEnter();
    }
    //keep track of the fighting loop to output harder/easier enemies
    combatCounter++;
}

void addToInventory(Player*& p, Weapon*& loot) {
    int userInput = -1;
    if (p->checkInventoryFull() == true) {
        std::cout << "\nInventory is full!\n";
        //show player's inventory to dump either replace existing item or discard the new loot
        p->viewInventory();
        std::cout << "\nChoose item to replace or press '0' to discard new item\n";
        while (true) {
            userInput = getValidIntInput(0, 5);

            if (userInput == 0) {
                std::cout << "\nDid not pick up item.\n";
                delete loot;
                loot = nullptr;
                return;
            }

            // Check first before deleting
            if (p->inventory[userInput - 1] != nullptr && p->inventory[userInput - 1]->getNumPointers() > 1) {
                std::cout << "\nCan't toss equipped weapon\n";
                continue;  // Ask again
            }

            // Valid replacement
            p->deleteWeapon(userInput);
            break;
        }
    }

    if (loot->getIdentifier() == "sword") {
        //make a copy of the loot item in the inventory before deleting the original loot
        p->addSword(loot->getName(), loot->getDamage(), loot->getRange());

    }
    if (loot->getIdentifier() == "bow") {
        //make a copy of the loot item in the inventory before deleting the original loot
        p->addBow(loot->getName(), loot->getDamage(), loot->getRange());
    }
    if (loot->getIdentifier() == "staff") {
        //make a copy of the loot item in the inventory before deleting the original loot
        p->addStaff(loot->getName(), loot->getDamage(), loot->getRange());
    }
    delete loot;
    loot = nullptr;
    std::cout << "\nSuccessfully added new item to inventory!\n";
    return;

}

bool validPlayerAttack (Player*& pl, Enemy*& en, GameManager& g) {
    if (en->getDistance() > pl->getWeaponRange()) {
        //false if didn't attack
        return false;
    }
    //if within striking distance, deal damage and set player's turn to false
    en->takeDamage(pl->getWeaponDamage());
    g.setPlayerTurn(false);
    //true if in range and attacked
    return true;

}

//when 'm' is entered in combat, move 5 units
void playerMove (Enemy*& en, GameManager& g) {
    if (en->getDistance() > 5 ) {
            en->subtractDistance(5);
            //end player turn
            g.setPlayerTurn(false);
            std::cout << "\nPlayer moved 5 units towards the enemy!\n";
            return;
    }
    else {
        en->subtractDistance(en->getDistance()-1);
        g.setPlayerTurn(false);
        std::cout << "\nPlayer moved " << en->getDistance() -1 << " units towards the enemy!\n";
        return;
    }
}


void enemyTurn (Player*& pl, Enemy*& en, GameManager& g) {
    //case where enemy weapon range is smaller than the distance between them
    if (en->getDistance() > en->getLootRange() ) {
        std::cout << "\n" << std::right << std::setw(80) << "Enemy moved towards you!\n";
        //shrink distance by 5
        if (en->getDistance() > 5 ) {
            en->subtractDistance(5);
            //set player's turn true
            g.setPlayerTurn(true);
            return;
        }
        else {
            //in the case where distance is smaller than 5 units, but still out of range of weapon,
            //move until there is 1 unit of space between them
            en->subtractDistance(en->getDistance()-1);
            return;
        }
    }

    //in the case where range > distance, enemy attacks
    else {
        std::cout << "\n" << std::right << std::setw(80) << "Enemy attacked you!\n";
        pl->takeDamage(en->getLootDamage());
        if (pl->getCurrentHealth() <= 0) {
            g.endGame();
        }
        return;

    }
}

//Check functioons
void checkPlayerStatus (Player*& pl, GameManager& g) {
    if (pl->getHealth() <= 0) {
        g.endGame();
    }
}
bool checkEnemyDead (Enemy*& en) {
    if (en->getHealth() <=0) {
        return true;
    }
    return false;
}

bool checkMinDistance (Enemy*& en) {
    if (en->getDistance() == 1) {
        return true;
    }
    return false;
}

Weapon* lootDrop (std::string name) {
    Bow* bp;
    Sword* sp;
    Staff* stp;
    if (name == "Archer Goblin") {
        switch (1+ rand()%3) {
        case 1:
            bp = new Bow(5,20);
            bp->setName("Old Bow");
            return bp;
        case 2:
            bp = new Bow (60,20);
            bp->setName("Superior Bow");
            return bp;
        case 3:
            bp = new Bow (90,40);
            bp->setName("God Bow");
            return bp;
        }
    }
    if (name == "Melee Goblin") {
        switch(1+ rand()%3) {
        case 1:
            stp = new Staff(95,15);
            stp->setName("Mythical Staff");
            return stp;
        case 2:
            sp = new Sword(5,1);
            sp->setName("Old Blade");
            return sp;
        case 3:
            sp = new Sword(95,10);
            sp->setName("Best Blade");
            return sp;

        }
    }

}


//GameManager methods
void GameManager::resumeGame () {
    this->paused = false;
}
void GameManager::pauseGame () {
    this->paused = true;
}
void GameManager::startGame () {
    this->ended = false;
}
void GameManager::endGame () {
    this->ended = true;
}
bool GameManager::getPause () {
    return this->paused;
}
bool GameManager::getEnd () {
    return this->ended;
}
bool GameManager::getPlayerTurn() {
    return this->playerTurn;
}
void GameManager::setPlayerTurn (bool turn) {
    // turn = true/false
    this->playerTurn = turn;
}


//Player methods
Player::Player(int sp, int h, int s, int d, int e, int i, int dex, int cH) {
    this->skillPoints = sp;
    this->strength = s;
    this->health = h;
    this->defense = d;
    this->evasion = e;
    this->intelligence = i;
    this->dexterity = dex;
    this->currentHealth = cH;
}
Player::~Player () {
    for (int i; i < INVENTORY_SIZE; i++) {
        delete inventory[i];
        inventory[i] = nullptr;
    }
}
void Player::addSkillPoints (int sp) {
        skillPoints += sp;
    }
void Player::addHealth (int h) {
    health += h;
}
void Player::addStrength (int s) {
    strength += s;
}
void Player::addDefense (int d) {
    defense += d;
}
void Player::addEvasion (int e) {
    evasion += e;
}
void Player::addIntelligence (int i) {
    intelligence += i;
}
void Player::addDexterity (int dex) {
    dexterity += dex;
}
void Player::takeDamage (int dmg) {
    this->currentHealth -= dmg;
}
void Player::addCurrentHealth (int cH) {
    this->currentHealth += cH;
}
void Player::viewStats () {
    std::cout << "\nPlayer Stats\n";
    std::cout << "\nStrength: " << this->strength << "\n"
              << "\nIntelligence: " << this->intelligence << "\n"
              << "\nDexterity: " << this->dexterity << "\n"
              << "\nDefense: " << this->defense << "\n"
              << "\nEvasion: " << this->evasion << "\n"
              << "\nHealth: " << this->health << "\n\n"
              << "\nSkill Points: " << this->skillPoints << "\n"
              << "Equipped Weapon: " << this->getEquippedWeapon() << " -- " << this->getWeaponDamage() << " damage; " << this->getWeaponRange() << " range";
}
void Player::viewInventory () {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (Player::inventory[i] == nullptr) {
            std::cout << i+1 << ". " << "Empty Slot" << std::endl;
            continue;
        }
        if (Player::inventory[i]->getNumPointers() > 1) {
            std::cout << i+1 << ". " << Player::inventory[i]->getName() << " -- " << inventory[i]->getDamage() << " damage; " << inventory[i]->getRange() << " range" << " -- EQUIPPED" << std::endl;
            continue;
        }

        std::cout << i+1 << ". " << Player::inventory[i]->getName() << " -- " << inventory[i]->getDamage() << " damage; " << inventory[i]->getRange() << " range" << std::endl;
    }
}
void Player::equipWeapon (int choice) {
    //choice - 1 because the index in the UI starts from 1
    if (Player::inventory[choice-1] != nullptr) {
        if (Player::equippedWeapon != nullptr) {
            Player::equippedWeapon->subtractNumPointers();
        }
        //equippedWeapon is a shallow copy, so we keep track of the number of pointers to the heap inventory items
        //to make sure we don't have a dangling pointer
        Player::equippedWeapon = Player::inventory[choice-1];
        Player::inventory[choice-1]->addNumPointers();
    }

}
void Player::deleteWeapon (int choice) {
    if (Player::inventory[choice - 1] == nullptr) {
        std::cout << "\nSlot already empty\n\n";
        return;
    }
    if (Player::inventory[choice-1]->getNumPointers() > 1) {
        std::cout << "\nCan't toss equipped weapon\n\n";
        return;
    }
    delete Player::inventory[choice -1];
    Player::inventory[choice-1] = nullptr;
}
void Player::addSword(std::string weaponName, int weaponDamage, int weaponRange) {
    int inventoryCount = 0;
    for (int i =0; i < INVENTORY_SIZE; i++) {
        inventoryCount++;
        if (Player::inventory[i] == nullptr) {
            Player::inventory[i] = new Sword;
            Player::inventory[i]->setDamage(weaponDamage);
            Player::inventory[i]->setRange(weaponRange);
            Player::inventory[i]->setName(weaponName);
            Player::inventory[i]->addNumPointers();
            break;
        }
    }
    if (inventoryCount == INVENTORY_SIZE) {
        std::cout << "\nInventory full.\n";
    }
    return;

}
void Player::addBow(std::string weaponName, int weaponDamage, int weaponRange) {
    int inventoryCount = 0;
    for (int i =0; i < INVENTORY_SIZE; i++) {
        inventoryCount++;
        if (Player::inventory[i] == nullptr) {
            Player::inventory[i] = new Bow;
            Player::inventory[i]->setDamage(weaponDamage);
            Player::inventory[i]->setRange(weaponRange);
            Player::inventory[i]->setName(weaponName);
            Player::inventory[i]->addNumPointers();
            break;
        }
    }
    if (inventoryCount == INVENTORY_SIZE) {
        std::cout << "\nInventory full.\n";
    }
    return;

}
void Player::addStaff(std::string weaponName, int weaponDamage, int weaponRange) {
    int inventoryCount = 0;
    for (int i =0; i < INVENTORY_SIZE; i++) {
        inventoryCount++;
        if (Player::inventory[i] == nullptr) {
            Player::inventory[i] = new Staff;
            Player::inventory[i]->setDamage(weaponDamage);
            Player::inventory[i]->setRange(weaponRange);
            Player::inventory[i]->setName(weaponName);
            Player::inventory[i]->addNumPointers();
            break;
        }
    }
    if (inventoryCount == INVENTORY_SIZE) {
        std::cout << "\nInventory full.\n";
    }
    return;

}

void Player::setInventorySize () {
    this->inventory.reserve(INVENTORY_SIZE);
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        this->inventory[i] = nullptr;
    }
}

void Player::setCurrentHealth (int h) {
    this->currentHealth = h;
}

std::string Player::getEquippedWeapon() {
    if (equippedWeapon == nullptr) {

        return "none";
    }
    return equippedWeapon->getName();
}
int Player::getCurrentHealth () {
    return currentHealth;
}
int Player::getHealth () {
    return health;
}
int Player::getWeaponDamage () {
    if (equippedWeapon == nullptr) {
        return 1;
    }
    return equippedWeapon->getDamage();
}
int Player::getWeaponRange() {
    if (equippedWeapon == nullptr) {
        return 1;
    }
    return equippedWeapon->getRange();
}
bool Player::checkInventoryFull () {
    for (int i = 0; i < INVENTORY_SIZE; i++) {
        if (inventory[i] == nullptr) {
            //inventory is not full
            return false;
        }
    }
    //if control gets to the end of the inventory with there no being nullptr
    return true;
}


//Since weapons are a multiplier factor, strength, intell, dex are starting off at 1
Knight::Knight(int sp = 0, int h = 100, int s = 1, int d = 5, int e = 0, int i = 0, int dex = 0, int cH = 100) : Player::Player(sp, h, s, d, e, i, dex, cH) {}
Mage::Mage(int sp = 0, int h = 80, int s = 0, int d = 1, int e = 1, int i = 1, int dex = 0, int cH = 80) : Player::Player(sp, h, s, d, e, i, dex, cH) {}
Thief::Thief(int sp = 0, int h = 50, int s = 0, int d = 1, int e = 5, int i = 0, int dex = 1, int cH = 50) : Player::Player(sp, h, s, d, e, i, dex, cH) {}

//Weapon methods
int Sword::dmgMultiplier (int strength) {
    return this->damage *= strength;
}
int Staff::dmgMultiplier (int intelligence) {
    return this->damage *= intelligence;
}
int Bow::dmgMultiplier (int dexterity) {
    return this->damage *= dexterity;
}
void Weapon::setDamage (int weaponDamage) {
    this->damage = weaponDamage;
}
void Weapon::setRange (int weaponRange) {
    this->range = weaponRange;
}
void Weapon::setName (std::string weaponName) {
    this->name = weaponName;
}
std::string Weapon::getName () {
    return this->name;
}
int Weapon::getDamage () {
    return this->damage;
}
int Weapon::getRange () {
    return this->range;
}
std::string Weapon::getIdentifier () {
    return this->identifier;
}
void Weapon::addNumPointers () {
    this->numPointers++;
}
void Weapon::subtractNumPointers () {
    this->numPointers--;
}
int Weapon::getNumPointers () {
    return this->numPointers;
}


Sword::Sword(int dmg, int rng) {
    damage = dmg;
    range = rng;
    identifier = "sword";
};
Staff::Staff(int dmg, int rng) {
    damage = dmg;
    range = rng;
    identifier = "staff";
}
Bow::Bow(int dmg, int rng) {
    damage = dmg;
    range = rng;
    identifier = "bow";
}


//Default Weapon is a fist of damage 1
Sword::Sword () {
    damage = 1;
    range =1;
}
Staff::Staff () {
    damage = 1;
    range =1;
}

Bow::Bow () {
    damage = 1;
    range =1;
}

//Enemy methods
void Enemy::setDamage (int dmg) {
    this->damage = dmg;
}
void Enemy::setDistance (int d) {
    this->distance = d;
}
void Enemy::setHealth (int h) {
    this->health = h;
}
int Enemy::getDamage () {
    return this->damage;
}
int Enemy::getHealth () {
    return this->health;
}
int Enemy::getDistance() {
    return this->distance;
}
void Enemy::takeDamage(int dmg) {
    this->health -= dmg;
}
std::string Enemy::getName() {
    return this->name;
}
int Enemy::getLootDamage () {
    return loot->getDamage();
}
int Enemy::getLootRange () {
    return loot->getRange();
}
void Enemy::subtractDistance (int dist) {
    this->distance -= dist;
}

//Enemy constructors
Enemy::Enemy (int h, int dmg) {
    this->health = h;
    this->damage = dmg;
}
Enemy::~Enemy () {
    delete loot;
    loot = nullptr;
}
Goblin::Goblin (int h, int dmg) : Enemy::Enemy (h, dmg) {}
KingGoblin::KingGoblin(int h, int dmg, int rng) : Goblin::Goblin (h, dmg) {
    this->loot = new Sword (dmg, rng);
    this->name = "King Goblin";
}
ArcherGoblin::ArcherGoblin (int h, int dmg, int rng) : Goblin (h, dmg) {
    this->loot = new Bow(dmg, rng);
    this->name = "Archer Goblin";
}
MeleeGoblin::MeleeGoblin (int h, int dmg, int rng) : Goblin::Goblin (h, dmg) {
    this->loot = new Sword (dmg, rng);
    this->name = "Melee Goblin";
}



int main()
{
    std::srand(std::time(0));
    Player* p = nullptr;
    Enemy* enemy = nullptr;
    Weapon* loot = nullptr;
    int userInput, userInput2, randomEnemy;
    int combatCounter = 0;
    GameManager gm;

    //Enter game loop
    while (true) {
        std::cout << "Welcome\n\n"
              << "1. Start Game\n"
              << "2. Quit\n"
              << "\nWhat would you like to do?";
        std::cin >> userInput;
        switch (userInput) {
        case 1:
            //START PHASE 1: PICK A CLASS
            gm.startGame();
            gm.resumeGame();
            clearsrc();
            std::cout << "You wake and see a Staff, Sword and Bow lying on the floor of your house. What do you do?\n"
                      << "\n1. Pick up the Staff - Mage Class"
                      << "\n2. Pick up the Sword - Knight Class"
                      << "\n3. Pick up the Bow - Thief Class\n";
            userInput2 = getValidIntInput(1, 3);

            switch (userInput2) {
            case 1:
                //initialise mage class and declare inventory size and auto add weapon and equip
                p = new Mage();
                p->setInventorySize();
                p->addStaff("Basic Staff", 20, 10);
                p->equipWeapon(1);
                clearsrc();
                std::cout << "\nStaff Acquired!\n";
                break;
            case 2:
                p = new Knight();
                p->setInventorySize();
                p->addSword("Basic Sword", 25, 1);
                p->equipWeapon(1);
                clearsrc();
                std::cout << "\nSword Acquired!\n";
                break;
            case 3:
                p = new Thief();
                p->setInventorySize();
                p->addBow("Basic Bow", 50, 15);
                p->equipWeapon(1);
                clearsrc();
                std::cout << "\nBow Acquired!\n";
                break;
            }
            getEnter();
            //END PHASE 1

            //PHASE 2: GOING ON AN ADVENTURE
            clearsrc();
            std::cout << "\nDonning your armour, weapon, and satchel, you leave with aspirations of glory in lieu of comfort.\n";
            getEnter();

            //Enter gameplay section until GameManager::getEnd() is true
            do {
                clearsrc();
                displayNonCombatUI(p);
                std::cout << "\nWhat would you like to do: ";
                userInput2 = getValidIntInput(1, 4);

                //Combat choice
                switch (userInput2) {
                case 1:
                    std::cout << "\nMoving...\n";
                    //get a random enemy
                    randomEnemy = 1+ rand() % 3;
                    switch (randomEnemy) {
                    case 1:
                        //melee combat loop
                        CombatLoop(p, enemy, gm, combatCounter, "melee");
                        //after every combat loop check to see if the game ended by 1) player death, 2) fleeing king goblin, 3) killing king goblin
                        if (gm.getEnd() == true) {
                            break;
                        }
                        //item drop if enemy defeated
                        if (checkEnemyDead(enemy)) {
                            loot = lootDrop("Melee Goblin");
                            std::cout << "\n" << loot->getName() << " dropped!";
                            addToInventory(p,loot);
                            getEnter();
                            break;
                        }

                        break;


                    case 2:
                        //Ranged combat loop
                        CombatLoop(p, enemy, gm, combatCounter, "archer");
                        //after every combat loop check to see if the game ended by 1) player death, 2) fleeing king goblin, 3) killing king goblin
                        if (gm.getEnd() == true) {
                            break;
                        }
                        if (checkEnemyDead(enemy)) {
                            loot = lootDrop("Archer Goblin");
                            std::cout << "\n" << loot->getName() << " dropped!";
                            addToInventory(p,loot);
                            getEnter();
                            break;
                        }

                        break;

                    case 3:
                        std::cout << "\nNo enemies encountered.\n";
                        getEnter();
                        break;
                    }
                    break;

                //Rest choice
                case 2:
                    p->setCurrentHealth(p->getHealth());
                    std::cout << "\nFully rested!\n";
                    getEnter();
                    break;

                //Inventory choice
                case 3:
                    clearsrc();
                    p->viewInventory();
                    std::cout << "\n\nWhat would you like to do?\n\n"
                              << "1. Equip Item\n"
                              << "2. Throw Item\n"
                              << "3. Back to menu\n";
                    equipOrThrowFromInventory(p);
                    break;

                //Weapon and Player stats choice
                case 4:
                    clearsrc();
                    p->viewStats();
                    getEnter();
                    break;

                }
            } while (gm.getEnd() == false);

            break;

        //QUIT GAME
        case 2:
            std::cout << "\nExiting Game\n";
            break;
        default:
            std::cout << "\nEnter a valid input\n";
            break;
        }

        //Quit game
        if (userInput = 2) {
            break;
        }

    }

        //reached the end as a coward (fled from final fight)
    if (gm.getEnd() == true && p->getCurrentHealth() > 0 && enemy->getHealth() > 0) {
        clearsrc();
        std::cout << "Scampering away with your tail between your legs, you live to fight another day.\n"
                  << "You live in fear, no longer daring to leave the confines of your home, sequestering yourself to a corner of your room\n"
                  << "atrophying until you finally perish -- an apt end for a poltroon who posed as an adventurer.\n";
      getEnter();
    }
        //Reached end as a perished warrior (died by any enemy)
    if (gm.getEnd() == true && p->getCurrentHealth() <= 0 && enemy->getHealth() > 0) {
        clearsrc();
        std::cout << "Brave warrior, your efforts are and will continue to be recognised for posterity.\n"
                  << "You have lived dauntlessly; be proud, for your life of sincerity and earnestness has brought you peace in death.\n"
                  << "Fear not, brave one, a man of you character need not fear what is to come.\n";
        getEnter();
    }
        //Reached end as a victor (killed king goblin)
    if (gm.getEnd() == true && p->getCurrentHealth() > 0 && enemy->getHealth() <= 0) {
        clearsrc();
        std::cout << "Stand proud, champion.\n"
                  << "Your victory shall be celebrated for millennia; your offspring bountiful, and your bloodline imperishable.\n"
                  << "You shall carry out the rest of your days and thereafter a legend in the annals of this world.\n";
        getEnter();
    }

    //clean up memory
    delete enemy;
    delete p;
    delete loot;

    std::cout << "\n\nThanks for playing\n\n";

    return 0;
}


//COMPLETED 23.07.2025//
//Learnt how to do an inventory management system, accouting for auto-pick of items when
//a space is free vs when inventory is full
//Implemented custom constructors and destructors for dynamic memory allocation
//Implemented a main menu page that is accessible whenever not in combat
//Devised a combat turn based system
//Implemented endings based on player decision in the last fight
