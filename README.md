TCP Device for MotionBuilder Open Reality SDK
=============================================
This is the `device_template` device from the MotionBuilder Open Reality SDK, fixed so that data over TCP works.
The plugin receives a stream of TCP packets that encode rotation information as a quaternion.
For a quaternion _w_ + _x_**i** + _y_**j** + _z_**k**, the wire format is a 36-byte packet:

- Byte 0: the header (`BYTE_HEADER = 0x3c`)
- Byte 1: the data packet identifier (`BYTE_DATA_PACKET = 0x44`)
- Bytes 2-9: _w_ in big-endian order
- Bytes 10-17: _x_ in big-endian order
- Bytes 18-25: _y_ in big-endian order
- Bytes 26-33: _z_ in big-endian order
- Byte 34: the checksum, which is the sum of all bytes 2-33 truncated to a byte
- Byte 35: the trailer (`BYTE_TRAILER = 0x3e`)

The solution comprises the plugin for MotionBuilder (a C++ project) and a TCP server (a C# project).
The server receives 36-byte quaternion data packets from a third party, negotiates the connection with MotionBuilder,
and transmits the data to the MotionBuilder plugin.

Quick Walkthrough
=================
1. Start the server on the local machine. It requires two TCP clients: MotionBuilder and some other client
that will send it quaternion data. The server will wait to connect to MotionBuilder and
a separate physical device that can send it the 36-byte quaternion packets.
2. Start MotionBuilder with the plugin installed. Go to the Asset Browser and choose Devices in the list.
This plugin device uses the default name `OR - Device Template`.
3. Configure it by going under the Communication tab and choosing `Network` instead of `Simulator`.
Leave the address as `127.0.0.1` (loopback); the server is configured to listen for the MotionBuilder plugin on port 3001.
4. Choose `Online` to connect to the server. The indicator should change from red to yellow (idle).
In addition, the server should indicate that MotionBuilder is connected in the console.
5. Choose a model binding, or use a Relation constraint on the plugin device's Rotation property.
(Note that a Translation property exists, but it will always return 0.)
6. Start a physical device that will stream data to the server.
Sample software is available for Windows Phone; see the [mb-device-tcp-phone](https://github.com/sdao/mb-device-tcp-phone)
repository for a sample Windows Phone 8.1 application that will stream the phone's orientation to the server.
At this point, the server should indicate that the physical device/phone is connected.
7. The device/phone should begin transmitting to MotionBuilder, and you should be able to see the
orientation change in MotionBuilder.
