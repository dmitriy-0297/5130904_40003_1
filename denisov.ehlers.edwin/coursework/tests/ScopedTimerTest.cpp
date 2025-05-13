#include "../include/ScopedTimer.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

namespace ScopedTimerTests {

    void testScopedTimerRunsAndPrints() {
        try {
            {
                ScopedTimer timer("ScopedTimerTest_Delay");
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        } catch (...) {
            assert(false && "ScopedTimer threw an exception");
        }
    }

    void runScopedTimerTest() {
        std::cout << "[ScopedTimerTest] Running...\n";
        testScopedTimerRunsAndPrints();
        std::cout << "[ScopedTimerTest] All tests passed\n";
    }

}
