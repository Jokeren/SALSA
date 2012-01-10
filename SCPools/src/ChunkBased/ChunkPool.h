/*
 * ChunkPool.h
 *
 *  Created on: Dec 29, 2011
 *      Author: dima39
 */

#ifndef CHUNKPOOL_H_
#define CHUNKPOOL_H_

#include "SPChunk.h"

class ChunkPool {
public:
	ChunkPool(int initialSize);
	virtual ~ChunkPool();

	SPChunk* getChunk(); // returns NULL if no chunk available
	void putChunk(SPChunk* c);
};

#endif /* CHUNKPOOL_H_ */
