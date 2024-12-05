# Overview
The MemoryAllocator class is designed to manage a pool of memory by allowing allocation and deallocation of memory blocks. This allocator uses a custom memory pool, effectively managing free and allocated blocks. It is implemented using a linked list structure with each block containing information about its size, whether it is free, and pointers to the next and previous blocks.

# Implementation Details
The allocator uses the following data structures and methods to manage the memory pool:

<ol>
  <h3><li>Block Structure:</li></h3>
  <ul>
    <li>size_t size: Size of the memory block.</li>
    <li>bool isFree: Indicates if the block is free.</li>
    <li>std::unique_ptr<Block> next: Pointer to the next block.</li>
    <li>Block* prev: Pointer to the previous block.</li>
  </ul>


<h3><li>MemoryAllocator Class</li></h3>

<ul>
  <h3><li>Constructor:</li></h3>
  <ul>
    <li>MemoryAllocator(void* memoryPool, size_t memoryPoolSize): Initializes the memory allocator with a given memory pool and its size.</li>
  </ul>

  <h3><li>Allocation Method:</li></h3>
  <ul>
    <li>void* allocate(size_t size): Allocates a memory block of the specified size.</li>
  </ul>

  <h3><li>Deallocation Method:</li></h3>
  <ul>
    <li>void deallocate(void* ptr): Frees a previously allocated memory block.</li>
  </ul>

  <h3><li>Memory Statistics Methods:</li></h3>
  <ul>
    <li>size_t getFreeMemory() const: Returns the total amount of free memory.</li>
    <li>size_t getAllocatedMemory() const: Returns the total amount of allocated memory.</li>
    <li>size_t getAllocatedBlocks() const: Returns the number of allocated blocks.</li>
    <li>void printAllocatedBlocks() const: Prints information about the allocated blocks.</li>
  </ul>
</ul>
</ol>

# Function Descriptions
<ol>
  <h3><li>MemoryAllocator(void* memoryPool, size_t memoryPoolSize)</li></h3>
  <ul>
    <li>Initializes the memory pool by creating the initial free block encompassing the entire memory pool.</li>
    <h5><li>Parameters:</li></h5>
    <ul>
      <li>void* memoryPool: Pointer to the memory pool.</li>
      <li>size_t memoryPoolSize: Size of the memory pool.</li>
    </ul>
  </ul>

  <h3><li>void allocate(size_t size)</li></h3>
  <ul>
    <li>Allocates a memory block of the specified size.</li>
    <li>Searches the list of blocks for a suitable free block.</li>
    <li>If a suitable block is found, it is allocated, and the remaining free space (if any) is split into a new free block.</li>
    <h5><li>Parameters:</li></h5>
    <ul>
      <li>size_t size: Size of the block to allocate.</li>
    </ul>
    <h5><li>Returns:</li></h5>
    <ul>
      <li>Pointer to the allocated memory block, or nullptr if no suitable block is found.</li>
    </ul>
  </ul>

  <h3><li>void deallocate(void *ptr)</li></h3>
  <ul>
    <li>Frees a previously allocated memory block.</li>
    <li>Marks the block as free and attempts to merge it with adjacent free blocks to reduce fragmentation.</li>
    <h5><li>Parameters:</li></h5>
    <ul>
      <li>void* ptr: Pointer to the block to deallocate.</li>
    </ul>
  </ul>

  <h3><li>size_t getFreeMemory() const</li></h3>
  <ul>
    <li>Computes and returns the total amount of free memory.</li>
    <h5><li>Returns:</li></h5>
    <ul>
      <li>Total size of free memory.</li>
    </ul>
  </ul>

  <h3><li>size_t getAllocatedMemory() const</li></h3>
  <ul>
    <li>Computes and returns the total amount of allocated memory.</li>
    <h5><li>Returns:</li></h5>
    <ul>
      <li>Total size of allocated memory.</li>
    </ul>
  </ul>

  <h3><li>size_t getAllocatedBlocks() const</li></h3>
  <ul>
    <li>Computes and returns the number of allocated blocks.</li>
    <h5><li>Returns:</li></h5>
    <ul>
      <li>Number of allocated blocks.</li>
    </ul>
  </ul>

  <h3><li>void printAllocatedBlocks() const</li></h3>
  <ul>
    <li>Prints information about the allocated blocks, including their sizes.</li>
  </ul>
</ol>

# Flow diagram
![codetoflow](https://github.com/user-attachments/assets/442de0e9-c296-45cd-b51c-dcfcab41ffdb)
