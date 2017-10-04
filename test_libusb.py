import usb.core
import usb.util
import struct
from time import sleep

def pad(string):
    return string + b'x' * (64 - len(string))

dev = usb.core.find(idVendor=0x16c0, idProduct=0x486)

ep = dev[0][(0,0)][0]
epo = dev[0][(0,0)][1]

## floats
epo.write(pad(b'f' + struct.pack('>f', 3.14)))
sleep(0.3)
res = ep.read(ep.wMaxPacketSize)

print('Expect ~6.14')
print(struct.unpack('>f', res[1:5]))

## char
epo.write(pad(b'c' + b'y'))
sleep(0.3)
res = ep.read(ep.wMaxPacketSize)
print('Expect y')
print(chr(res[1]))

## short (int)
epo.write(pad(b'i' + struct.pack('>h', 6)))
sleep(0.3)
res = ep.read(64)
print('Expect 18')
print(struct.unpack('>h', res[1:3]))

## long
epo.write(pad(b'l' + struct.pack('>l', 1327)))
sleep(0.3)
res = ep.read(64)
print('Expect 13270')
print(struct.unpack('>l', res[1:5]))

