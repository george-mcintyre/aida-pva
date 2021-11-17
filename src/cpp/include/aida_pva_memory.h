/** @file
 *  @brief The Header File for the memory management functions and macros.
 *       **CMS**=C_INC
 */
#ifndef aida_pva_memory_h
#define aida_pva_memory_h
#ifdef __cplusplus
extern "C" {
#endif

#include "aida_pva.h"

/**
 * The maximum number of pointers that the Memory tracking and management functions can handle
 */
#define MAX_POINTERS 100

/**
 * Create tracking variables so that memory can be freed with FREE_MEMORY macro.
 * Creates up to MAX_POINTERS pointers to track all memory allocations so that
 * they can be safely freed, when needed.
 * Creates local variables to store the tracking information so these macros can only
 * be used within a single block.
 */
#define TRACK_ALLOCATED_MEMORY \
    int _nAllocationsToFree = 0, _n_jsonValuesToFree = 0; \
    void *_memoryAllocationsToFree[MAX_POINTERS] ; \
    json_value *_jsonValuesToFree[MAX_POINTERS] ;

/**
 * Register this newly allocated memory so that it will be freed by FREE_MEMORY.
 */
#define TRACK_MEMORY(_ptr) \
    if (_ptr) _memoryAllocationsToFree[_nAllocationsToFree++] = (_ptr);

/**
 * Register this newly allocated json value so that it will be freed by FREE_JSON_MEMORY.
 */
#define TRACK_JSON(_ptr) \
    if (_ptr) _jsonValuesToFree[_n_jsonValuesToFree++] = (_ptr);

/**
 * Allocate memory.  Allocates memory of the given size
 *
 * @param _env      The JNI environment.  Used in all functions involving JNI
 * @param _size    size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 */
#define ALLOCATE_MEMORY(_env, _size, _purpose) allocateMemory(_env, NULL, _size, false, "Could not allocate space for " _purpose)

/**
 * Allocate memory and set its contents to the given buffer of given size.
 *
 * @param _env      The JNI environment.  Used in all functions involving JNI
 * @param _source buffer to copy contents from
 * @param _size size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 */
#define ALLOCATE_AND_COPY_MEMORY(_env, _source, _size, _purpose) allocateMemory(_env, _source, _size, false, "Could not allocate space for " _purpose)

/**
 * Allocate memory for a string and copy the given string into this allocated space.
 *
 * @param _env      The JNI environment.  Used in all functions involving JNI
 * @param _string buffer to copy contents from
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 */
#define ALLOCATE_STRING(_env, _string, _purpose) ALLOCATE_AND_COPY_MEMORY(_env, _string, strlen(_string)+1, _purpose)

/**
 * Allocate space for a fixed length string and copy data from the given string into
 * the newly allocated space.  You need to specify size as one bigger than the
 * fixed length string so that it can be null terminated
 *
 * @param _env      The JNI environment.  Used in all functions involving JNI
 * @param _string buffer to copy contents from
 * @param _size size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 */
#define ALLOCATE_FIXED_LENGTH_STRING(_env, _string, _size, _purpose) allocateMemory(_env, _string, _size, true, "Could not allocate space for " _purpose)

/**
 * Allocate memory and on error return the given value.
 *
 * @param _env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _size size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @param _r the specified return value
 * @return This MACRO will return the specified return value from your function if it fails
 */
#define ALLOCATE_MEMORY_AND_ON_ERROR_RETURN_(_env, _var, _size, _purpose, _r) { \
    void * _aptr = allocateMemory(_env, NULL, _size, false, "Could not allocate space for " _purpose); \
    if (!_aptr) \
        return _r; \
    (_var) = _aptr; \
}

/**
 * Allocate memory for a string and copy the given string into this allocated space
 *
 * @param _env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _string buffer to copy contents from
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @param _r the specified return value
 * @return This MACRO will return the specified return value from your function if it fails
 */
#define ALLOCATE_STRING_AND_ON_ERROR_RETURN_(_env, _var, _string, _purpose, _r) \
if (!( (_var) = ALLOCATE_STRING(_env, _string, _purpose))) { \
    return _r; \
}

/**
 * Allocate memory for a string and copy the given string into this allocated space
 * The specified variable is set to point to the allocated memory
 * The given purpose is a string that will be contained in the error message if the allocation fails
 *
 * @param _env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _string buffer to copy contents from
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @return This MACRO will return from your function if it fails
 */
#define ALLOCATE_STRING_AND_ON_ERROR_RETURN_VOID(_env, _var, _string, _purpose) \
if (!( (_var) = ALLOCATE_STRING(_env, _string, _purpose))) { \
    return; \
}

/**
 * Allocate space for a fixed length string and copy date from the given string into
 * the newly allocated space.  You need to specify size as one bigger than the
 * fixed length string so that it can be null terminated
 *
 * @param _env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _string buffer to copy contents from
 * @param _size size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @return This MACRO will return from your function if it fails
 */
#define ALLOCATE_FIXED_LENGTH_STRING_AND_ON_ERROR_RETURN_VOID(_env, _var, _string, _size, _purpose) \
if (!( (_var) = ALLOCATE_FIXED_LENGTH_STRING(_env, _string, _size, _purpose))) { \
    return; \
}

/**
 * Allocate space for a fixed length string and copy date from the given string into
 * the newly allocated space.  You need to specify size as one bigger than the
 * fixed length string so that it can be null terminated
 *
 * @param _env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _string buffer to copy contents from
 * @param _size size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @param _r the specified return value
 * @return This MACRO will return the specified return value from your function if it fails
 */
#define ALLOCATE_AND_TRACK_FIXED_LENGTH_STRING_AND_ON_ERROR_RETURN_(_env, _var, _string, _size, _purpose, _r) \
{ \
    void *_aptr = ALLOCATE_FIXED_LENGTH_STRING(_env, _string, _size, _purpose); \
    if ( !_aptr ) { \
        FREE_MEMORY \
        return _r; \
    } \
    TRACK_MEMORY(_aptr) \
    (_var) = _aptr; \
}

/**
 * Allocate memory and add it to the tracked memory list so that it can be freed automatically later
 *
 * @param _env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _size size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @param _r the specified return value
 * @return This MACRO will return the specified return value from your function if it fails
 */
#define ALLOCATE_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(_env, _var, _size, _purpose, _r) \
{ \
    void *_aptr = ALLOCATE_MEMORY(_env, _size, _purpose); \
    if ( !_aptr ) { \
        FREE_MEMORY \
        return _r; \
    } \
    TRACK_MEMORY(_aptr) \
    (_var) = _aptr; \
}

/**
 * Allocate memory and set its contents to the given buffer of given size
 *
 * @param _env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _source buffer to copy contents from
 * @param _size size of memory to allocate
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @param _r the specified return value
 * @return This MACRO will return the specified return value from your function if it fails
 */
#define ALLOCATE_COPY_AND_TRACK_MEMORY_AND_ON_ERROR_RETURN_(_env, _var, _source, _size, _purpose, _r) \
{ \
    void *_aptr = ALLOCATE_AND_COPY_MEMORY(_env, _source, _size, _purpose); \
    if ( !_aptr ) { \
        FREE_MEMORY \
        return _r; \
    } \
    TRACK_MEMORY(_aptr) \
    (_var) = _aptr; \
}

/**
 * Allocate and track a string
 *
 * @param _env      The JNI environment.  Used in all functions involving JNI
 * @param _var the specified variable is set to point to the allocated memory
 * @param _string buffer to copy contents from
 * @param _purpose the given purpose is a string that will be contained in the error message if the allocation fails
 * @param _r the specified return value
 * @return This MACRO will return the specified return value from your function if it fails
 */
#define ALLOCATE_COPY_AND_TRACK_STRING_AND_ON_ERROR_RETURN_(_env, _var, _string, _purpose, _r) \
{ \
    void *_aptr = ALLOCATE_STRING(_env, _string, _purpose); \
    if ( !_aptr ) { \
        FREE_MEMORY \
        return _r; \
    } \
    TRACK_MEMORY(_aptr) \
    (_var) = _aptr; \
}

/**
 * Free any allocated json memory
 */
#define FREE_JSON \
{                              \
    while ( _n_jsonValuesToFree-- > 0) { \
        if ( _jsonValuesToFree[_n_jsonValuesToFree] ) \
            json_value_free(_jsonValuesToFree[_n_jsonValuesToFree]); \
    } \
}

/**
 * Free any allocated memory
 */
#define FREE_MEMORY \
{                              \
    while ( _nAllocationsToFree-- > 0) { \
        if ( _memoryAllocationsToFree[_nAllocationsToFree])  \
            free (_memoryAllocationsToFree[_nAllocationsToFree]); \
    } \
    FREE_JSON \
}

/**
 * Free a single tracked memory allocation and remove from list
 *
 * @param _ptr name of a pointer that points to the memory to free
 */
#define FREE_TRACKED_MEMORY(_ptr) \
{ \
    if ( _ptr) {  \
    \
        bool found = false; \
        for ( int i = 0 ; i < _nAllocationsToFree; i++ ) { \
            if ( (_ptr) == _memoryAllocationsToFree[i] ) {   \
                free (_ptr);                    \
                found = true;                    \
            } \
            if ( found && i != (_nAllocationsToFree-1) )   \
                _memoryAllocationsToFree[i] = _memoryAllocationsToFree[i+1]; \
        } \
        if ( found )   \
            _nAllocationsToFree--;     \
    }\
}

#ifdef __cplusplus
}
#endif
#endif

