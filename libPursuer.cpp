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
#include <boost/asio.hpp>
#include <list>
#include "libPacket.h"
#include "libAddress.h"
#include "libPursuer.h"

stream::stream(TCPv4packet *SYN)
{
    timeEpoch = SYN->getEpoch();
    timeMillis = SYN->getMillis();
    first_mac = SYN->getSenderMac();
    second_mac = SYN->getTargetMac();
    first_ip = SYN->getSenderIp();
    second_ip = SYN->getTargetIp();
    first_port = SYN->getSenderPort();
    second_port = SYN->getTargetPort();
    first_sn = SYN->getSequenceNumber();
    second_sn = 0;
    flagFirstFIN = false;
    flagSecondFIN = false;

    delete SYN;

    return;
}

bool stream::streamSynAck(TCPv4packet *SYN)
{

    if(first_sn + 1 == SYN->getAcknowledgmentNumber() && SYN->isACK() && SYN->isSYN())
    {
        second_sn = SYN->getSequenceNumber();
        return true;
    }

    delete SYN;

    return false;

}


bool stream::addPacket(TCPv4packet *newPacket)
{

    using namespace std;

    if(!newPacket->isSYN())
    {

        if(newPacket->getSenderPort() == first_port)
        {
            // Siamo nel first_buffer

            if(newPacket->isACK())
            {

                for (list<TCPv4packet*>::iterator it = second_buffer.begin(); it != second_buffer.end(); it++)
                {

                    if( (*it)->getSequenceNumber() == newPacket->getAcknowledgmentNumber() - ((*it)->getPayLoad().size()/2))
                    {
                        (*it)->public_flag = true;
                        break;
                    }
                }

            }

            if(newPacket->getPayLoad().size() != 0)
            {
                first_buffer.push_back(newPacket);
		std::cerr << "first_buffer++" << std::endl;
            }

            return true;
        }
        else if (newPacket->getSenderPort() == second_port)
        {

            if(newPacket->isACK())
            {

                for (list<TCPv4packet*>::iterator it = first_buffer.begin(); it != first_buffer.end(); it++)
                {

                    if( (*it)->getSequenceNumber() == newPacket->getAcknowledgmentNumber() - (((*it)->getPayLoad().size()/2)))
                    {
                        (*it)->public_flag = true;
                        break;
                    }
                }

            }

            if(newPacket->getPayLoad().size() != 0)
            {
                second_buffer.push_back(newPacket);
		std::cerr << "second_buffer++" << std::endl;
            }

            return true;
        } else return false;


    }
    return false;

}

void stream::flushFirstBuffer()
{
    bool isFound = false;

    do
    {

        for (std::list<TCPv4packet*>::iterator it = first_buffer.begin(); it != first_buffer.end(); it++)
        {
            std::cerr << "Trovato pacchetto nel primo buffer " << (*it)->getSequenceNumber() << " == " << first_sn + 1 << std::endl;
            if(first_sn + 1 == (*it)->getSequenceNumber() && (*it)->public_flag)
            {
                std::string payload = (*it)->getPayLoad();
                first_flow += payload;
                std::cerr << "Nel buffer: " << payload << std::endl;
                first_sn += payload.size()/2; // unsigned, si azzera come avviene nel tcp.
                first_buffer.remove(*it);
                isFound == true;
                break;
            }
            else
            {
                isFound == false;
            }
        }

    } while (isFound);

}

void stream::flushSecondBuffer()
{
    bool isFound = false;

    do
    {

        for (std::list<TCPv4packet*>::iterator it = second_buffer.begin(); it != second_buffer.end(); it++)
        {
            std::cerr << "Trovato pacchetto nel secondo buffer " << (*it)->getSequenceNumber() << " == " << second_sn + 1 << std::endl;

            if(second_sn + 1 == (*it)->getSequenceNumber() && (*it)->public_flag)
            {
                std::string payload = (*it)->getPayLoad();
                second_flow += payload;
                std::cerr << "Nel buffer: " << payload << std::endl;
                second_sn += payload.size()/2; // unsigned, si azzera come avviene nel tcp.
                second_buffer.remove(*it);
                isFound == true;
                break;
            }
            else
            {
                isFound == false;
            }
        }

    } while (isFound);


}

std::string stream::exportFlow()
{
    return decodeHexText(first_flow) + "|" + decodeHexText(second_flow);
}


long int stream::getTimeEpoch()
{
    return timeEpoch;
}

int stream::getTimeMillis()
{
    return timeMillis;
}

mac_address stream::getFirstMacAddress()
{
    return first_mac;
}

mac_address stream::getSecondMacAddress()
{
    return second_mac;
}

boost::asio::ip::address stream::getFirstIpAddress()
{
    return first_ip;
}

boost::asio::ip::address stream::getSecondIpAddress()
{
    return second_ip;
}

unsigned int stream::getFirstPort()
{
    return first_port;
}

unsigned int stream::getSecondPort()
{
    return second_port;
}

unsigned int stream::getFirstSN()
{
    return first_sn;
}

unsigned int stream::getSecondSN()
{
    return second_sn;
}

bool stream::isFIN()
{
    return flagFirstFIN && flagSecondFIN;
}

std::string stream::decodeHexText(std::string raw)
{

  std::string text;
  
  for(int i = 0; i <= raw.size(); i += 2)
  {
    std::string comp;
    comp += (char)raw[i];
    comp += (char)raw[i+1];
    std::stringstream convert(comp);
    int temp;
    convert >> std::hex >> temp;
    text += (char)temp;
  }
  
  return text;
  
}

