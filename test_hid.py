import hid
import struct
from time import sleep

dev = hid.device()
dev.open(0x16c0, 0x486)
dev.set_nonblocking(True)

## floats
dev.write(b'x' + b'f' + struct.pack('>f', 3.14))
sleep(0.3)
res = dev.read(64)

# should be ~ 6.14 (added 3 to the input)
print('Expect ~ 6.14')
print(struct.unpack('>f', bytearray(res[1:5])))

## char
dev.write(b'x' + b'c' + b'y')
sleep(0.3)
res = dev.read(64)
print('Expect y')
print(chr(res[1])) # should be y

## short (int)
dev.write(b'x' + b'i' + struct.pack('>h', 6))
sleep(0.3)
res = dev.read(64)
print('Expect 18')
print(struct.unpack('>h', bytearray(res[1:3])))

## long
print('Expect 13270')
dev.write(b'x' + b'l' + struct.pack('>l', 1327))
sleep(0.3)
res = dev.read(64)
print(struct.unpack('>l', bytearray(res[1:5])))

dev.close()