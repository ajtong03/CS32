#ifndef Game_h
#define Game_h
class Mesa;

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nGarks);
    ~Game();

        // Mutators
    void play();

  private:
    Mesa* m_mesa;
};
int decodeDirection(char dir);
#endif /* Game_h */
