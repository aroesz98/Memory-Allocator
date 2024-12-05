/*
 * MemoryAllocator
 * Author: Arkadiusz Szlanta
 * Date: 05 Dec 2024
 *
 * License:
 * This source code is provided for hobbyist and private use only.
 * Any commercial or industrial use, including distribution, reproduction, or
 * incorporation in commercial or industrial products or services is prohibited.
 * Use at your own risk. The author(s) hold no responsibility for any damages
 * or losses resulting from the use of this software.
 *
 */

#ifndef MEMORYALLOCATOR_H
#define MEMORYALLOCATOR_H

#include <cstddef>
#include <memory>

class MemoryAllocator
{
    public:
        /**
         * Constructor for MemoryAllocator.
         * @param memoryPool - pointer to the memory pool
         * @param memoryPoolSize - size of the memory pool
         */
        MemoryAllocator(void* memoryPool, size_t memoryPoolSize);

        /**
         * Allocates a block of memory of the specified size.
         * @param size - size of the block to allocate
         * @return pointer to the allocated memory or nullptr if there is not enough memory
         */
        void* allocate(size_t size);

        /**
         * Frees a previously allocated block of memory.
         * @param ptr - pointer to the block of memory to free
         */
        void deallocate(void* ptr);

        /**
         * Returns the total amount of free memory.
         * @return size of free memory
         */
        size_t getFreeMemory() const;

        /**
         * Returns the total amount of allocated memory.
         * @return size of allocated memory
         */
        size_t getAllocatedMemory() const;

        /**
         * Returns the number of allocated blocks.
         * @return number of allocated blocks
         */
        size_t getAllocatedBlocks() const;

        /**
         * Prints information about the allocated blocks.
         */
        void printAllocatedBlocks() const;

    private:
        struct Block {
            size_t size;
            bool isFree;
            std::unique_ptr<Block> next;
            Block* prev;
        };

        char* mMemoryPool;
        size_t mMemoryPoolSize;
        std::unique_ptr<Block> mHead;
};

#endif // MEMORYALLOCATOR_H
