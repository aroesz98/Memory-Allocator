/*
 * MemoryAllocator
 * Author: Arkadiusz Szlanta
 * Date: 05 Dec 2024
 *
 * License:
 * Use at your own risk. The author(s) hold no responsibility for any damages
 * or losses resulting from the use of this software.
 */

#ifndef MEMORYALLOCATOR_HPP
#define MEMORYALLOCATOR_HPP

#include <cstddef>
#include <memory>

class MemoryAllocator
{
    public:
        /**
         * Constructor for MemoryAllocator.
         */
        MemoryAllocator();

        /**
         * Initializes the memory pool.
         * @param memoryPool - pointer to the memory pool
         * @param memoryPoolSize - size of the memory pool
         */
        void init(void* memoryPool, uint32_t memoryPoolSize);

        /**
         * Allocates a block of memory of the specified size.
         * @param size - size of the block to allocate
         * @return pointer to the allocated memory or nullptr if there is not enough memory
         */
        void* allocate(uint32_t size);

        /**
         * Frees a previously allocated block of memory.
         * @param ptr - pointer to the block of memory to free
         */
        void deallocate(void* ptr);

        /**
         * Returns the total amount of free memory.
         * @return size of free memory
         */
        uint32_t getFreeMemory() const;

        /**
         * Returns the total amount of allocated memory.
         * @return size of allocated memory
         */
        uint32_t getAllocatedMemory() const;

        /**
         * Prints information about the allocated blocks.
         */
        void printAllocatedBlocks() const;

    private:
        struct Block
        {
            uint32_t size;
            bool free;
            Block *prev;
            Block *next;
            uint32_t startMarker;
            uint32_t endMarker;
        };

        Block *mHead;
        Block *mTail;
        void *mPool;
        uint32_t mPoolSize;

        uint32_t align8(uint32_t size);
        void split(Block *block, uint32_t size);
        void join(Block *block);
};

#endif // MEMORYALLOCATOR_HPP
