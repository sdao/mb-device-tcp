#ifndef __ORDEVICE_TEMPLATE_LAYOUT_H__
#define __ORDEVICE_TEMPLATE_LAYOUT_H__
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

/**	\file	fbsimpleoutput_layout.h
*	Declaration of the layout class for a simple output device.
*	Declaration of the FBSimpleOutputDeviceLayout class, the layout for
*	a simple output device.
*/

//--- Class declaration
#include "ordevice_template_device.h"

//! Simple device layout.
class ORDevice_Template_Layout : public FBDeviceLayout
{
	//--- FiLMBOX declaration.
	FBDeviceLayoutDeclare( ORDevice_Template_Layout, FBDeviceLayout );
public:
	//--- FiLMBOX Creation/Destruction.
	virtual bool FBCreate();		//!< FiLMBOX Constructor.
	virtual void FBDestroy();		//!< FiLMBOX Destructor.

	// UI Management
	void	UICreate				();
	void		UICreateLayout0		();
	void		UICreateLayout1		();
	void	UIConfigure				();
	void		UIConfigureLayout0	();
	void		UIConfigureLayout1	();
	void	UIRefresh				();		// Idle refresh
	void	UIReset					();		// Reset from device values

	// Main Layout: Events
	void	EventDeviceStatusChange					( HISender pSender, HKEvent pEvent );
	void	EventUIIdle								( HISender pSender, HKEvent pEvent );
	void	EventTabPanelChange						( HISender pSender, HKEvent pEvent );

	// Layout 0: Events
	void	EventEditNumberSamplingRateChange		( HISender pSender, HKEvent pEvent );
	void	EventListSamplingTypeChange				( HISender pSender, HKEvent pEvent );
	void	EventButtonSetCandidateClick			( HISender pSender, HKEvent pEvent );
	void	EventButtonTestClick					( HISender pSender, HKEvent pEvent );

	// Layout 1: Events
	void	EventButtonSerialEnableClick			( HISender pSender, HKEvent pEvent );
	void	EventListSerialPortChange				( HISender pSender, HKEvent pEvent );
	void	EventListSerialBaudRateChange			( HISender pSender, HKEvent pEvent );
	void	EventButtonNetworkEnableClick			( HISender pSender, HKEvent pEvent );
	void	EventEditNetworkAddressChange			( HISender pSender, HKEvent pEvent );
	void	EventEditNetworkPortChange				( HISender pSender, HKEvent pEvent );
	void	EventListNetworkProtocolChange			( HISender pSender, HKEvent pEvent );
	void	EventButtonSimulatorEnableClick			( HISender pSender, HKEvent pEvent );
	void	EventEditNumberSimulatorFrequencyChange	( HISender pSender, HKEvent pEvent );
	void	EventEditNumberSimulatorAmplitudeChange	( HISender pSender, HKEvent pEvent );

	// Layout configuration functions
	void	SelectCommunicationType( int pType );

private:
	FBTabPanel			mTabPanel;

	FBLayout			mLayoutGeneral;
		FBLabel				mLabelSamplingRate;
		FBEditNumber		mEditNumberSamplingRate;
		FBLabel				mLabelSamplingType;
		FBList				mListSamplingType;
		FBButton			mButtonSetCandidate;
		FBButton			mButtonTest;

	FBLayout			mLayoutCommunication;
		FBButton			mButtonSerialEnable;
			FBLabel				mLabelSerialPort;
			FBList				mListSerialPort;
			FBLabel				mLabelSerialBaudRate;
			FBList				mListSerialBaudRate;
		FBButton			mButtonNetworkEnable;
			FBLabel				mLabelNetworkAddress;
			FBEdit				mEditNetworkAddress;
			FBLabel				mLabelNetworkPort;
			FBEdit				mEditNetworkPort;
			FBLabel				mLabelNetworkProtocol;
			FBList				mListNetworkProtocol;
		FBButton			mButtonSimulatorEnable;
			FBLabel				mLabelSimulatorFrequency;
			FBEditNumber		mEditNumberSimulatorFrequency;
			FBLabel				mLabelSimulatorAmplitude;
			FBEditNumber		mEditNumberSimulatorAmplitude;

private:
	FBSystem			mSystem;		//!< System interface.
	ORDevice_Template*	mDevice;		//!< Handle onto device.
};

#endif /* __ORDEVICE_TEMPLATE_HARDWARE_H__ */
