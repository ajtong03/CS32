#include "AttributeTranslator.h"
#include <fstream>
#include <sstream>
#include "provided.h"

using namespace std;

AttributeTranslator::AttributeTranslator()
{
    
}

bool AttributeTranslator::Load(std::string filename)
{
    ifstream infile(filename);
    if(!infile)
        return false;
    string line;
    while(getline(infile, line)) //parse through each line of filename
    {
        if(!line.empty()) //if line is empty, nothing happens
        {
            
            //string variables to keep track of the source/compatible attributes and values
            string sourceAtt;
            string sourceVal;
            string compatAtt;
            string compatVal;
     
            istringstream iss(line);
            getline(iss, sourceAtt, ','); //sourceAtt is set equal to everything up until the first comma is
            getline(iss, sourceVal, ','); //sourceVal is set equal to everything from first comma up until second comma
            getline(iss, compatAtt, ','); //compatAtt is set equal to everything from second comma up until third comma
            getline(iss, compatVal); //compatVal is set equal to the rest of the line
            
            string source = sourceAtt + sourceVal; //variable combining sourceAtt and sourceVal into a single string
            vector<AttValPair>* temp = m_compatible.search(source); //temp is a vector of compatible AttValPairs to the source AttValPair
            if(temp == nullptr) //check if a vector of compatible AttValPairs exists
            {
                //no vector associated with the AttVal pair so new vector of AttVal pairs is created and inserted into m_compatible
                vector<AttValPair> newVect = {AttValPair(compatAtt, compatVal)};
                m_compatible.insert(source, newVect);
            }
            else //vector associated with a given source AttValPair already exists
            {
                AttValPair toInsert = AttValPair(compatAtt, compatVal);
                bool exists = false;
                for(int i = 0; i < temp->size(); i++)
                {
                    if((*temp)[i] == toInsert)
                    {
                        exists = true;
                        break;
                    }
                }
                if(!exists)
                    temp->push_back(AttValPair(compatAtt, compatVal)); //add compatible AttValPair to vector associated with source AttValPair
            }
        }
    }
    return true; 
}

std::vector<AttValPair> AttributeTranslator::FindCompatibleAttValPairs(const AttValPair& source) const
{
    string source_str = source.attribute + source.value; //variable combining attribute and value of source into a single string
    vector<AttValPair>* temp = m_compatible.search(source_str); //pointer to vector that holds all compatible AttValPairs to the source AttValPair
    
    if(temp == nullptr) //if there are no compatible AttValPairs, return an empty vector
        return {};
    return *(temp); //return vector of all compatible AttValPairs
}


