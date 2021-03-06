#include "stdafx.h"

#include "src-a-star-one.h"
#include "src-a-star-two.h"
#include "src-8puzzle.h"
#include "src-8circle.h"

// A* one
// The first A* implementation uses a priority queue for the open list
// but does not have a closed list. It skips the overhead of checking
// the closed list, which seems like a clever move at first. However,
// it uses nearly 1/3 more loops than if a closed list was used. One
// would need to profile both implementations for accurate details.

// A* two
// The second A* implementation uses two sets (ideally an ordered set
// and an unordered set, but for C++ there's a ton of groundwork needed
// for A* and I ran into some issues with the unordered version). Most
// sources do a check into the open list when dealing with neighbors,
// but I found it to be a useless check. They also check the closed list
// for each neighbor, which is good, but they don't check the closed
// list for the current node, which is bad. This extra check is able to
// pass over a significant amount of cost and heuristic function calls.

// Profiling (in Release configuration)
// I'm not an expert with profiling, but one of the details that stook
// out to me was the call count to std::lower_bound. In one instance,
// A* one showed 24,415 calls whereas A* two showed only 8,238. This was
// the most significant difference. Since the 8puzzle problem is fairly
// easy to solve, even with a ton of moves, a more complicated domain
// will be necessary to compare the A* implementations effectively.

// Conclusion
// My educated guess says that using the closed list will improve overall
// performance for the A* algorithm.

int main ()
{
    srand(static_cast <unsigned int>(time(nullptr)));

    //test_8puzzle_performance();
    test_8puzzle();
    //test_8circle();

    return 0;
}
