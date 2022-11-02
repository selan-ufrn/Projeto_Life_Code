/**
 * @file life_game.cpp
 *
 * @description
 * This program simulates the Conway's Game of Life.
 * It reads a initial game configuration from a input file and
 * keeps updating it, if the user so whishes.
 *
 * @author	Selan R dos Santos, <selan.rds@gmail.com>
 * @date	2015, updated 2015-04-03
 *
 * @remark On 2015-09-19 changed the way stability is verified.
 * In the previous version we just looked back one configuration and compare it
 * with the current configuration.
 * Now, the stability may span more than one configuration. Because of that we
 * need to keep track of all configurations and check whether the current configuration
 * has already been generated. If this is the case, the game has reached stability and
 * must stop.
 */

#include <cstdlib>  // EXIT_SUCCESS
#include <iostream>
#include <iomanip>

#include "tip.h"

int main(int argc, char* argv[])
{
    TIP reader{ ".config/glife.ini" };
    // Check for any parser error.
    if (not reader.parsing_ok()) {
        std::cerr << ">> Sorry, parser failed with message: " << reader.parser_error_msg() << "\n";
        return 1;
    }

    // [2]== RETRIEVE DATA.
    auto sections = reader.get_sections_list();
    for (const auto& sec_name : sections) {
        auto fields = reader.get_fields_list(sec_name);
        std::cout << ">>> Field: " << std::quoted(sec_name) << "\n\t";
        for (const auto& field_name : fields)
            std::cout << std::quoted(field_name) << " ";
        std::cout << '\n';
    }

    // Try to get pi value.
    auto val = reader.get_real("ROOT", "max_gen");
    if (not reader.parsing_ok()) {
        std::cout << ">>> Error while retrieving \"max_gen\" field." << '\n';
        std::cout << "    Msg = " << std::quoted(reader.parser_error_msg()) << '\n';
    } else {
        std::cout << "Max gen is " << val << '\n';
    }

    // Try to get user current active status.
    auto create_img = reader.get_bool("ROOT", "generate_image");
    if (not reader.parsing_ok()) {
        std::cout << ">>> Error while retrieving \"generate_image\" field." << '\n';
        std::cout << "    Msg = " << std::quoted(reader.parser_error_msg()) << '\n';
    } else {
        std::cout << "Generate Image situation is " << std::boolalpha << create_img << '\n';
    }

    // Try to get user current active status.
    auto fps = reader.get_int("text", "fps");
    if (not reader.parsing_ok()) {
        std::cout << ">>> Error while retrieving \"fps\" field." << '\n';
        std::cout << "    Msg = " << std::quoted(reader.parser_error_msg()) << '\n';
    } else {
        std::cout << "FPS value is " << fps << '\n';
    }

    std::cout << "\n>>> Pretty print:" << '\n';
    std::cout << reader.pretty_print() << '\n';

    return EXIT_SUCCESS;
}
