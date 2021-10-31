# wrls1.tcl
# A 3-node example for ad-hoc simulation with DSR

# Define options
set val(chan)           Channel/WirelessChannel    ;# channel type
set val(prop)           Propagation/TwoRayGround   ;# radio-propagation model
set val(netif)          Phy/WirelessPhy            ;# network interface type
set val(mac)            Mac/802_11                 ;# MAC type
set val(ifq)            CMUPriQueue                ;# interface queue type
set val(ll)             LL                         ;# link layer type
set val(ant)            Antenna/OmniAntenna        ;# antenna model
set val(ifqlen)         50                         ;# max packet in ifq
set val(nn)             8                          ;# number of mobilenodes
set val(rp)             DSR                       ;# routing protocol
set val(x)              1000   			   ;# X dimension of topography
set val(y)              1000   			   ;# Y dimension of topography  
set val(stop)		150			   ;# time of simulation end
#AODV- Adhoc OnDemand Distance Vector

#Creation of Simulator
set ns [new Simulator] 

#Creation of Trace File
set tracefd [open dsr.tr w]
$ns trace-all $tracefd

#Creation of Network Animation file
set namtrace [open simwrls.nam w]    
$ns namtrace-all-wireless $namtrace $val(x) $val(y)

# set up topography object
set topo [new Topography]
$topo load_flatgrid $val(x) $val(y); # No z-axis in flatgrid (2D)

#GOD creation- General Operations Director
#GOD object handles the routing, packet exchange etc...
create-god $val(nn)

#Create nn mobilenodes [$val(nn)] and attach them to the channel. 


# configure the nodes
        $ns node-config -adhocRouting $val(rp) \
			 -llType $val(ll) \
			 -macType $val(mac) \
			 -ifqType $val(ifq) \
			 -ifqLen $val(ifqlen) \
			 -antType $val(ant) \
			 -propType $val(prop) \
			 -phyType $val(netif) \
			 -channelType $val(chan) \
			 -topoInstance $topo \
			 -agentTrace ON \
			 -routerTrace ON \
			 -macTrace OFF \
			 -movementTrace ON
			 
	for {set i 0} {$i < $val(nn) } { incr i } {
		set node_($i) [$ns node]	
	}

# Provide initial location of mobilenodes
$node_(0) set X_ 602.0
$node_(0) set Y_ 467.0
$node_(0) set Z_ 0.0

$node_(1) set X_ 743.0
$node_(1) set Y_ 557.0
$node_(1) set Z_ 0.0

$node_(2) set X_ 932.0
$node_(2) set Y_ 556.0
$node_(2) set Z_ 0.0

$node_(3) set X_ 1124.0
$node_(3) set Y_ 566.0
$node_(3) set Z_ 0.0

$node_(4) set X_ 1268.0
$node_(4) set Y_ 507.0
$node_(4) set Z_ 0.0

$node_(7) set X_ 1209.0
$node_(7) set Y_ 291.0
$node_(7) set Z_ 0.0

$node_(6) set X_ 968.0
$node_(6) set Y_ 300.0
$node_(6) set Z_ 0.0

$node_(5) set X_ 752.0
$node_(5) set Y_ 302.0
$node_(5) set Z_ 0.0

# Generation of movements
$ns at 20.0 "$node_(5) setdest 500.0 100.0 100.0"
$ns at 75.0 "$node_(5) setdest 752.0 302.0 100.0"

# Set a TCP connection between node_(0) and node_(1)
set tcp [new Agent/TCP]
$ns attach-agent $node_(0) $tcp

set sink [new Agent/TCPSink]
$ns attach-agent $node_(7) $sink
$ns connect $tcp $sink
#Traffic generator
set ftp [new Application/FTP]
$ftp attach-agent $tcp
$ns at 10.0 "$ftp start" 

# Define node initial position in nam
for {set i 0} {$i < $val(nn)} { incr i } {
$ns initial_node_pos $node_($i) 125 ;#30 defines the node size for NAM
}

# Telling nodes when the simulation ends
for {set i 0} {$i < $val(nn) } { incr i } {
    $ns at $val(stop) "$node_($i) reset";
}

# ending nam and the simulation 
$ns at $val(stop) "$ns nam-end-wireless $val(stop)"
$ns at $val(stop) "stop"
proc stop {} {
    global ns tracefd namtrace
    $ns flush-trace
    close $tracefd
    close $namtrace
exec nam simwrls.nam &
exit 0
}

$ns run

