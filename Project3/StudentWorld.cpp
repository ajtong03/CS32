#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_Peach = nullptr;
    m_levelComp = false;
    m_gameComp = false;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
   
    m_levelComp = false;
    m_gameComp = false;
    Level lev(assetPath());
    string level_file = loadLevel(); //call loadLevel function to generate level file string
    Level::LoadResult result = lev.loadLevel(level_file);
    
    Level::GridEntry ge;
    if(result != Level::load_success)
        return GWSTATUS_LEVEL_ERROR;
    
    //go through level file and insert characters at appropriate positions
    for(int i = 0; i < 32; i++)
    {
        for(int j = 0; j < 32; j++)
        {
            ge = lev.getContentsOf(i, j);
            int x = i * SPRITE_WIDTH;
            int y = j * SPRITE_WIDTH;
            switch (ge)
            {
                case Level::peach:
                {
                    m_Peach = new Peach(x, y, this);
                    break;
                }
                case Level::block:
                {
                    Block* b = new Block(x, y, this);
                    addActor(b);
                    break;
                }
               case Level::pipe:
                {
                    Pipe* p = new Pipe(x, y, this);
                    addActor(p);
                    break;
                }
                case Level::flower_goodie_block:
                {
                    FlowBlock* b = new FlowBlock(x, y, this);
                    addActor(b);
                    break;
                }
                case Level::mushroom_goodie_block:
                {
                    MushBlock* b = new MushBlock(x, y, this);
                    addActor(b);
                    break;
                }
                case Level::star_goodie_block:
                {
                    StarBlock* b = new StarBlock(x, y, this);
                    addActor(b);
                    break;
                }
                case Level::flag:
                {
                    Flag* f = new Flag(x, y, this);
                    addActor(f);
                    break;
                }
                case Level::mario:
                {
                    Mario* m = new Mario(x, y, this);
                    addActor(m);
                    break;
                }
                case Level::koopa:
                {
                    Koopa* k = new Koopa(x, y, this);
                    addActor(k);
                    break;
                }
                case Level::goomba:
                {
                    Goomba* g = new Goomba(x, y, this);
                    addActor(g);
                    break;
                }
                case Level::piranha:
                {
                    Piranha* p = new Piranha(x, y, this);
                    addActor(p);
                    break;
                }
                case Level::empty:
                {
                    break;
                }
            }
        }
    }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    m_Peach->doSomething(); //call Peach's doSomething() function
    
    vector<Actor*>::iterator i = m_actors.begin(); //iterator variable to help iterate through all the Actors
    size_t j = m_actors.size(); //variable to hold the number of current Actors in the world //ISSUE - need size_t or can use int?
    
    int k = 0; //variable to keep track of how many actors we have passed through
    for(; i != m_actors.end() && k < j; i++) //don't call doSomething for any newly added actors; k < j since the size of the vectors changes if new actors are created and added
    {
        if((*i)->isAlive()) //if the Actor the iterator is pointing to is alive, it performs an action by calling its doSomething function
        {
            (*i)->doSomething();
        }
        k++; //increment k
    }
    
    if(!(m_Peach->isAlive())) //if Peach isn't alive, play player died sound and return that the player has died
    {
        playSound(SOUND_PLAYER_DIE);
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    else if(m_levelComp) //if m_levelComp is true, the level has been passed and finished level sound plays. return that the level is finished
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    else if(m_gameComp) //if m_gameComp is true, the game has been successfully finished and game over sound plays. return that the player has won
    {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }
    
    deleteActors(); //call function to delete all dead actors from the game
    
    setGameStatText(displayStatus()); //display the game stats
    
    return GWSTATUS_CONTINUE_GAME; //game continues
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator i = m_actors.begin(); //iterator variable to iterate through all actors in the world
    while(i != m_actors.end()) //loop until end of vector m_actors reached
    {
        delete *i; //delete the Actor that is pointed to by the pointer that the iterator is pointing to
        i = m_actors.erase(i); //delete the element that i points to. i now points to element after the one just deleted
    }
    delete m_Peach; //delete Peach pointer
}

void StudentWorld::addActor(Actor* x) //function that adds a pointer to an actor to the vector m_actors
{
    m_actors.push_back(x); //add x to the end of the vector
}


bool StudentWorld::overlaps(int x, int y, int action) //checks to see if there is anything overlapping at coordinates x and y. Third parameter is to determine if any action should be taken if there is overlap (1 = bonk, 2 = damage)
{
    for(vector<Actor*>::iterator i = m_actors.begin(); i != m_actors.end(); i++) //iterate through m_actors using iterator
    {
        //variables to hold coordinates of the actor currently being visited
        int aX = (*i)->getX();
        int aY = (*i)->getY();
        
        if(checkBounds(aX, aY, x, y)) //call checkBounds on the actor being visited and parameters passed; if no overlap, function returns false
        {
            //execute if there is overlap
            
            if(action == ACTION_DAMAGE && (*i)->damageable()) //check if action is to damage and the actor being visited can be damaged
            {
                (*i)->damage(); //if actor can be damaged, call that actor's damage function
                return true; //return true (action has been properly taken)
            }
            if((*i)->hinderMvmt()) //check whether the Actor can block others
            {
                if(action == ACTION_BONK) //check if action is to bonk
                    (*i)->bonk(); //if actor can block others and is to be bonked, call the actor's bonk function
                return true; //return true (action has been properly taken)
            }
            
        }
    }
    return false; //return false if no action has been taken or if there was no overlap
}

//function to return a pointer to Peach
Peach* StudentWorld::getPeach() const
{
    return m_Peach;
}

bool StudentWorld::peachOverlap(Actor* a) //function to check if an Actor overlaps with Peach
{
    return checkBounds(m_Peach->getX(), m_Peach->getY(), a->getX(), a->getY()); //return whether there is overlap between Peach's coordinates and the coordinates Actor pointed to by a
}

bool StudentWorld::peachBonk(Actor* a) //function to bonk Peach if there is overlap with another Actor
{
    if(peachOverlap(a)) //check if Peach overlaps with Actor pointed to by a
    {
        if(m_Peach->hasStar()) //if Peach has Star power, the actor should be bonked
        {
            a->bonk();
            return false;
        }
        m_Peach->bonk(); //if there is overlap, call Peach's bonk function
        return true; //return true that Peach has been bonked
    }
    return false; //return false that there is no overlap and Peach hasn't been bonked
}

void StudentWorld::setLevComp(bool level) //function to set m_levelComp to level
{
    m_levelComp = level;
}

void StudentWorld::setGameComp(bool game) //function to set m_gameComp to game
{
    m_gameComp = game;
}

void StudentWorld::deleteActors() //function to delete dead actors from game
{
    vector<Actor*>::iterator i = m_actors.begin(); //iterator variable to iterate through all Actors
    while(i != m_actors.end()) //loop until end of m_actors
    {
        if(!(*i)->isAlive()) //check if Actor is dead
        {
            delete *i; //if Actor is dead, delete the Actor
            i = m_actors.erase(i); //delete the pointer being pointed to by i. i now points to the following element after the one just deleted
        }
        else
            i++; //if actor is alive, i now points to the following element
    }
}

//helper method
bool StudentWorld::checkBounds(int x1, int y1, int x2, int y2) //helper function to check whether there is overlap between two sets of coordinates
{
    bool xOver; //variable to hold whether there is overlap in x direction
    bool yOver; //variable to hold whether there is overlap in y direction
    
    //variables to hold the x coordinate that Actors extend to
    int x1R = x1 + SPRITE_WIDTH - 1;
    int x2R = x2 + SPRITE_WIDTH - 1;
    
    //variables to hold the y coordinate that Actors extend to
    int y1T = y1 + SPRITE_HEIGHT - 1;
    int y2T = y2 + SPRITE_HEIGHT - 1;
    
    //xOver = ((x2 <= x1R && x2 >= x1) || (x1R <= x2R && x1R >= x2) || (x1 >= x2 && x1 <= x2R) || (x2R >= x1 && x2R <= x1R));
    xOver = ((x2 <= x1R && x1R <= x2R) || (x1 >= x2 && x1 <= x2R)); //check for overlap between the x values
    //yOver = ((y2 <= y1T && y2 >= y1) || (y1T <= y2T && y1T >= y2) || (y1 >= y2 && y1 <= y2T) || (y2T >= y1 && y2T <= y1T));
    yOver = ((y2 <= y1T && y2 >= y1) || (y1 <= y2T && y1 >= y2)); //check for overlap between the y values
    
    return xOver && yOver; //return whether there is any overlap between either x values, y values, or both
}

string StudentWorld::loadLevel() //helper function to load level text
{
    ostringstream oss;
    oss << "level";
    oss.fill('0');
    oss << setw(2) << getLevel(); //a level can be 1 or 2 digits, if it's 1, it should be preceded by 0
    string s = oss.str() + ".txt"; //every level text ends with .txt
    return s;
}

string StudentWorld::displayStatus() //helper function to create string that displays game stats
{
    ostringstream oss;
    oss << "Lives: " << getLives();
    oss << setw(9) << "Level: "; //preceded by two spaces
    oss.fill('0');
    oss << setw(2) << getLevel(); //a level can be 1 or 2 digits, if it's 1, it should be preceded by 0
    oss.fill(' ');
    oss << setw(10) << "Points: "; //preceded by two spaces
    oss.fill('0');
    oss << setw(6) << getScore(); //a score can be up to 6 digits, if it's <6 digits, it should be preceded by 0s
    
    //all powerups are preceded by one space
    oss.fill(' ');
    if(m_Peach->hasStar())
        oss << setw(11) << "StarPower!";
    if(m_Peach->hasShoot())
        oss << setw(12) << "ShootPower!";
    if(m_Peach->hasJump())
        oss << setw(11) << "JumpPower!";
    string s = oss.str();
    return s;
    
}

