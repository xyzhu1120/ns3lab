#ifndef _MYHEADER_H_
#define _MYHEADER_H_
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"

#include "ns3/nstime.h"
#include "ns3/event-id.h"
#include "ns3/node.h"
#include "ns3/channel.h"
#include "ns3/packet.h"
#include "ns3/mac48-address.h"
#include "ns3/ipv4-header.h"
#include "ns3/ipv4-l3-protocol.h"
using namespace ns3;
using namespace std;

/************************************************************************
    ipv4 protocol 
    Tx: Send ipv4 packet to outgoing interface.
    Rx: Receive ipv4 packet from incoming interface.
    Drop: Drop ipv4 packet
    SendOutgoing: A newly-generated packet by this node is about to be queued for transmission
    UnicastForward: A unicast IPv4 packet was received by this node and is being forwarded to another node
    LocalDeliver: An IPv4 packet was received by/for this node, and it is being forward up the stack

************************************************************************/
int ipv4Tx=0;
int ipv4Rx=0;
int ipv4Drop=0;

void TraceIpv4Drop (std::string context, ns3::Ipv4Header const &hdr, Ptr<const Packet> packet,
                      ns3::Ipv4L3Protocol::DropReason reason, Ptr<Ipv4> dummy_ipv4, uint32_t interface)
{
  cout<<"Trace Ipv4 Drop in "<<ipv4Drop++<<" ";
  cout<<context<<" interface: "<<interface<<endl;
}
void
TraceIpv4Tx (std::string context, Ptr<const Packet> packet, Ptr<Ipv4> ipv4, uint32_t interface)
{
  cout<<"Trace Ipv4 Tx in "<<ipv4Tx++<<" ";
  cout<<context<<" interface: "<<interface<<endl;
}
void
TraceIpv4Rx (std::string context, Ptr<const Packet> packet, Ptr<Ipv4> ipv4, uint32_t interface)
{
  cout<<"Trace Ipv4 Rx in "<<ipv4Rx++<<" ";
  cout<<context<<" interface: "<<interface<<endl;
}

/* simple net device
*/
int simpleDeviceDrops=0;
void
SimpleTraceDeviceDrop (std::string context, Ptr<const Packet> packet)
{
  cout<<"Trace Simple Device Drops in "<<simpleDeviceDrops++<<" "<<context<<endl;
}

/******************************************************************************************************************
    MacTx: Trace source indicating a packet has arrived for transmission by this device
    MacTxDrop: Trace source indicating a packet has been dropped by the device before transmission
    MacPromiscRx: A packet has been received by this device, has been passed up from the physical layer and is being forwarded up the local protocol stack. This is a promiscuous trace,
    MacRx: A packet has been received by this device, has been passed up from the physical layer and is being forwarded up the local protocol stack. This is a non-promiscuous trace,
    PhyTxBegin: Trace source indicating a packet has begun transmitting over the channel
    PhyTxEnd: Trace source indicating a packet has been completely transmitted over the channel
    PhyTxDrop: Trace source indicating a packet has been dropped by the device during transmission
    PhyRxEnd: Trace source indicating a packet has been completely received by the device
    PhyRxDrop: Trace source indicating a packet has been dropped by the device during reception
    Sniffer: Trace source simulating a non-promiscuous packet sniffer attached to the device
    PromiscSniffer: Trace source simulating a promiscuous packet sniffer attached to the device
******************************************************************************************************************/
int p2pMacTx=0;
int p2pMacTxDrop=0;
int p2pMacRx=0;
int p2pPhyTxBegin=0;
int p2pPhyTxDrop=0;
int p2pPhyRxDrop=0;
int p2pPhyRxEnd=0;
void
TraceP2PMacTx (std::string context, Ptr<const Packet> packet)
{
  cout<<"Trace P2P Device MacTx "<<p2pMacTx++<<" ";
  cout<<context<<endl;
}
void
TraceP2PMacTxDrop (std::string context, Ptr<const Packet> packet)
{
  cout<<"Trace P2P Device MacTxDrop "<<p2pMacTxDrop++<<" ";
  cout<<context<<endl;
}
void
TraceP2PMacRx (std::string context, Ptr<const Packet> packet)
{
  cout<<"Trace P2P Device MacRx "<<p2pMacRx++<<" ";
  cout<<context<<endl;
}
void
TraceP2PPhyTxBegin (std::string context, Ptr<const Packet> packet)
{
  cout<<"Trace P2P Device PhyTxBegin "<<p2pPhyTxBegin++<<" ";
  cout<<context<<endl;
}
void
TraceP2PPhyTxDrop (std::string context, Ptr<const Packet> packet)
{
  cout<<"Trace P2P Device PhyTxDrop "<<p2pPhyTxDrop++<<" ";
  cout<<context<<endl;
}
void
TraceP2PPhyRxDrop (std::string context, Ptr<const Packet> packet)
{
  cout<<"Trace P2P Device PhyRxDrop "<<p2pPhyRxDrop++<<" ";
  cout<<context<<endl;
}
void
TraceP2PPhyRxEnd (std::string context, Ptr<const Packet> packet)
{
  cout<<"Trace P2P Device PhyRxEnd "<<p2pPhyRxEnd++<<" ";
  cout<<context<<endl;
}
void AddAllTraces(){
  Config::Connect ("/NodeList/*/$ns3::Ipv4L3Protocol/Drop",MakeCallback(&TraceIpv4Drop));
  Config::Connect ("/NodeList/*/$ns3::Ipv4L3Protocol/Tx",MakeCallback(&TraceIpv4Tx));
  Config::Connect ("/NodeList/*/$ns3::Ipv4L3Protocol/Rx",MakeCallback(&TraceIpv4Rx));

  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::PointToPointNetDevice/MacTx",MakeCallback(&TraceP2PMacTx));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::PointToPointNetDevice/MacTxDrop",MakeCallback(&TraceP2PMacTxDrop));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::PointToPointNetDevice/MacRx",MakeCallback(&TraceP2PMacRx));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::PointToPointNetDevice/PhyTxBegin",MakeCallback(&TraceP2PPhyTxBegin));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::PointToPointNetDevice/PhyTxDrop",MakeCallback(&TraceP2PPhyTxDrop));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::PointToPointNetDevice/PhyRxDrop",MakeCallback(&TraceP2PPhyRxDrop));
  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::PointToPointNetDevice/PhyRxEnd",MakeCallback(&TraceP2PPhyRxEnd));

  Config::Connect ("/NodeList/*/DeviceList/*/$ns3::SimpleNetDevice/PhyRxDrop",MakeCallback(&SimpleTraceDeviceDrop));
}
#endif
