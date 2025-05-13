#include "../include/Encoder.h"
#include "../include/file_io.h"
#include <cassert>
#include <iostream>
#include <cstdio>  // std::remove

namespace EncoderTests {

  void testEncoderWorksAndProducesOutput() {
    const std::string inputFile = "test_encoder_input.tmp";
    const std::string outputFile = "test_encoder_output.tmp";

    Packed input = { 'H', 'e', 'l', 'l', 'o' };
    file_io::writeToFile(inputFile, input);

    Encoder::encode(inputFile, outputFile);

    size_t outputSize = file_io::getFileSize(outputFile);
    assert(outputSize > 0);

    std::remove(inputFile.c_str());
    std::remove(outputFile.c_str());
  }

  void runEncoderTest() {
    std::cout << "[EncoderTest] Running...\n";
    testEncoderWorksAndProducesOutput();
    std::cout << "[EncoderTest] All tests passed\n";
  }

}
