//main.cpp
#include "PersonProfile.h"
#include "AttributeTranslator.h"
#include "MemberDatabase.h"
#include "MatchMaker.h"
#include "provided.h"
#include <iostream>
#include <string>
//#include <vector>
#include "RadixTree.h"

const std::string MEMBERS_FILE    = "/Users/alissatong/members.txt";
const std::string TRANSLATOR_FILE = "/Users/alissatong/translator.txt";

bool findMatches(const MemberDatabase& mdb, const AttributeTranslator& at);

int main() {
    MemberDatabase mdb;
    if (!mdb.LoadDatabase(MEMBERS_FILE))
    {
        std::cout << "Error loading " << MEMBERS_FILE << std::endl;
        return 1;
    }
    /*const PersonProfile* c = mdb.GetMemberByEmail("AugustusBarr87744@charter.net");
    if(c != nullptr)
        std::cout << c->GetName() << " " << c->GetNumAttValPairs() << std::endl;*/
    AttributeTranslator at;
    if (!at.Load(TRANSLATOR_FILE))
    {
        std::cout << "Error loading " << TRANSLATOR_FILE << std::endl;
        return 1;
    }
    //std::vector<AttValPair> t = at.FindCompatibleAttValPairs(test);
    /*if(t.size() > 0)
        std::cout << "yes";*/
    /*RadixTree<int> r;
    r.insert("hi", 1);
    r.insert("loud", 3);
    r.insert("hello", 2);
    std::cout<<(*(r.search("hi")));*/
    
    while (findMatches(mdb, at))
        ;
    //AttValPair test = AttValPair("trait", "transparent");
    /*std::vector<AttValPair> t = at.FindCompatibleAttValPairs(test);
    for(int i = 0; i < t.size(); i++)
    {
        std::cout << t[i].attribute << " " << t[i].value << std::endl;
    }*/
    /*std::vector<std::string> test2 = mdb.FindMatchingMembers(test);
    std::cout << test2.size();
    for(int i = 0; i < test2.size(); i++)
    {
        std::cout << test2[i] << std::endl;
    }*/
     
    std::cout << "Happy dating!" << std::endl;
}

bool findMatches(const MemberDatabase& mdb, const AttributeTranslator& at)
{
      // Prompt for email
    std::string email;
    const PersonProfile* pp;
    for (;;) {
        std::cout << "Enter the member's email for whom you want to find matches: ";
        std::getline(std::cin, email);
        if (email.empty())
            return false;
        pp = mdb.GetMemberByEmail(email);
        if (pp != nullptr)
            break;
        std::cout << "That email is not in the member database." << std::endl;
    }

      // Show member's attribute-value pairs
    std::cout << "The member has the following attributes:" << std::endl;
    for (int k = 0; k != pp->GetNumAttValPairs(); k++) {
        AttValPair av;
        pp->GetAttVal(k, av);
        std::cout << av.attribute << " --> " << av.value << std::endl;
    }

      // Prompt user for threshold
    int threshold;
    std::cout << "How many shared attributes must matches have? ";
    std::cin >> threshold;
    std::cin.ignore(10000, '\n');

      // Print matches and the number of matching translated attributes
    MatchMaker mm(mdb, at);
    std::vector<EmailCount> emails = mm.IdentifyRankedMatches(email, threshold);
    if (emails.empty())
        std::cout << "No member was a good enough match." << std::endl;
    else {
        std::cout << "The following members were good matches:" << std::endl;;
        for (const auto& emailCount : emails) {
            const PersonProfile* pp = mdb.GetMemberByEmail(emailCount.email);
            std::cout << pp->GetName() << " at " << emailCount.email << " with "
                      << emailCount.count << " matches!" << std::endl;
        }
    }
    std::cout << std::endl;
    return true;
}







//main.cpp
/*
#include "PersonProfile.h" 
#include "AttributeTranslator.h"
#include "MatchMaker.h"
#include "provided.h"
#include <iostream>
#include <string>
#include <vector>

const std::string MEMBERS_FILE    = "members.txt";
const std::string TRANSLATOR_FILE = "translator.txt";

bool findMatches(const MemberDatabase& mdb, const AttributeTranslator& at);

*//*
#include "PersonProfile.h"
#include "provided.h"
#include <iostream>
#include <string>
#include <vector>
#include "MemberDatabase.h"
#include "AttributeTranslator.h"

const std::string MEMBERS_FILE    = "/Users/alissatong/membersTest.txt"; //CHANGE THIS TO members.txt WHEN TESTING
const std::string TRANSLATOR_FILE = "/Users/alissatong/translatorTest.txt"; //CHANGE THIS BACK LATER

using namespace std;

int main() {
    
    string email = "alissatong@ucla";
    string name = "alissa";
    string email2 = "alissa@ucla";
    string email3 = "ali@ucla";
    string name2 = "ann";
    
    string attribute = "hobby";
    string value = "baking";
    string value2 = "cooking";
    
    PersonProfile* p = new PersonProfile(name, email);
    AttValPair* a = new AttValPair(attribute, value);
    p->AddAttValPair(*a);
    AttValPair* b = new AttValPair(attribute, value2);
    p->AddAttValPair(*b);
    p->AddAttValPair(*b);
    cout << p->GetNumAttValPairs() << endl;
    
    MemberDatabase md;
    if(!md.LoadDatabase(MEMBERS_FILE))
        cout << "unable to load";
    AttributeTranslator at;
    if (!at.Load(TRANSLATOR_FILE))
    {
        std::cout << "Error loading " << TRANSLATOR_FILE << std::endl;
        return 1;
    }
    //if((md.GetMemberByEmail("ELand@live.com")) == nullptr)
       // cout << "bad";
    const PersonProfile* c = md.GetMemberByEmail("TiAdkins81471@charter.net");
    if(c != nullptr)
        cout << c->GetName() << " " << c->GetNumAttValPairs() << endl;
    AttValPair test1 = AttValPair("job", "architect");
    vector<string> result =  md.FindMatchingMembers(test1);
    for(int i = 0; i < result.size(); i++)
    {
        cout << result[i] << endl;
    }
    
    AttValPair hello = AttValPair("job", "teacher");
    std::vector<AttValPair> temp = at.FindCompatibleAttValPairs(hello);
    
    for(int i = 0; i < temp.size(); i++)
    {
        cout << temp[i].attribute << " " << temp[i].value;
    }

    
    MemberDatabase mdb;
    if (!mdb.LoadDatabase(MEMBERS_FILE))
    {
        std::cout << "Error loading " << MEMBERS_FILE << std::endl;
        return 1;
    }
    AttributeTranslator at;
    if (!at.Load(TRANSLATOR_FILE))
    {
        std::cout << "Error loading " << TRANSLATOR_FILE << std::endl;
        return 1;
    }

    while (findMatches(mdb, at))
        ;

    std::cout << "Happy dating!" << std::endl;
}


bool findMatches(const MemberDatabase& mdb, const AttributeTranslator& at)
{
      // Prompt for email
    std::string email;
    const PersonProfile* pp;
    for (;;) {
        std::cout << "Enter the member's email for whom you want to find matches: ";
        std::getline(std::cin, email);
        if (email.empty())
            return false;
        pp = mdb.GetMemberByEmail(email);
        if (pp != nullptr)
            break;
        std::cout << "That email is not in the member database." << std::endl;
    }

      // Show member's attribute-value pairs
    std::cout << "The member has the following attributes:" << std::endl;
    for (int k = 0; k != pp->GetNumAttValPairs(); k++) {
        AttValPair av;
        pp->GetAttVal(k, av);
        std::cout << av.attribute << " --> " << av.value << std::endl;
    }

      // Prompt user for threshold
    int threshold;
    std::cout << "How many shared attributes must matches have? ";
    std::cin >> threshold;
    std::cin.ignore(10000, '\n');

      // Print matches and the number of matching translated attributes
    MatchMaker mm(mdb, at);
    std::vector<EmailCount> emails = mm.IdentifyRankedMatches(email, threshold);
    if (emails.empty())
        std::cout << "No member was a good enough match." << std::endl;
    else {
        std::cout << "The following members were good matches:" << std::endl;;
        for (const auto& emailCount : emails) {
            const PersonProfile* pp = mdb.GetMemberByEmail(emailCount.email);
            std::cout << pp->GetName() << " at " << emailCount.email << " with "
                      << emailCount.count << " matches!" << std::endl;
        }
    }
    std::cout << std::endl;
    return true;*/
//}


