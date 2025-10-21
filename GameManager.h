#ifndef GAMEMANAGER_H_INCLUDED
#define GAMEMANAGER_H_INCLUDED

class Player; //fwd decl.

class GameManager {
protected:
    bool paused;
    bool ended;
    bool playerTurn;

public:
    void startGame();
    void endGame();
    void pauseGame();
    void resumeGame();
    void setPlayerTurn(bool);
    bool getPlayerTurn();
    bool getPause();
    bool getEnd ();


};


#endif // GAMEMANAGER_H_INCLUDED

