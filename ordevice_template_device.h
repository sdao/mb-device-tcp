#ifndef __ORDEVICE_TEMPLATE_DEVICE_H__
#define __ORDEVICE_TEMPLATE_DEVICE_H__
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

/**	\file	ordevice_template_device.h
*	Declaration of a simple output device.
*	Declaration of a simple output device, FBSimpleOutputDevice.
*/

//--- SDK include
#include <fbsdk/fbsdk.h>

//--- Class declaration
#include "ordevice_template_hardware.h"

//--- Registration defines
#define ORDEVICETEMPLATE__CLASSNAME		ORDevice_Template
#define ORDEVICETEMPLATE__CLASSSTR		"ORDevice_Template"

//! Device Template.
class ORDevice_Template : public FBDevice
{
	//--- FiLMBOX declaration
	FBDeviceDeclare( ORDevice_Template, FBDevice );
public:
	//--- FiLMBOX Construction/Destruction
	virtual bool FBCreate();		//!< FiLMBOX constructor.
	virtual void FBDestroy();		//!< FiLMBOX destructor.

	//--- The following will be called by the real-time engine.
	virtual bool AnimationNodeNotify	(	FBAnimationNode* pAnimationNode,	FBEvaluateInfo* pEvaluateInfo			);	//!< Real-time evaluation for node.
	virtual void DeviceIONotify			(	kDeviceIOs  pAction,				FBDeviceNotifyInfo &pDeviceNotifyInfo	);	//!< Notification of/for Device IO.
    virtual bool DeviceEvaluationNotify	(	kTransportMode pMode,				FBEvaluateInfo* pEvaluateInfo			);	//!< Evaluation the device (write to hardware).
	virtual bool DeviceOperation		(	kDeviceOperations pOperation												);	//!< Operate device.

	//--- FBX Load/Save.
	virtual bool FbxStore	( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat	);	//!< Store in FBX file.
	virtual bool FbxRetrieve( FBFbxObject* pFbxObject, kFbxObjectStore pStoreWhat	);	//!< Retrieve from FBX file.

	//--- Initialisation/Shutdown
	bool		Init();			//!< Initialize/create device.
	bool		Start();		//!< Start device (online).
	bool		Reset();		//!< Reset device.
	bool		Stop();			//!< Stop device (offline).
	bool		Done();			//!< Remove device.

	//--- Recording
	void		DeviceRecordFrame( FBDeviceNotifyInfo &pDeviceNotifyInfo );

	//--- Aggregation of hardware parameters
	void		SetCommunicationType( FBCommType pType)		{ mHardware.SetCommunicationType( pType );		}
	int			GetCommunicationType()						{ return mHardware.GetCommunicationType();		}

	void		SetNetworkAddress	(const char* pIPAddress){ mHardware.SetNetworkAddress(pIPAddress);		}
	const char*	GetNetworkAddress	()						{ return mHardware.GetNetworkAddress();			}
	void		SetNetworkPort		(int pPort)				{ mHardware.SetNetworkPort(pPort);				}
	int			GetNetworkPort		()						{ return mHardware.GetNetworkPort();			}
	void		SetSerialPort		(int pPort)				{ mHardware.SetSerialPort(pPort);				}
	int			GetSerialPort		()						{ return mHardware.GetSerialPort();				}
	void		SetSerialBaudRate	(int pRate)				{ mHardware.SetSerialBaudRate(pRate);			}
	int			GetSerialBaudRate	()						{ return mHardware.GetSerialBaudRate();			}
	void		SetStreaming		(bool pStreaming)		{ mHardware.SetStreaming( pStreaming );			}
	bool		GetStreaming		()						{ return mHardware.GetStreaming();				}

	//--- Aggregation of simulator parameters
	void		SetXSpeed			( double pSpeed )		{ mHardware.SetXSpeed( pSpeed );			}
	double		GetXSpeed			()						{ return mHardware.GetXSpeed();				}
	void		SetYSpeed			( double pSpeed )		{ mHardware.SetYSpeed( pSpeed );			}
	double		GetYSpeed			()						{ return mHardware.GetYSpeed();				}

	bool		GetSetCandidate		()						{ return mSetCandidate;							}
	void		SetSetCandidate		( bool pState )			{ mSetCandidate = pState;						}
	double		GetSamplingRate		()						{ return mSamplingRate;							}
	void		SetSamplingRate		( double pRate )		{ mSamplingRate = pRate;						}
	FBDeviceSamplingMode	GetSamplingType()								{ return mSamplingType;			}
	void					SetSamplingType( FBDeviceSamplingMode pType )	{ mSamplingType = pType;		}

	void		SetCandidates		();

public:
	FBModelTemplate*				mTemplateRoot;			//!< Root model template.
	FBModelTemplate*				mTemplateMarker;		//!< Marker model template.
	FBAnimationNode*				mNodeMarker_InT;		//!< Light input animation node (translation).
	FBAnimationNode*				mNodeMarker_InR;		//!< Light input animation node (rotation).

private:
	bool							mSetCandidate;			//!< Are we setting the candidate or writing to the connector?
	double							mSamplingRate;			//!< Device sampling rate.
	FBDeviceSamplingMode			mSamplingType;			//!< Device sampling type.
	ORDevice_Template_Hardware		mHardware;				//!< Hardware member.
	FBPlayerControl					mPlayerControl;			//!< In order to query the play state for recording.
};

#endif /* __ORDEVICE_TEMPLATE_DEVICE_H__ */
