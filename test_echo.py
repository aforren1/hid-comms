import hid
import struct
from time import sleep

dev = hid.device()
dev.open(0x16c0, 0x486)
dev.set_nonblocking(True)

dev.write(b'xabcdef')
sleep(0.3)
res = dev.read(64)
print('Expect abcdef')
print(struct.unpack('s'*6, bytearray(res[0:5]))

dev.close()
