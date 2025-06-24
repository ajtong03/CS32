#include "MemberDatabase.h"
#include "PersonProfile.h"
#include "provided.h"
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

MemberDatabase::MemberDatabase()
{
    
}

MemberDatabase::~MemberDatabase()
{
    auto it = m_delete.begin();
    while(it != m_delete.end())
    {
        delete (*it);
        it = m_delete.erase(it);
    }
}

bool MemberDatabase::LoadDatabase(std::string filename)
{
    ifstream infile(filename);
    if(!infile)
        return false;
    string line;
    int i = 0; //variable to track what type of line we are at in the file
    string name = ""; //variable to hold name of person
    string email = ""; //variable to hold email of person
    int numAtt = 0; //variable to hold the number of AttValPairs associated with a person
    PersonProfile* c;
    while(getline(infile, line))
    {
        if(line.empty())
        {
            i = -1; //-1 is associated with an empty line
        }
        else if(i == 0) //if i is 0, line contains a name
        {
            name = line; //set name equal to line currently being read
        }
        else if(i == 1) //if i is 1, line contains an email
        {
            email = line; //set email equal to line currently being read
            c =  new PersonProfile(name, email); //create new PersonProfile with name and email
        }
        else if(i == 2) //if i is 2, line contains number of AttValPairs a person has (not distinct)
        {
            stringstream ss;
            ss << line;
            ss >> numAtt; //insert the number of AttValPairs into numAtt
        }
        else if(i > 2)
        {
            
            string attribute; //variable to hold attribute
            string value; //variable to hold value associated with that attribute
            
            istringstream iss(line);
            getline(iss, attribute, ','); //insert everything up until the first comma into attribute
            getline(iss, value); //value now contains the rest of line
            string attval = attribute + value; //variable that combines the attribute and value into a single string
            c->AddAttValPair(AttValPair(attribute, value));
                        
            unordered_set<string>* temp = m_mapAtt.search(attval); //retrieve vector of emails associated with an attribute-value pair
            
            if(temp == nullptr) //if there is no key equal to the attribute value pair, create a vector with c's email and insert the key-value pair into m_mapAtt
            {
                unordered_set<string> t = {email};
                m_mapAtt.insert(attval, t);
            }
            else //if key already exists in map, add email to the set of strings associated with that key
            {
                temp->insert(email);
            }
            
                
        }
        i++; //increment i to keep track of the line being read

        if(i > numAtt + 2) //all the information associated with a person has been parsed
        {
            m_mapPer.insert(email, *c); //insert person's email and their profile into m_mapPer
            m_delete.push_back(c); //keep track of all the PersonProfile pointers to deallocate at end
        }
        
    }
    
    return true;
}
std::vector<std::string> MemberDatabase::FindMatchingMembers(const AttValPair& input) const
{
    unordered_set<string>* compatible = m_mapAtt.search(input.attribute + input.value); //create a set of emails of people that have input as an AttVal pair
    
    //if there are no individuals with input as an AttValPair, return an empty vector
    if(compatible == nullptr)
        return {};
    
    //if there are people with input as an AttValPair, create a vector from compatible and return that vector
    vector<string> comp(compatible->begin(), compatible->end());
    return comp;
}

const PersonProfile* MemberDatabase::GetMemberByEmail(std::string email) const
{
    return m_mapPer.search(email);
}
