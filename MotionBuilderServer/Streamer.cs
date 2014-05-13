using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace MotionBuilderServer
{
    class Streamer
    {
        private volatile bool didStart;
        private volatile bool shouldStop;
        private volatile bool didStop;
        private NetworkStream s;

        public Streamer(NetworkStream stream)
        {
            shouldStop = false;
            didStop = false;
            s = stream;
        }

        public void Start()
        {
            if (didStart) {
                throw new InvalidOperationException("Already started.");
            }
            didStart = true;

            Thread t = new Thread(DoWork);
            t.Start();
        }

        public void Stop()
        {
            shouldStop = true;
        }

        private void DoWork()
        {
            while (!shouldStop)
            {
                // Do work!
                byte[] buffer = new byte[2048];
                long[] rot = new long[4];

                double seconds = (DateTime.Now - DateTime.MinValue).TotalSeconds;
                double freq = 1.0, ampl = 1.0;

                rot[0] = BitConverter.DoubleToInt64Bits(100 * ampl * Math.Sin(freq * seconds));
                rot[1] = BitConverter.DoubleToInt64Bits(100 * ampl * Math.Sin(freq * seconds));
                rot[2] = BitConverter.DoubleToInt64Bits(100 * ampl * Math.Sin(freq * seconds));
                rot[3] = 0;

                buffer[0] = ORProtocol.BYTE_HEADER;
                buffer[1] = ORProtocol.BYTE_DATA_PACKET;

                buffer[2] = (byte)(rot[0] >> 56);
                buffer[3] = (byte)(rot[0] >> 48);
                buffer[4] = (byte)(rot[0] >> 40);
                buffer[5] = (byte)(rot[0] >> 32);
                buffer[6] = (byte)(rot[0] >> 24);
                buffer[7] = (byte)(rot[0] >> 16);
                buffer[8] = (byte)(rot[0] >> 8);
                buffer[9] = (byte)(rot[0] >> 0);

                buffer[10] = (byte)(rot[1] >> 56);
                buffer[11] = (byte)(rot[1] >> 48);
                buffer[12] = (byte)(rot[1] >> 40);
                buffer[13] = (byte)(rot[1] >> 32);
                buffer[14] = (byte)(rot[1] >> 24);
                buffer[15] = (byte)(rot[1] >> 16);
                buffer[16] = (byte)(rot[1] >> 8);
                buffer[17] = (byte)(rot[1] >> 0);

                buffer[18] = (byte)(rot[2] >> 56);
                buffer[19] = (byte)(rot[2] >> 48);
                buffer[20] = (byte)(rot[2] >> 40);
                buffer[21] = (byte)(rot[2] >> 32);
                buffer[22] = (byte)(rot[2] >> 24);
                buffer[23] = (byte)(rot[2] >> 16);
                buffer[24] = (byte)(rot[2] >> 8);
                buffer[25] = (byte)(rot[2] >> 0);

                buffer[26] = (byte)(rot[3] >> 56);
                buffer[27] = (byte)(rot[3] >> 48);
                buffer[28] = (byte)(rot[3] >> 40);
                buffer[29] = (byte)(rot[3] >> 32);
                buffer[30] = (byte)(rot[3] >> 24);
                buffer[31] = (byte)(rot[3] >> 16);
                buffer[32] = (byte)(rot[3] >> 8);
                buffer[33] = (byte)(rot[3] >> 0);

				byte checkSum = 0;
				for(int i = 2; i < 34; i++)
				{
					checkSum += buffer[i];
				}

				buffer[34] = checkSum;
                buffer[35] = ORProtocol.BYTE_TRAILER;

                s.Write(buffer, 0, 2048);

                Thread.Sleep((int)(1000.0 / 24.0));
            }
            didStop = true;
        }

        public bool Running
        {
            get
            {
                return didStart && !shouldStop;
            }
        }
    }
}
