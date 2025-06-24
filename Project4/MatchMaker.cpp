#include "MatchMaker.h"
#include "MemberDatabase.h"
#include "AttributeTranslator.h"
#include "provided.h"
#include "PersonProfile.h"
#include <algorithm>
#include <set>

using namespace std;

MatchMaker::MatchMaker(const MemberDatabase& mdb, const AttributeTranslator& at)
:m_database(&mdb), m_translator(&at) //is this okay??
{
    m_numMatch = new map<string, int>(); //initialise m_numMatch as a map that maps from strings to ints
}

MatchMaker::~MatchMaker()
{
    delete m_numMatch;
}

std::vector<EmailCount> MatchMaker::IdentifyRankedMatches(std::string email, int threshold) const
{
    const PersonProfile* p = m_database->GetMemberByEmail(email); //acquire PersonProfile associated with email
    
    int numPairs = p->GetNumAttValPairs(); //variable that holds the number of AttValPairs the person has
    vector<AttValPair> pairs = vector<AttValPair>();
    for(int i = 0; i < numPairs; i++) //insert all of the person's AttValPairs into a vector
    {
        AttValPair temp;
        p->GetAttVal(i, temp);
        pairs.push_back(temp);
    }
    
    vector<AttValPair> compatPairs = vector<AttValPair>();
   
    set<string> checkMatch = set<string>(); //use a set to make sure that no duplicate compatible AttValPairs are being added
    for(int i = 0; i < pairs.size(); i++) //iterate through vector with all of the person's AttValPairs
    {
        vector<AttValPair> temp = m_translator->FindCompatibleAttValPairs(pairs[i]); //vector holding all compatible AttValPairs for the ith AttValPair of person
        for(int i = 0; i < temp.size(); i++) //iterate through vector of compatible AttValPairs for single source AttValPair
        {
            if(checkMatch.find(temp[i].attribute + temp[i].value) == checkMatch.end())
            {
                checkMatch.insert(temp[i].attribute + temp[i].value);
                compatPairs.push_back(temp[i]); //add each compatible AttValPair to compatPairs
            }
        }
    }
    
    
    for(int i = 0; i < compatPairs.size(); i++)//for(auto it = compatPairs.begin(); it != compatPairs.end(); it++)//iterate through compatPairs
    {
        vector<string> match = m_database->FindMatchingMembers(compatPairs[i]);//*it);//compatPairs[i]); //retrieve vector of emails with that attribute value pair
        
        for(int j = 0; j < match.size(); j++) //iterate through the email addresses of people with compatible AttValPairs
        {
            if(match[j] != email) //check to make sure not matching individual to themselves
            {
                auto it = m_numMatch->find(match[j]); //check to see if email already exists in match
                if(it == m_numMatch->end()) //email doesn't exist yet
                {
                    m_numMatch->insert({match[j], 1}); //insert that email with 1, the number of compatible AttValPairs, to m_numMatch
                }
                else //email already exists
                {
                    it->second = it->second + 1; //increment the given email's number of matches by 1
                }
            }
        }
    }
    
    
    map<int, set<string>> orderMatches = map<int, set<string>>(); //variable that maps from ints to a set of strings (the number of matches to the emails with that specified number of matches)
    for(auto it = m_numMatch->begin(); it != m_numMatch->end(); it++) //use iterator to iterate through m_numMatch
    {
        int num = it->second; //variable to hold the number of matches
        if(num >= threshold) //check if the number of matches is equal to or greater than the required number of matches
        {
            auto temp = orderMatches.find(num); //acquire iterator to element with num as key in orderMatches
            if(temp == orderMatches.end()) //check if that key doesn't exist
            {
                set<string> s = {it->first}; //create a new set with person's email
                orderMatches.insert({num, s}); //insert the num matches and set of email into orderMatches
            }
            else //key already exists in orderMatches
            {
                set<string>* s = &(temp->second); //acquire the set of strings associated with num
                s->insert(it->first); //insert email into existing set of strings
            }
        }
    }
    
    vector<EmailCount> rankedMatches = vector<EmailCount>(); //create a vector of EmailCount objects to store matches in correct order
    for(auto it = orderMatches.rbegin(); it != orderMatches.rend(); it++) //orderMatches is in ascending order, but rankedMatches returns in descending order of num matches, so map needs to be traversed backwards
    {
        //set<string> temp = orderMatches[i].second;
        set<string> temp = it->second;
        for(auto it2 = temp.begin(); it2 != temp.end(); it2++) //the emails associated with an int are in alphabetical order so traverse from beginning to end
        {
            EmailCount email = EmailCount((*it2), it->first); //create an EmailCount object with the email and num that iterators are pointing to
            rankedMatches.push_back(email); //add newly created EmailCount object to rankedMatches
        }
    }
    
    return rankedMatches; //return vector of ranked EmailCount objects
}
