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

#ifndef LIBHEADER_H
#define LIBHEADER_H

#define ETHER_V2_CODE		0x0600
#define ETHER_TYPE_IPV4		0x0800
#define ETHER_TYPE_ARP		0x0806
#define ETHER_TYPE_IEEE802	0x8100
#define ETHER_TYPE_IPV6		0x86DD
#define ETHER_TYPE_PPP		0x880B

#define IPV4_TYPE_TCP		0x06
#define IPV4_TYPE_UDP		0x11

#include <string>
#include "libAddress.h"

/* Class for managing Ethernet Header */
class ethernet_header
{
  public:
  mac_address mac_dst;
  mac_address mac_src;
  short int protocol_type;
};

/* Class for managing ARP Header */
class arp_header
{
  public:
  mac_address mac_dst;
  mac_address mac_src;
  ipv4_address ip_dst;
  ipv4_address ip_src;
  short int protocol_type;
  short int opcode;
};

class ipv4_header
{
  public:
  ipv4_address ip_dst;
  ipv4_address ip_src;
  short int protocol_type;
};

/* Class for managing packets*/
class packet
{
  
  private:
    
    
    std::string *rawData;
    long int timeEpoch;
    int timeMillis;
    int pkgLength;
  
  public:
    
    /* Costruttore Pacchetto */
    packet(long int timeEpoch_i, int timeMillis_i, std::string rawData_i);
  
    /* Distruttore Pacchetto */
    ~packet();
    
    /* Ottieni lunghezza in byte */
    int getLenght();
    
    /* Ottieni epoch */
    long int getEpoch();
    
    /* Ottienti millisecondi passati dopo epoch */
    int getMillis();
    
    /* Legge n byte e li restituisce in stringa. */
    std::string getHexString(int string_cursor, int read_byte);
    
    /* Salva MAC address a partire da un punto della stringa rawData */
    mac_address getMacAddress(int string_cursor);
    
    /* Salva IPv4 address a partire da un punto della stringa rawData */
    ipv4_address getIPv4Address(int string_cursor);
    
    /* Ottieni header ethernet */
    ethernet_header getEthernetHeader();
    
    /* True se è un pacchetto ARP */
    bool isArp();
    
    /* Ottieni header ARP */
    arp_header getArpHeader();
    
    /* True se è un pacchetto IPv4 */
    bool isIPv4();
    
    ipv4_header getIPv4Header();
    
    /* True se è un pacchetto IPv6*/
    bool isIPv6();
    
};

#endif //LIBHEADER_H