//
//  PoreNetwork.h
//  networkgen
//
//  Created by Lucas van Oosterhout on 05-12-14.
//  Copyright (c) 2014 Lucas van Oosterhout. All rights reserved.
//

#ifndef networkgen_PoreNetwork_h
#define networkgen_PoreNetwork_h



struct NetworkSpecs {
    int Ni, Nj, Nk;
    float Length;
    float Pi, Pj, Pk;
    float *C;
    
};

class PoreNetwork{
    
public:
    
    const NetworkSpecs *ns;
    PoreNetwork(NetworkSpecs *ns);
    PoreNetwork(const char * networkSpecsFile);
    
    int ***arr; //contains the pb number at the lattic coordinates, (Known as a Lookup Table)
    int **throatCounter; //number of prore throats per pb nr (1 based!) and number of throats preceding.
    int **throatList; // Connection map from pb nr -> pb nr. Is the half map!
    int **throatList_full; //Full Connection map
    int nrOfConnections;
    int nrOfActivePBs;
    float **locationList; // Location of a pb using its nr as index
  

/*
 * Cleans out a throatList in a PN, deleting all Flagged Entries
 * Flag should be smaller then 0!
 * It does so by doing a member copy to an new array! Which takes some tim
 */
    void removeFlaggedThroats(const int Flag = 0);

    void removeFlaggedPBs(char *pb_flag_list, int minFlag);

    void generateLocation();
    
    void generateConnectivity();
    
    void generateFullConnectivity();
    
    void generate_naive_array();
    
    int delelteThroat(int i, int deleted, int flag);

};
void writeConnectivity(const char * filename, PoreNetwork *P_net);

void writeLocation(const char * filename, PoreNetwork *P_net);
#endif
