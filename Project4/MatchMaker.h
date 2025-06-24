#ifndef MatchMaker_h
#define MatchMaker_h
#include <map>
#include <vector>
#include <iostream>


struct EmailCount;
class MemberDatabase;
class AttributeTranslator;
class PersonProfile;


class MatchMaker
{
    public:
        MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at);
        ~MatchMaker();
    
        //function that returns a vector of EmailCount objects of people who are compatible with the person with a given email. The vector is organised by descending number of matches and alphabetically should individuals have the same number of matching AttValPairs
        std::vector<EmailCount> IdentifyRankedMatches(std::string email, int threshold) const;
    private:
        std::map<std::string, int>* m_numMatch; //pointer to a map that maps a string to an int (person's email to the number of matching AttValPairs they have)
        const MemberDatabase* m_database; //variable to hold the MemberDatabase object being used 
        const AttributeTranslator* m_translator; //variabel to hold the AttributeTranslator object being used
};

#endif /* MatchMaker_h*/
