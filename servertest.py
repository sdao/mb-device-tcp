import socket
import struct
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect(("127.0.0.1", 3002)) # At this point the server should say "Connect to a phone".

header = bytearray()
header.append(0x3C) # Note: the server expects packets to begin with 0x3C 0x44.
header.append(0x44) # See https://github.com/sdao/mb-device-tcp README for wire format details.

w = struct.pack(">d", -0.092296) # Quaternion corresponding to <45 deg, 60 deg, 90 deg> in XYZ rotation order.
x = struct.pack(">d", 0.560986)
y = struct.pack(">d", 0.430459)
z = struct.pack(">d", 0.701057)

checksum = 0
for i in w:
  checksum += i
for i in x:
  checksum += i
for i in y:
  checksum += i
for i in z:
  checksum += i

checksum = checksum % 256

trailer = bytearray()
trailer.append(checksum)
trailer.append(0x3E)

msg = b"".join([header, w, x, y, z, trailer])
s.send(msg)
