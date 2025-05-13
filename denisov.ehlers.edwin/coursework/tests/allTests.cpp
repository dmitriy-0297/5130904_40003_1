#include <iostream>

namespace BitUtilsTests {
    void runBitUtilsTest();
}

namespace FileIOTests {
    void runFileIOTest();
}

namespace ScopedTimerTests {
    void runScopedTimerTest();
}

namespace TableTests {
    void runTableTest();
}

namespace DataTests {
    void runDataTest();
}

namespace EncoderTests {
    void runEncoderTest();
}

namespace DecoderTests {
    void runDecoderTest();
}


int main() {
    std::cout << "Running all tests...\n";
    BitUtilsTests::runBitUtilsTest();
    FileIOTests::runFileIOTest();
    ScopedTimerTests::runScopedTimerTest();
    TableTests::runTableTest();
    DataTests::runDataTest();
    EncoderTests::runEncoderTest();
    DecoderTests::runDecoderTest();

    std::cout << "All tests completed.\n";
    return 0;
}
