set ns [new Simulator]
set traceFile [open ls.tr w]
$ns trace-all $traceFile


set namFile [open LS.nam w]
$ns namtrace-all $namFile


proc finish {} {
global ns traceFile namFile
$ns flush-trace
close $traceFile
close $namFile
exec nam LS.nam &
exit 0
}

$ns rtproto LS 


set node1 [$ns node]
set node2 [$ns node]
set node3 [$ns node]
set node4 [$ns node]
set node5 [$ns node]
set node6 [$ns node]
set node7 [$ns node]
set node8 [$ns node]
set node9 [$ns node]
set node10 [$ns node]
set node11 [$ns node]

$node1 color red
$node2 color green
$node3 color red
$node4 color green
$node5 color red
$node6 color green
$node7 color red
$node8 color green
$node9 color red
$node10 color green
$node11 color red


$ns duplex-link $node1 $node4 10Mb 8ms DropTail
$ns duplex-link $node2 $node4 10Mb 8ms DropTail
$ns duplex-link $node3 $node4 10Mb 8ms DropTail
$ns duplex-link $node4 $node5 10Mb 8ms DropTail
$ns duplex-link $node5 $node6 10Mb 8ms DropTail
$ns duplex-link $node6 $node7 10Mb 8ms DropTail
$ns duplex-link $node3 $node8 10Mb 8ms DropTail
$ns duplex-link $node8 $node9 10Mb 8ms DropTail
$ns duplex-link $node7 $node9 10Mb 8ms DropTail
$ns duplex-link $node7 $node10 10Mb 8ms DropTail
$ns duplex-link $node7 $node11 10Mb 8ms DropTail
$ns duplex-link $node9 $node10 10Mb 8ms DropTail
$ns duplex-link $node9 $node11 10Mb 8ms DropTail

set udp1 [new Agent/UDP]
$ns attach-agent $node5 $udp1
set null1 [new Agent/Null]
$ns attach-agent $node8 $null1
$ns connect $udp1 $null1
$udp1 set fid_ 1

set tcp [new Agent/TCP]
$ns attach-agent $node5 $tcp

set sink [new Agent/TCPSink]
$ns attach-agent $node8 $sink
$ns connect $tcp $sink

set ftp [new Application/FTP]
$ftp attach-agent $tcp


set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp1
$cbr1 set packetSize_ 500
$cbr1 set rate_ 1.5Mb

$ns at 2 "$cbr1 start"
$ns at 30 "$ftp start" 
$ns rtmodel-at 8.0 down $node4 $node5
$ns rtmodel-at 12.0 down $node5 $node6
$ns rtmodel-at 16.0 up $node5 $node6
$ns rtmodel-at 24.0 up $node4 $node5
$ns at 40.0 "finish"
$ns run