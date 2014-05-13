using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MotionBuilderServer
{
    static class ORProtocol
    {
        public const int BYTE_HEADER = 0x3c;
        public const int BYTE_INFO_PACKET = 0x49;
        public const int BYTE_DATA_PACKET = 0x44;
        public const int BYTE_STREAM_START = 0x41;
        public const int BYTE_STREAM_STOP = 0x42;
        public const int BYTE_TRAILER = 0x3e;
    }
}
