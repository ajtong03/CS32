#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>

class Actor;
class Peach;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    
    //function that takes in a pointer to an actor and adds actor to the game
    void addActor(Actor* x);
    
    //function that checks whether there is an Actor that overlaps with the x and y parameters passed and whether action was taken
    //the third parameter action indicates whether action should be taken if an Actor overlaps with the x and y parameters
    //if 1 is passed, it indicates that the Actor should be bonked
    //if 2 is passed, it indicates that the Actor should be destroyed
    //if any other int is passed, no action is taken
    bool overlaps(int x, int y, int action);
    
    //function that sets m_levelComp to level; m_levelComp keeps track of whether the current level has been completed or not
    void setLevComp(bool level);
    
    //function that sets m_gameComp to game; m_gameComp keeps track of whether the game has been successfully completed
    void setGameComp(bool game);
    
    //function that returns pointer to Peach; allows actors to have access to her
    Peach* getPeach() const;
    
    //function that checks whether an actor overlaps with Peach, and if so, bonks Peach
    bool peachBonk(Actor* a);
    
    //function that checks whether an actor overlaps with Peach
    bool peachOverlap(Actor* a);
    

private:
    std::vector<Actor*> m_actors;
    Peach* m_Peach;
    
    //private member variable to keep track of whether the current level has been successfully completed or not
    bool m_levelComp;
    
    //private member variable to keep track of whether the game has been successfully completed or not
    bool m_gameComp;
    
    //private helper function to check whether there is overlap between two sets of coordinates
    bool checkBounds(int x1, int y1, int x2, int y2);
    
    //private helper function to create the text for loading the level of the game
    std::string loadLevel();
    
    //private helper function to create the text for displaying the game stats at the top of the screen
    std::string displayStatus();
    
    //private helper function to delete all dead actors from the game
    void deleteActors();
};

#endif // STUDENTWORLD_H_
