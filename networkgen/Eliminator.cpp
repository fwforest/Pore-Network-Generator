//
//  Eliminator.cpp
//  networkgen
//
/* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */



#include "Eliminator.h"
//#include "vtk.h"
#include <random>
#include <stack>

/*

 * C[0] = x-dir; C[1] = y-dir; C[2] = z-dir
 * C[3] = x - -y; C[4] = x - +y;
 * C[5] = x - -z; C[6] = x - +z;
 * C[7] = y - -z; C[8] = y - +z;
 * C[9] = -y - -z;C[10]= -y - +
 *
 * Max coordination number is 12 at the moment (in forward x dir!)
 *
 */

void eliminateThroats(PoreNetwork *P_net){
   
     
    std::cout<<"Deleting throats" << std::endl;

    float * ChanceList = P_net->ns->C;
    
    if(!P_net){
        std::cerr << "Eliminator Error! \n\t No PoreNetwork Specified!" << std::endl;
        return;
    }
    
    //    int nrPB = P_net->ns->Ni * P_net->ns->Nj * P_net->ns->Nk;
    int Ni = P_net->ns->Ni;
    int Nj = P_net->ns->Nj;
    int Nk = P_net->ns->Nk;
    
    //Set up Random Generator
    std::random_device r_dev{};
    std::default_random_engine e{r_dev()};
    std::uniform_real_distribution<float> d{0.0f,1.0f};
    
    int *coord   = new int[3];
    int *coord_n = new int[3];
    size_t deleted = 0;
    size_t pn = 1, i = 0;
    
    
    // Go through throatList
    while(P_net->throatList[0][i] != 0){
        
        //Get coordnaites for pb
        deflatten_3d(P_net->throatList[0][i], Ni, Nj, Nk, coord);
        
        pn = P_net->throatList[0][i];
        /*if(pn < Nj*Nk || pn > Ni*Nj*Nk - Nj*Nk){
            i++;
            continue;
        }
        */
        while (P_net->throatList[0][i] == (int)pn) {
            // check for this pore, all its connections
            deflatten_3d(P_net->throatList[1][i], Ni, Nj, Nk, coord_n);
            
            // We have a connection in the x-dir
            if(d(e) >= ChanceList[0]
                    && coord[0] - coord_n[0] == -1
                    && coord[1] - coord_n[1] == 0
                    && coord[2] - coord_n[2] == 0){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            // Connection in y-dir
            else if(d(e) >= ChanceList[1]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == -1
                    && coord[2] - coord_n[2] == 0){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            // Connection in z-dir
            else if(d(e) >= ChanceList[2]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == 0
                    && coord[2] - coord_n[2] == -1){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            
            //------
            // Connection in x - -y diagonal
            else if(d(e) >= ChanceList[3]
                    && coord[0] - coord_n[0] == -1
                    && coord[1] - coord_n[1] == 1
                    && coord[2] - coord_n[2] == 0){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            // Connection in x - +y diagonal
            else if(d(e) >= ChanceList[4]
                    && coord[0] - coord_n[0] == -1
                    && coord[1] - coord_n[1] == -1
                    && coord[2] - coord_n[2] == 0){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            // Connection in x - -z diagonal
            else if(d(e) >= ChanceList[5]
                    && coord[0] - coord_n[0] == -1
                    && coord[1] - coord_n[1] == 0
                    && coord[2] - coord_n[2] == 1){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            // Connection in x - +z diagonal
            else if(d(e) >= ChanceList[6]
                    && coord[0] - coord_n[0] == -1
                    && coord[1] - coord_n[1] == 0
                    && coord[2] - coord_n[2] == -1){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            // Connection in -y - -z diagonal
            else if(d(e) >= ChanceList[7]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == -1
                    && coord[2] - coord_n[2] == 1){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            // Connection in -y - +z diagonal
            else if(d(e) >= ChanceList[8]
                    && coord[0] - coord_n[0] == 0
                    && coord[1] - coord_n[1] == -1
                    && coord[2] - coord_n[2] == -1){
                deleted = P_net->delelteThroat(i, deleted, -1);
            }
            
            i++;
        }// while
        
        
        //P_net->throatCounter[1][pn] -= deleted; //Not really nice, Only the PoreNetwork Class should change the PoreNetwork...
    }// while
    
    if(P_net->ns->periodicBounndaries){
        size_t pn_n;
        //Periodic throats can be eliminated as well!
        for(size_t perdiocC = 0; P_net->periodicThroats[perdiocC] != 0; perdiocC++){
            pn   = P_net->throatList[0][P_net->periodicThroats[perdiocC]];
            pn_n = P_net->throatList[1][P_net->periodicThroats[perdiocC]];
            
            deflatten_3d(pn, Ni, Nj, Nk, coord);
            deflatten_3d(pn_n, Ni, Nj, Nk, coord_n);
            
            // Delete in x-dir
            if( d(e) >= ChanceList[0] &&
               coord[0] != coord_n[0] &&
               coord[2] == coord_n[2]){
                deleted = P_net->delelteThroat((size_t)P_net->periodicThroats[perdiocC], deleted, -1);
                //std::cout << P_net->periodicThroats[perdiocC] << '\t' << pn << " - " << pn_n << std::endl;
                
            }
            // Delete in y-dir
            else if( d(e) >= ChanceList[1] &&
               coord[1] == coord_n[1] &&
               coord[2] != coord_n[2]){
                deleted = P_net->delelteThroat((size_t)P_net->periodicThroats[perdiocC], deleted, -1);
                //std::cout << P_net->periodicThroats[perdiocC] << '\t' << pn << " - " << pn_n << std::endl;
                
            } // delete in z-dir
            else if(d(e) >= ChanceList[2] &&
               coord[1] != coord_n[1] &&
               coord[2] == coord_n[2]){
                deleted = P_net->delelteThroat((size_t)P_net->periodicThroats[perdiocC], deleted, -1);
            }
        } //for periodicThroat
    } // if periodicBoundaries
    
    delete [] coord;
    delete [] coord_n;
    
    
}


/*
 * Returns the index of the last of the connections to pb i
 * And thus with index i, returns all connection to pb
 * Maybe INLINE?
 */

size_t returnAdjecentNodes(int **throatList, size_t i, size_t max){
    
    int pn = throatList[0][i];
    while(i < max && throatList[0][i] == pn) {
        i++;
    }
    return (i);
}

/*
 1  procedure DFS(G,v):
 2      label v as discovered
 3      for all edges from v to w in G.adjacentEdges(v) do
 4          if vertex w is not labeled as discovered then
 5              recursively call DFS(G,w)
 */

/*
 * Recursive Depth-first Search on FullConnected ThroatList
 * Places a flag in the flagged_pb array, when check is found!
 */

void DFS(size_t node, int ** TL, char* flagged_PB, size_t TL_Length, char flag, char check){
 
    // Check if the pb has been visited
    if(flagged_PB[TL[0][node]] == flag)
        return;
	
	// Flag PB as discoverd and check all adjecent nodes
    flagged_PB[TL[0][node]] = flag;

    // from TL[1][node] to TL[1][max] are connected pbs
    size_t max = returnAdjecentNodes(TL, node, TL_Length);
	    
    //For all throats connected to pb do:
    for(size_t i = node; i <= max; i++){
        if(flagged_PB[TL[1][i]] == check){
            //search edges of this node
            for(size_t j = 0; j < TL_Length; j++)
            {// do DFS on all edges of this node
                if(TL[0][j] == TL[1][i]){
                    DFS(j, TL, flagged_PB, TL_Length, flag, check);
                }
            }
        }
    }
}

//1  procedure DFS-iterative(G,v):
//2      let S be a stack
//3      S.push(v)
//4      while S is not empty
//5            v = S.pop()
//6            if v is not labeled as discovered:
//7                label v as discovered
//8                for all edges from v to w in G.adjacentEdges(v) do
//9                    S.push(w)

void DFS_iterative(size_t node, int ** TL, char* flagged_PB, size_t TL_Length, char flag, char check) {

//Node is the position in the throatlist[0][node]

    // simplest to use, not really the fastest
    std::stack<size_t> stack;

    int counter = 0;

    size_t stackSize = 0;
    stack.push(node);

    while (!stack.empty()) {
        size_t v = stack.top();
        stack.pop();

//        std::cout << "Root at pb: " << TL[0][v] << std::endl;

        if (flagged_PB[TL[0][v]] != flag) {
            flagged_PB[TL[0][v]] = flag;


            size_t Last_AJ_index = returnAdjecentNodes(TL, v, TL_Length);
//            std:: cout << " has " << Last_AJ_index - v << " nodes" << std::endl;
//            std::cout << "Nodes: ";

            for (size_t i = v; i < Last_AJ_index; i++) { // each neightbour of v
                size_t pb_neighbor = TL[1][i];
//                std::cout << pb_neighbor << ' ';
                if (flagged_PB[pb_neighbor] == check) { // if not visited

                    //Find the position in throatlist for pb_neighbor
                    //and push it to the stack
                    for (size_t j = 0; j < TL_Length; j++) {
                        if (TL[0][j] == pb_neighbor) {
                            stack.push(j);
                            break;
                        }
                    }
                }
            }
        }
        if(stack.size() > stackSize)
            stackSize = stack.size();

//        else
//            std::cout << "Already Checked!" << std::endl;

//        std::cout << std::endl;
    }
//    std::cout << "Largest Stack : " << stackSize << ", is " << stackSize* sizeof(size_t)/1000 << " kbytes" << std::endl;
}


/*
 * Search for isolated PoreBodies and return a Flagged Pore Bodies List
 * Search the FULL connectivity map foward and backward, checking
 * if pbs are connected to inlet AND outlet
 *
 * Call removeFlaggedPBs when this has been run, the network is invalid!
 */

char *searchForIsolatedPB_iterative(PoreNetwork *P_net, size_t lengthTL){
    
    bool verbose = false;
	if(!P_net->throatList_full){
		std::cerr << "ERROR: Full Connectivity Map Not Available, Check input!" << std::endl;
	}

    std::cout<< "Starting Search for Isolated PBs and Clusters: \n "<< lengthTL << std::endl;
   
    size_t i;
    
    //size_t lengthTL = P_net->nrOfConnections * 2;
    
    
    // Allocata a chunk of mem and set it to zero
    char *flagged_PB = new char[P_net->nrOfActivePBs + 1];
    for(i = 0; (int)i <= P_net->nrOfActivePBs; i++){
        flagged_PB[i] = 0;
    }
    
    //Do a DepthFirst Search on all inlets
    for(i = 0; P_net->throatList_full[0][i] <= P_net->nrOfInlets; i++){
        if(flagged_PB[P_net->throatList_full[0][i]] == 0){
                DFS_iterative(i, P_net->throatList_full, flagged_PB, lengthTL, (char)1, (char)0);
            if(verbose){
                std::cout << "root at pb: " << P_net->throatList_full[0][i] << std::endl;
            }
        }
    }
    std::cout << "Checked Inlets" << std::endl;
    
	
    if(verbose){
        for(int i = 1; i <= P_net->nrOfActivePBs; i ++){
            if (flagged_PB[i] == (char) 0) {
                std::cout << "Isolated PB: " <<i << " on:\t " << P_net->locationList[0][i] << '\t' << P_net->locationList[1][i] << '\t' << P_net->locationList[2][i] << std::endl;
            } else {
                std::cout << "Skipped: " << i << std::endl;
            }
        }
        std::cout << "---" << std::endl;
    }
    
    bool brokenNetwork = true;
    
    //Do a Depth First Search on all outlets
    //for(i = lengthTL - P_net->nrOfOutlets; P_net->throatList_full[1][i] <= P_net->nrOfActivePBs; i++){
    for(i = lengthTL - P_net->nrOfOutlets * 2; i < lengthTL; i++){
	// Check if pb the qualifies
        if(flagged_PB[ P_net->throatList_full[0][i] ] == (char)1){
            DFS_iterative(i, P_net->throatList_full, flagged_PB, lengthTL, (char)2, (char)1);
            brokenNetwork = false;
            if(verbose){
                std::cout << "root at pb: " << P_net->throatList_full[0][i] << std::endl;
            }
        } else if(verbose)
            std::cout << "PB: " << i << " on:\t " << P_net->locationList[0][i] << '\t' << P_net->locationList[1][i] << '\t' << P_net->locationList[2][i] << std::endl;
    }
    
    if(verbose)
        for(int i = 1; i <= P_net->nrOfActivePBs; i ++){
            if (flagged_PB[i] == (char) 1) {
                std::cout << "PB: " <<i << " on:\t " << P_net->locationList[0][i] << '\t' << P_net->locationList[1][i] << '\t' << P_net->locationList[2][i] << std::endl;
            }
        }
    if (brokenNetwork){
        std::cout << "!!! --- 2 : Network is Broken no Connection between Inlet and Outlet ---!!!" << std::endl;
        return nullptr;
    }

    // we now have a flagged list of pb's which are connected to the inlets AND outlets
    std::cout << "Done!"<< std::endl;
    return flagged_PB;
}


char * searchForIsolatedPB(PoreNetwork *P_net, size_t lengthTL){
    std::cerr << "Warning: Non-iterative Searching is Decrapted!" << std::endl;

    bool verbose = false;
    if(!P_net->throatList_full){
        std::cerr << "ERROR: Full Connectivity Map Not Available, Check input!" << std::endl;
    }

    std::cout<< "Starting Search for Isolated PBs and Clusters: \n "<< lengthTL << std::endl;

    size_t i;

    //size_t lengthTL = P_net->nrOfConnections * 2;


    // Allocata a chunk of mem and set it to zero
    char *flagged_PB = new char[P_net->nrOfActivePBs + 1];
    for(i = 0; (int)i <= P_net->nrOfActivePBs; i++){
        flagged_PB[i] = 0;
    }

    //Do a DepthFirst Search on all inlets
    for(i = 0; P_net->throatList_full[0][i] < P_net->nrOfInlets; i++){
        if(flagged_PB[P_net->throatList_full[0][i]] == 0){
            DFS(i, P_net->throatList_full, flagged_PB, lengthTL, (char)1, (char)0);
            if(verbose){
                std::cout << "root at pb: " << P_net->throatList_full[0][i] << std::endl;
            }
        }
    }
    std::cout << "Checked Inlets" << std::endl;


    if(verbose){
        for(int i = 1; i <= P_net->nrOfActivePBs; i ++){
            if (flagged_PB[i] == (char) 0) {
                std::cout << "Isolated PB: " <<i << " on:\t " << P_net->locationList[0][i] << '\t' << P_net->locationList[1][i] << '\t' << P_net->locationList[2][i] << std::endl;
            } else {
                std::cout << "Skipped: " << i << std::endl;
            }
        }
        std::cout << "---" << std::endl;
    }

    bool brokenNetwork = true;

    //Do a Depth First Search on all outlets
    //for(i = lengthTL - P_net->nrOfOutlets; P_net->throatList_full[1][i] <= P_net->nrOfActivePBs; i++){
    for(i = lengthTL - P_net->nrOfOutlets * 2; i < lengthTL; i++){
        // Check if pb the qualifies
        if(flagged_PB[ P_net->throatList_full[0][i] ] == (char)1){
            DFS(i, P_net->throatList_full, flagged_PB, lengthTL, (char)2, (char)1);
            brokenNetwork = false;
            if(verbose){
                std::cout << "root at pb: " << P_net->throatList_full[0][i] << std::endl;
            }
        } else if(verbose)
            std::cout << "PB: " << i << " on:\t " << P_net->locationList[0][i] << '\t' << P_net->locationList[1][i] << '\t' << P_net->locationList[2][i] << std::endl;
    }

    if(verbose)
        for(int i = 1; i <= P_net->nrOfActivePBs; i ++){
            if (flagged_PB[i] == (char) 1) {
                std::cout << "PB: " <<i << " on:\t " << P_net->locationList[0][i] << '\t' << P_net->locationList[1][i] << '\t' << P_net->locationList[2][i] << std::endl;
            }
        }
    if (brokenNetwork){
        std::cout << "!!! --- 2 : Network is Broken no Connection between Inlet and Outlet ---!!!" << std::endl;
        return nullptr;
    }

    // we now have a flagged list of pb's which are connected to the inlets AND outlets
    std::cout << "Done!"<< std::endl;
    return flagged_PB;
}


