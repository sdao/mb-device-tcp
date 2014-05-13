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
                short[] pos = new short[3];
                short[] rot = new short[3];

                double seconds = (DateTime.Now - DateTime.MinValue).TotalSeconds;
                double freq = 1.0, ampl = 1.0;

                pos[0] = (short)(100 * ampl * Math.Sin(freq * seconds));
                pos[1] = (short)(100 * ampl * Math.Sin(freq * seconds));
                pos[2] = (short)(100 * ampl * Math.Sin(freq * seconds));

                buffer[0] = OpenRealityProtocol.BYTE_HEADER;
                buffer[1] = OpenRealityProtocol.BYTE_DATA_PACKET;

				buffer[2] = (byte)  (pos[0] >> 8	);
				buffer[3] = (byte)  (pos[0] >> 0 );
				buffer[4] = (byte)  (pos[1] >> 8 );
				buffer[5] = (byte)  (pos[1] >> 0 );
				buffer[6] = (byte)  (pos[2] >> 8 );
				buffer[7] = (byte)  (pos[2] >> 0 );
				buffer[8] = (byte)  (rot[0] >> 8 );
				buffer[9] = (byte)  (rot[0] >> 0 );
				buffer[10] = (byte) (rot[1] >> 8 );
				buffer[11] = (byte) (rot[1] >> 0 );
				buffer[12] = (byte) (rot[2] >> 8 );
				buffer[13] = (byte) (rot[2] >> 0 );

				byte checkSum = 0;
				for(int i = 2; i < 14; i++)
				{
					checkSum += buffer[i];
				}

				buffer[14] = checkSum;
                buffer[15] = OpenRealityProtocol.BYTE_TRAILER;

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
