BEGIN{
    TCPSent = 0;
    TCPRecieved = 0;
    TCPLost = 0;
    UDPSent = 0;
    UDPRecieved = 0;
    UDPLost = 0;
    totalSent = 0;
    totalRecieved = 0;
    totalLost = 0;

    curr_pktid = -1;
    curr_delay = 0;
    start_time = 0;
    end_time = 0;
    total_delay = 0;
    total_packets = 0;
    avg_delay = 0.0;

    recv=0;
    gotime = 1;
    packet_size = 50;
    time_interval=1;
    total_throughput=0;
    avg_throughput=0.0
}
{
    packetType = $5
    event = $1
    pktid = $12
    time = $2
    node_id = $3
    level = $4
    
    # Throughput
     if(time>gotime) 
    {
        #print gotime, (packet_size * recv * 8.0)/1000; #gives results in kbps
        total_throughput += (packet_size * recv * 8.0)/1000;
        gotime+= time_interval;
        recv=0;
    }
    if ( event == "r") 
    {
        recv++;
    }
    
    # Delivery delay timings
    if(!(pktid in packet_array))
    {
        time_array[pktid,"start"] = time;
        time_array[pktid,"end"] = time;
        packet_array[pktid];
        total_packets++;
    }
    else
    {
        time_array[pktid,"end"] = time;
    }

    # Packet delivery ratio
    if(packetType == "tcp")
    {
        if(event == "+")
        {
            TCPSent++;
        }
        else if(event == "r")
        {
            TCPRecieved++;
        }
        else if(event == "d")
        {
            TCPLost++;
        }
    }
    else if(packetType == "cbr")
    {
        if(event == "+")
        {
            UDPSent++;
        }
        else if(event == "r")
        {
            UDPRecieved++;
        }
        else if(event == "d")
        {
            UDPLost++;
        }
    }
}
END{
# Packet delivery ratio
    totalSent = TCPSent + UDPSent;
    totalLost = TCPLost + UDPLost;
    printf("\n");
    printf("TCP packets sent : %d\n",TCPSent);
    printf("TCP packets recieved : %d\n",TCPRecieved);
    printf("TCP packets dropped: %d\n", TCPLost);
if(TCPSent>0)
printf("PDR  %d\n", (TCPRecieved/TCPSent)*100);

    printf("UDP packets sent : %d\n",UDPSent);
    printf("UDP packets recieved : %d\n",UDPRecieved);
    printf("UDP packets dropped: %d\n", UDPLost);
if(UDPSent>0)
printf("PDR  %d\n", (UDPRecieved/UDPSent)*100);

    printf("Total Sent: %d\n",totalSent);
    printf("Total Dropped: %d\n\n",totalLost);


# Average delay
    for (i in packet_array)
    {
        curr_delay = time_array[i,"end"] - time_array[i,"start"];
        total_delay += curr_delay;
    }
    avg_delay = total_delay / total_packets;
    printf("Average delay: %f\n\n",avg_delay);

# Average Throughput
    avg_throughput = total_throughput/gotime;
    printf("Average throughput: %f\n",avg_throughput);

}