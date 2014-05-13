using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace MotionBuilderServer
{
    class ORSession
    {
        private TcpListener l;
        private volatile bool streaming = false;
        private volatile NetworkStream s = null;
        private object streamLock = new object();

        public ORSession(TcpListener listener)
        {
            l = listener;
        }

        public void SendData(byte[] data)
        {
            if (data.Length > 2048)
            {
                throw new ArgumentException("The field data cannot be larger than 2048 bytes.");
            }

            lock (streamLock)
            {
                if (streaming && s != null)
                {
                    byte[] buffer = new byte[2048];
                    Array.Copy(data, buffer, data.Length);
                    s.Write(buffer, 0, 2048);
                }
            }
        }

        public void Accept()
        {
            Console.WriteLine("Waiting for MotionBuilder...");
            Socket sock = l.AcceptSocket();
            lock (streamLock)
            {
                s = new NetworkStream(sock);
            }

            Console.WriteLine("Connected to MotionBuilder.");

            try
            {
                while (true)
                {
                    // All MotionBuilder OR plugin client commands are 4-byte packets.

                    byte[] packet = new byte[4];
                    s.Read(packet, 0, 4);

                    if (packet[0] != ORProtocol.BYTE_HEADER || packet[3] != ORProtocol.BYTE_TRAILER)
                    {
                        throw new InvalidOperationException("The client has closed the connection.");
                    }

                    switch (packet[1])
                    {
                        case ORProtocol.BYTE_STREAM_STOP:
                            Console.WriteLine("Client requested stream stop.");
                            lock (streamLock)
                            {
                                if (streaming)
                                {
                                    streaming = false;
                                    Console.WriteLine("--> Stream stopped.");
                                }
                                else
                                {
                                    Console.WriteLine("--> Stream already stopped.");
                                }
                            }
                            break;

                        case ORProtocol.BYTE_STREAM_START:
                            Console.WriteLine("Client requested stream start.");
                            lock (streamLock)
                            {
                                if (!streaming)
                                {
                                    streaming = true;
                                    Console.WriteLine("--> Stream started.");
                                }
                                else
                                {
                                    Console.WriteLine("--> Stream already started.");
                                }
                            }
                            break;

                        case ORProtocol.BYTE_INFO_PACKET:
                            Console.WriteLine("Client requested server info.");

                            // Write back a 4-byte packet.
                            byte[] response = new byte[] { ORProtocol.BYTE_HEADER,
                                ORProtocol.BYTE_INFO_PACKET,
                                0x0,
                                ORProtocol.BYTE_TRAILER };
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

            lock (streamLock)
            {
                streaming = false;
                s.Close();
                s = null;
                sock.Close();
            }
        }
    }
}
