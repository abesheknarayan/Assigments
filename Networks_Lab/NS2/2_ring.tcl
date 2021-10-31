#To create Ring Topology

set ns [new Simulator]
set nf [open out.nam w]
$ns namtrace-all $nf

set nf1 [open ring.tr w]
$ns trace-all $nf1

set n0 [$ns node]
set n1 [$ns node]
set n2 [$ns node]
set n3 [$ns node]
set n4 [$ns node]
set n5 [$ns node]

$ns duplex-link $n0 $n1 2Mb 10ms DropTail
$ns duplex-link $n1 $n2 2Mb 10ms DropTail
$ns duplex-link $n2 $n3 2Mb 10ms DropTail
$ns duplex-link $n3 $n4 2Mb 10ms DropTail
$ns duplex-link $n4 $n5 2Mb 10ms DropTail
$ns duplex-link $n5 $n0 2Mb 10ms DropTail

proc finish {} {
global ns nf nf1
$ns flush-trace
close $nf
exec nam out.nam &
exit 0
}

$ns at 2.0 "finish"
$ns run
