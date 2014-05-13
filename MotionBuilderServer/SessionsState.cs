using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MotionBuilderServer
{
    class SessionsState
    {
        public volatile ORSession currentOrSession = null;
        public volatile PhoneDeviceSession currentPhoneSession = null;
    }
}
