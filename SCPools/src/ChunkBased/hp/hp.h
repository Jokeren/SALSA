/*
 * hp.h
 *  A straightforward hazard pointer implementation.
 *
 *  Implemented as described in:
 *  "Hazard Pointers: Safe Memory Reclamation for Lock-Free Objects"
 *  (http://www.research.ibm.com/people/m/michael/ieeetpds-2004.pdf)
 *
 *      Author: Elad Gidron
 */

#ifndef HP_H_
#define HP_H_

extern "C" {
#include "stack.h"
#include "simpleSet.h"
}

namespace HP {

//A struct used to initialize the hazard pointers system
//generated by initHPData()
typedef struct HPData_t *HPData;

//A record that holds the hazard pointers of each thread.
typedef struct HPRecord_t HPRecord;

//Hold local data structures needed for each thread.
typedef struct HPLocal_t *HPLocal, HPLocalT;

//Function for creating an HPData for use in the threadRegister function.
HPData initHPData(int hpCountPerThread, int ThreadCount, int recCount);

/*
 * Adds a thread to the HPReord list.
 * Returns an HPLocal var that the thread will hold locally,
 * so it will be used later when accessing hazard pointers.
 * Head - the head of the HPRecord list, or NULL if this is the first element
 * hpData - An HPData object created by initHPData
 */
void threadRegister(HPData hpData);

void resetHPSystem();

HPLocal getHPLocal();

/* Marks that the node needs to be freed and calls scan if needed. */
void retireNode(void* addr, ReclaimationFunc reclaimationFunc,
		HPLocal localData);

/* Set hazard pointer with index of idx to point to addr */
void setHP(int idx, void* addr, HPLocal localData);

/*check if I have a HP pointing at addr - for tests*/
int isHP(void* addr, HPLocal localData);

}
#endif /* HP_H_ */
