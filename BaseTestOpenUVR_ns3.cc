


//     Node0                                                     Node4
//+------------+                                   +-------------------------------+                            pc                            gateway                          
// I    tap    I                                   I        EmuFdNetDevice         I  192.168.178.201------ 192.168.178.100 --------------- 192.168.178.1
// +------------+                                  +-------------------------------+  
// I  CSMA      I                                  I             CSMA              I
// +------------+                                  +-------------------------------+
//  192.168.10.1                                       192.168.11.2
//     I                                                      I
//     ========Node2==test_connection======Node3================

#include <iostream>
#include <fstream>

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/mobility-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/wifi-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-module.h"
#include "ns3/applications-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/tap-bridge-module.h"
#include "ns3/internet-apps-module.h"
#include "ns3/netanim-module.h"
#include "ns3/fd-net-device-module.h"
#include "ns3/ipv4-list-routing-helper.h"
#include "ns3/ipv4-static-routing-helper.h"

#include "ns3/mobility-module.h"
#include "ns3/nr-module.h"
#include "ns3/antenna-module.h"
#include "ns3/point-to-point-helper.h"



using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("OpenUVRtest");


static void
PingRtt(std::string context, uint16_t seqNo, Time rtt)
{
    NS_LOG_UNCOND("Received " << seqNo << " Response with RTT = " << rtt);
}




int 
main (int argc, char *argv[])
{
  std::string mode = "UseBridge";   //tapbridge mode 
  std::string tapName ="tap-test1";  //name of Tap bridge
  
  std::string deviceName("enp6s0"); 
  std::string remote("192.168.178.1"); 
  std::string localAddress("192.168.178.201"); //FdNetDevice 
  std::string localGateway("192.168.178.1"); //Gateway
  std::string emuMode("raw");
  
  Ipv4Address remoteIp(remote.c_str());
  Ipv4Address localIp(localAddress.c_str());  
  Ipv4Mask localMask("255.255.255.0");
         

  CommandLine cmd;
  cmd.AddValue ("mode", "Mode setting of TapBridge", mode);
  cmd.AddValue ("tapName", "Name of the OS tap device", tapName);
  cmd.Parse (argc, argv);

  GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));
  GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));
    
    //MID subnet
    NodeContainer nodesLeft;
    nodesLeft.Create(2);

    CsmaHelper csmaM;
    csmaM.SetChannelAttribute ("DataRate", StringValue ("10Mbps"));
    csmaM.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (656)));
    
   
    NetDeviceContainer devicesM = csmaM.Install (nodesLeft);

    InternetStackHelper internetLeft;
    internetLeft.Install(nodesLeft);

    Ipv4AddressHelper ipv4M;
    ipv4M.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesLeft = ipv4M.Assign(devicesM);
    
    NodeContainer nodes;
    nodes.Create(2);
    
    InternetStackHelper internetnodes;
    internetnodes.Install(nodes);
    
    //left subnet
    
    CsmaHelper csmaL;
    csmaL.SetChannelAttribute ("DataRate", StringValue ("1000Mbps"));
    csmaL.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (656)));
    
    NodeContainer nodesL = NodeContainer(nodes.Get(0), nodesLeft.Get(0));
    NetDeviceContainer devicesL = csmaL.Install (nodesL);
    
    Ipv4AddressHelper ipv4PL;
    ipv4PL.SetBase("192.168.10.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesPL = ipv4PL.Assign(devicesL);
    
    //tap
    
    TapBridgeHelper tapBridge (interfacesPL.GetAddress (0));
    tapBridge.SetAttribute ("Mode", StringValue (mode));
    tapBridge.SetAttribute ("DeviceName", StringValue (tapName));
    tapBridge.Install (nodesL.Get (0), devicesL.Get (0));
  
    

    
    //right subnet
    
    
    CsmaHelper csmaR;
    csmaR.SetChannelAttribute ("DataRate", StringValue ("1000Mbps"));
    csmaR.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (656)));
    
    NodeContainer nodesR = NodeContainer(nodes.Get(1), nodesLeft.Get(1));
    NetDeviceContainer devicesR = csmaR.Install (nodesR);
    
    Ipv4AddressHelper ipv4PR;
    ipv4PR.SetBase("192.168.11.0", "255.255.255.0");
    Ipv4InterfaceContainer interfacesPR = ipv4PR.Assign(devicesR);
    
    //FdNetDevice
    
    FdNetDeviceHelper* helper = nullptr;


    EmuFdNetDeviceHelper* raw = new EmuFdNetDeviceHelper;
    raw->SetDeviceName(deviceName);
    helper = raw;
    
    NetDeviceContainer devices = helper->Install(nodesR.Get (1));
    Ptr<NetDevice> device = devices.Get(0);
    device->SetAttribute("Address", Mac48AddressValue(Mac48Address::Allocate()));
  
    Ptr<Ipv4> ipv4Node1 = nodesR.Get (0)->GetObject<Ipv4>();
    Ptr<Ipv4> ipv4 = nodesR.Get (1)->GetObject<Ipv4>();
    
    uint32_t interface = ipv4->AddInterface(device);
    Ipv4InterfaceAddress address = Ipv4InterfaceAddress(localIp, localMask);
    ipv4->AddAddress(interface, address);
    ipv4->SetMetric(interface, 1);
    ipv4->SetForwarding (interface, true);
    ipv4->SetUp(interface);
  
    Ipv4Address gateway(localGateway.c_str());
    //Set default route
    Ipv4StaticRoutingHelper ipv4RoutingHelper;
    Ptr<Ipv4StaticRouting> staticRouting1 = ipv4RoutingHelper.GetStaticRouting(ipv4Node1);
    staticRouting1->SetDefaultRoute("192.168.11.2", 1); 
    
    Ptr<Ipv4> ipv4NodeL1 = nodesL.Get (1)->GetObject<Ipv4>();
    Ptr<Ipv4StaticRouting>staticRoutingL1 = ipv4RoutingHelper.GetStaticRouting(ipv4NodeL1);
    staticRoutingL1->SetDefaultRoute("10.1.1.2", 1); 
    
    Ptr<Ipv4> ipv4NodeL2 = nodesL.Get (0)->GetObject<Ipv4>();
    Ptr<Ipv4StaticRouting>staticRoutingL2 = ipv4RoutingHelper.GetStaticRouting(ipv4NodeL2);
    staticRoutingL2->SetDefaultRoute("192.168.178.2", 1); 
    
    
  
  
  
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();//fill routing tabels
  



   

  
    //
    // Enable a promiscuous pcap trace to see what is coming and going on our device.
    //
    helper->EnablePcap(emuMode + "-emu-ping", device, true);
    csmaL.EnablePcapAll("testingPing",true);
    csmaR.EnablePcapAll("testingPing",true);


  NS_LOG_INFO("Run Emulation in " << emuMode << " mode.");
     Simulator::Stop (Seconds (600.));
  Simulator::Run ();
  Simulator::Destroy ();
    delete helper;
    NS_LOG_INFO("Done.");



}
