//============================================================================
// Name        : Riddle
// Author      : Andrea Bontempi
// Version     : 0.1
// Copyright   : GNU GPL3
// Description : Network Sniffer
//
// Special Thanks to fede.tft for the big help :-)
//
//============================================================================

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <boost/program_options.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "libCigarette.h"
#include "libExtract.h"

using namespace std;
using namespace boost::program_options;

int main(int argc, char **argv) {
	options_description desc("Cigarette - Network Packet Parser");
	desc.add_options()
		("help", "prints this")
		("verbose", "enable verbose mode") // TODO
	;

	variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);

	if(vm.count("help"))
	{
	    cout<<desc<<"\n";
	    return 1;
	}

	while(1)
	{
		string packet;
		getline(cin,packet);
		if(cin.eof()) break;

		std::vector< std::string > line;
		boost::algorithm::split(line, packet, boost::algorithm::is_any_of("!"));

		header_ethernet etherhead;

		etherhead = parseEthernet(line[2]);
		std::cout<<"----- ["<<line[0]<<" "<<line[1]; 
		std::cout<<"] Packet ("<<std::dec<<line[2].length()<<" byte)"<<std::endl;
		std::cout<<"Ether | "<<print_mac_address(etherhead.mac_src);
		std::cout<<" --> "<<print_mac_address(etherhead.mac_dst)<<std::endl;
		std::cout<<"Ether | Type: 0x"<<std::hex<<etherhead.protocol_type<<" ";
		std::cout<<"("<<ether_type_decode(etherhead.protocol_type)<<")"<<std::endl;

		switch(etherhead.protocol_type)
		{
			case ETHER_TYPE_ARP:
			header_arp arp;
			arp = parseArp(line[2]);
			if(arp.opcode == 1)
			{
				// Request
				cout<<"ARP   | Who has "<<print_ipv4_address(arp.ip_dst)<<"? ";
				cout<<"Tell "<<print_mac_address(arp.mac_src)<<" ";
				cout<<"("<<print_ipv4_address(arp.ip_src)<<")"<<endl;
			}
			else
			{
				// Reply
				cout<<"ARP   | "<<print_ipv4_address(arp.ip_src)<<" is at ";
				cout<<print_mac_address(arp.mac_src)<<endl;
			}
			break;
			case ETHER_TYPE_IPV4:
			header_ipv4 ipv4;
			ipv4 = parseIPV4(line[2]);
			
			cout<<"IPV4  | "<<print_ipv4_address(ipv4.ip_src)<<" --> "<<print_ipv4_address(ipv4.ip_dst)<<endl;
			
				switch(ipv4.protocol_type)
				{
					case IPV4_TYPE_TCP:
					break;
					default:
					break;
				}
			break;
			case ETHER_TYPE_IPV6:
			break;
			default:
			break;
		}

		std::cout<<std::endl;

	}

	return EXIT_SUCCESS;
}
