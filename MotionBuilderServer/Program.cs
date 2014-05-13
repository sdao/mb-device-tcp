using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace MotionBuilderServer
{
    class Program
    {
        static void Main(string[] args)
        {
            TcpListener listener = new TcpListener(IPAddress.Loopback, 3001);
            listener.Start();

            Console.WriteLine("Server online.");

            while (true)
            {
                Session session = new Session(listener);
                session.Accept();

                Console.WriteLine("Press any key to re-connect, or 'q' to quit.");
                if (Console.ReadKey(true).Key == ConsoleKey.Q)
                {
                    break;
                }
            }
        }
    }
}
