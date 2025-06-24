#ifndef PersonProfile_h
#define PersonProfile_h
#include <vector>
#include <unordered_set>
#include "RadixTree.h" 

struct AttValPair;

class PersonProfile
{
    public:
        PersonProfile(std::string name, std:: string email);
    
        //returns name associated with PersonProfile
        std::string GetName() const;
    
        //returns email associaed with PersonProfile
        std::string GetEmail() const;
    
        //check whether that AttValPair has already been added to a PersonProfile and if not, adds it to the profile
        void AddAttValPair(const AttValPair& attval);
    
        //returns the number of AttValPairs a PersonProfile has
        int GetNumAttValPairs() const;
    
        //returns the attribute_numth AttValPair associated with a PersonProfile
        bool GetAttVal(int attribute_num, AttValPair& attval) const;
        
    private:
        std::string m_name; //variable holding person's name
        std::string m_email; //variable holding person's email
        RadixTree<std::unordered_set<std::string>> m_map; //RadixTree mapping from an attribute to a set containing all values associated with that attribute
        std::vector<AttValPair> m_vector; //vector used for quick access of AttValPairs and to determine number of AttValPairs a PersonProfile has
    
        std::vector<std::unordered_set<std::string>*> m_delete;
};

#endif /* PersonProfile_h */
