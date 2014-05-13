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

/**	\file	ordevice_template_device.cxx
*	Definition of a simple output device.
*	Function definitions of the ORDevice_Template class.
*/

//--- Class declaration
#include "ordevice_template_device.h"

//--- Registration defines
#define ORDEVICETEMPLATE__CLASS		ORDEVICETEMPLATE__CLASSNAME
#define ORDEVICETEMPLATE__NAME		ORDEVICETEMPLATE__CLASSSTR
#define ORDEVICETEMPLATE__LABEL		"OR - Device Template"
#define ORDEVICETEMPLATE__DESC		"OR - Device Template Description"
#define ORDEVICETEMPLATE__PREFIX	"DeviceTemplate"

//--- FiLMBOX implementation and registration
FBDeviceImplementation	(	ORDEVICETEMPLATE__CLASS	);
FBRegisterDevice		(	ORDEVICETEMPLATE__NAME,
							ORDEVICETEMPLATE__CLASS,
							ORDEVICETEMPLATE__LABEL,
							ORDEVICETEMPLATE__DESC,
							FB_DEFAULT_SDK_ICON		);	// Icon filename (default=Open Reality icon)


/************************************************
 *	FiLMBOX Constructor.
 ************************************************/
bool ORDevice_Template::FBCreate()
{
	mHardware.SetParent( this );

	// Create animation nodes
	mNodeMarker_InT	= AnimationNodeOutCreate( 0, "Translation",	ANIMATIONNODE_TYPE_LOCAL_TRANSLATION	);
	mNodeMarker_InR	= AnimationNodeOutCreate( 1, "Rotation",	ANIMATIONNODE_TYPE_LOCAL_ROTATION		);

	// Create model templates
	mTemplateRoot	= new FBModelTemplate( ORDEVICETEMPLATE__PREFIX, "Reference",	kFBModelTemplateRoot	);
	mTemplateMarker	= new FBModelTemplate( ORDEVICETEMPLATE__PREFIX, "Marker",		kFBModelTemplateMarker	);

	// Build model template hierarchy
	ModelTemplate.Children.Add(mTemplateRoot);
	mTemplateRoot->Children.Add(mTemplateMarker);

	// Bind the model templates (if applicable) to device's animation nodes
	mTemplateMarker->Bindings.Add( mNodeMarker_InR );
	mTemplateMarker->Bindings.Add( mNodeMarker_InT );

	// Set sampling rate to 60 Hz
	FBTime	lPeriod;
	lPeriod.SetSecondDouble(1.0/60.0);
	SamplingPeriod	= lPeriod;

	CommType = kFBCommTypeSimulator;
	mSetCandidate = false;

	return true;
}

/************************************************
 *	FiLMBOX Destructor.
 ************************************************/
void ORDevice_Template::FBDestroy()
{
}


/************************************************
 *	Device operation.
 ************************************************/
bool ORDevice_Template::DeviceOperation( kDeviceOperations pOperation )
{
	switch (pOperation)
	{
		case kOpInit:	return Init();
		case kOpStart:	return Start();
		case kOpStop:	return Stop();
		case kOpReset:	return Reset();
		case kOpDone:	return Done();
	}
	return FBDevice::DeviceOperation( pOperation );
}


/************************************************
 *	Initialization of device.
 ************************************************/
bool ORDevice_Template::Init()
{
    FBProgress	lProgress;

    lProgress.Caption	= "Device Template";
	lProgress.Text		= "Initializing device...";

	return true;
}


/************************************************
 *	Device is put online.
 ************************************************/
bool ORDevice_Template::Start()
{
	FBProgress	lProgress;
	lProgress.Caption	= "Starting up device";

	// Step 1: Open device communications
	lProgress.Text	= "Opening device communications";
	Status			= "Opening device communications";

	if(!mHardware.Open())
	{
		Status = "Could not open device";
		return false;
	}

	// Step 2: Ask hardware to get channel information
	lProgress.Text	= "Device found, getting setup information";
	Status			= "Getting setup information";

	if(!mHardware.GetSetupInfo())
	{
		Status = "Could not get setup information from device.";
		return false;
	}
	else
	{
		HardwareVersionInfo = "Device Template, v1.0";
		Information			= "";
	}

	if( mHardware.GetStreaming() )
	{
		// Step 3: Start streaming data from device
		lProgress.Text	= "Sending START STREAM command";
		Status			= "Starting device streaming";

		if(!mHardware.StartStream())
		{
			Status	= "Could not start stream mode";
			return false;
		}
	}
	Status = "Ok";
	return true;
}


/************************************************
 *	Device is stopped (offline).
 ************************************************/
bool ORDevice_Template::Stop()
{
	FBProgress	lProgress;
	lProgress.Caption	= "Shutting down device";

	if( mHardware.GetStreaming() )
	{
		// Step 1: Stop streaming data
		lProgress.Text	= "Sending STOP STREAM command";
		Status			= "Stopping device streaming";

		if(!mHardware.StopStream())
		{
			Status = "Could not stop streaming";
			return false;
		}
	}

	// Step 1: Stop streaming data
	lProgress.Text	= "Stopping device communications";
	Status			= "Stopping device communications";

	if(!mHardware.Close())
	{
		Status = "Could not close device";
		return false;
	}

	Status			= "?";

    return false;
}


/************************************************
 *	Removal of device.
 ************************************************/
bool ORDevice_Template::Done()
{
	return false;
}


/************************************************
 *	Reset of device.
 ************************************************/
bool ORDevice_Template::Reset()
{
    Stop();
    return Start();
}


/************************************************
 *	Real-Time Engine Evaluation.
 ************************************************/
bool ORDevice_Template::AnimationNodeNotify(FBAnimationNode* pAnimationNode ,FBEvaluateInfo* pEvaluateInfo)
{
	double	lPos[3];
	double	lRot[3];

	mHardware.GetPosition( lPos );
	mHardware.GetRotation( lRot );

	mNodeMarker_InT->WriteData( lPos, pEvaluateInfo );
	mNodeMarker_InR->WriteData( lRot, pEvaluateInfo );

    return true;
}


/************************************************
 *	Device Evaluation Notify.
 ************************************************/
bool ORDevice_Template::DeviceEvaluationNotify( kTransportMode pMode, FBEvaluateInfo* pEvaluateInfo )
{
	return true;
}


/************************************************
 *	Real-Time Synchronous Device IO.
 ************************************************/
void ORDevice_Template::DeviceIONotify( kDeviceIOs  pAction,FBDeviceNotifyInfo &pDeviceNotifyInfo)
{
	int		i;
	int		lNumberOfPackets;
	FBTime	lPacketTimeCode;

    switch (pAction)
	{
		// Output devices
		case kIOPlayModeWrite:
		case kIOStopModeWrite:
		{
		}
		break;

		// Input devices
		case kIOStopModeRead:
		case kIOPlayModeRead:
		{
			lNumberOfPackets = mHardware.FetchData();

			for( i=0; i<lNumberOfPackets; i++ )
			{
				DeviceRecordFrame	( pDeviceNotifyInfo );
				AckOneSampleReceived( );
			}

			if( !mHardware.GetStreaming() )
			{
				mHardware.PollData();
			}
		break;
		}
	}
}


/************************************************
 *	Record a frame of the device (recording).
 ************************************************/
void ORDevice_Template::DeviceRecordFrame( FBDeviceNotifyInfo &pDeviceNotifyInfo )
{
	FBAnimationNode*	lDataT	= NULL;
	FBAnimationNode*	lDataR = NULL;

	double	lPos[3];
	double	lRot[3];

	FBTime	lTime;

	lTime = pDeviceNotifyInfo.GetLocalTime();

	// Translation information.
	lDataT = mNodeMarker_InT->GetAnimationToRecord();
	if (lDataT)
	{
		if( mPlayerControl.GetTransportMode() == kFBTransportPlay )
		{
			mHardware.GetPosition( lPos );

			switch( SamplingMode )
			{
				case kFBHardwareTimestamp:
				case kFBSoftwareTimestamp:
				{
					lDataT->KeyAdd(lTime, lPos);
				}
				break;

				case kFBHardwareFrequency:
				case kFBAutoFrequency:
				{
					lDataT->KeyAdd(lPos);
				}
				break;
			}
		}
	}

	// Rotation information.
	lDataR = mNodeMarker_InR->GetAnimationToRecord();
	if (lDataR)
	{
		if( mPlayerControl.GetTransportMode() == kFBTransportPlay )
		{
			mHardware.GetRotation( lRot );
			switch( SamplingMode )
			{
				case kFBHardwareTimestamp:
				case kFBSoftwareTimestamp:
				{
					lDataR->KeyAdd(lTime, lRot);
				}
				break;

				case kFBHardwareFrequency:
				case kFBAutoFrequency:
				{
					lDataR->KeyAdd(lRot);
				}
				break;
			}
		}
	}
}

void ORDevice_Template::SetCandidates()
{
	double	lPos[3];
	double	lRot[3];

	mHardware.GetPosition( lPos );
	mHardware.GetRotation( lRot );

	mNodeMarker_InT->SetCandidate( lPos );
	mNodeMarker_InR->SetCandidate( lRot );
}

//--- FBX load/save tags
#define FBX_VERSION_TAG		"Version"
#define FBX_VERSION_VAL		100
#define FBX_COMMPARAM_TAG	"Communication"

/************************************************
 *	Store data in FBX.
 ************************************************/
bool ORDevice_Template::FbxStore(FBFbxObject* pFbxObject,kFbxObjectStore pStoreWhat)
{
	if (pStoreWhat & kAttributes)
	{
		// Write version
		pFbxObject->FieldWriteI		(FBX_VERSION_TAG,FBX_VERSION_VAL);

		// Write communications settings
		pFbxObject->FieldWriteBegin	(FBX_COMMPARAM_TAG);
		{
		}
	    pFbxObject->FieldWriteEnd();
	}
	return true;
}


/************************************************
 *	Retrieve data from FBX.
 ************************************************/
bool ORDevice_Template::FbxRetrieve(FBFbxObject* pFbxObject,kFbxObjectStore pStoreWhat)
{
	int			Version;
	FBString	name;

	if (pStoreWhat & kAttributes)
	{
		// Get version
		Version	= pFbxObject->FieldReadI(FBX_VERSION_TAG);

		// Get communications settings
		if (pFbxObject->FieldReadBegin(FBX_COMMPARAM_TAG))
		{

			pFbxObject->FieldReadEnd();
		}
	}

	return true;
}
