#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

const int ACTION_BONK = 1;
const int ACTION_DAMAGE = 2;
const int NO_ACTION = 0;

class StudentWorld;

class Actor: public GraphObject
{
    public:
        Actor(int ID, int x, int y, int dir, int depth, double size, StudentWorld* w, bool hinder, bool damage);
        virtual ~Actor();
    
        //pure virtual function since no Actor object should be made; different Actors have different actions to take during a tick
        virtual void doSomething() = 0; //responsible for an actor's action during a single tick of a game
        StudentWorld* getWorld(); //returns a pointer to StudentWorld
        bool isAlive() const; //returns whether an actor is alive or not
        bool hinderMvmt() const; //returns whether an actor blocks other actors
        virtual void dead(); //sets an actor's life state to dead
        virtual void bonk(); //responsible for an actor's action when bonked by another actor
        bool damageable() const; //returns whether an actor can be damaged
        virtual void damage(); //responsible for attempting to damage an actor
        int movement(); //causes a actor to move and returns -1 if successful. Returns 0 or 180 if the actor needs to face a certain direction.
    
    private:
        bool m_alive;
        StudentWorld* m_world;
        bool m_hinder;
        bool m_damageable;
};

class Peach: public Actor
{
    public:
        Peach(int x, int y, StudentWorld* w);
        virtual ~Peach();
        virtual void doSomething();
        virtual void bonk();
        virtual void damage();
        void updateHit(int amt); //update's Peach's health points to amt
    
        bool hasStar() const; //returns whether Peach has star power
        bool hasShoot() const; //returns whether Peach has shoot power
        bool hasJump() const; //returns whether Peach has jump power
    
        void setStar(bool star); //sets Peach's star power to star
        void setShoot(bool shoot); //sets Peach's shoot power to shoot
        void setJump(bool jump); //sets Peach's jump power to jump
    
    
    private:
        int m_hit;
        int m_invincible;
        int m_remJumpDist;
        int m_recharge;
        int m_tempInvinc;
        bool m_star;
        bool m_shoot;
        bool m_jump;
        
        
};

class Stationary: public Actor //blocks and pipes
{
    public:
        Stationary(int ID, int x, int y, StudentWorld* w);
        virtual ~Stationary();
        virtual void doSomething(); //does nothing
    
};

class Block: public Stationary
{
    public:
    Block(int x, int y, StudentWorld* w);
        virtual void bonk();
        virtual ~Block();
};

class GoodieBlock:public Block
{
    public:
        GoodieBlock(int x, int y, StudentWorld* w);
        virtual void bonk();
        virtual void addGoodie() = 0; //responsible for adding appropriate goodie to the game
        virtual ~GoodieBlock();
    private:
        bool active;
};

class StarBlock:public GoodieBlock
{
    public:
        StarBlock(int x, int y, StudentWorld* w);
        virtual ~StarBlock();
    private:
        virtual void addGoodie();
};

class MushBlock:public GoodieBlock
{
    public:
        MushBlock(int x, int y, StudentWorld* w);
        virtual ~MushBlock();
    private:
        virtual void addGoodie();
};

class FlowBlock:public GoodieBlock
{
    public:
        FlowBlock(int x, int y, StudentWorld* w);
        virtual ~FlowBlock();
    private:
        virtual void addGoodie();
};

class Pipe: public Stationary
{
    public:
        Pipe(int x, int y, StudentWorld* w);
        virtual ~Pipe();
};

class Character: public Actor
{
    public:
        Character(int id, int x, int y, int dir, int depth, double size, StudentWorld* w, int points);
        virtual ~Character();
        virtual void doSomething();
        virtual int getSoundHit() = 0; //returns the id of the sound that should be played when a character is hit
        virtual void indivAct() = 0; //responsible for the action of a character that differs from their shared behaviour
        int getPoints() const; //returns the amount of points a character awards the player
    private:
        int m_points;
    
};

class Finished: public Character
{
    public:
        Finished(int id, int x, int y, StudentWorld* w);
        virtual ~Finished();
        
        virtual void notify() = 0; //responsible for notifying StudentWorld of the status of the game (level finished or level completed)
    private:
        virtual void indivAct();
};

class Flag: public Finished
{
    public:
        Flag(int x, int y, StudentWorld* w);
        virtual ~Flag();
    private:
        virtual int getSoundHit();
        virtual void notify();
};

class Mario:public Finished
{
    public:
        Mario(int x, int y, StudentWorld* w);
        virtual ~Mario();
    private:
        virtual int getSoundHit();
        virtual void notify();
};

class Goodies:public Character
{
    public:
        Goodies(int id, int x, int y, StudentWorld* w, int points);
        virtual ~Goodies();
        virtual void setPow() = 0; //responsible for turning on Peach's appropriate power depending on what goodie she attains
    private:
        virtual int getSoundHit();
        virtual void indivAct();
};

class Flower:public Goodies
{
    public:
        Flower(int x, int y, StudentWorld* w);
        virtual  ~Flower();
    private:
        virtual void setPow();
};

class Mushroom:public Goodies
{
    public:
        Mushroom(int x, int y, StudentWorld* w);
        virtual  ~Mushroom();
    private:
        virtual void setPow();
};

class Star:public Goodies
{
    public:
        Star(int x, int y, StudentWorld* w);
        virtual ~Star();
    private:
        virtual void setPow();
};




class Enemies: public Actor
{
    public:
        Enemies(int id, int x, int y, StudentWorld* w);
        virtual ~Enemies();
        virtual void doSomething();
        virtual void bonk();
        virtual void damage();
    private:
        int generateDir();
};

class Goomba: public Enemies
{
    public:
        Goomba(int x, int y, StudentWorld* w);
        virtual ~Goomba();
};

class Koopa:public Enemies
{
    public:
        Koopa(int x, int y, StudentWorld* w);
        virtual ~Koopa();
        virtual void dead();
};

class Piranha:public Enemies
{
    public:
        Piranha(int x, int y, StudentWorld* w);
        virtual ~Piranha();
        virtual void doSomething();
    private:
        int m_delay;
};

class Projectile:public Actor
{
    public:
        Projectile(int id, int x, int y, int dir, StudentWorld* w);
        virtual ~Projectile();
        virtual void doSomething();
        virtual bool damageActor() = 0; //responsible for attempting to damage actor and return true if so, false if unsuccessful
};

class PiranFire:public Projectile //projectile damages Peach
{
    public:
        PiranFire(int x, int y, int dir, StudentWorld* w);
        virtual ~PiranFire();
    private:
        virtual bool damageActor();
};

class PeachProj:public Projectile //damages enemies
{
    public:
        PeachProj(int id, int x, int y, int dir, StudentWorld* w);
        virtual ~PeachProj();
    private:
        virtual bool damageActor();
};

class PeachFire:public PeachProj
{
    public:
        PeachFire(int x, int y, int dir, StudentWorld* w);
        virtual ~PeachFire();
};

class Shell:public PeachProj
{
    public:
        Shell(int x, int y, int dir, StudentWorld* w);
        virtual ~Shell();
};

#endif // ACTOR_H_

