using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace MotionBuilderServer
{
    class Session
    {
        private TcpListener l;

        public Session(TcpListener listener)
        {
            l = listener;
        }

        public void Accept()
        {
            Console.WriteLine("Waiting for a client...");
            Socket sock = l.AcceptSocket();
            NetworkStream s = new NetworkStream(sock);
            Streamer stream = null;

            Console.WriteLine("Connected to a client.");

            try
            {
                while (true)
                {
                    // All MotionBuilder OR plugin client commands are 4-byte packets.

                    byte[] packet = new byte[4];
                    s.Read(packet, 0, 4);

                    if (packet[0] != OpenRealityProtocol.BYTE_HEADER || packet[3] != OpenRealityProtocol.BYTE_TRAILER)
                    {
                        throw new InvalidOperationException("The client has closed the connection.");
                    }

                    switch (packet[1])
                    {
                        case OpenRealityProtocol.BYTE_STREAM_STOP:
                            Console.WriteLine("Client requested stream stop.");
                            if (stream != null)
                            {
                                stream.Stop();
                                Console.WriteLine("--> Stream stopped.");
                            }
                            else
                            {
                                Console.WriteLine("--> Stream already stopped.");
                            }
                            break;

                        case OpenRealityProtocol.BYTE_STREAM_START:
                            Console.WriteLine("Client requested stream start.");
                            if (stream == null || !stream.Running)
                            {
                                stream = new Streamer(s);
                                stream.Start();
                                Console.WriteLine("--> Stream started.");
                            }
                            else
                            {
                                Console.WriteLine("--> Stream already started.");
                            }
                            break;

                        case OpenRealityProtocol.BYTE_INFO_PACKET:
                            Console.WriteLine("Client requested server info.");

                            // Write back a 4-byte packet.
                            byte[] response = new byte[] { OpenRealityProtocol.BYTE_HEADER,
                                OpenRealityProtocol.BYTE_INFO_PACKET,
                                0x0,
                                OpenRealityProtocol.BYTE_TRAILER };
                            s.Write(response, 0, 4);
                            Console.WriteLine("--> Response sent.");
                            break;

                        default:
                            throw new Exception();
                    }
                }
            }
            catch (InvalidOperationException e)
            {
                Console.WriteLine(e.Message);
            }
            catch (Exception e)
            {
                Console.WriteLine("There was a fatal error, and the socket must be closed: " + e.Message);
            }

            s.Close();
            sock.Close();
        }
    }
}
