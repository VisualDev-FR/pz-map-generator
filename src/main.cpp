#include <crtdbg.h>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

#include <fmt/format.h>
#include <cpptrace/from_current.hpp>

#include "files/lotheader.h"
#include "io/binary_reader.h"
#include "io/file_reader.h"

const std::string LOTHEADER_PATH = "data/B42/27_38.lotheader";

void read_header()
{
    BytesBuffer buffer = FileReader::read(LOTHEADER_PATH);
    LotHeader header = LotHeader::read(buffer);

    fmt::print("magic: {}, version: {}\n", header.magic, header.version);

    for (const std::string &tilename : header.tileNames)
    {
        std::cout << tilename << std::endl;
    }
}

int main()
{
    CPPTRACE_TRY
    {
        read_header();
    }
    CPPTRACE_CATCH(const std::exception &e)
    {
        std::cerr << "Exception: " << e.what() << std::endl;
        cpptrace::from_current_exception().print();
    }
}