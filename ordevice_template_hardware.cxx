/***************************************************************************************
 Autodesk(R) Open Reality(R) Samples
 
 (C) 2009 Autodesk, Inc. and/or its licensors
 All rights reserved.
 
 AUTODESK SOFTWARE LICENSE AGREEMENT
 Autodesk, Inc. licenses this Software to you only upon the condition that 
 you accept all of the terms contained in the Software License Agreement ("Agreement") 
 that is embedded in or that is delivered with this Software. By selecting 
 the "I ACCEPT" button at the end of the Agreement or by copying, installing, 
 uploading, accessing or using all or any portion of the Software you agree 
 to enter into the Agreement. A contract is then formed between Autodesk and 
 either you personally, if you acquire the Software for yourself, or the company 
 or other legal entity for which you are acquiring the software.
 
 AUTODESK, INC., MAKES NO WARRANTY, EITHER EXPRESS OR IMPLIED, INCLUDING BUT 
 NOT LIMITED TO ANY IMPLIED WARRANTIES OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR 
 PURPOSE REGARDING THESE MATERIALS, AND MAKES SUCH MATERIALS AVAILABLE SOLELY ON AN 
 "AS-IS" BASIS.
 
 IN NO EVENT SHALL AUTODESK, INC., BE LIABLE TO ANYONE FOR SPECIAL, COLLATERAL, 
 INCIDENTAL, OR CONSEQUENTIAL DAMAGES IN CONNECTION WITH OR ARISING OUT OF PURCHASE 
 OR USE OF THESE MATERIALS. THE SOLE AND EXCLUSIVE LIABILITY TO AUTODESK, INC., 
 REGARDLESS OF THE FORM OF ACTION, SHALL NOT EXCEED THE PURCHASE PRICE OF THE 
 MATERIALS DESCRIBED HEREIN.
 
 Autodesk, Inc., reserves the right to revise and improve its products as it sees fit.
 
 Autodesk and Open Reality are registered trademarks or trademarks of Autodesk, Inc., 
 in the U.S.A. and/or other countries. All other brand names, product names, or 
 trademarks belong to their respective holders. 
 
 GOVERNMENT USE
 Use, duplication, or disclosure by the U.S. Government is subject to restrictions as 
 set forth in FAR 12.212 (Commercial Computer Software-Restricted Rights) and 
 DFAR 227.7202 (Rights in Technical Data and Computer Software), as applicable. 
 Manufacturer is Autodesk, Inc., 10 Duke Street, Montreal, Quebec, Canada, H3C 2L7.
***************************************************************************************/

/**	\file	ordevice_template_hardware.cxx
*	Definition of simple network output hardware.
*	Definition of the ORDevice_Template_Hardware, representing an output
*	device, driving multiple lights on a stage.
*/

//--- Class declaration
#include "ordevice_template_hardware.h"

#include <math.h>

/************************************************
 *	Constructor.
 ************************************************/
ORDevice_Template_Hardware::ORDevice_Template_Hardware()
{
	mParent				= NULL;

	mSerialPort			= 1;
	mSerialBaudRate		= 19200;
	mNetworkAddress		= "127.0.0.1";
	mNetworkPort		= 3001;
	mStreaming			= true;
	mReadState			= eORReadStateHeader;
	mDataCount			= 0;

	mAmplitude			= 1.0;
	mFrequency			= 1.0;

	mRequest[0]			= BYTE_HEADER;
	mRequest[1]			= BYTE_DATA_PACKET;
	mRequest[2]			= 0;
	mRequest[3]			= BYTE_TRAILER;

	mPosition[0]		= 0;
	mPosition[1]		= 0;
	mPosition[2]		= 0;

	mRotation[0]		= 0;
	mRotation[1]		= 0;
	mRotation[2]		= 0;
}

/************************************************
 *	Destructor.
 ************************************************/
ORDevice_Template_Hardware::~ORDevice_Template_Hardware()
{
}

/************************************************
 *	Set the parent.
 ************************************************/
void ORDevice_Template_Hardware::SetParent(FBDevice* pParent)
{
	mParent = pParent;
}

/************************************************
 *	Open device communications.
 ************************************************/
bool ORDevice_Template_Hardware::Open()
{
	bool lSuccess = false;

	switch( GetCommunicationType() )
	{
		case kFBCommTypeSerial:
		{
			mCommPort.PortNumber	= mSerialPort;
			mCommPort.BaudRate		= mSerialBaudRate;

			mCommPort.ByteSize		= 8;
			mCommPort.Parity		= kFBParityNone;
			mCommPort.StopBits		= 1;
			mCommPort.UseHardwareFC	= false;

			mCommPort.Active		= true;
			lSuccess				= (bool) mCommPort.Active;
		}
		break;
		case kFBCommTypeNetworkTCP:
		{
			if( mTCPIP.CreateSocket( mNetworkSocket, kFBTCPIP_Stream ) )
			{
				lSuccess = mTCPIP.Connect( mNetworkSocket, mNetworkAddress, mNetworkPort );
				if( !lSuccess )
				{
					mTCPIP.CloseSocket( mNetworkSocket );
				}
			}
		}
		break;
		case kFBCommTypeNetworkUDP:
		{
			lSuccess = false;
			/*
			if( mTCPIP.CreateSocket( mNetworkSocket, kFBTCPIP_DGRAM ) )
			{
				lSuccess = mTCPIP.Connect( mNetworkSocket, mNetworkAddress, mNetworkPort );
				if( lSuccess )
				{
					lSuccess = mTCPIP.Bind( mNetworkSocket, inet_addr( mNetworkAddress ), mNetworkPort );

					if( ! lSuccess )
					{
						mTCPIP.CloseSocket( mNetworkSocket );
					}
				}
				else
				{
					mTCPIP.CloseSocket( mNetworkSocket );
				}
			}
			*/
		}
		break;
		case kFBCommTypeSimulator:
		{
			lSuccess = true;
		}
		break;
	}

	mReadState = eORReadStateHeader;
	mDataCount = 0;

	return lSuccess;
}


/************************************************
 *	Close device communications.
 ************************************************/
bool ORDevice_Template_Hardware::Close()
{
	bool lSuccess = false;

	switch( GetCommunicationType() )
	{
		case kFBCommTypeSerial:
		{
			mCommPort.Active	= false;
			lSuccess			= (mCommPort.Active == false);
		}
		break;
		case kFBCommTypeNetworkTCP:
		case kFBCommTypeNetworkUDP:
		{
			lSuccess = mTCPIP.CloseSocket( mNetworkSocket );
		}
		break;
		case kFBCommTypeSimulator:
		{
			lSuccess = true;
		}
		break;
	}
	return lSuccess;
}


/************************************************
 *	Poll device.
 ************************************************/
bool ORDevice_Template_Hardware::PollData()
{
	bool	lSuccess		= false;
	int		lBytesWritten	= 0;

	lSuccess = WriteData( mRequest, MAX_REQUEST_SIZE, &lBytesWritten );

	return lSuccess;
}


/************************************************
 *	Fetch a data packet from the device.
 ************************************************/
int ORDevice_Template_Hardware::FetchData()
{
	bool			lSuccess	= false;
	int				lBytesRead	= 0;
	int				lPacketsRead= 0;
	unsigned char	lByte		= 0;
	unsigned char	lCheckSum	= 0;
	int				i,j;

	lSuccess = ReadData( mBuffer, MAX_BUFFER_SIZE, &lBytesRead );

	for( i=0; i<lBytesRead; i++ )
	{
		lByte = mBuffer[i];

		switch( mReadState )
		{
			case eORReadStateHeader:
			{
				if( lByte == BYTE_HEADER )
				{
					mReadState = eORReadStateType;
				}
			}
			break;
			case eORReadStateType:
			{
				if( lByte == BYTE_DATA_PACKET )
				{
					mDataCount = 0;
					mReadState = eORReadStateData;
				}
				else
				{
					mReadState = eORReadStateHeader;
				}
			}
			break;
			case eORReadStateData:
			{
				mData[mDataCount] = lByte;
				mDataCount++;
				if( mDataCount >= MAX_DATA_SIZE )
				{
					mReadState = eORReadStateCRC;
				}
			}
			break;
			case eORReadStateCRC:
			{
				lCheckSum = 0;
				for( j=0; j<MAX_DATA_SIZE; j++ )
				{
					lCheckSum += mData[j];
				}

				if( lByte == lCheckSum )
				{
					/*
					*	Apply the data here.
					*/
					mPosition[0] =	(short) (mData[0]  << 8 | mData[1]  << 0);
					mPosition[1] =	(short) (mData[2]  << 8 | mData[3]  << 0);
					mPosition[2] =	(short) (mData[4]  << 8 | mData[5]  << 0);
					mRotation[0] =	(short) (mData[6]  << 8 | mData[7]  << 0);
					mRotation[1] =	(short) (mData[8]  << 8 | mData[9]  << 0);
					mRotation[2] =	(short) (mData[10] << 8 | mData[11] << 0);

					mReadState = eORReadStateTrailer;
				}
				else
				{
					mReadState = eORReadStateHeader;
				}
			}
			break;
			case eORReadStateTrailer:
			{
				if( lByte == BYTE_TRAILER )
				{
					lPacketsRead++;
					mReadState = eORReadStateHeader;
				}
			}
			break;
		}
	}
	return lPacketsRead;
}


/************************************************
 *	Get the device setup information.
 ************************************************/
bool ORDevice_Template_Hardware::GetSetupInfo()
{
	unsigned char	lPacket[ MAX_REQUEST_SIZE ];
	bool			lSuccess = false;;
	int				lBytesWritten, lBytesRead;
	bool			lStatus[8];
	bool			lStreaming;

	// If streaming, stop & wait for effect
	lStreaming = GetStreaming();
	if( lStreaming )
	{
		StopStream();
		FBSleep( 50 );
	}

	// Emptry buffer
	EmptyBuffer();

	// Build info packet for request
	lPacket[0] = BYTE_HEADER;
	lPacket[1] = BYTE_INFO_PACKET;
	lPacket[2] = 0;
	lPacket[3] = BYTE_TRAILER;

	// Send request for info
	lSuccess = WriteData( lPacket, MAX_REQUEST_SIZE, &lBytesWritten );

	if( lSuccess )
	{
		// Read & assign packet information
		lSuccess = false;
		int delay = 50;
		for ( int i = 0; i < 4 && !lSuccess; i++ ) {
			// Four tries; exponential time increase.
			FBSleep( delay );
			delay *= 2;
			lSuccess = ReadData( lPacket, MAX_REQUEST_SIZE, &lBytesRead );
		}

		if( lSuccess )
		{
			lStatus[0] = (lPacket[2] & 0x01) == 0x01;
			lStatus[1] = (lPacket[2] & 0x02) == 0x02;
			lStatus[2] = (lPacket[2] & 0x04) == 0x04;
			lStatus[3] = (lPacket[2] & 0x08) == 0x08;
			lStatus[4] = (lPacket[2] & 0x10) == 0x01;
			lStatus[5] = (lPacket[2] & 0x20) == 0x02;
			lStatus[6] = (lPacket[2] & 0x40) == 0x04;
			lStatus[7] = (lPacket[2] & 0x80) == 0x08;
		}
	}

	// If streaming re-start streaming
	if( lStreaming )
	{
		StartStream();
	}

	return lSuccess;
}


/************************************************
 *	Start the device streaming mode.
 ************************************************/
bool ORDevice_Template_Hardware::StartStream()
{
	unsigned char	lPacket[ MAX_REQUEST_SIZE ];
	bool			lSuccess = false;
	int				lBytesWritten;

	lPacket[0] = BYTE_HEADER;
	lPacket[1] = BYTE_STREAM_START;
	lPacket[2] = 0;
	lPacket[3] = BYTE_TRAILER;

	lSuccess = WriteData( lPacket, MAX_REQUEST_SIZE, &lBytesWritten );

	return lSuccess;
}


/************************************************
 *	Stop the device streaming mode.
 ************************************************/
bool ORDevice_Template_Hardware::StopStream()
{
	unsigned char	lPacket[ MAX_REQUEST_SIZE ];
	bool			lSuccess = false;
	int				lBytesWritten;

	lPacket[0] = BYTE_HEADER;
	lPacket[1] = BYTE_STREAM_STOP;
	lPacket[2] = 0;
	lPacket[3] = BYTE_TRAILER;

	lSuccess = WriteData( lPacket, MAX_REQUEST_SIZE, &lBytesWritten );

	return lSuccess;
}


/************************************************
 *	Read buffer from hardware.
 ************************************************/
bool ORDevice_Template_Hardware::ReadData( unsigned char* pBuffer, int pSize, int* pNumberOfBytesRead )
{
	bool lSuccess = false;
	switch( GetCommunicationType() )
	{
		case kFBCommTypeSerial:
		{
			lSuccess = mCommPort.Read( pBuffer, pSize, pNumberOfBytesRead);
		}
		break;
		case kFBCommTypeNetworkTCP:
		{
			lSuccess = mTCPIP.Read( mNetworkSocket, pBuffer, pSize, pNumberOfBytesRead);
		}
		break;
		case kFBCommTypeNetworkUDP:
		{
			//mTCPIP.ReadDatagram( mNetworkSocket, pBuffer, pSize );
			lSuccess = false;
		}
		break;
		case kFBCommTypeSimulator:
		{
			lSuccess = true;
			if( pSize == MAX_BUFFER_SIZE )
			{
				short			lPos[3], lRot[3];
				int				i;
				unsigned char	lCheckSum;
				FBTime			lTime;

				lTime = mSystem.SystemTime;

				lPos[0] = (short) 100*mAmplitude*sin(mFrequency*lTime.GetSecondDouble());
				lPos[1] = (short) 100*mAmplitude*sin(mFrequency*lTime.GetSecondDouble());
				lPos[2] = (short) 100*mAmplitude*sin(mFrequency*lTime.GetSecondDouble());
				lRot[0] = 0;
				lRot[1] = 0;
				lRot[2] = 0;

				pBuffer[0] = BYTE_HEADER;
				pBuffer[1] = BYTE_DATA_PACKET;

				pBuffer[2] = (unsigned char)  (lPos[0] >> 8	);
				pBuffer[3] = (unsigned char)  (lPos[0] >> 0 );
				pBuffer[4] = (unsigned char)  (lPos[1] >> 8 );
				pBuffer[5] = (unsigned char)  (lPos[1] >> 0 );
				pBuffer[6] = (unsigned char)  (lPos[2] >> 8 );
				pBuffer[7] = (unsigned char)  (lPos[2] >> 0 );
				pBuffer[8] = (unsigned char)  (lRot[0] >> 8 );
				pBuffer[9] = (unsigned char)  (lRot[0] >> 0 );
				pBuffer[10] = (unsigned char) (lRot[1] >> 8 );
				pBuffer[11] = (unsigned char) (lRot[1] >> 0 );
				pBuffer[12] = (unsigned char) (lRot[2] >> 8 );
				pBuffer[13] = (unsigned char) (lRot[2] >> 0 );

				lCheckSum = 0;
				for( i=0; i<MAX_DATA_SIZE; i++ )
				{
					lCheckSum += pBuffer[2+i];
				}
				pBuffer[14] = lCheckSum;
				pBuffer[15] = BYTE_TRAILER;

				if( pNumberOfBytesRead )
				{
					(*pNumberOfBytesRead) = 2 + MAX_DATA_SIZE + 2;
				}
			}
		}
		break;
	}
	return lSuccess;
}


/************************************************
 *	Send buffer to hardware.
 ************************************************/
bool ORDevice_Template_Hardware::WriteData( unsigned char* pBuffer, int pSize, int* pNumberOfBytesWritten )
{
	bool lSuccess = false;
	switch( GetCommunicationType() )
	{
		case kFBCommTypeSerial:
		{
			lSuccess = mCommPort.Write( pBuffer, pSize, pNumberOfBytesWritten );
		}
		break;
		case kFBCommTypeNetworkTCP:
		{
			lSuccess = mTCPIP.Write( mNetworkSocket, pBuffer, pSize, pNumberOfBytesWritten );
		}
		break;
		case kFBCommTypeNetworkUDP:
		{
			//lBytesWritten = mTCPIP.WriteDatagram( mNetworkSocket, pBuffer, pSize, inet_addr( mNetworkAddress ), mNetworkPort );
			//lSuccess = (lBytesWritten != SOCKET_ERROR);
			lSuccess = false;
		}
		break;
		case kFBCommTypeSimulator:
		{
			lSuccess = true;
		}
		break;
	}
	return lSuccess;
}


/************************************************
 *	EmptyBuffer.
 ************************************************/
void ORDevice_Template_Hardware::EmptyBuffer()
{
	unsigned char	lBuffer[4096];

	switch( GetCommunicationType() )
	{
		case kFBCommTypeSerial:
		{
			mCommPort.EmptyBuffer();
		}
		break;
		case kFBCommTypeNetworkTCP:
		{
			mTCPIP.ReadBlocking(mNetworkSocket, lBuffer, 4096 );
		}
		break;
		case kFBCommTypeNetworkUDP:
		{
			mTCPIP.Read( mNetworkSocket, lBuffer, 4096 );
		}
		break;
		case kFBCommTypeSimulator:
		{
		}
		break;
	}

}


/************************************************
 *	Get the current position.
 ************************************************/
void ORDevice_Template_Hardware::GetPosition(double* pPos)
{
	pPos[0] = mPosition[0];
	pPos[1] = mPosition[1];
	pPos[2] = mPosition[2];
}


/************************************************
 *	Get the current rotation.
 ************************************************/
void ORDevice_Template_Hardware::GetRotation(double* pRot)
{
	pRot[0] = mRotation[0];
	pRot[1] = mRotation[1];
	pRot[2] = mRotation[2];
}

/************************************************
 *	Communications type.
 ************************************************/
void ORDevice_Template_Hardware::SetCommunicationType(FBCommType pType)
{
	mParent->CommType = pType;
}
int ORDevice_Template_Hardware::GetCommunicationType()
{
	return mParent->CommType;
}
