cmake_minimum_required(VERSION 3.28)
project(fano)

set(CMAKE_CXX_STANDARD 14)
# Main target
add_executable(fano main.cpp
        src/ByteEntry.cpp
        include/ByteEntry.h
        src/bit_utils.cpp
        include/bit_utils.h
        src/Table.cpp
        include/Table.h
        src/Data.cpp
        include/Data.h
        src/Encoder.cpp
        include/Encoder.h
        src/Decoder.cpp
        include/Decoder.h
        src/file_io.cpp
        include/file_io.h
        include/types.h
        src/ScopedTimer.cpp
        include/ScopedTimer.h
        include/FanoExceptions.h
        include/Command.h
        src/Command.cpp
        src/StreamGuard.cpp
        include/StreamGuard.h
)
if (MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /arch:AVX2")
else()
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native")
endif()


# Tests target
add_executable(tests
        tests/allTests.cpp
        src/ByteEntry.cpp
        src/bit_utils.cpp
        src/Table.cpp
        src/Data.cpp
        src/Encoder.cpp
        src/Decoder.cpp
        src/file_io.cpp
        src/ScopedTimer.cpp
        tests/BitUtilsTest.cpp
        tests/FileIOTest.cpp
        tests/ScopedTimerTest.cpp
        tests/TableTest.cpp
        tests/DataTest.cpp
        tests/EncoderTest.cpp
        tests/DecoderTest.cpp
)

