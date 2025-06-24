#ifndef MemberDatabase_h
#define MemberDatabase_h
#include <vector>
#include <unordered_set>
#include "RadixTree.h"
#include <iostream>

class PersonProfile;

struct AttValPair; 

class MemberDatabase
{
public:
    MemberDatabase();
    
    ~MemberDatabase();
    
    //function to load the file full of names of people, their emails, and their attributes/values
    bool LoadDatabase(std::string filename);
    
    //function that returns a vector of the emails of members that have input as an AttValPair
    std::vector<std::string> FindMatchingMembers(const AttValPair& input) const;
    
    //function that returns the PersonProfile associated with an email
    const PersonProfile* GetMemberByEmail(std::string email) const;
    
    private:
        RadixTree<std::unordered_set<std::string>> m_mapAtt; //maps an attribute-value pair to an email
        RadixTree<PersonProfile> m_mapPer; //maps email addresses to a PersonProfile
        std::vector<PersonProfile*> m_delete; //keep track of dynamically allocated PersonProfiles that need to be destructed
    
};
#endif /* MemberDatabase_h */
