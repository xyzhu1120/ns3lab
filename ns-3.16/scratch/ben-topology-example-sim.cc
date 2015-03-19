/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Tommaso Pecorella <tommaso.pecorella@unifi.it>
 * Author: Valerio Sartini <valesar@gmail.com>
 *
 * This program conducts a simple experiment: It builds up a topology based on
 * either Inet or Orbis trace files. A random node is then chosen, and all the
 * other nodes will send a packet to it. The TTL is measured and reported as an histogram.
 *
 */

#include <ctime>

#include <sstream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-l3-protocol.h"
#include "ns3/ipv4-static-routing-helper.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/ipv4-nix-vector-helper.h"

#include "ns3/topology-read-module.h"
#include "myheader.h"
#include <list>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("TopologyCreationExperiment");


static std::list<unsigned int> data;

static void SinkRx (Ptr<const Packet> p, const Address &ad)
{
  Ipv4Header ipv4;
  p->PeekHeader (ipv4);
  std::cout << "TTL: " << (unsigned)ipv4.GetTtl () << std::endl;
}


// ----------------------------------------------------------------------
// -- main
// ----------------------------------------------
int main (int argc, char *argv[])
{

  std::string format ("Orbis");
  std::string input ("topology/simple.dat");

  // Set up command line parameters used to control the experiment.
  CommandLine cmd;
  cmd.AddValue ("format", "Format to use for data input [Orbis|Inet|Rocketfuel].",
                format);
  cmd.AddValue ("input", "Name of the input file.",
                input);
  cmd.Parse (argc, argv);


  // ------------------------------------------------------------
  // -- Read topology data.
  // --------------------------------------------

  // Pick a topology reader based in the requested format.

  Ptr<TopologyReader> inFile = 0;
  TopologyReaderHelper topoHelp;

  NodeContainer nodes;

  topoHelp.SetFileName (input);
  topoHelp.SetFileType (format);
  inFile = topoHelp.GetTopologyReader ();

  if (inFile != 0)
    {
      nodes = inFile->Read ();
    }

  if (inFile->LinksSize () == 0)
    {
      NS_LOG_ERROR ("Problems reading the topology file. Failing.");
      return -1;
    }

  // ------------------------------------------------------------
  // -- Create nodes and network stacks
  // --------------------------------------------
  NS_LOG_INFO ("creating internet stack");
  InternetStackHelper stack;

  // Setup NixVector Routing
  Ipv4NixVectorHelper nixRouting;
  Ipv4StaticRoutingHelper staticRouting;
  Ipv4GlobalRoutingHelper g;

  Ipv4ListRoutingHelper listRH;
  listRH.Add (g, 10);
  //listRH.Add (nixRouting, 10);

  stack.SetRoutingHelper (g);  // has effect on the next Install ()
  stack.Install (nodes);

  NS_LOG_INFO ("creating ip4 addresses");
  Ipv4AddressHelper address;
  address.SetBase ("10.0.0.0", "255.255.255.252");

  int totlinks = inFile->LinksSize ();

  NS_LOG_INFO ("creating node containers");
  NodeContainer* nc = new NodeContainer[totlinks];
  TopologyReader::ConstLinksIterator iter;
  int i = 0;
  for ( iter = inFile->LinksBegin (); iter != inFile->LinksEnd (); iter++, i++ )
    {
      nc[i] = NodeContainer (iter->GetFromNode (), iter->GetToNode ());
    }

  NS_LOG_INFO ("creating net device containers");
  NetDeviceContainer* ndc = new NetDeviceContainer[totlinks];
  PointToPointHelper p2p;
  for (int i = 0; i < totlinks; i++)
    {
      // p2p.SetChannelAttribute ("Delay", TimeValue(MilliSeconds(weight[i])));
      p2p.SetChannelAttribute ("Delay", StringValue ("2ms"));
      p2p.SetDeviceAttribute ("DataRate", StringValue ("100Mbps"));
      ndc[i] = p2p.Install (nc[i]);
    }

  // it crates little subnets, one for each couple of nodes.
  NS_LOG_INFO ("creating ipv4 interfaces");
  Ipv4InterfaceContainer* ipic = new Ipv4InterfaceContainer[totlinks];
  for (int i = 0; i < totlinks; i++)
    {
      ipic[i] = address.Assign (ndc[i]);
      address.NewNetwork ();
    }

  //AddAllTraces();
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  uint32_t totalNodes = nodes.GetN ();
  unsigned int randomServerNumber = totalNodes - 1;
  Ptr<Node> randomServerNode = nodes.Get (randomServerNumber);
  Ptr<Ipv4> ipv4Server = randomServerNode->GetObject<Ipv4> ();
  Ipv4InterfaceAddress iaddrServer = ipv4Server->GetAddress (1,0);
  Ipv4Address ipv4AddrServer = iaddrServer.GetLocal ();

  uint16_t port = 4000;
  UdpServerHelper server (port);
  ApplicationContainer apps = server.Install (nodes.Get (totalNodes - 1));
  apps.Start (Seconds (1.0));
  //apps.Stop (Seconds (90000.0));
  apps.Stop (Seconds (20000.0));

//
// Create one UdpClient application to send UDP datagrams from node zero to
// node one.
//
  uint32_t MaxPacketSize = 1024;
  Time interPacketInterval = Seconds (0.05);
  uint32_t maxPacketCount = 450000;
  UdpClientHelper client (ipv4AddrServer, port);
  client.SetAttribute ("MaxPackets", UintegerValue (maxPacketCount));
  client.SetAttribute ("Interval", TimeValue (interPacketInterval));
  client.SetAttribute ("PacketSize", UintegerValue (MaxPacketSize));
  apps = client.Install (nodes.Get (0));
  apps.Start (Seconds (2.0));
  apps.Stop (Seconds (20000.0));
  //apps.Stop (Seconds (90000.0));

 /*
  uint32_t totalNodes = nodes.GetN ();
  Ptr<UniformRandomVariable> unifRandom = CreateObject<UniformRandomVariable> ();
  unifRandom->SetAttribute ("Min", DoubleValue (0));
  unifRandom->SetAttribute ("Max", DoubleValue (totalNodes - 1));

//  unsigned int randomServerNumber = unifRandom->GetInteger (0, totalNodes - 1);
  unsigned int randomServerNumber = totalNodes - 1;

  Ptr<Node> randomServerNode = nodes.Get (randomServerNumber);
  Ptr<Ipv4> ipv4Server = randomServerNode->GetObject<Ipv4> ();
  Ipv4InterfaceAddress iaddrServer = ipv4Server->GetAddress (1,0);
  Ipv4Address ipv4AddrServer = iaddrServer.GetLocal ();

  // ------------------------------------------------------------
  // -- Send around packets to check the ttl
  // --------------------------------------------
  Config::SetDefault ("ns3::Ipv4RawSocketImpl::Protocol", StringValue ("2"));
  InetSocketAddress dst = InetSocketAddress ( ipv4AddrServer );

  OnOffHelper onoff = OnOffHelper ("ns3::Ipv4RawSocketFactory", dst);
  onoff.SetConstantRate (DataRate (15000));
  onoff.SetAttribute ("PacketSize", UintegerValue (1200));

  std::cout<<randomServerNumber<<std::endl;
  NodeContainer clientNodes;
  for ( unsigned int i = 0; i < nodes.GetN (); i++ )
    {
      if (i != randomServerNumber )
        {
          Ptr<Node> clientNode = nodes.Get (i);
          clientNodes.Add (clientNode);
        }
    }

  ApplicationContainer apps = onoff.Install (clientNodes);
  apps.Start (Seconds (1.0));
  apps.Stop (Seconds (2.0));

  PacketSinkHelper sink = PacketSinkHelper ("ns3::Ipv4RawSocketFactory", dst);
  apps = sink.Install (randomServerNode);
  apps.Start (Seconds (0.0));
  apps.Stop (Seconds (3.0));
 */
  // we trap the packet sink receiver to extract the TTL.
  Config::ConnectWithoutContext ("/NodeList/*/ApplicationList/*/$ns3::PacketSink/Rx",
                                 MakeCallback (&SinkRx));
  Ptr<OutputStreamWrapper> routingStream = Create<OutputStreamWrapper> ("dynamic-global-routing.routes", std::ios::out);
  g.PrintRoutingTableAllAt (Seconds (12), routingStream);
  g.PrintRoutingTableAllAt (Seconds (14), routingStream);


  AsciiTraceHelper ascii;
  Ptr<OutputStreamWrapper> stream = ascii.CreateFileStream ("dynamic-global-routing.tr");
  p2p.EnableAsciiAll (stream);
  //internet.EnableAsciiIpv4All (stream);
  //p2p.EnablePcapAll ("dynamic-global-routing");
  //p2p.EnablePcapAll ("dynamic-global-routing");

  //nodes.Get(2)->GetObject<Ipv4L3Protocol>()->isBadGuy = true;

  // ------------------------------------------------------------
  // -- Run the simulation
  // --------------------------------------------
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();

  delete[] ipic;
  delete[] ndc;
  delete[] nc;

  NS_LOG_INFO ("Done.");

  return 0;

  // end main
}
