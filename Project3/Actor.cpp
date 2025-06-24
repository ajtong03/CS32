#include "Actor.h"
#include "GameConstants.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

//------ACTOR IMPLEMENTATIONS------
Actor::Actor(int ID, int x, int y, int dir, int depth, double size, StudentWorld* w, bool hinder, bool damage)
:GraphObject(ID, x, y, dir, depth, size), m_alive(true), m_world(w), m_hinder(hinder), m_damageable(damage)
{
}

Actor::~Actor()
{
}

bool Actor::isAlive() const //returns whether an actor is alive
{
    return m_alive;
}

StudentWorld* Actor::getWorld() //return a pointer to the world
{
    return m_world;
}

bool Actor::hinderMvmt() const //accessor method determining if actor blocks other actors' movement
{
    return m_hinder;
}

void Actor::dead() //set an actor's life state to dead
{
    m_alive = false;
}

void Actor::bonk() //actors that cannot be bonked call Actor's bonk function which does nothing
{
    return;
}

bool Actor::damageable() const //returns whether an actor can be damaged
{
    return m_damageable;
}

void Actor::damage() //actors that cannot be damaged call Actor's damage function which does nothing
{
    return;
}

int Actor::movement()
{
    StudentWorld* w = getWorld();
    int x = getX();
    int y = getY();
    
    if(!w->overlaps(x, y - 2, NO_ACTION)) //check if actor overlaps with anything 2 pixels below
    {
        moveTo(x, y - 2);//if not, move down 2 pixels
        y = getY();
    }
    if(getDirection() == 0) //check if actor is facing right
    {
        if(w->overlaps(x + 2, y, NO_ACTION)) //if actor overlaps with an object two pixels to the right, return 180 as the actor may need to turn around
            return 180;
        else
        {
            moveTo(x + 2, y); //if actor doesn't overlap with anything two pixels to the right, move to that position and return -1 indicating that no further movement/change is necessary
            return -1;
        }
    }
    else
    {
        if(w->overlaps(x - 2, y, NO_ACTION))//if actor overlaps with an object two pixels to the left, return 0 as the actor may need to turn around
            return 0;
        else
        {
            moveTo(x - 2, y); //if actor doesn't overlap with anything two pixels to the left, move to that position and return -1 indicating that no further movement/change is necessary
            return -1;
        }
    }
}

//------PEACH IMPLEMENTATIONS------
Peach::Peach(int x, int y, StudentWorld* w)
:Actor(IID_PEACH, x, y, 0, 0, 1.0, w, false, true), m_hit(1), m_invincible(0), m_remJumpDist(0), m_recharge(0), m_tempInvinc(0), m_star(false), m_shoot(false), m_jump(false)
{
    
}

Peach::~Peach()
{
}

void Peach::updateHit(int amt) //mutator method to update Peach's health points
{
    m_hit = amt;
}


void Peach::bonk()
{
    if(hasStar() || m_tempInvinc > 0) //if Peach is invincible or temporarily invincinble, nothing happens to her
    {
        
        return;
    }
    m_hit--; //decrease Peach's health points
    m_tempInvinc = 10; //Peach becomes temporarily invincible after being bonked
    
    //turn off Peach's powers, except Star power
    if(hasShoot())
        m_shoot = false;
    if(hasJump())
        m_jump = false;

    if(m_hit >= 1)
    {
        getWorld()->playSound(SOUND_PLAYER_HURT); //play hurt sound if Peach still has health points
    }
    else
    {
        dead(); //Peach has died if she has no health points
    }
}

void Peach::doSomething()
{
    if(!isAlive()) //Peach must be alive for the rest of this function to execute
        return;
    
    if(m_invincible > 0) //if Peach is invincible, decrease the number of ticks left until she loses invincibility
        m_invincible--;
    else
    {
        //getWorld()->updateStar(false); //if Peach has run out of invincibility, turn Star power off
        m_star = false;
    }
    if(m_recharge > 0) //if Peach is in recharge mode and cannot use shoot power, decrement ticks until she can
        m_recharge--;
    if(m_tempInvinc > 0) //if Peach is temporarily invincible, decrease ticks until she looses temporary invincibility
        m_tempInvinc--;
    
    //variables to hold Peach's current coordinates
    int x = getX();
    int y = getY();
    
    getWorld()->overlaps(x, y, ACTION_BONK); //test to see if she overlaps and if so, bonk
    
    if(m_remJumpDist > 0) //Peach has initiated jump
    {
        bool a = getWorld()->overlaps(x, y + 4, ACTION_BONK); //see if overlap @ targeted destination and if so, bonk
        if(a == 0) //no overlap @ targeted destination
        {
            moveTo(x, y + 4); //update location
            m_remJumpDist--; //decrement remaining jump distance
        }
        else
        {
            m_remJumpDist = 0; //abort jump
        }
    }
    else //no jump initiated => check if falling
    {
        int i = 0;
        for(; i <= 3; i++) //check if the 3 pixels below Peach are blocked
        {
            if(getWorld()->overlaps(x, y - i, NO_ACTION)) //0 as third parameter->don't want to bonk object below
            {
                i = -1;
                break;
            }
        }
        if(i != -1)
            moveTo(x, y - 4);
    }
    int ch;
    if (getWorld()->getKey(ch))
    {
    // user hit a key during this tick!
        switch (ch)
        {
            case KEY_PRESS_LEFT: //player wants Peach to move left
            {
                if(!getWorld()->overlaps(x - 4, y, ACTION_BONK)) //check if overlap. if so, bonk the object. if not, face left and move 4 pixels to the left
                {
                    setDirection(180);
                    moveTo(x - 4, y);
                }
                break;
            }
            case KEY_PRESS_RIGHT: //player wants Peach to move right
            {
                
                if(!getWorld()->overlaps(x + 4, y, ACTION_BONK)) //check if overlap. if so, bonk the object. if not, face right and move 4 pixels right
                {
                    setDirection(0);
                    moveTo(x + 4, y);
                }
                break;
            }
            case KEY_PRESS_UP: //player wants Peach to jump
            {
                if(getWorld()->overlaps(x, y - 1, NO_ACTION))//check to see if there is anything below her to support a jump
                {
                   if(hasJump()) //if Peach has jump power, set remaining jump distance to 12
                       m_remJumpDist = 12;
                    else //if Peach has no jump power, set remaining jump distance to 12
                        m_remJumpDist = 8;
                    getWorld()->playSound(SOUND_PLAYER_JUMP); //play Peach's jumping sound
                }
                break;
            }
            case KEY_PRESS_SPACE: //player wants Peach to shoot
            {
                if(m_recharge == 0 && hasShoot()) //check that Peach is not under recharge and has shoot power
                {
                    getWorld()->playSound(SOUND_PLAYER_FIRE); //play Peach's firing sound
                    m_recharge = 8; //set Peach's recharge to 8, she must wait 8 ticks before she can fire again
                    getWorld()->addActor(new PeachFire(x, y, getDirection(), getWorld())); //introduce fire ball at Peach's current location facing same direction as her
                }
                break;
            }
        }
    }
}

bool Peach::hasJump() const //return whether Peach has jump power
{
    return m_jump;
}

bool Peach::hasStar() const //return whether Peach has star power (invincibility)
{
    return m_star;
}

bool Peach::hasShoot() const //return whether Peach has shoot power
{
    return m_shoot;
}

void Peach::setJump(bool jump) //set Peach's jump member variable to jump
{
    m_jump = jump;
}

void Peach::setStar(bool star) //set Peach's star member variable to star; give her invincibility for 150 ticks
{
    m_star = star;
    if(m_star)
        m_invincible = 150;
}

void Peach::setShoot(bool shoot) //set Peach's shoot member variable to shoot
{
    m_shoot = shoot;
}

void Peach::damage()
{
    bonk(); //call Peach's bonk method since she reacts the same when damaged
}

//------STATIONARY IMPLEMENTATIONS------ (Blocks and Pipes)
Stationary::Stationary(int ID, int x, int y, StudentWorld* w)
: Actor(ID, x, y, 0, 2, 1, w, true, false)
{
}

Stationary::~Stationary()
{
}

void Stationary::doSomething() //Stationary objects do nothing during each tick
{
    return;
}

//------BLOCK IMPLEMENTATIONS------
Block::Block(int x, int y, StudentWorld* w)
:Stationary(IID_BLOCK, x, y, w)
{
}

Block::~Block()
{
}

void Block::bonk() 
{
    getWorld()->playSound(SOUND_PLAYER_BONK);//play bonk sound when bonked
}


//------PIPE IMPLEMENTATIONS------
Pipe::Pipe(int x, int y, StudentWorld* w)
:Stationary(IID_PIPE, x, y, w)
{
}

Pipe::~Pipe()
{
}


//------CHARACTER IMPLEMENTATIONS------
Character::Character(int id, int x, int y, int dir, int depth, double size, StudentWorld* w, int points)
:Actor(id, x, y, dir, depth, size, w, false, false), m_points(points)
{
    
}

Character::~Character()
{
    
}

int Character::getPoints() const
{
    return m_points; //return how many points a character is worth
}

void Character::doSomething()
{
    if(isAlive()) //character must be alive to do something
    {
        StudentWorld* w = getWorld(); //variable to hold pointer to world
        if(w->peachOverlap(this)) //check if character overaps with Peach
        {
            w->increaseScore(getPoints()); //if overlap, increase score by character's points
            dead(); //character is now dead
            
        }
        indivAct(); //call each character's individual act as they do differing things
    }
    
}

//------FINISHED IMPLEMENTATIONS------ (when a level or game is completed)
Finished::Finished(int id, int x, int y, StudentWorld* w)
:Character(id, x, y, 0, 1, 1, w, 1000)
{
    
}

Finished::~Finished()
{
}

void Finished::indivAct()
{
    if(getWorld()->peachOverlap(this)) //check if Finished object overlaps with Peach
        notify(); //if overlaps, call Finished object's notify function
}


//------FLAG IMPLEMENTATIONS------
Flag::Flag(int x, int y, StudentWorld* w)
:Finished(IID_FLAG, x, y, w)
{
}

Flag::~Flag()
{
}

void Flag::notify() //call StudentWorld's setLevComp function to notify that the level has been completed
{
    getWorld()->setLevComp(true);
}

int Flag::getSoundHit()
{
    return SOUND_FINISHED_LEVEL;
}


//------MARIO------
Mario::Mario(int x, int y, StudentWorld* w)
:Finished(IID_MARIO, x, y, w)
{
}

Mario::~Mario()
{
}

void Mario::notify() //call StudentWorld's setLevComp function to notify that the game has been completed
{
    getWorld()->setGameComp(true);
}

int Mario::getSoundHit()
{
    return SOUND_GAME_OVER;
}


//------GOODIES------
Goodies::Goodies(int id, int x, int y, StudentWorld* w, int points)
:Character(id, x, y, 0, 1, 1, w, points)
{
}

Goodies::~Goodies()
{
    
}

void Goodies::indivAct()
{
    //variables to hold pointer to world and goodies' x and y coordinates
    StudentWorld* w = getWorld();
    
    if(w->peachOverlap(this)) //check if goodie overlaps with Peach
    {
        setPow();  //set goodie power to true
        w->getPeach()->updateHit(2); //Peach gains 2 health points
        w->playSound(getSoundHit()); //play sound for when goodie hit
        return;
    }
    
    int changeDir = Actor::movement(); //call actor's movement() function which will either return the direction a goodie needs to turn to or -1 if nothing is blocking the goodie
    
    if(changeDir == -1) //goodie was able to move in its current direction so function terminates
        return;
    else if(changeDir == 0) //goodie ran into a wall on the left and needs to turn to face the right
        setDirection(0);
    else
        setDirection(180); //goodie ran into a wall on the right and needs to turn to face the lect
}

int Goodies::getSoundHit()
{
    return SOUND_PLAYER_POWERUP; //return sound that goodie plays when hit
}


//------FLOWER------
Flower::Flower(int x, int y, StudentWorld* w)
:Goodies(IID_FLOWER, x, y, w, 50)
{
}

Flower::~Flower()
{
}

void Flower::setPow() //update shoot power to pow
{
    getWorld()->getPeach()->setShoot(true);
}


//------MUSHROOM------
Mushroom::Mushroom(int x, int y, StudentWorld* w)
:Goodies(IID_MUSHROOM, x, y, w, 75)
{
}

Mushroom::~Mushroom()
{
}

void Mushroom::setPow() //update jump power to pow
{
    getWorld()->getPeach()->setJump(true);
}


//------STAR------
Star::Star(int x, int y, StudentWorld* w)
:Goodies(IID_STAR, x, y, w, 100)
{
}

Star::~Star()
{
}

void Star::setPow() //update star power to pow
{
    getWorld()->getPeach()->setStar(true);
}


//------ENEMIES------
Enemies::Enemies(int id, int x, int y, StudentWorld* w)
:Actor(id, x, y, 180, 0, 1, w, false, true)
{
    
}

Enemies::~Enemies()
{
}

void Enemies::doSomething()
{
    if(!isAlive()) //if enemy object isn't alive, do nothing
        return;
    
    StudentWorld* w = getWorld(); //variable to hold pointer to world
    
    if(w->peachBonk(this)) //if enemy overlaps with Peach, bonk her
    {
        dead(); //enemy is now dead and nothing function terminates
        return;
    }
    
    //variables to hold x and y coordinates of enemy
    int x = getX();
    int y =getY();
    
    
    if(getDirection() == 0) //check if enemy is facing right
    {
        if(w->overlaps(x + 1, y, NO_ACTION)) //if enemy can't move one pixel to the right without hitting something, turn around
            setDirection(180);
        else if(!w->overlaps(x +(SPRITE_WIDTH), y - 1, NO_ACTION)) //if the enemy can't move to the right without going partially off a block, turn around
        {
            setDirection(180);
        }
    }
    else
    {
        if(w->overlaps(x - 1, y, NO_ACTION))//if enemy can't move one pixel to the left without hitting something, turn around
        {
            setDirection(0);
        }
        else if(!w->overlaps(x - (SPRITE_WIDTH), y - 1, NO_ACTION))//if the enemy can't move to the left without going partially off a block, turn around
        {
            setDirection(0);
        }
    }
    
    if(getDirection() == 0) //check if direction is to the right
    {
        if(!w->overlaps(x + 1, y, NO_ACTION)) //if enemy can move one pixel to the right, move. If not, the function terminates
            moveTo(x + 1, y);
        else
            return;
    }
    else
    {
        if(!w->overlaps(x - 1, y, NO_ACTION))//if enemy can move one pixel to the left, move. If not, the function terminates
            moveTo(x - 1, y);
        else
            return;
    }
}

void Enemies::bonk()
{
    StudentWorld* w = getWorld(); //variable to hold pointer to world
    if(!(w->peachOverlap(this))) //check if Peach overlaps with enemy. If not, function terminates
        return;
    if(w->getPeach()->hasStar()) //check if Peach has star power, if not, nothing happens
    {
        w->playSound(SOUND_PLAYER_KICK); //play sound
        w->increaseScore(100); //increase score by 100
        dead(); //enemy is now dead
    }
}

void Enemies::damage()
{
    getWorld()->increaseScore(100); //increase score by 100
    dead(); //call respective enemy's dead function
}

//------GOOMBA IMPLEMENTATIONS------
Goomba::Goomba(int x, int y, StudentWorld* w)
:Enemies(IID_GOOMBA, x, y, w)
{
}

Goomba::~Goomba()
{
}

//------KOOPA IMPLEMENTATIONS------
Koopa::Koopa(int x, int y, StudentWorld* w)
:Enemies(IID_KOOPA, x, y, w)
{
}

Koopa::~Koopa()
{
}


void Koopa::dead() //Koopa has different dead() function from other enemies since Koopas take additional action
{
    getWorld()->addActor(new Shell(getX(), getY(), getDirection(), getWorld())); //add a Shell to the world at same position and direction as Koopa
    Actor::dead(); //call Actor's dead method; enemy is now dead
}

//------PIRANHA IMPLEMENTATIONS------
Piranha::Piranha(int x, int y, StudentWorld* w)
:Enemies(IID_PIRANHA, x, y, w), m_delay(0)
{
}

Piranha::~Piranha()
{
}

void Piranha::doSomething()
{
    if(!isAlive()) //if piranha is dead, no action taken
        return;
    
    increaseAnimationNumber();
    
    StudentWorld* w = getWorld(); //create variable to hold pointer to the world
   
    if(w->peachBonk(this)) //check if piranha overlaps with peach and if so, peach's bonk is called; return since no further action taken
    {
        return;
    }
    
    //variable to hold Peach's coordinates
    int pX = w->getPeach()->getX();
    int pY = w->getPeach()->getY();
    
    //variable to hold Actor's coordinates
    int x = getX();
    int y = getY();
    if(!(pY > y - (1.5 * SPRITE_HEIGHT) && pY < y + (1.5 * SPRITE_HEIGHT)))//w->peachInYRange(this)) //if Peach isn't on the same level as piranha, return since no further action taken
    {
        return;
    }
    else //peach is on same level as piranha
    {
        if(pX < x) //check if Peach is to the left of the Actor and if so, make the Actor face left
           setDirection(180);
        else
            setDirection(0); //if Peach is to the right of the Actor, make the Actor face right
        
        if(m_delay > 0) //if piranha still has shooting delay, decrement the number of ticks until can shoot again, return since no further action taken
        {
            m_delay--;
            return;
        }
        else
        {
            
            if(abs(x - pX) < (8 * SPRITE_WIDTH)) //check to see if Peach is in shootable range
            {
                w->addActor(new PiranFire(getX(), getY(), getDirection(), w)); //add a piranha fireball to the world
                w->playSound(SOUND_PIRANHA_FIRE); //play sound
                m_delay = 40; //set delay to 40 so piranha can't shoot for the next 40 ticks
            }
        }
    }
}

//------GOODIEBLOCK------
GoodieBlock::GoodieBlock(int x, int y, StudentWorld* w)
:Block(x, y, w), active(true)
{
}

GoodieBlock::~GoodieBlock()
{
}

void GoodieBlock::bonk()
{
    if(active) //check if the goodie block has released its goodie yet
    {
        //goodie hasn't been released yet
        getWorld()->playSound(SOUND_POWERUP_APPEARS); //play powerup sound
        addGoodie(); //a goodie is added to the game
        active = false; //the goodie block now holds no goodie
    }
    else
    {
        Block::bonk(); //goodie has been released and the goodie block now acts as a normal block
    }
}
//------FLOWBLOCK------
FlowBlock::FlowBlock(int x, int y, StudentWorld* w)
:GoodieBlock(x, y, w)
{
}

FlowBlock::~FlowBlock()
{
}

void FlowBlock::addGoodie()
{
    getWorld()->addActor(new Flower(getX(), getY() + 8, getWorld())); //adds a Flower to the game
}


//------MUSHBLOCK IMPLEMENTATIONS------
MushBlock::MushBlock(int x, int y, StudentWorld* w)
:GoodieBlock(x, y, w)
{
}

MushBlock::~MushBlock()
{
}

void MushBlock::addGoodie()
{
    getWorld()->addActor(new Mushroom(getX(), getY() + 8, getWorld())); //adds a Mushroom to the game
}

//------STARBLOCK IMPLEMENTATIONS------
StarBlock::StarBlock(int x, int y, StudentWorld* w)
:GoodieBlock(x, y, w)
{
}

StarBlock::~StarBlock()
{
}

void StarBlock::addGoodie()
{
    getWorld()->addActor(new Star(getX(), getY() + 8, getWorld())); //adds a Star to the game
}

//------PROJECTILE IMPLEMENTATIONS------
Projectile::Projectile(int id, int x, int y, int dir, StudentWorld* w)
:Actor(id, x, y, dir, 1, 1, w, false, false)
{
}

Projectile::~Projectile()
{
}

void Projectile::doSomething()
{    
    if(damageActor()) //attempt to damage an actor and check if successful
    {
        dead(); //if successful, the projectile is now dead and function terminates
        return;
    }
    else
    {
        if(Actor::movement() == -1) //call Actor's movement; -1 indicates that the projectile is able to move in the needed direction so the function terminates
            return;
        dead(); //projectile has run into something that blocks it. Projectile is now dead and function terminates
        return;
    }
}


//------PIRANFIRE IMPLEMENTATIONS------
PiranFire::PiranFire(int x, int y, int dir, StudentWorld* w)
:Projectile(IID_PIRANHA_FIRE, x, y, dir, w)
{
}

PiranFire::~PiranFire()
{
    
}

bool PiranFire::damageActor()
{
    if(getWorld()->peachOverlap(this)) //check to see if PiranFire overlaps with Peach
    {
        getWorld()->getPeach()->damage(); //if overlap, damage Peach and return true
        return true;
    }
    return false;
}

//PEACHPROJ IMPLEMENTATIONS
PeachProj::PeachProj(int id, int x, int y, int dir, StudentWorld* w)
:Projectile(id, x, y, dir, w)
{
}

PeachProj::~PeachProj()
{
}

bool PeachProj::damageActor()
{
    if(getWorld()->overlaps(getX(), getY(), ACTION_DAMAGE)) //check to see if PeachProj overlaps with an actor and damage if possible
    {
        return true; //return true if damaged actor
    }
    return false; //return false if no actor damaged
}

//------PEACHFIRE IMPLEMENTATIONS------
PeachFire::PeachFire(int x, int y, int dir, StudentWorld* w)
:PeachProj(IID_PEACH_FIRE, x, y, dir, w)
{
}

PeachFire::~PeachFire()
{
}

//------SHELL IMPLEMENTATIONS------
Shell::Shell(int x, int y, int dir, StudentWorld* w)
:PeachProj(IID_SHELL, x, y, dir, w)
{
}

Shell::~Shell()
{
}


//------HELPER METHODS------

int Enemies::generateDir()
{
    int x = randInt(0, 1); //randomly generate either a 0 or a 1
    if(x == 0) //if 0 is generated, direction starts as 0
        return 0;
    else
        return 180; //if 1 is generated, direction starts as 180
}

 
