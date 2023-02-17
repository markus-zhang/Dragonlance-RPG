#pragma once

class cEntity;
class cGameScene;
class cMap;
class cPlayer;

/*
* - AIController: This controller loops through all non-player entities:
      - It has access to the internal data of each entity
      - It *thinks* for the entity:
        - Has it seen the player?
        - What is its next move?
        - If it moves, how does it move towards the player?
        - If it fights, which weapon does it wield?
        - etc.
      - AIController also has a movement controll part as you can see from above
*
*/

class cAIController {
public:
    void markSeenPlayer(cEntity* e);
    void unmarkSeenPlayer(cEntity* e);
    void think(cEntity* e, cGameScene* s);
public:
    void moveRandomly(cEntity* e, cMap* m);
    void moveEntity(cEntity* e, int dir, cMap* m);
    bool canMoveDirection(cEntity* e, int dir, cMap* m);
    void chasePlayer(cEntity* e, cPlayer* p);
    bool inSameMap(cEntity* e, cPlayer* p);
};