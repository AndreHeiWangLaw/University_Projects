# Andre Hei Wang Law
# 4017 5600
# coen366 FL-X
# mininet a2

from mininet.net import Mininet
from mininet.node import Controller
from mininet.cli import CLI
from mininet.link import TCLink

def create_topology():
	net = Mininet(controller=Controller, link=TCLink)

	# Q1: add a controller
	c0 = net.addController('c0')

	# add hosts
	h1 = net.addHost('h1')
	h2 = net.addHost('h2')
	h3 = net.addHost('h3')

	# add switches
	s1 = net.addSwitch('s1')
	s2 = net.addSwitch('s2')

	# create links 
	net.addLink(h1, s1, bw=20, delay='10ms')
	net.addLink(h2, s1, bw=25, delay='10ms')
	net.addLink(s1, s2, bw=11, delay='40ms')
	net.addLink(s2, h3, bw=15, delay='7ms')

	# build
	net.build()
	c0.start()
	s1.start([c0])
	s2.start([c0])
    
	# Q2: test the reachability between every host
	net.pingAll()    
    
	# Q3: run UDP traffic between H1 and H2 for 10 seconds
    h2.cmd('iperf3 -s &')
    q3 = (h1.cmd('iperf3 -c {} -u -t 10'.format(h2.IP())))
	print(q3)

	# Q4: run UDP traffic between H1 and H3 for 20 seconds
	h3.cmd('iperf3 -s &')
	q4=  (h1.cmd('iperf3 -c {} -u -t 20'.format(h3.IP())))
	print(q4)

	# Q5: run UDP traffic sending 1 Gbytes between H1 and H2
	h2.cmd('iperf3 -s &')
	q5 = (h1.cmd('iperf3 -c {} -u -n 1'.format(h2.IP())))
	print(q5)

	# Q6: run UDP traffic sending 2 Gbytes between H1 and H3
	h3.cmd('iperf3 -s &')
	q6 = (h1.cmd('iperf3 -c {} -u -n 2'.format(h3.IP())))
	print(q6)

	# Q7: run TCP traffic between H1 and H3 for 20 seconds
	h3.cmd('iperf3 -s &')
	q7 = (h1.cmd('iperf3 -c {} -t 20 -i 1'.format(h3.IP())))
	print(q7)

	# Q8: store the output of a command (parts 3-7)
	with open('output.txt', 'w') as f:
		f.write('Q3 Output:\n')
		f.write(q3 + '\n\n')

		f.write('Q4 Output:\n')
		f.write(q4 + '\n\n')

		f.write('Q5 Output:\n')
		f.write(q5 + '\n\n')

        f.write('Q6 Output:\n')
		f.write(q6 + '\n\n')

		f.write('Q7 Output:\n')
		f.write(q7 + '\n\n')

	CLI(net)
	net.stop()

if __name__ == '__main__':
	create_topology()