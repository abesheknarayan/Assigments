#Creating Simulator
set ns [new Simulator]

# Network Animation 
set nf [open t4.nam w]
$ns namtrace-all $nf

set nf1 [open t4.tr w]
$ns trace-all $nf1

#Creating nodes
set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]

#Creating links between nodes
$ns duplex-link $n0 $n2 20Mb 10ms DropTail
$ns duplex-link $n1 $n2 20Mb 10ms DropTail
$ns duplex-link $n2 $n3 20Mb 10ms DropTail

#Creation of Protocols
set tcp [new Agent/TCP]
$ns attach-agent $n0 $tcp


#Traffic Generation Initialize
set ftp [new Application/FTP]
$ftp attach-agent $tcp


#Setting Destination point
set null1 [new Agent/TCPSink]
$ns attach-agent $n3 $null1


#Connection of Protocol and Destination point
$ns connect $tcp $null1

#Finish Procedure
proc finish {} {
	global ns nf null1 nf1
	$ns flush-trace
	close $nf
	exec nam t4.nam &
	exit 0
}


$ns at 0.1 "$ftp start"
$ns at 19.0 "$ftp stop"
$ns at 20.0 "finish"
$ns run



