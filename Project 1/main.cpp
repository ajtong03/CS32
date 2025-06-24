// garks.cpp


#include "Game.h"
///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);

      // Play the game
    g.play();
}


/*
#include "Gark.h"
 int main()
 {}
*/

//Test #1
/*
 #include "Game.h"
 #include "Game.h"
 #include "Mesa.h"
 #include "Mesa.h"
 #include "History.h"
 #include "History.h"
 #include "Player.h"
 #include "Player.h"
 #include "Gark.h"
 #include "Gark.h"
 #include "globals.h"
 #include "globals.h"
 int main()
 {}
 */

//Test #2
/*
#include "History.h"
    int main()
    {
        History h(2, 2);
        h.record(1, 1);
        h.display();
    }

 Output:
 
 A.
 ..

 Program ended with exit code: 0
*/

//Test #3
/*
 #include "Gark.h"
     int main()
     {
         Gark g(nullptr, 1, 1);
     }
 
 Output:
 ***** A gark must be created in some Mesa!
 Program ended with exit code: 1
 */

//Test #4
/*
#include "Player.h"
    int main()
    {
        Player p(nullptr, 1, 1);
    }

 Output:
 ***** The player must be created in some Mesa!
 Program ended with exit code: 1
*/

//Test #5
/*
#include "Mesa.h"
    int main()
    {
        Mesa m(10, 20);
        m.addPlayer(2, 2);
    }
*/

//Test #6
/*
#include "globals.h"
    #include "Player.h"
    #include "Mesa.h"
    int main()
    {
        Mesa m(10, 20);
        Player p(&m, 2, 3);
    }
*/

//Test #7
/*
#include "Mesa.h"
#include "Player.h"
int main()
{
    Mesa m(10, 20);
    Player p(&m, 2, 3);
}
*/

//Test #8
/*
#include "Player.h"
#include "Mesa.h"
int main()
{
 Mesa m(10, 20);
    Player p(&m, 2, 3);
}
*/

//Test #9
/*
#include "History.h"
#include "Mesa.h"
#include <iostream>
using namespace std;

int main()
{
    Mesa m(1, 3);
    m.history().record(1, 2);
    Mesa m2(1, 2);
    m2.history().record(1, 1);
    m.history().display();
    cout << "===" << endl;
}
*/


//Test #10
/*
#include "Mesa.h"
#include "Player.h"
#include "History.h"
#include "globals.h"
#include <iostream>
using namespace std;
int main()
{
    Mesa m(3, 4);
    m.addPlayer(2, 4);
    m.addGark(2, 3);
    m.addGark(3, 4);
    m.player()->moveOrAttack(LEFT);
    m.player()->moveOrAttack(DOWN);
    m.player()->moveOrAttack(LEFT);
    m.player()->moveOrAttack(UP);
    m.player()->moveOrAttack(LEFT);
    m.player()->moveOrAttack(DOWN);
    m.history().display();
    cout << "====" << endl;
}
*/


//Test #11
/*
#include "Gark.h"
#include "Mesa.h"
int main()
{
    Mesa m(10, 20);
    Player p(&m, 2, 3); //produces error "Variable has incomplete type 'Player'"
    Gark g(&m, 1, 1);
}
*/

//Test #12
/*
#include "Player.h"
#include "Mesa.h"
int main()
{
    Mesa m(10, 20);
    Player p(&m, 2, 3);
    Gark g(&m, 1, 1);//produces error "Variable has incomplete type 'Gark'"
}
*/

//Test #13
/*
#include "globals.h"
#include "Gark.h"
#include "Player.h"
int main()
{
    Mesa m(10, 10);//produces error "Variable has incomplete type 'Mesa'"
}
*/

//Test #14
/*
#include "History.h"
int main()
{
    History h;//produces error "No matching constructor for initialization of 'History'"
}
*/
