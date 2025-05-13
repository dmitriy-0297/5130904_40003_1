#include "../include/Decoder.h"
#include "../include/Encoder.h"
#include "../include/file_io.h"
#include <cassert>
#include <iostream>
#include <cstdio>

namespace DecoderTests {

  void testDecoderWorksCorrectly() {
    const std::string inputFile = "decoder_test_input.tmp";
    const std::string encodedFile = inputFile + ".encoded.tmp";
    const std::string outputFile = "decoder_test_output.tmp";

    Packed input = { 'a', 'b', 'a', 'c' };

    file_io::writeToFile(inputFile, input);
    Encoder::encode(inputFile, encodedFile);
    Decoder::decode(encodedFile, outputFile);

    Buffer decoded = file_io::readFileToBuffer(outputFile);
    assert(decoded == input);

    std::remove(inputFile.c_str());
    std::remove(encodedFile.c_str());
    std::remove(outputFile.c_str());
  }

  void runDecoderTest() {
    std::cout << "[DecoderTest] Running...\n";
    testDecoderWorksCorrectly();
    std::cout << "[DecoderTest] All tests passed\n";
  }

}
