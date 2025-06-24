#include "PersonProfile.h"
#include "provided.h"

using namespace std;

PersonProfile::PersonProfile(std::string name, std::string email)
:m_name(name), m_email(email)
{
}

std::string PersonProfile::GetName() const
{
    return m_name;
}

std::string PersonProfile::GetEmail() const
{
    return m_email;
}

void PersonProfile::AddAttValPair(const AttValPair &attval) 
{
    std::string attribute = attval.attribute; //variable to hold attribute of attval
    std::string val = attval.value; //variable to hold value of attval

    unordered_set<string>* temp = (m_map.search(attribute));
    m_delete.push_back(temp);
    
    if(temp == nullptr) //check if attribute exists in this person's profile
    {
        //if attribute doesn't exist, create a new set with val and insert it into m_map
        unordered_set<string> t = {val};
        m_map.insert(attribute, t);
        m_vector.push_back(attval); //add attval to vector
    }
    else //attribute already exists in this person's profile
    {
        size_t size = temp->size();
        temp->insert(val); //insert value into set of values associated with attribute
        if(temp->size() == size + 1)
            m_vector.push_back(attval);
    }
}

int PersonProfile::GetNumAttValPairs() const
{
    return int(m_vector.size()); //return the size of m_vector which is equal to the number of AttValPairs associated with a person
}


//when would this ever return false?
bool PersonProfile::GetAttVal(int attribute_num, AttValPair& attval) const
{
    attval = m_vector[attribute_num]; //set attval equal the attribute_num'th AttValPair
    return true; //return whether successfully acquired attribute_num'th AttValPair
}

