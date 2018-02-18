/*
 * CS252: MyMalloc Project
 *
 * The current implementation gets memory from the OS
 * every time memory is requested and never frees memory.
 *
 * You will implement the allocator as indicated in the handout,
 * as well as the deallocator.
 *
 * You will also need to add the necessary locking mechanisms to
 * support multi-threaded programs.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <pthread.h>
#include "MyMalloc.h"

static pthread_mutex_t mutex;

const int arenaSize = 2097152;

void increaseMallocCalls()  { _mallocCalls++; }

void increaseReallocCalls() { _reallocCalls++; }

void increaseCallocCalls()  { _callocCalls++; }

void increaseFreeCalls()    { _freeCalls++; }

extern void atExitHandlerInC()
{
    atExitHandler();
}

/* 
 * Initial setup of allocator. First chunk is retrieved from the OS,
 * and the fence posts and freeList are initialized.
 */
void initialize()
{
    // Environment var VERBOSE prints stats at end and turns on debugging
    // Default is on
    _verbose = 1;
    const char *envverbose = getenv("MALLOCVERBOSE");
    if (envverbose && !strcmp(envverbose, "NO")) {
        _verbose = 0;
    }

    pthread_mutex_init(&mutex, NULL);
    void *_mem = getMemoryFromOS(arenaSize);

    // In verbose mode register also printing statistics at exit
    atexit(atExitHandlerInC);

    // establish fence posts
    ObjectHeader * fencePostHead = (ObjectHeader *)_mem;
    fencePostHead->_allocated = 1;
    fencePostHead->_objectSize = 0;

    char *temp = (char *)_mem + arenaSize - sizeof(ObjectHeader);
    ObjectHeader * fencePostFoot = (ObjectHeader *)temp;
    fencePostFoot->_allocated = 1;
    fencePostFoot->_objectSize = 0;

    // Set up the sentinel as the start of the freeList
    _freeList = &_freeListSentinel;

    // Initialize the list to point to the _mem
    temp = (char *)_mem + sizeof(ObjectHeader);
    ObjectHeader *currentHeader = (ObjectHeader *)temp;
    currentHeader->_objectSize = arenaSize - (2*sizeof(ObjectHeader)); // ~2MB
    currentHeader->_leftObjectSize = 0;
    currentHeader->_allocated = 0;
    currentHeader->_listNext = _freeList;
    currentHeader->_listPrev = _freeList;
    _freeList->_listNext = currentHeader;
    _freeList->_listPrev = currentHeader;

    // Set the start of the allocated memory
    _memStart = (char *)currentHeader;

    _initialized = 1;
}

/* 
 * TODO: In allocateObject you will handle retrieving new memory for the malloc
 * request. The current implementation simply pulls from the OS for every
 * request.
 *
 * @param: amount of memory requested
 * @return: pointer to start of useable memory
 */
void * allocateObject(size_t size)
{
    // Make sure that allocator is initialized
    if (!_initialized)
        initialize();
    /* Add the ObjectHeader to the size and round the total size up to a 
     * multiple of 8 bytes for alignment.
     */
    size_t roundedSize = (size + sizeof(ObjectHeader) + 7) & ~7;

    // Naively get memory from the OS every time
    //void *_mem = getMemoryFromOS(arenaSize); 

    // Store the size in the header
    //ObjectHeader *o = (ObjectHeader *)_mem;
    //o->_objectSize = roundedSize;
   ObjectHeader* o = &_freeListSentinel;
   int state = -1;
   do {
  	if(o->_objectSize == roundedSize) {
		state = 0;
		break;
	} else if( o->_objectSize > roundedSize) {
		state = 1;
		break;
			
	}	
	   o = o->_listNext;
   } while(o!=&_freeListSentinel);
   if(state == -1) {
	//2MB has to be requested
	ObjectHeader * _mem = (ObjectHeader*)(getMemoryFromOS(2000));
	o = (_freeListSentinel._listNext);
	_freeListSentinel._listNext = _mem;
	_mem->_listPrev = &_freeListSentinel;
	_mem->_listNext = o;
	o->_listPrev = _mem;
	o = _mem;
    }
    if(state == 0) {
    	//block size matches request size
    	return (void *)((char *)o + sizeof(ObjectHeader));
    }
    //block has to be split
    ((ObjectHeader*)((char*)o+o->_objectSize))->_leftObjectSize = roundedSize;
    o->_objectSize -= roundedSize;
    ObjectHeader * header2 = (ObjectHeader*)(((char*)o)+o->_objectSize);
    header2->_objectSize = roundedSize;// - sizeof(ObjectHeader);
    header2->_leftObjectSize = o->_objectSize;
    header2->_allocated = 1;
    header2->_listNext = o->_listNext;
    header2->_listPrev = o->_listPrev;
    
    pthread_mutex_unlock(&mutex);
    // Return a pointer to useable memory
    return (void *)((char *)header2 + sizeof(ObjectHeader));
    //return (void *)((char *)o + sizeof(ObjectHeader));
}

/* 
 * TODO: In freeObject you will implement returning memory back to the free
 * list, and coalescing the block with surrounding free blocks if possible.
 *
 * @param: pointer to the beginning of the block to be returned
 * Note: ptr points to beginning of useable memory, not the block's header
 */
void freeObject(void *ptr)
{
    // Add your implementation here 
    ObjectHeader * header = (ptr-sizeof(ObjectHeader));
    header->_allocated = 0;
    //Coalescing business here
    //check if rightside is free and merge on.
    ObjectHeader * right =(ObjectHeader*) ((char*)header+(header->_objectSize));
    if(right!=&_freeListSentinel&&right->_allocated == 0) {
    	
    	//merge with right.
	header->_objectSize = header->_objectSize + right->_objectSize;// + sizeof(ObjectHeader);
	//header->_listNext = right->_listNext;
	//right->_listNext->_leftObjectSize = header->_objectSize;
	//right->_listNext->_listPrev=header;
//	right = header->_listNext;
	header->_listNext = right->_listNext;
	header->_listPrev = right->_listPrev;
	header->_listNext->_listPrev = header;
	header->_listPrev->_listNext = header;
    }

    ObjectHeader * left = (ObjectHeader*)((char*)header - (header->_leftObjectSize  /*+sizeof(ObjectHeader)*/));

    //printf("%ld\t%zd\t%ld\t%d\n",(long)(header)-(long)(_memStart),header->_objectSize,(long)left-(long)(_memStart),left->_allocated);
    //left = &_freeListSentinel;
    if(left!=&_freeListSentinel&&left->_allocated == 0) {
    	//merge with left
    	if(right!=&_freeListSentinel && right->_allocated == 0) {
		left->_listNext = header->_listNext;
		left->_listNext->_listPrev = left;
	}
	ObjectHeader * right = header;
	header = left;
	header->_objectSize = header->_objectSize + right->_objectSize;// + sizeof(ObjectHeader);
    
    } else if(!(right!=&_freeListSentinel&&right->_allocated == 0)) {
    	header->_listNext = _freeListSentinel._listNext;
    	header->_listPrev = &_freeListSentinel;
    	_freeListSentinel._listNext->_listPrev = header;
    	_freeListSentinel._listNext = header;
    }   
    return;
}

/* 
 * Prints the current state of the heap.
 */
void print()
{
    printf("\n-------------------\n");

    printf("HeapSize:\t%zd bytes\n", _heapSize );
    printf("# mallocs:\t%d\n", _mallocCalls );
    printf("# reallocs:\t%d\n", _reallocCalls );
    printf("# callocs:\t%d\n", _callocCalls );
    printf("# frees:\t%d\n", _freeCalls );

    printf("\n-------------------\n");
}

/* 
 * Prints the current state of the freeList
 */
void print_list() {
    printf("FreeList: ");
    if (!_initialized) 
        initialize();

    ObjectHeader * ptr = _freeList->_listNext;

    while (ptr != _freeList) {
        long offset = (long)ptr - (long)_memStart;
        printf("[offset:%ld,size:%zd]", offset, ptr->_objectSize);
        ptr = ptr->_listNext;
        if (ptr != NULL)
            printf("->");
    }
    printf("\n");
}

/* 
 * This function employs the actual system call, sbrk, that retrieves memory
 * from the OS.
 *
 * @param: the chunk size that is requested from the OS
 * @return: pointer to the beginning of the chunk retrieved from the OS
 */
void * getMemoryFromOS(size_t size)
{
    _heapSize += size;

    // Use sbrk() to get memory from OS
    void *_mem = sbrk(size);

    // if the list hasn't been initialized, initialize memStart to mem
    if (!_initialized)
        _memStart = _mem;

    return _mem;
}

void atExitHandler()
{
    // Print statistics when exit
    if (_verbose)
        print();
}

/*
 * C interface
 */

extern void * malloc(size_t size)
{
    pthread_mutex_lock(&mutex);
    increaseMallocCalls();

    return allocateObject(size);
}

extern void free(void *ptr)
{
    pthread_mutex_lock(&mutex);
    increaseFreeCalls();

    if (ptr == 0) {
        // No object to free
        pthread_mutex_unlock(&mutex);
        return;
    }

    freeObject(ptr);
}

extern void * realloc(void *ptr, size_t size)
{
    pthread_mutex_lock(&mutex);
    increaseReallocCalls();

    // Allocate new object
    void *newptr = allocateObject(size);

    // Copy old object only if ptr != 0
    if (ptr != 0) {

        // copy only the minimum number of bytes
        ObjectHeader* hdr = (ObjectHeader *)((char *) ptr - sizeof(ObjectHeader));
        size_t sizeToCopy =  hdr->_objectSize;
        if (sizeToCopy > size)
            sizeToCopy = size;

        memcpy(newptr, ptr, sizeToCopy);

        //Free old object
        freeObject(ptr);
    }

    return newptr;
}

extern void * calloc(size_t nelem, size_t elsize)
{
    pthread_mutex_lock(&mutex);
    increaseCallocCalls();

    // calloc allocates and initializes
    size_t size = nelem *elsize;

    void *ptr = allocateObject(size);

    if (ptr) {
        // No error; initialize chunk with 0s
        memset(ptr, 0, size);
    }

    return ptr;
}

