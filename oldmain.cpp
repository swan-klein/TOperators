/**
 * T Operator Product Generation Main File
 * @file main.cpp
 * @author Connor Mooney
 * @author Michael Jarret
 * @author Andrew Glaudell
 * @author Jacob Weston
 * @author Mingzhen Tian
 * @version 5/20/21
 */

#include <algorithm>
#include <iostream>
#include <vector>
#include <thread>
#include <future>
#include "Z2.hpp"
#include "SO6.hpp"


using namespace std;
/**
 * Returns the SO6 object corresponding to T[i+1, j+1]
 * @param i the first index to determine the T matrix
 * @param j the second index to determine the T matrix
 * @return T[i+1,j+1]
 */

SO6 tMatrix(int j, int i){
    // Generates the T Matrix T[i+1, j+1]
    SO6 t;
    int sign;
    if((i+1==j&&i<=4&&i>=0)||(j+1==i&&j<=4&&j>=0))
        sign = 1;
    else
        sign = -1;
    for(int k=0; k<6; k++){
        t(k,k) = Z2(1,0,0);
    }
    t(i,i) = Z2(0,1,1);
    t(i,j) = Z2(0,-sign, 1);
    t(j,i) = Z2(0, sign, 1);
    t(j,j) = Z2(0,1,1);
    t.sort();
    return(t);
}

/**
 * Returns whether or not an SO6 is contained in a vector of SO6
 * @param v the vector of SO6 to be checked for v
 * @param entry the SO6 to be compared to elements of v
 * @return whether or not v is contained in entry
 */
//bool containedIn(vector<SO6>& v, SO6& entry){
//    for(SO6 item: v){
//        if(entry == item){
//            return(true);
//        }
//    }
//    return(false);
//}

/**
 * Returns the itemwise union of two vectors of vectors of SO6
 * @param a a vector of vectors of SO6
 * @param a another vector of vectors of SO6
 * @return m, where m[i] = a[i] union b[i]
 */
//vector<vector<SO6>> mergedVect(vector<vector<SO6>>& a, vector<vector<SO6>>& b){
//    //Takes two vectors of vectors of equal length a and b
//    //and gives back the vector m where m[i] = a[i] union b[i]
//    vector<vector<SO6>> m;
//    vector<SO6> u;
//    for(int i = 0; i<a.size(); i++){
//        u = vector<SO6>();
//        for(int j = 0; j<a[i].size(); j++)
//            u.push_back(a[i][j]);
//        for(int j = 0; j<b[i].size(); j++)
//            u.push_back(b[i][j]);
//        m.push_back(u);
//    }
//    return(m);
//}

//vector<SO6> mergedVect(vector<SO6>& a, vector<SO6>& b){
//    vector<SO6> m;
//    for(int i = 0; i<a.size(); i++)
//        m.push_back(a[i]);
//    for(int i = 0; i<b.size(); i++)
//        m.push_back(b[i]);
//    return m;
//}

// Divides a vect of vects evenly over every LDE into numThreads vectors of vectors
//vector<vector<vector<SO6>>> divideVect(vector<vector<SO6>>& toDivide, int numThreads){
//    vector<vector<vector<SO6>>> toReturn(numThreads, vector<vector<SO6>>(toDivide.size()));
//    int numberthread;
//    int remaining;
//    for(int i = 0; i<toDivide.size(); i++){
//        numberthread = toDivide[i].size()/numThreads;
//        for(int j = 0; j<numThreads; j++){
//            for(int k = j*numberthread; k<(j+1)*numberthread; k++){
//                toReturn[j][i].push_back(toDivide[i][k]);
//            }
//        }
//        remaining = toDivide[i].size()-numberthread*numThreads;
//        for(int j = 0; j<remaining; j++){
//            toReturn[j][i].push_back(toDivide[i][numberthread*numThreads+j]);
//        }
//    }
//    return toReturn;
//}

//vector<vector<SO6>> divideVect(vector<SO6>& toDivide, int numThreads){
//    vector<vector<SO6>> toReturn(numThreads);
//    int numberPerThread = toDivide.size() / numThreads;
//    for(int i = 0; i<numThreads; i++){
//        for(int j = i*numberPerThread; j<(i+1)*numberPerThread; j++){
//            toReturn[i].push_back(toDivide[j]);
//        }
//    }
//    for(int i = numThreads*numberPerThread; i<toDivide.size(); i++)
//        toReturn[i-numThreads*numberPerThread].push_back(toDivide[i]);
//    return toReturn;
//}


/**
 * multiplies every entry of a vector of SO6 with every entry of another vector, and returns it sorted by LDE
 * @param oneLDE vector of SO6
 * @param tCount the T-count that the vector came from (used to determine the number of LDEs of the returned vector)
 * @param tmats another vector of SO6 to multiply, we will always pass it the vector of the 15 T-Count 1 matrices though
 * @return a vector of vectors of SO6, containing the products of elements of tCounts and oneLDE, sorted by LDE
 */
vector<vector<SO6>> genProds(vector<vector<SO6>>& batch, vector<SO6>& tmats){
    // Takes one strata of LDEs from T count tCount, and multiplies by all T-count 1 matrices
    // Does NOT check for permutation
    int numLDES = batch.size()+1;
    SO6 prod;
    vector<vector<SO6>> toReturn(numLDES);
    for(int i = 0; i<15; i++){
        for(int j = 0; j<batch.size(); j++){
            for(SO6 m : batch[j]){
                prod = tmats[i]*m;
                toReturn[prod.getLDE()].push_back(prod);
            }
        }
    }
    return(toReturn);
}


/**
 * Takes a vector of vectors of SO6, all with the same T-Count, broken out by LDE, and multiplies each element
 * by those of another vector of SO6 and returns them sorted by LDE as well
 * @param TminusOne the vector of vectors of SO6, sorted by LDE, to be multiplied
 * @param tcount the T-Count of TminusOne
 * @param tmats the vector of SO6 whose elements will be multiplying those of TminusOne
 * @return the product of all elements of TminusOne and tmats, sorted by LDE
 */
vector<vector<SO6>> genAllProds(vector<vector<SO6>>& TminusOne, vector<SO6>& tmats, int numThreads){
    vector<vector<vector<SO6>>> threadinput = divideVect(TminusOne, numThreads);
    future<vector<vector<SO6>>> threads[numThreads];
    vector<vector<SO6>> prod[numThreads];
    for(int i = 0; i<numThreads; i++)
        threads[i] = async(launch::async, genProds, ref(threadinput[i]), ref(tmats));
    for(int i = 0; i<numThreads; i++){
        prod[i] = threads[i].get();
    }
    vector<vector<SO6>> toReturn(prod[0].size());
    for(int i = 0; i<numThreads; i++)
        toReturn = mergedVect(toReturn, prod[i]);
    return(toReturn);
}

/**
 * Takes a vector of SO6 all with the same LDE, and compares to same LDE of lower T counts to check for permutation
 * @param oneLDE A vector of SO6 all with the same LDE
 * @param LDE the LDE of oneLDE
 * @param lowerTs the vector the distributions of all lower T-count operators
 * @return a vector of all elements of oneLDE not equivalent up to signed column-permutation, to any elements of lowerTs
 */

vector<vector<SO6>> genPerms(vector<vector<SO6>>& batch, vector<vector<SO6>>& tMinusTwo, int numofthread){
    // Takes one strata of LDEs with LDE LDE, and then compares with relevant prior sets in lowerTs
    vector<vector<SO6>> toReturn(batch.size());
    bool isPerm;
    int LDE;
    for(int i = 0; i < batch.size(); i++){
        for(SO6 m : batch[i]){
            isPerm = false;
            LDE = m.getLDE();
            if(LDE<tMinusTwo.size()){
                isPerm = isPerm || containedIn(tMinusTwo[LDE], m);
            }
            isPerm = isPerm || containedIn(toReturn[LDE], m);
            if(!isPerm)
                toReturn[LDE].push_back(m);
        }
    }
    return(toReturn);
}

int getLength(vector<vector<SO6>>& input){
    int l = 0;
    for(int i = 0; i<input.size(); i++){
        l+= input[i].size();
    }
    return l;
}


//Prunes a vector of SO6 for equivalent matrices
void genPerm(vector<SO6>& vec, SO6& check, int a, int b){
    for(int i = a; i<b; i++){
        if(check==vec[i]){
            vec[i].setName("None");
        }
    }
}

//bool isNone(SO6& toCheck){
//    return(toCheck.getName()=="None");
//}

/**
 * Takes a vector of vectors of SO6, all with the same T-Count, broken out by LDE,
 * and checks for equivalence to lower T-Count operators
 * @param unReduced the vector of vectors of SO6, sorted by LDE, to be checked
 * @param tMinusTwo the vector the distribution of T-count N-2
 * @return a vector of vectors of SO6, broken out by LDE, of the unique elements of unReduced, up to signed column permutation
 */

//std::vector<std::vector<SO6>> genAllPerms(std::vector<std::vector<SO6>>& unReduced, std::vector<std::vector<SO6>>& tMinusTwo, int numThreads){
//    // Checking compared to tMinusTwo
//    // Works
//    std::vector<std::vector<SO6>> toReturn(unReduced.size());
//    std::vector<std::vector<std::vector<SO6>>> threadinput = divideVect(unReduced, numThreads);
//    std::future<std::vector<std::vector<SO6>>> threads[numThreads];
//    std::vector<std::vector<SO6>> prod[numThreads];
//    for(int i = 0; i<numThreads; i++){
//        threads[i] = std::async(std::launch::async, genPerms, std::ref(threadinput[i]), std::ref(tMinusTwo), i);
//    }
//    for(int i = 0; i<numThreads; i++){
//        prod[i] = threads[i].get();
//    }
//    toReturn = std::vector<std::vector<SO6>>(unReduced.size());
//    for(std::vector<std::vector<SO6>> v : prod){
//        toReturn = mergedVect(toReturn, v);
//    }
//    // Self Checking. Not yet working
//    // For some reason the numbers change based on the number of threads
//    // Works with 1 thread, but breaks for more than that
//    std::thread threads2[numThreads];
//    int numPerThread;
//
//    //iterating over all LDEs
//    for(int i = 0; i<toReturn.size(); i++){
//        std::cout<<toReturn[i].size()<<"\n";
//
//        //iterating over all entries
//        for(int j = 0; j<toReturn[i].size(); j++){
//            //popping off entries marked as "None"
//            while(toReturn[i][j].getName() == "None" && (j<toReturn.size()-1)) ++j;
//            //determining how many elements per thread
//            //Looking at elements to the right of j and seeing if equivalent
//
//            //Finding the number per thread. Notice this is integer division, so the floor of the division
//            numPerThread = (toReturn[i].size()-j-1)/numThreads;
//            //allocating to threads
//            //will turn equivalent matrices to "None" name
//            for(int k = 0; k<numThreads-1; k++){
//                //looking through toReturn[i] in parallel
//                threads2[k] = std::thread(genPerm, std::ref(toReturn[i]), std::ref(toReturn[i][j]), j+1+k*numPerThread, j+1+(k+1)*numPerThread);
//            }
//            //making sure all elements of toReturn[i] are allocated to a thread to search through
//            threads2[numThreads-1] = std::thread(genPerm, std::ref(toReturn[i]), std::ref(toReturn[i][j]), j+1+(numThreads-1)*numPerThread, toReturn[i].size());
//            //waiting for all to be performed
//            for(int k = 0; k<numThreads; k++){
//                threads2[k].join();
//            }
//        }
//        //Removes all "None" SO6s
//        toReturn[i].erase(std::remove_if(toReturn[i].begin(), toReturn[i].end(), isNone), toReturn[i].end());
//    }
//    return toReturn;
//}

//vector<int> findRedundantIndices(SO6 unReducedOneLDE[], vector<SO6>& tMinusTwo, int numThreads){
//    int indices[unReduced.size()];
//    int numPerThread = unReduced.size()/numThreads;
//    thread threads[numThreads];
//    for(int i =0; i < numThreads -1; i++){
//        threads[i] = thread(checkPerm)
//    }
//}


int main(){
    //generating list of T matrices
    //in the order Andrew wanted
    vector<SO6> ts; //t count 1 matrices
    for(int i = 0; i<15; i++){
        if(i<5)
            ts.push_back(tMatrix(0,i+1));
        else if(i<9)
            ts.push_back(tMatrix(1, i-3));
        else if(i<12)
            ts.push_back(tMatrix(2, i-6));
        else if(i<14)
            ts.push_back(tMatrix(3, i-8));
        else
            ts.push_back(tMatrix(4,5));
    }
    cout<<"Generated T count 1 \n";
    vector<vector<SO6>> t1 = vector<vector<SO6>>{vector<SO6>(), ts}; //vector of the distributions of odd t-count matrices

    cout<<t1.size()<<"\n";
    //generating t count 2
    //for some reason this generates 166 as opposed to 165
    //the weird thing is that the higher t counts all agree perfectly with Andrew
    //The reason is that it doesn't catch the first operator which is just a Clifford operator
    //leaving it for now
    //int numThreads;
    //cin>>numThreads;
    vector<vector<SO6>> t0;
    vector<vector<SO6>> t2;
    vector<vector<SO6>> t2s;
    t2 = genAllProds(t1, ts, numThreads);
    t2 = genAllPerms(t2, t0, numThreads);
    cout<<"Generated T count 2 \n";
    for(int i = 0; i<t2.size(); i++){
        cout<<"LDE"<<i<<": "<<t2[i].size()<< "\n";
    }

    //generating t count 3
    std::vector<std::vector<SO6>> t3;
    t3 = genAllProds(t2, ts, 7);
    t3 = genAllPerms(t3, t1, 7);

    std::cout<<"Generated T Count 3 \n";
    for(int i = 0; i<t3.size(); i++){
        std::cout<<"LDE"<<i<<": "<<t3[i].size()<< "\n";
    }


    //generating t count 4
    //this step takes something in the order of 100 times longer,
    //Reduced runtime including this to ~1-2 minutes, but it goes from 3 seconds to get to T-Count 3
    //to that for 4... not a good growth rate
    std::vector<std::vector<SO6>> t4(5);
    t4  = genAllProds(t3, ts, 7);
//    for(int i = 0; i<t4.size(); i++){
//        std::cout<<"LDE"<<i<<": "<<t4[i].size()<< "\n";
//    }

    t4 = genAllPerms(t4, t2, 5);
    std::cout<<"Generated T Count 4 \n";
    for(int i = 0; i<t4.size(); i++){
        std::cout<<"LDE"<<i<<": "<<t4[i].size()<< "\n";
    }
//
//    std::vector<std::vector<SO6>> t5(6);
//    t5 = genAllProds(t4, 4, ts);
//    for(int i = 0; i<t5.size(); i++){
//        std::cout<<"LDE"<<i<<": "<<t5[i].size()<< "\n";
//    }
//
////    t5 = genAllPerms(t5, odds);
////    std::cout<<"Generated T Count 5 \n";
////    for(int i = 0; i<t5.size(); i++){
////        std::cout<<"LDE"<<i<<": "<<t5[i].size()<< "\n";
////    }

    return 0;
}



