//
//  PoreNetwork.h
//  networkgen
//
//  Created by Lucas van Oosterhout on 05-12-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#ifndef networkgen_PoreNetwork_h
#define networkgen_PoreNetwork_h
#include <cstdlib>

struct NetworkSpecs {
    char* name;
    unsigned int Ni, Nj, Nk;
    float C[26];
    unsigned int coordNr;
    double searchDistance;
    float pbDist;
    bool periodicBounndaries;
    bool flowDirs[3]; // flowDirs[0] -> x-boundaries, flowDirs[1] -> y-boundaries, flowDirts[2] -> z-boundaries
};

class PoreNetwork{
    
    
private:
    int checkInput();
    template <typename T> T** paddedList(size_t amount, T **List, size_t nrOfCols ,size_t currentSize);
    
public:
    
    NetworkSpecs *ns;
    PoreNetwork(NetworkSpecs *ns);
    PoreNetwork(const char * networkSpecsFile);
    
    int ***arr; //contains the pb number at the lattic coordinates, (Known as a Lookup Table)
    int **throatCounter; //number of prore throats per pb nr (1 based!) and number of throats preceding.
    int **throatList; // Connection map from pb nr -> pb nr. Is the half map!
    int **throatList_full; //Full Connection map
    int *outlets, *inlets; //The Inlets and Outlets list, lists of inlet and outlet pores.
    size_t nrOfConnections;
    size_t nrOfActivePBs;
    float **locationList; // Location of a pb using its nr as index
    size_t *periodicThroats; // position in the throaList which has a periodic connection
    
    void generateBoundary(int dir);
  

/*
 * Cleans out a throatList in a PN, deleting all Flagged Entries
 * Flag should be smaller then 0!
 * It does so by doing a member copy to an new array! Which takes some tim
 */
    void removeFlaggedThroats(const int Flag = 0);

    void removeFlaggedPBs(char *pb_flag_list, char minFlag);

    void generateLocation();
    
    void generateConnectivity();
    
    size_t generateFullConnectivity();
    
    void generate_naive_array();
    
    
    size_t delelteThroat(size_t i, size_t deleted, int flag);
    
    size_t delelteThroat(int i, int deleted, int flag);

};

#endif
