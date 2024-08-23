from mininet.net import Mininet
from mininet.node import Controller, OVSKernelSwitch
from mininet.cli import CLI

def topo():
	# mininet network object
	net = Mininet(controller=Controller, switch=OVSKernelSwitch)

	# add controller, hosts and switches
	c0 = net.addController('c0')
	h1 = net.addHost('h1')
	h2 = net.addHost('h2')
	h3 = net.addHost('h3') 
	s1 = net.addSwitch('s1')
	s2 = net.addSwitch('s2')
	s3 = net.addSwitch('s3')

	# links logic: 
	# s1-s2-s3 all connected together in triangle connection
	# s1-h1, s2-h2, s3-h3 connections
	net.addLink(s1, s2)
	net.addLink(s1, s2)
	net.addLink(s2, s3)
	net.addLink(h1, s1)
	net.addLink(h2, s2)
	net.addLink(h3, s3)

	# begin network
	net.build()
	c0.start()
	s1.start([c0])
	s2.start([c0])
	s3.start([c0])
	h1.cmd('ifconfig h1-eth0 10.0.0.1/24')
	h2.cmd('ifconfig h2-eth0 10.0.0.2/24')
	h3.cmd('ifconfig h3-eth0 10.0.0.3/24')

	# open cli mininet
	CLI(net)

	# stop network
	net.stop()

if __name__ == '__main__':
	topo()
