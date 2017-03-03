from approxeng.pi2arduino import _p2a as p2a, send as send
import time

p2a.init()
while 1:
	for hue in range(0,250,10):
		print "Sending hue {}".format(hue)
		send(0x61,1,hue,20)
		time.sleep(1)

