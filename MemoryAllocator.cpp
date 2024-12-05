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

#include <MemoryAllocator.hpp>
#include <cassert>
#include <cstdio>

/**
 * Constructor for MemoryAllocator.
 * @param memoryPool - pointer to the memory pool
 * @param memoryPoolSize - size of the memory pool
 */
MemoryAllocator::MemoryAllocator(void* memoryPool, size_t memoryPoolSize)
    : mMemoryPool(reinterpret_cast<char*>(memoryPool)), mMemoryPoolSize(memoryPoolSize)
{
    Block* initialBlock = reinterpret_cast<Block*>(memoryPool);
    initialBlock->size = memoryPoolSize - sizeof(Block);
    initialBlock->isFree = true;
    initialBlock->next = nullptr;
    initialBlock->prev = nullptr;

    mHead.reset(initialBlock);
}

/**
 * Allocates a block of memory of the specified size.
 * @param size - size of the block to allocate
 * @return pointer to the allocated memory or nullptr if there is not enough memory
 */
void* MemoryAllocator::allocate(size_t size)
{
    assert(size > 0 && "Allocation size must be greater than zero");

    Block* current = mHead.get();

    while (current != nullptr)
    {
        if (current->isFree && current->size >= size)
        {
            if (current->size > size + sizeof(Block))
            {
                auto newBlock = std::make_unique<Block>();

                newBlock->size = current->size - size - sizeof(Block);
                newBlock->isFree = true;
                newBlock->next = std::move(current->next);
                newBlock->prev = current;

                current->size = size;
                current->next = std::move(newBlock);

                if (current->next->next != nullptr)
                {
                    current->next->next->prev = current->next.get();
                }
            }

            current->isFree = false;
            return reinterpret_cast<void*>(reinterpret_cast<char*>(current) + sizeof(Block));
        }

        current = current->next.get();
    }

    return nullptr;
}

/**
 * Frees a previously allocated block of memory.
 * @param ptr - pointer to the block of memory to free
 */
void MemoryAllocator::deallocate(void* ptr)
{
    if (ptr == nullptr) return;

    Block* blockToFree = reinterpret_cast<Block*>(reinterpret_cast<char*>(ptr) - sizeof(Block));
    assert(blockToFree >= mHead.get() && "Memory pool boundary exceeded");
    blockToFree->isFree = true;

    if (blockToFree->next != nullptr && blockToFree->next->isFree)
    {
        blockToFree->size += sizeof(Block) + blockToFree->next->size;
        blockToFree->next = std::move(blockToFree->next->next);

        if (blockToFree->next != nullptr)
        {
            blockToFree->next->prev = blockToFree;
        }
    }

    if (blockToFree->prev != nullptr && blockToFree->prev->isFree)
    {
        blockToFree->prev->size += sizeof(Block) + blockToFree->size;
        blockToFree->prev->next = std::move(blockToFree->next);

        if (blockToFree->prev->next != nullptr)
        {
            blockToFree->prev->next->prev = blockToFree->prev;
        }
    }
}

/**
 * Returns the total amount of free memory.
 * @return size of free memory
 */
size_t MemoryAllocator::getFreeMemory() const
{
    Block* current = mHead.get();
    size_t freeMemory = 0;

    while (current != nullptr)
    {
        if (current->isFree)
        {
            freeMemory += current->size;
        }

        current = current->next.get();
    }

    return freeMemory;
}

/**
 * Returns the total amount of allocated memory.
 * @return size of allocated memory
 */
size_t MemoryAllocator::getAllocatedMemory() const
{
    Block* current = mHead.get();
    size_t allocatedMemory = 0;

    while (current != nullptr)
    {
        if (!current->isFree)
        {
            allocatedMemory += current->size;
        }

        current = current->next.get();
    }

    return allocatedMemory;
}

/**
 * Returns the number of allocated blocks.
 * @return number of allocated blocks
 */
size_t MemoryAllocator::getAllocatedBlocks() const
{
    Block* current = mHead.get();
    size_t allocatedBlocks = 0;

    while (current != nullptr)
    {
        if (!current->isFree)
        {
            allocatedBlocks++;
        }

        current = current->next.get();
    }

    return allocatedBlocks;
}

/**
 * Prints information about the allocated blocks.
 */
void MemoryAllocator::printAllocatedBlocks() const
{
    Block* current = mHead.get();
    printf("Number of allocated blocks: %u\n", static_cast<unsigned>(getAllocatedBlocks()));
    printf("Allocated blocks (size in bytes): ");

    while (current != nullptr)
    {
        if (!current->isFree)
        {
            printf("%u ", static_cast<unsigned>(current->size));
        }

        current = current->next.get();
    }

    printf("\n");
}
