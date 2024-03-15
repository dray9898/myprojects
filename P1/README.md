

# My Malloc
### Dhyey Ray and Eric Yang
### DR1015, EZY1

## Heap Construction
Definitions: In this context, a line represents 8 bytes. Specifically, the heap memory consists of 512 lines.

The `malloc` function requires some sort of metadata for the function to be coded correctly, so we decided to make this a struct called `chunk_header`, so that all the elements are easy to access and manipulate. The data in the struct all have an important use. The first shorts in the beginning tell us how long the corresponding allocation is, in **lines**. This is useful because in order to find the target allocation, we must iterate through each allocations. We need to iterate through data in order to ensure that the pointer being freed is at the beginning of the allocation and is not in the middle of a allocation or a non-heap pointer.

The `prevlength` signifies the length of the last allocation. This is useful when we are trying to combine the previous allocation with the current allocation because we need both their sizes to know the size of the new allocation. The boolean `free` signifies if the allocation is free or not so it can be coalesced or filled with data.

For the multiple errors associated with `free` such as freeing a pointer in the middle of allocated space, or freeing a non-allocated pointer, we decided to exit the function instead of just printing an error message so that we can prevent undefined behavior.

Our `malloc` function works by rounding up the requested size to a multiple of 8 so that only a whole number of lines are assigned. Afterwards, the function iterates through the memory looking for a free allocation that is greater than or equal to the space required. It allocates the required space and then the leftover space is given its own header allocation and the header allocation for the allocated space is updated.

The `free` function works by checking if the given pointer is within the bounds of the heap; if not, it's not a heap pointer. Then it iterates through the memory by starting at the start of the heap and iterates by the size of the "length." As it goes through the heap, it checks if the target matches the current allocation, and if it goes out of the bounds of the heap, then that means that the pointer was in the middle of a allocation . It calls an error and exits.

## Tests

The `make test` command runs all tests.

The "copycat" test allocates 4000 bytes. It then takes the header of the 4000 bytes and pastes it to the first allocated line. Subsequently, we call `free` on a pointer pointing to the first byte in the duplicate header. This ensures that `free` only frees using the actual header because the user data may be identical to the header.

The "notatbeginning" test allocates data to the heap using `malloc` and then adds 8 to the returned pointer. After adding to the pointer, it tries to free that pointer. This ensures that `free` only frees the data segment using the pointer it allocated, which is at the beginning of the heap.

The "double free" test allocates space using `malloc` and then frees a pointer twice.

The "notaheappointer" test declares an int, takes its address, and attempts to free it. This should fail because the address is not in the heap, but rather a local variable.

All the tests work by performing incorrect operations and then writing the error to a text file. The text file is then read by another program that checks if that error matches the expected error.

The two performance tests that were designed by us do the following. The fourth test allocates up to 400 bytes or deallocates previous allocations. This test ensures that `malloc` can handle large amounts of allocations and deallocations at the same time in random orders. This also consequently ensures that coalesce is being done properly because of this. The fifth and last test ensures that `malloc` can handle structs and arrays of structs. Although `malloc` doesn't know they are structs and just treats them like any other data, it is still useful to provide a variety of tests that have large allocations. No additional malloc tests were included because these tests are rigorous and measure all aspects of malloc, such as coalescing, assigning a sufficient amount of space, and returning a pointer.

## Directory Structure

To compile all necessary programs, run `make all`.

The `mymalloc` file is in the folder `src`, and the test `.c` files are in `src` under the `test` folder. However, the performance tests are in `src` named `memgrind`. 

To run a test named `<testname>`, begin by running the corresponding executable in `tests/<testname>test`, then run the verifying executable `tests/<testname>`. To run all tests, run `make test`./common/users/shared/cs214s24pa1dr101ezy1/P1/src/tests