/**
 * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * -
 *
 * Name        :  Project Riddle
 * Author      :  Andrea Bontempi
 * Version     :  0.1 aplha
 * Description :  Modular Network Sniffer
 *
 * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * -
 *
 * This file is part of the project Riddle.
 *
 *  The project Riddle is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  The project Riddle is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this project.  If not, see <http://www.gnu.org/licenses/>.
 *
 * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * - * -
 */

#include <iostream>
#include <string>
#include <vector>
#include <boost/regex.hpp>
#include <boost/program_options.hpp>
#include "./commons/classPacket.h"

using namespace std;
using namespace boost::program_options;
using namespace libNetwork;

int main ( int argc, char **argv ) {
    options_description desc ( "Breeder - Network TCP Flux Seletor" );
    desc.add_options()
    ( "help", "prints this" )
    ( "http", "select the http protocol." )
    ;

    variables_map vm;
    store ( parse_command_line ( argc, argv, desc ), vm );
    notify ( vm );

    if ( vm.count ( "help" ) ) {
        cout<<desc<<"\n";
        return EXIT_SUCCESS;
    }

    list<std::string> regularexpressions;

    if ( vm.count ( "http" ) ) {
        regularexpressions.push_front ( "HTTP.*" );
    }

    if ( regularexpressions.empty() ) {
        std::cerr<<"ERROR >> You have not selected any protocol!"<<std::endl;
        return EXIT_FAILURE;
    }

    while ( 1 ) {
        try {
            string r_flux;
            getline ( cin,r_flux );
            if ( cin.eof() ) break;

// TODO

        } catch ( packet::Overflow ) {
            std::cerr<<"Overflow! :-P"<<std::endl;
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}