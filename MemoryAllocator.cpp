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

static constexpr uint32_t MARKER = 0xDEADBEEFul;

/**
 * Constructor for MemoryAllocator.
 * @param memoryPool - pointer to the memory pool
 * @param memoryPoolSize - size of the memory pool
 */
MemoryAllocator::MemoryAllocator() : mHead(nullptr),
                                     mTail(nullptr),
                                     mPool(nullptr),
                                     mPoolSize(0u)
{
}

/**
 * Constructor for MemoryAllocator.
 * @param memoryPool - pointer to the memory pool
 * @param memoryPoolSize - size of the memory pool
 */
void MemoryAllocator::init(void *memoryPool, uint32_t totalSize)
{
    mHead = (Block *)memoryPool;
    mHead->size = totalSize - sizeof(Block) - 2 * sizeof(uint32_t);
    mHead->free = true;
    mHead->prev = nullptr;
    mHead->next = nullptr;
    mHead->startMarker = MARKER;
    mHead->endMarker = MARKER;

    mPool = memoryPool;
    mPoolSize = totalSize;
    mTail = mHead;
}

/**
 * Allocates a block of memory of the specified size.
 * @param size - size of the block to allocate
 * @return pointer to the allocated memory or nullptr if there is not enough memory
 */
void* MemoryAllocator::allocate(uint32_t size)
{
    if (size == 0)
    {
        return nullptr;
    }

    size = align8(size);

    Block* forward = mHead;
    Block* backward = mTail;

    while (forward || backward)
    {
        if (forward)
        {
            if (forward->free && forward->size >= size)
            {
                if (forward->size >= size + sizeof(Block) + 2 * sizeof(uint32_t))
                {
                    split(forward, size);
                }
                forward->free = false;
                return (void*)((char*)forward + sizeof(Block) + sizeof(uint32_t));
            }
            forward = forward->next;
        }

        if (backward && backward != forward)
        {
            if (backward->free && backward->size >= size)
            {
                if (backward->size >= size + sizeof(Block) + 2 * sizeof(uint32_t))
                {
                    split(backward, size);
                }
                backward->free = false;
                return (void*)((char*)backward + sizeof(Block) + sizeof(uint32_t));
            }
            backward = backward->prev;
        }
    }

    return nullptr;
}

/**
 * Frees a previously allocated block of memory.
 * @param ptr - pointer to the block of memory to free
 */
void MemoryAllocator::deallocate(void *ptr)
{
    if (!ptr)
    {
        return;
    }

    Block *block = (Block *)((char *)ptr - sizeof(Block) - sizeof(uint32_t));
    if (block->startMarker != MARKER || block->endMarker != MARKER)
    {
        assert(false);
        return;
    }
    block->free = true;

    if (block->prev && block->prev->free)
    {
        block->prev->size += block->size + sizeof(Block) + 2 * sizeof(uint32_t);
        block->prev->next = block->next;

        if (block->next)
        {
            block->next->prev = block->prev;
        }

        block->prev->endMarker = MARKER;
        block = block->prev;
    }
    if (block->next && block->next->free)
    {
        block->size += block->next->size + sizeof(Block) + 2 * sizeof(uint32_t);
        block->next = block->next->next;

        if (block->next)
        {
            block->next->prev = block;
        }

        block->endMarker = MARKER;
    }

    if (block->next == nullptr)
    {
        mTail = block;
    }

    join(block);
}

uint32_t MemoryAllocator::align8(uint32_t size)
{
    return (size + 7) & ~7;
}

void MemoryAllocator::split(Block *block, uint32_t size)
{
    Block *newBlock = (Block *)((char *)block + sizeof(Block) + size + 2 * sizeof(uint32_t));
    newBlock->size = block->size - size - sizeof(Block) - 2 * sizeof(uint32_t);
    newBlock->free = true;
    newBlock->prev = block;
    newBlock->next = block->next;
    newBlock->startMarker = MARKER;
    newBlock->endMarker = MARKER;
    if (block->next)
    {
        block->next->prev = newBlock;
    }
    block->next = newBlock;
    block->size = size;
    block->endMarker = MARKER;

    if (newBlock->next == nullptr)
    {
        mTail = newBlock;
    }
}

void MemoryAllocator::join(Block *block)
{
    if (block->prev && block->prev->free)
    {
        block->prev->size += block->size + sizeof(Block) + 2 * sizeof(uint32_t);
        block->prev->next = block->next;

        if (block->next)
        {
            block->next->prev = block->prev;
        }

        block->prev->endMarker = MARKER;
        block = block->prev;
    }

    if (block->next && block->next->free)
    {
        block->size += block->next->size + sizeof(Block) + 2 * sizeof(uint32_t);
        block->next = block->next->next;

        if (block->next)
        {
            block->next->prev = block;
        }

        block->endMarker = MARKER;
    }

    if (block->next == nullptr)
    {
        mTail = block;
    }
}

/**
 * Returns the total amount of free memory.
 * @return size of free memory
 */
uint32_t MemoryAllocator::getFreeMemory() const
{
    uint32_t freeMemory = 0;
    Block *current = mHead;

    while (current)
    {
        if (current->free)
        {
            freeMemory += current->size;
        }
        current = current->next;
    }
    return freeMemory;
}

/**
 * Returns the total amount of allocated memory.
 * @return size of allocated memory
 */
uint32_t MemoryAllocator::getAllocatedMemory() const
{
    uint32_t allocatedMemory = 0;
    Block *current = mHead;
    while (current)
    {
        if (!current->free)
        {
            allocatedMemory += current->size;
        }
        current = current->next;
    }
    return allocatedMemory;
}

/**
 * Prints information about the allocated blocks.
 */
void MemoryAllocator::printAllocatedBlocks() const
{
    Block *current = mHead;
    printf("Allocated Blocks:\r\n");
    while (current)
    {
        if (!current->free)
        {
            printf("Address: %p, Size: %lu bytes\r\n", (void*)((char*)current + sizeof(Block) + sizeof(uint32_t)), current->size);
        }
        current = current->next;
    }
}
