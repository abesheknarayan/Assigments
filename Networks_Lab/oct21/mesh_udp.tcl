#Creating Simulator
set ns [new Simulator]

# Network Animation 
set nf [open u2.nam w]
$ns namtrace-all $nf

set nf1 [open u2.tr w]
$ns trace-all $nf1

#Creating nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]
set n6 [$ns node]
set n7 [$ns node]
set n8 [$ns node]

#Creating links between nodes
$ns duplex-link $n0 $n1 20Mb 10ms DropTail
$ns duplex-link $n1 $n2 20Mb 10ms DropTail
$ns duplex-link $n3 $n4 20Mb 10ms DropTail
$ns duplex-link $n4 $n5 20Mb 10ms DropTail
$ns duplex-link $n6 $n7 20Mb 10ms DropTail
$ns duplex-link $n7 $n8 20Mb 10ms DropTail
$ns duplex-link $n0 $n3 20Mb 10ms DropTail
$ns duplex-link $n3 $n6 20Mb 10ms DropTail
$ns duplex-link $n1 $n4 20Mb 10ms DropTail
$ns duplex-link $n4 $n7 20Mb 10ms DropTail
$ns duplex-link $n2 $n5 20Mb 10ms DropTail
$ns duplex-link $n5 $n8 20Mb 10ms DropTail

#Creation of Protocols
set udp [new Agent/UDP]
$ns attach-agent $n1 $udp

#Traffic Generation Initialize
set cbr [new Application/Traffic/CBR]
$cbr attach-agent $udp 
$cbr set packet_size_ 1000
$cbr set interval_ 2

#Setting Destination point
set null2 [new Agent/Null]
$ns attach-agent $n3 $null2

#Connection of Protocol and Destination point
$ns connect $udp $null2



#Finish Procedure
proc finish {} {
	global ns nf null2 nf1
	$ns flush-trace
	close $nf
	exec nam u2.nam &
	exit 0
}


$ns at 0.5 "$cbr start"
$ns at 19.5 "$cbr stop"


$ns at 20.0 "finish"
$ns run



