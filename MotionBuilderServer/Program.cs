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
        private static SessionsState sessions = new SessionsState();

        static void Main(string[] args)
        {
            TcpListener orClientListener = new TcpListener(IPAddress.Any, 3001);
            orClientListener.Start();

            TcpListener phoneListener = new TcpListener(IPAddress.Any, 3002);
            phoneListener.Start();

            Console.WriteLine("Server online.");

            OpenRealityLoop(orClientListener);
            PhoneLoop(phoneListener);

            SemaphoreSlim sema = new SemaphoreSlim(0);
            sema.Wait();
        }

        static async void OpenRealityLoop(TcpListener orClientListener)
        {
            await Task.Run(() =>
                {
                    while (true)
                    {
                        sessions.currentOrSession = new ORSession(orClientListener);
                        sessions.currentOrSession.Accept();
                    }
                });
        }

        static async void PhoneLoop(TcpListener phoneListener)
        {
            await Task.Run(() =>
                {
                    while (true)
                    {
                        sessions.currentPhoneSession = new PhoneDeviceSession(phoneListener, sessions);
                        sessions.currentPhoneSession.Accept();
                    }
                });
        }
    }
}
