

 #Aim : To create  two nodes using TCl

#We are going to define a very simple topology with two nodes that are connected by a link. 


set ns [new Simulator]


set nf [open o.nam w]
$ns namtrace-all $nf

set nf1 [open out.tr w]
$ns trace-all $nf1

set n0 [$ns node]
set n1 [$ns node]

$n0 color red
$n1 color blue

$ns duplex-link $n0 $n1 2Mb 10ms DropTail

proc finish {} {
global ns nf nf1
$ns flush-trace
close $nf
exec nam o.nam &
exit 0
}
$ns at 2.0 "finish"
$ns run






