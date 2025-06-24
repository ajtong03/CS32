#ifndef AttributeTranslator_h
#define AttributeTranslator_h
#include "RadixTree.h"
#include <vector>

struct AttValPair;

class AttributeTranslator
{
    public:
        AttributeTranslator();
    
        //function to load file containing all possible AttValPairs and their compatible AttValPairs
        bool Load(std::string filename);
    
        //function that returns a vector of compatible AttValPairs to source
        std::vector<AttValPair> FindCompatibleAttValPairs(const AttValPair& source) const;
    private:
        RadixTree<std::vector<AttValPair>> m_compatible; //Radix tree that maps a string representing an AttValPair to the compatible AttValPairs
};
#endif /* AttributeTranslator_h */

