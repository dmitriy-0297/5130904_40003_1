#include "../include/file_io.h"
#include <cassert>
#include <iostream>
#include <cstdio>

namespace FileIOTests {

    void testCheckFilesValid() {
        std::string in = "input.tmp";
        std::string out = "output.tmp";
        file_io::checkFiles(in, out);
    }

    void testCheckFilesSameThrows() {
        bool caught = false;
        try {
            std::string path = "same.tmp";
            file_io::checkFiles(path, path);
        } catch (const FileException&) {
            caught = true;
        }
        assert(caught);
    }

    void testCheckFilesEmptyThrows() {
        bool caught = false;
        try {
            std::string in = "";
            std::string out = "some.tmp";
            file_io::checkFiles(in, out);
        } catch (const FileException&) {
            caught = true;
        }
        assert(caught);
    }

    void testWriteAndReadFile() {
        std::string filename = "test_io.tmp";

        Packed packed = { 0x41, 0x42, 0x43 }; // 'A', 'B', 'C'
        file_io::writeToFile(filename, packed);

        Buffer readBack = file_io::readFileToBuffer(filename);
        assert(readBack == packed);

        std::remove(filename.c_str());
    }

    void testReadNonexistentThrows() {
        std::string missing = "this_file_does_not_exist.tmp";
        bool caught = false;
        try {
            file_io::readFileToBuffer(missing);
        } catch (const FileException&) {
            caught = true;
        }
        assert(caught);
    }

    void testGetFileSize() {
        std::string filename = "size_check.tmp";

        Packed packed;
        for (int i = 0; i < 100; ++i)
            packed.push_back(static_cast<uint8_t>(i));

        file_io::writeToFile(filename, packed);
        size_t size = file_io::getFileSize(filename);
        assert(size == 100);

        std::remove(filename.c_str());
    }

    void runFileIOTest() {
        std::cout << "[FileIOTest] Running...\n";
        testCheckFilesValid();
        testCheckFilesSameThrows();
        testCheckFilesEmptyThrows();
        testWriteAndReadFile();
        testReadNonexistentThrows();
        testGetFileSize();
        std::cout << "[FileIOTest] All tests passed\n";
    }

}
