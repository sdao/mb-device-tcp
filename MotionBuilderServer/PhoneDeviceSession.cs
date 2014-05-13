using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace MotionBuilderServer
{
    class PhoneDeviceSession
    {
        private TcpListener l;
        private SessionsState sessions;

        public PhoneDeviceSession(TcpListener listener, SessionsState s)
        {
            l = listener;
            sessions = s;
        }

        public void Accept()
        {
            Console.WriteLine("Waiting for a phone...");
            Socket sock = l.AcceptSocket();
            NetworkStream s = new NetworkStream(sock);

            Console.WriteLine("Connected to a phone.");

            try
            {
                while (true)
                {
                    // All phone packets are 36 bytes (header + type + 4 x 8 bytes + checksum + trailer).

                    byte[] packet = new byte[36];
                    s.Read(packet, 0, 36);

                    if (packet[0] != ORProtocol.BYTE_HEADER || packet[35] != ORProtocol.BYTE_TRAILER)
                    {
                        throw new InvalidOperationException("The phone has closed the connection.");
                    }

                    ORSession session = sessions.currentOrSession;
                    if (session != null)
                    {
                        session.SendData(packet);
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
