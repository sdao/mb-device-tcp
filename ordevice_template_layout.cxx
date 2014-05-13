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

/**	\file	ordevice_template_layout.cxx
*	Definition of the layout for a simple output device.
*	Definition of the ORDevice_Template_Layout class, representing the
*	layout for an output device.
*/

//--- Class declarations
#include "ordevice_template_device.h"
#include "ordevice_template_layout.h"

//--- Registration define
#define ORDEVICETEMPLATE__LAYOUT	ORDevice_Template_Layout

//--- FiLMBOX implementation and registration
FBDeviceLayoutImplementation(	ORDEVICETEMPLATE__LAYOUT	);
FBRegisterDeviceLayout		(	ORDEVICETEMPLATE__LAYOUT,
								ORDEVICETEMPLATE__CLASSSTR,
								FB_DEFAULT_SDK_ICON			);	// Icon filename (default=Open Reality icon)

/************************************************
 *	FiLMBOX constructor.
 ************************************************/
bool ORDevice_Template_Layout::FBCreate()
{
	// Get a handle on the device.
	mDevice = ((ORDevice_Template *)(FBDevice *)Device);

	// Create/configure UI
	UICreate	();
	UIConfigure	();
	UIReset		();

	// Add device & system callbacks
	mDevice->OnStatusChange.Add	( this,(FBCallback)&ORDevice_Template_Layout::EventDeviceStatusChange		);
	OnIdle.Add					( this,(FBCallback)&ORDevice_Template_Layout::EventUIIdle					);

	return true;
}

/************************************************
 *	FiLMBOX destructor.
 ************************************************/
void ORDevice_Template_Layout::FBDestroy()
{
	// Remove device & system callbacks
	OnIdle.Remove					( this,(FBCallback)&ORDevice_Template_Layout::EventUIIdle				);
	mDevice->OnStatusChange.Remove	( this,(FBCallback)&ORDevice_Template_Layout::EventDeviceStatusChange	);
}


/************************************************
 *	Create the UI.
 ************************************************/
void ORDevice_Template_Layout::UICreate()
{
	int lS, lH;		// space, height
	lS = 4;
	lH = 25;

	// Create regions
	AddRegion	( "TabPanel",	"TabPanel",		0,		kFBAttachLeft,		"",			1.00,
												0,		kFBAttachTop,		"",			1.00,
												0,		kFBAttachRight,		"",			1.00,
												lH,		kFBAttachNone,		NULL,		1.00 );
	AddRegion	( "MainLayout",	"MainLayout",	lS,		kFBAttachLeft,		"TabPanel",	1.00,
												lS,		kFBAttachBottom,	"TabPanel",	1.00,
												-lS,	kFBAttachRight,		"TabPanel",	1.00,
												-lS,	kFBAttachBottom,	"",			1.00 );

	// Assign regions
	SetControl	( "TabPanel",	mTabPanel		);
	SetControl	( "MainLayout",	mLayoutGeneral	);

	// Create sub layouts
	UICreateLayout0();
	UICreateLayout1();
}


/************************************************
 *	Create User layout.
 ************************************************/
void ORDevice_Template_Layout::UICreateLayout0()
{
	int lS, lW, lH;		// space, width, height.
	lS = 4;
	lW = 150;
	lH = 18;

	// Add regions
	mLayoutGeneral.AddRegion ( "LabelSamplingRate",	"LabelSamplingRate",
													lS,		kFBAttachLeft,		"",		1.00,
													lS,		kFBAttachTop,		"",		1.00,
													100,	kFBAttachNone,		NULL,	1.00,
													lH,		kFBAttachNone,		NULL,	1.00 );
	mLayoutGeneral.AddRegion ( "EditNumberSamplingRate",	"EditNumberSamplingRate",
													lS,		kFBAttachRight,		"LabelSamplingRate",	1.0,
													0,		kFBAttachTop,		"LabelSamplingRate",	1.0,
													100,	kFBAttachNone,		NULL,					1.0,
													0,		kFBAttachHeight,	"LabelSamplingRate",	1.0 );
	mLayoutGeneral.AddRegion ( "LabelSamplingType",	"LabelSamplingType",
													0,		kFBAttachLeft,		"LabelSamplingRate",	1.0,
													lS,		kFBAttachBottom,	"LabelSamplingRate",	1.0,
													0,		kFBAttachWidth,		"LabelSamplingRate",	1.0,
													0,		kFBAttachHeight,	"LabelSamplingRate",	1.0 );
	mLayoutGeneral.AddRegion ( "ListSamplingType",	"ListSamplingType",
													lS,		kFBAttachRight,		"LabelSamplingType",	1.0,
													0,		kFBAttachTop,		"LabelSamplingType",	1.0,
													150,	kFBAttachNone,		NULL,					1.0,
													0,		kFBAttachHeight,	"LabelSamplingType",	1.0 );
	mLayoutGeneral.AddRegion ( "ButtonSetCandidate",	"ButtonSetCandidate",
													0,		kFBAttachLeft,		"ListSamplingType",	1.0,
													lS,		kFBAttachBottom,	"ListSamplingType",	1.0,
													100,	kFBAttachNone,		"ListSamplingType",	1.0,
													0,		kFBAttachHeight,	"ListSamplingType",	1.0 );
	mLayoutGeneral.AddRegion ( "ButtonTest",	"ButtonTest",
													0,		kFBAttachLeft,		"ButtonSetCandidate",	1.0,
													lS,		kFBAttachBottom,	"ButtonSetCandidate",	1.0,
													0,		kFBAttachWidth,		"ButtonSetCandidate",	1.0,
													0,		kFBAttachHeight,	"ButtonSetCandidate",	1.0 );

	// Assign regions
	mLayoutGeneral.SetControl("LabelSamplingRate",		mLabelSamplingRate		);
	mLayoutGeneral.SetControl("EditNumberSamplingRate", mEditNumberSamplingRate	);
	mLayoutGeneral.SetControl("LabelSamplingType",		mLabelSamplingType		);
	mLayoutGeneral.SetControl("ListSamplingType",		mListSamplingType		);
	mLayoutGeneral.SetControl("ButtonSetCandidate",		mButtonSetCandidate		);
	mLayoutGeneral.SetControl("ButtonTest",				mButtonTest				);
}


/************************************************
 *	Create the communications layout.
 ************************************************/
void ORDevice_Template_Layout::UICreateLayout1()
{
	int lS		= 4;
	int lSx		= 10;
	int lSy		= 15;
	int lS_y	= -10;
	int lW		= 90;
	int lH		= 18;
	int lHlr	= 55;
	int lHlr2	= 80;
	int lWlr	= 200;
	int lWrb	= 55;
	int lWrb2	= 70;
	int lWrb3	= 75;
	int lSlbx	= 15;
	int lSlby	= 10;
	int lWlb	= 80;

	// Add regions (serial)
	mLayoutCommunication.AddRegion( "LayoutRegionSerial",	"LayoutRegionSerial",
													lSx,	kFBAttachLeft,		"",						1.00,
													lSy,	kFBAttachTop,		"",						1.00,
													lWlr,	kFBAttachNone,		NULL,					1.00,
													lHlr,	kFBAttachNone,		NULL,					1.00 );
	mLayoutCommunication.AddRegion( "ButtonSerialEnable",	"ButtonSerialEnable",
													lS,		kFBAttachLeft,		"LayoutRegionSerial",	1.00,
													lS_y,	kFBAttachTop,		"LayoutRegionSerial",	1.00,
													lWrb,	kFBAttachNone,		NULL,					1.00,
													lH,		kFBAttachNone,		NULL,					1.00 );
	mLayoutCommunication.AddRegion( "LabelSerialPort",		"LabelSerialPort",
													lSlbx,	kFBAttachLeft,		"LayoutRegionSerial",	1.00,
													lSlby,	kFBAttachTop,		"LayoutRegionSerial",	1.00,
													lWlb,	kFBAttachNone,		NULL,					1.00,
													lH,		kFBAttachNone,		NULL,					1.00 );
	mLayoutCommunication.AddRegion( "ListSerialPort",		"ListSerialPort",
													lS,		kFBAttachRight,		"LabelSerialPort",		1.00,
													0,		kFBAttachTop,		"LabelSerialPort",		1.00,
													lW,		kFBAttachNone,		NULL,					1.00,
													lH,		kFBAttachNone,		NULL,					1.00 );
	mLayoutCommunication.AddRegion( "LabelSerialBaudRate",	"LabelSerialBaudRate",
													0,		kFBAttachLeft,		"LabelSerialPort",		1.00,
													lS,		kFBAttachBottom,	"LabelSerialPort",		1.00,
													0,		kFBAttachWidth,		"LabelSerialPort",		1.00,
													0,		kFBAttachHeight,	"LabelSerialPort",		1.00 );
	mLayoutCommunication.AddRegion( "ListSerialBaudRate",	"ListSerialBaudRate",
													lS,		kFBAttachRight,		"LabelSerialBaudRate",	1.00,
													0,		kFBAttachTop,		"LabelSerialBaudRate",	1.00,
													0,		kFBAttachWidth,		"ListSerialPort",		1.00,
													0,		kFBAttachHeight,	"LabelSerialBaudRate",	1.00 );

	// Assign regions (serial)
	mLayoutCommunication.SetControl( "ButtonSerialEnable",	mButtonSerialEnable		);
	mLayoutCommunication.SetControl( "LabelSerialPort",		mLabelSerialPort		);
	mLayoutCommunication.SetControl( "ListSerialPort",		mListSerialPort			);
	mLayoutCommunication.SetControl( "LabelSerialBaudRate",	mLabelSerialBaudRate	);
	mLayoutCommunication.SetControl( "ListSerialBaudRate",	mListSerialBaudRate		);


	// Add regions (network)
	mLayoutCommunication.AddRegion( "LayoutRegionNetwork",	"LayoutRegionNetwork",
													0,		kFBAttachLeft,		"LayoutRegionSerial",	1.00,
													lSy,	kFBAttachBottom,	"LayoutRegionSerial",	1.00,
													0,		kFBAttachWidth,		"LayoutRegionSerial",	1.00,
													lHlr2,	kFBAttachNone,		NULL,					1.00 );
	mLayoutCommunication.AddRegion( "ButtonNetworkEnable",	"ButtonNetworkEnable",
													lS,		kFBAttachLeft,		"LayoutRegionNetwork",	1.00,
													lS_y,	kFBAttachTop,		"LayoutRegionNetwork",	1.00,
													lWrb2,	kFBAttachNone,		NULL,					1.00,
													0,		kFBAttachHeight,	"ButtonSerialEnable",	1.00 );
	mLayoutCommunication.AddRegion( "LabelNetworkAddress",		"LabelNetworkAddress",
													lSlbx,	kFBAttachLeft,		"LayoutRegionNetwork",	1.00,
													lSlby,	kFBAttachTop,		"LayoutRegionNetwork",	1.00,
													lWlb,	kFBAttachNone,		NULL,					1.00,
													lH,		kFBAttachNone,		NULL,					1.00);
	mLayoutCommunication.AddRegion( "EditNetworkAddress",		"EditNetworkAddress",
													lS,		kFBAttachRight,		"LabelNetworkAddress",	1.00,
													0,		kFBAttachTop,		"LabelNetworkAddress",	1.00,
													lW,		kFBAttachNone,		NULL,					1.00,
													lH,		kFBAttachNone,		NULL,					1.00 );
	mLayoutCommunication.AddRegion( "LabelNetworkPort",	"LabelNetworkPort",
													0,		kFBAttachLeft,		"LabelNetworkAddress",	1.00,
													lS,		kFBAttachBottom,	"LabelNetworkAddress",	1.00,
													0,		kFBAttachWidth,		"LabelNetworkAddress",	1.00,
													0,		kFBAttachHeight,	"LabelNetworkAddress",	1.00 );
	mLayoutCommunication.AddRegion( "EditNetworkPort",	"EditNetworkPort",
													lS,		kFBAttachRight,		"LabelNetworkPort",		1.00,
													0,		kFBAttachTop,		"LabelNetworkPort",		1.00,
													lW,		kFBAttachNone,		NULL,					1.00,
													0,		kFBAttachHeight,	"LabelNetworkPort",		1.00 );
	mLayoutCommunication.AddRegion( "LabelNetworkProtocol",	"LabelNetworkProtocol",
													0,		kFBAttachLeft,		"LabelNetworkPort",		1.00,
													lS,		kFBAttachBottom,	"LabelNetworkPort",		1.00,
													0,		kFBAttachWidth,		"LabelNetworkPort",		1.00,
													0,		kFBAttachHeight,	"LabelNetworkPort",		1.00 );
	mLayoutCommunication.AddRegion( "ListNetworkProtocol",	"ListNetworkProtocol",
													lS,		kFBAttachRight,		"LabelNetworkProtocol",	1.00,
													0,		kFBAttachTop,		"LabelNetworkProtocol",	1.00,
													0,		kFBAttachWidth,		"EditNetworkPort",		1.00,
													0,		kFBAttachHeight,	"LabelNetworkProtocol",	1.00 );

	// Assign regions (network)
	mLayoutCommunication.SetControl( "ButtonNetworkEnable",		mButtonNetworkEnable	);
	mLayoutCommunication.SetControl( "LabelNetworkAddress",		mLabelNetworkAddress	);
	mLayoutCommunication.SetControl( "EditNetworkAddress",		mEditNetworkAddress		);
	mLayoutCommunication.SetControl( "LabelNetworkPort",		mLabelNetworkPort		);
	mLayoutCommunication.SetControl( "EditNetworkPort",			mEditNetworkPort		);
	mLayoutCommunication.SetControl( "LabelNetworkProtocol",	mLabelNetworkProtocol	);
	mLayoutCommunication.SetControl( "ListNetworkProtocol",		mListNetworkProtocol	);


	mLayoutCommunication.AddRegion( "LayoutRegionSimulator",		"LayoutRegionSimulator",
													lSx,	kFBAttachRight,		"LayoutRegionSerial",	1.00,
													0,		kFBAttachTop,		"LayoutRegionSerial",	1.00,
													0,		kFBAttachWidth,		"LayoutRegionSerial",	1.00,
													0,		kFBAttachHeight,	"LayoutRegionSerial",	1.00 );
	mLayoutCommunication.AddRegion( "ButtonSimulatorEnable",		"ButtonSimulatorEnable",
													lS,		kFBAttachLeft,		"LayoutRegionSimulator",	1.00,
													lS_y,	kFBAttachTop,		"LayoutRegionSimulator",	1.00,
													lWrb3,	kFBAttachNone,		NULL,					1.00,
													lH,		kFBAttachNone,		NULL,					1.00 );
	mLayoutCommunication.AddRegion( "LabelSimulatorFrequency",			"LabelSimulatorFrequency",
													lSlbx,	kFBAttachLeft,		"LayoutRegionSimulator",	1.00,
													lSlby,	kFBAttachTop,		"LayoutRegionSimulator",	1.00,
													lWlb,	kFBAttachNone,		NULL,					1.00,
													lH,		kFBAttachNone,		NULL,					1.00 );
	mLayoutCommunication.AddRegion( "EditNumberSimulatorFrequency",		"EditNumberSimulatorFrequency",
													lS,		kFBAttachRight,		"LabelSimulatorFrequency",		1.00,
													0,		kFBAttachTop,		"LabelSimulatorFrequency",		1.00,
													lW,		kFBAttachNone,		NULL,					1.00,
													lH,		kFBAttachNone,		NULL,					1.00 );
	mLayoutCommunication.AddRegion( "LabelSimulatorAmplitude",			"LabelSimulatorAmplitude",
													0,		kFBAttachLeft,		"LabelSimulatorFrequency",		1.00,
													lS,		kFBAttachBottom,	"LabelSimulatorFrequency",		1.00,
													0,		kFBAttachWidth,		"LabelSimulatorFrequency",		1.00,
													0,		kFBAttachHeight,	"LabelSimulatorFrequency",		1.00 );
	mLayoutCommunication.AddRegion( "EditNumberSimulatorAmplitude",		"EditNumberSimulatorAmplitude",
													lS,		kFBAttachRight,		"LabelSimulatorAmplitude",	1.00,
													0,		kFBAttachTop,		"LabelSimulatorAmplitude",	1.00,
													0,		kFBAttachWidth,		"EditNumberSimulatorFrequency",		1.00,
													0,		kFBAttachHeight,	"LabelSimulatorAmplitude",	1.00 );

	// Assign regions (Simulator)
	mLayoutCommunication.SetControl( "ButtonSimulatorEnable",		mButtonSimulatorEnable		);
	mLayoutCommunication.SetControl( "LabelSimulatorFrequency",		mLabelSimulatorFrequency		);
	mLayoutCommunication.SetControl( "EditNumberSimulatorFrequency",mEditNumberSimulatorFrequency			);
	mLayoutCommunication.SetControl( "LabelSimulatorAmplitude",		mLabelSimulatorAmplitude	);
	mLayoutCommunication.SetControl( "EditNumberSimulatorAmplitude",mEditNumberSimulatorAmplitude		);
}


/************************************************
 *	Configure the UI.
 ************************************************/
void ORDevice_Template_Layout::UIConfigure()
{
	SetBorder ("MainLayout", kFBStandardBorder, false,true, 1, 0,90,0);

	mTabPanel.Items.SetString("General~Communication");
	mTabPanel.OnChange.Add( this, (FBCallback) &ORDevice_Template_Layout::EventTabPanelChange );

	UIConfigureLayout0();
	UIConfigureLayout1();
}


/************************************************
 *	Configure the user layout.
 ************************************************/
void ORDevice_Template_Layout::UIConfigureLayout0()
{
	mLabelSamplingRate.Caption = "Sampling Rate :";
	mLabelSamplingType.Caption = "Sampling Type :";

	mListSamplingType.Items.Add( "kFBHardwareTimestamp",	kFBHardwareTimestamp	);
	mListSamplingType.Items.Add( "kFBHardwareFrequency",	kFBHardwareFrequency	);
	mListSamplingType.Items.Add( "kFBAutoFrequency",		kFBAutoFrequency		);
	mListSamplingType.Items.Add( "kFBSoftwareTimestamp",	kFBSoftwareTimestamp	);

	mEditNumberSamplingRate.LargeStep = 0.0;
	mEditNumberSamplingRate.SmallStep = 0.0;
	mEditNumberSamplingRate.OnChange.Add( this, (FBCallback)&ORDevice_Template_Layout::EventEditNumberSamplingRateChange );
	mListSamplingType.OnChange.Add( this, (FBCallback)&ORDevice_Template_Layout::EventListSamplingTypeChange );

	mButtonSetCandidate.Style	= kFB2States;
	mButtonSetCandidate.OnClick.Add(this, (FBCallback)&ORDevice_Template_Layout::EventButtonSetCandidateClick );
	mButtonSetCandidate.Caption = "Set Candidate";

	mButtonTest.OnClick.Add( this, (FBCallback)&ORDevice_Template_Layout::EventButtonTestClick );
	mButtonTest.Caption = "Test";
}


/************************************************
 *	Configure the communications layout.
 ************************************************/
void ORDevice_Template_Layout::UIConfigureLayout1()
{
    mLayoutCommunication.SetBorder( "LayoutRegionSerial",	kFBEmbossBorder,false,true,2,1,90.0,0);
    mLayoutCommunication.SetBorder( "LayoutRegionNetwork",	kFBEmbossBorder,false,true,2,1,90.0,0);
    mLayoutCommunication.SetBorder( "LayoutRegionSimulator",kFBEmbossBorder,false,true,2,1,90.0,0);

	mButtonSerialEnable.Caption		= "Serial";
	mButtonSerialEnable.Style		= kFBRadioButton;
	mButtonSerialEnable.State		= 1;
	mButtonSerialEnable.OnClick.Add( this, (FBCallback) &ORDevice_Template_Layout::EventButtonSerialEnableClick );

	mLabelSerialPort.Caption		= "Port :";
	int		i;
	int		lPortNumber;
	char	lBuffer[40];
	for( i=0; i<mSystem.CommPorts.GetCount(); i++ )
	{
		lPortNumber = mSystem.CommPorts[i];
		sprintf(lBuffer, "Comm %d", lPortNumber );
		mListSerialPort.Items.Add( lBuffer, lPortNumber );
	}
	mListSerialPort.OnChange.Add( this, (FBCallback) &ORDevice_Template_Layout::EventListSerialPortChange );

	mLabelSerialBaudRate.Caption	= "Speed :";
	mListSerialBaudRate.Items.Add( "9600",		9600	);
	mListSerialBaudRate.Items.Add( "19200",		19200	);
	mListSerialBaudRate.Items.Add( "38400",		38400	);
	mListSerialBaudRate.Items.Add( "57600",		57600	);
	mListSerialBaudRate.Items.Add( "115200",	115200	);
	mListSerialBaudRate.Items.Add( "230400",	230400	);
	mListSerialBaudRate.OnChange.Add( this, (FBCallback) &ORDevice_Template_Layout::EventListSerialBaudRateChange );

	mButtonNetworkEnable.Caption	= "Network";
	mButtonNetworkEnable.Style		= kFBRadioButton;
	mButtonNetworkEnable.State		= 0;
	mButtonNetworkEnable.OnClick.Add( this, (FBCallback) &ORDevice_Template_Layout::EventButtonNetworkEnableClick );

	mLabelNetworkAddress.Caption	= "Address :";
	mEditNetworkAddress.Text		= "127.0.0.1";
	mEditNetworkAddress.OnChange.Add( this, (FBCallback) &ORDevice_Template_Layout::EventEditNetworkAddressChange );

	mLabelNetworkPort.Caption		= "Port :";
	mEditNetworkPort.Text			= "3001";
	mEditNetworkPort.OnChange.Add( this, (FBCallback) &ORDevice_Template_Layout::EventEditNetworkPortChange );

	mLabelNetworkProtocol.Caption	= "Protocol :";
	mListNetworkProtocol.Items.SetString("TCP~UDP");
	mListNetworkProtocol.OnChange.Add( this, (FBCallback) &ORDevice_Template_Layout::EventListNetworkProtocolChange );

	mButtonSimulatorEnable.Caption		= "Simulator";
	mButtonSimulatorEnable.Style		= kFBRadioButton;
	mButtonSimulatorEnable.State		= 0;
	mButtonSimulatorEnable.OnClick.Add( this, (FBCallback) &ORDevice_Template_Layout::EventButtonSimulatorEnableClick );

	mLabelSimulatorFrequency.Caption		= "Frequency :";
	mEditNumberSimulatorFrequency.Value		= 1.0;
	mEditNumberSimulatorFrequency.Precision	= 3.3;
	mEditNumberSimulatorFrequency.Min		= 0.0;
	mEditNumberSimulatorFrequency.Max		= 10.0;
	mEditNumberSimulatorFrequency.SmallStep = 0.001;
	mEditNumberSimulatorFrequency.LargeStep	= 0.010;
	mEditNumberSimulatorFrequency.OnChange.Add( this, (FBCallback) &ORDevice_Template_Layout::EventEditNumberSimulatorFrequencyChange );

	mLabelSimulatorAmplitude.Caption		= "Amplitude :";
	mEditNumberSimulatorAmplitude.Value		= 1.0;
	mEditNumberSimulatorAmplitude.Precision	= 3.3;
	mEditNumberSimulatorAmplitude.Min		= 0.0;
	mEditNumberSimulatorAmplitude.Max		= 10.0;
	mEditNumberSimulatorAmplitude.SmallStep = 0.001;
	mEditNumberSimulatorAmplitude.LargeStep	= 0.010;
	mEditNumberSimulatorAmplitude.OnChange.Add( this, (FBCallback) &ORDevice_Template_Layout::EventEditNumberSimulatorAmplitudeChange );

	SelectCommunicationType( kFBCommTypeSerial );
}


/************************************************
 *	Refresh the UI.
 ************************************************/
void ORDevice_Template_Layout::UIRefresh()
{
	// Update real-time values from device (i.e. spreadsheet, etc.)
}


/************************************************
 *	Reset the UI values from the device.
 ************************************************/
void ORDevice_Template_Layout::UIReset()
{
	char	lBuffer[40];
	sprintf( lBuffer, "%d", mDevice->GetNetworkPort());

	SelectCommunicationType( mDevice->GetCommunicationType() );

	mListSerialPort.ItemIndex			= mListSerialPort.Items.Find	( mDevice->GetSerialPort()		);
	mListSerialBaudRate.ItemIndex		= mListSerialBaudRate.Items.Find( mDevice->GetSerialBaudRate()	);
	mEditNetworkAddress.Text			= mDevice->GetNetworkAddress();
	mEditNetworkPort.Text				= lBuffer;
	mListNetworkProtocol.ItemIndex		= (mDevice->GetCommunicationType() == kFBCommTypeNetworkUDP) ? 1:0;
	mEditNumberSimulatorFrequency.Value = mDevice->GetFrequency();
	mEditNumberSimulatorAmplitude.Value = mDevice->GetAmplitude();

	mEditNumberSamplingRate.Value		= 1.0/((FBTime)mDevice->SamplingPeriod).GetSecondDouble();

	mListSamplingType.ItemIndex			= mListSamplingType.Items.Find( mDevice->SamplingMode );
	mButtonSetCandidate.State			= mDevice->GetSetCandidate();
}


/************************************************
 *	Tab panel change callback.
 ************************************************/
void ORDevice_Template_Layout::EventTabPanelChange( HISender pSender, HKEvent pEvent )
{
	switch( mTabPanel.ItemIndex )
	{
		case 0:	SetControl("MainLayout", mLayoutGeneral			);	break;
		case 1:	SetControl("MainLayout", mLayoutCommunication	);	break;
	}
}


/************************************************
 *	When the serial port list changes.
 ************************************************/
void ORDevice_Template_Layout::EventListSerialPortChange( HISender pSender, HKEvent pEvent )
{
	if( mDevice->Online )
	{
		mDevice->DeviceSendCommand( FBDevice::kOpStop );
	}

	mDevice->SetSerialPort(mListSerialPort.Items.GetReferenceAt(mListSerialPort.ItemIndex));
	mListSerialPort.ItemIndex = mListSerialPort.Items.Find(mDevice->GetSerialPort());
}


/************************************************
 *	Serial baud rate change callback.
 ************************************************/
void ORDevice_Template_Layout::EventListSerialBaudRateChange( HISender pSender, HKEvent pEvent )
{
	if( mDevice->Online )
	{
		mDevice->DeviceSendCommand( FBDevice::kOpStop );
	}

	mDevice->SetSerialBaudRate( mListSerialBaudRate.Items.GetReferenceAt(mListSerialBaudRate.ItemIndex) );
	mListSerialBaudRate.ItemIndex = mListSerialBaudRate.Items.Find( mDevice->GetSerialBaudRate() );
}


/************************************************
 *	Network port change callback.
 ************************************************/
void ORDevice_Template_Layout::EventEditNetworkPortChange( HISender pSender, HKEvent pEvent )
{
	int		lPort;
	char	lBuffer[40];

	sscanf(mEditNetworkPort.Text.AsString(), "%d", &lPort);
	mDevice->SetNetworkPort(lPort);
	lPort = mDevice->GetNetworkPort();
	sprintf(lBuffer, "%d", lPort );
	mEditNetworkPort.Text = lBuffer;
}


/************************************************
 *	Network address address callback.
 ************************************************/
void ORDevice_Template_Layout::EventEditNetworkAddressChange( HISender pSender, HKEvent pEvent )
{
	mDevice->SetNetworkAddress(mEditNetworkAddress.Text.AsString());
	mEditNetworkAddress.Text = mDevice->GetNetworkAddress();
}


/************************************************
 *	Network protocol address callback.
 ************************************************/
void ORDevice_Template_Layout::EventListNetworkProtocolChange( HISender pSender, HKEvent pEvent )
{
	mDevice->SetCommunicationType( (mListNetworkProtocol.ItemIndex == 0) ? kFBCommTypeNetworkTCP:kFBCommTypeNetworkUDP );
	mListNetworkProtocol.ItemIndex = (mDevice->GetCommunicationType() == kFBCommTypeNetworkTCP) ? 0:1;
}


/************************************************
 *	Test button callback.
 ************************************************/
void ORDevice_Template_Layout::EventButtonTestClick( HISender pSender, HKEvent pEvent )
{
}


/************************************************
 *	Serial communcations enable button callback.
 ************************************************/
void ORDevice_Template_Layout::EventButtonSerialEnableClick( HISender pSender, HKEvent pEvent )
{
	if( mDevice->Online )
	{
		mDevice->DeviceSendCommand( FBDevice::kOpStop );
	}
	mDevice->SetCommunicationType( kFBCommTypeSerial );
	SelectCommunicationType( mDevice->GetCommunicationType() );
}


/************************************************
 *	Network communications enable button callback.
 ************************************************/
void ORDevice_Template_Layout::EventButtonNetworkEnableClick( HISender pSender, HKEvent pEvent )
{
	if( mDevice->Online )
	{
		mDevice->DeviceSendCommand( FBDevice::kOpStop );
	}
	mDevice->SetCommunicationType( (mListNetworkProtocol.ItemIndex==0) ? kFBCommTypeNetworkTCP:kFBCommTypeNetworkUDP );
	SelectCommunicationType( mDevice->GetCommunicationType() );
}


/************************************************
 *	Simulator enable button callback.
 ************************************************/
void ORDevice_Template_Layout::EventButtonSimulatorEnableClick( HISender pSender, HKEvent pEvent )
{
	if( mDevice->Online )
	{
		mDevice->DeviceSendCommand( FBDevice::kOpStop );
	}
	mDevice->SetCommunicationType( kFBCommTypeSimulator );
	SelectCommunicationType( mDevice->GetCommunicationType() );
}


/************************************************
 *	Simulator frequency change callback.
 ************************************************/
void ORDevice_Template_Layout::EventEditNumberSimulatorFrequencyChange( HISender pSender, HKEvent pEvent )
{
	mDevice->SetFrequency( mEditNumberSimulatorFrequency.Value );
	mEditNumberSimulatorFrequency.Value = mDevice->GetFrequency();
}


/************************************************
 *	Simulator amplitude change callback.
 ************************************************/
void ORDevice_Template_Layout::EventEditNumberSimulatorAmplitudeChange( HISender pSender, HKEvent pEvent )
{
	mDevice->SetAmplitude( mEditNumberSimulatorAmplitude.Value );
	mEditNumberSimulatorAmplitude.Value = mDevice->GetAmplitude();
}


/************************************************
 *	Device status change callback.
 ************************************************/
void ORDevice_Template_Layout::EventDeviceStatusChange( HISender pSender, HKEvent pEvent )
{
	UIReset();
}


/************************************************
 *	UI Idle callback.
 ************************************************/
void ORDevice_Template_Layout::EventUIIdle( HISender pSender, HKEvent pEvent )
{
	if( mDevice->Online && mDevice->Live )
	{
		if( mDevice->GetSetCandidate() )
		{
			mDevice->SetCandidates();
		}
		UIRefresh();
	}
}


/************************************************
 *	Select the communication type.
 ************************************************/
void ORDevice_Template_Layout::SelectCommunicationType( int pType )
{
	bool	lSerialTrue		= false;
	bool	lNetworkTrue	= false;
	bool	lSimulatorTrue	= false;

	switch( pType )
	{
		case kFBCommTypeSerial:
		{
			lSerialTrue	= true;
		}
		break;
		case kFBCommTypeNetworkTCP:
		case kFBCommTypeNetworkUDP:
		{
			lNetworkTrue	= true;
		}
		break;
		case kFBCommTypeSimulator:
		{
			lSimulatorTrue	= true;
		}
		break;
	}

	mButtonSerialEnable.State				= lSerialTrue ? 1 : 0;
	mLabelSerialPort.Enabled				= lSerialTrue;
	mListSerialPort.Enabled					= lSerialTrue;
	mLabelSerialBaudRate.Enabled			= lSerialTrue;
	mListSerialBaudRate.Enabled				= lSerialTrue;

	mButtonNetworkEnable.State				= lNetworkTrue ? 1 : 0;
	mLabelNetworkAddress.Enabled			= lNetworkTrue;
	mEditNetworkAddress.Enabled				= lNetworkTrue;
	mLabelNetworkPort.Enabled				= lNetworkTrue;
	mEditNetworkPort.Enabled				= lNetworkTrue;
	mLabelNetworkProtocol.Enabled			= false;
	mListNetworkProtocol.Enabled			= false;

	mButtonSimulatorEnable.State			= lSimulatorTrue ? 1 : 0;
	mLabelSimulatorFrequency.Enabled		= lSimulatorTrue;
	mEditNumberSimulatorFrequency.Enabled	= lSimulatorTrue;
	mLabelSimulatorAmplitude.Enabled		= lSimulatorTrue;
	mEditNumberSimulatorAmplitude.Enabled	= lSimulatorTrue;
}

void ORDevice_Template_Layout::EventEditNumberSamplingRateChange( HISender pSender, HKEvent pEvent )
{
	bool lOnline = mDevice->Online;
	double lVal = mEditNumberSamplingRate.Value;

	if( lVal > 0.0 )
	{
		if( lOnline )
		{
			mDevice->DeviceSendCommand( FBDevice::kOpStop );
		}

		FBTime lTime;
		lTime.SetSecondDouble( 1.0 / lVal );
		mDevice->SamplingPeriod = lTime;

		if( lOnline )
		{
			mDevice->DeviceSendCommand( FBDevice::kOpStart );
		}

		UIReset();
	}
}

void ORDevice_Template_Layout::EventListSamplingTypeChange(  HISender pSender, HKEvent pEvent )
{
	mDevice->SamplingMode = (FBDeviceSamplingMode)mListSamplingType.Items.GetReferenceAt( mListSamplingType.ItemIndex );
	UIReset();
}

void ORDevice_Template_Layout::EventButtonSetCandidateClick( HISender pSender, HKEvent pEvent )
{
	mDevice->SetSetCandidate( mButtonSetCandidate.State != 0 );
	UIReset();
}
