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

#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>
#include <ios>
#include "libHeader.h"
#include "libAddress.h"

void ethernet_header::getMacAddress(std::string start)
{
  mac_dst.set(start, 0);
  mac_src.set(start, 12);
  return;
}

void ethernet_header::getProtocolType(std::string start)
{
  std::string temp;
  temp = "0x";
  for(int i=24;i<=27;i++)
  {
    temp += start[i];
  }
  std::stringstream convert ( temp );
  convert>>std::hex>>protocol_type;
  return;
}

void arp_header::getMacAddress(std::string start)
{
  mac_src.set(start, 44);
  mac_dst.set(start, 64);
  return;
}

void arp_header::getIpAddress(std::string start)
{
  ip_src.set(start, 56);
  ip_dst.set(start, 76);
  return;
}

void arp_header::getProtocolType(std::string start)
{
  std::string temp;
  temp = "0x";
  for(int i=32;i<=35;i++)
  {
    temp += start[i];
  }
  std::stringstream convert ( temp );
  convert>>std::hex>>protocol_type;
  return;
}

void arp_header::getOpcode(std::string start)
{
  std::string temp;
  temp = "0x";
  for(int i=40;i<=43;i++)
  {
    temp += start[i];
  }
  std::stringstream convert ( temp );
  convert>>std::hex>>opcode;
  return;  
}

void ipv4_header::getIpAddress(std::string start)
{
  ip_src.set(start, 52);
  ip_dst.set(start, 60);
  return;
}

void ipv4_header::getProtocolType(std::string start)
{
  std::string temp;
  temp = "0x";
  for(int i=46;i<=47;i++)
  {
    temp += start[i];
  }
  std::stringstream convert ( temp );
  convert>>std::hex>>protocol_type;
  return;
}