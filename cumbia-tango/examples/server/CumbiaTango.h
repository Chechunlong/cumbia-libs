/*----- PROTECTED REGION ID(CumbiaTango.h) ENABLED START -----*/
//=============================================================================
//
// file :        CumbiaTango.h
//
// description : Include file for the CumbiaTango class
//
// project :     
//
// This file is part of Tango device class.
// 
// Tango is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// Tango is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with Tango.  If not, see <http://www.gnu.org/licenses/>.
// 
// $Author: giacomo $
//
// $Revision: 1.1 $
// $Date: 2017-03-14 13:11:06 $
//
// $HeadURL:  $
//
//=============================================================================
//                This file is generated by POGO
//        (Program Obviously used to Generate tango Object)
//=============================================================================


#ifndef CumbiaTango_H
#define CumbiaTango_H

#include <tango.h>
#include <CumbiaAbstractDevice.h>


/*----- PROTECTED REGION END -----*/	//	CumbiaTango.h

/**
 *  CumbiaTango class description:
 *    
 */

namespace CumbiaTango_ns
{
/*----- PROTECTED REGION ID(CumbiaTango::Additional Class Declarations) ENABLED START -----*/

//	Additional Class Declarations

/*----- PROTECTED REGION END -----*/	//	CumbiaTango::Additional Class Declarations

class CumbiaTango : public CumbiaAbstractDevice_ns::CumbiaAbstractDevice
{

/*----- PROTECTED REGION ID(CumbiaTango::Data Members) ENABLED START -----*/

//	Add your own data members

/*----- PROTECTED REGION END -----*/	//	CumbiaTango::Data Members


//	Attribute data members
public:
	Tango::DevDouble	*attr_double_scalar1_read;
	Tango::DevDouble	*attr_double_scalar2_read;

//	Constructors and destructors
public:
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	CumbiaTango(Tango::DeviceClass *cl,string &s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device Name
	 */
	CumbiaTango(Tango::DeviceClass *cl,const char *s);
	/**
	 * Constructs a newly device object.
	 *
	 *	@param cl	Class.
	 *	@param s 	Device name
	 *	@param d	Device description.
	 */
	CumbiaTango(Tango::DeviceClass *cl,const char *s,const char *d);
	/**
	 * The device object destructor.
	 */
	~CumbiaTango() {delete_device();};


//	Miscellaneous methods
public:
	/*
	 *	will be called at device destruction or at init command.
	 */
	void delete_device();
	/*
	 *	Initialize the device
	 */
	virtual void init_device();
	/*
	 *	Always executed method before execution command method.
	 */
	virtual void always_executed_hook();


//	Attribute methods
public:
	//--------------------------------------------------------
	/*
	 *	Method      : CumbiaTango::read_attr_hardware()
	 *	Description : Hardware acquisition for attributes.
	 */
	//--------------------------------------------------------
	virtual void read_attr_hardware(vector<long> &attr_list);
	//--------------------------------------------------------
	/*
	 *	Method      : CumbiaTango::write_attr_hardware()
	 *	Description : Hardware writing for attributes.
	 */
	//--------------------------------------------------------
	virtual void write_attr_hardware(vector<long> &attr_list);

/**
 *	Attribute double_scalar1 related methods
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_double_scalar1(Tango::Attribute &attr);
	virtual void write_double_scalar1(Tango::WAttribute &attr);
	virtual bool is_double_scalar1_allowed(Tango::AttReqType type);
/**
 *	Attribute double_scalar2 related methods
 *	Description: 
 *
 *	Data type:	Tango::DevDouble
 *	Attr type:	Scalar
 */
	virtual void read_double_scalar2(Tango::Attribute &attr);
	virtual void write_double_scalar2(Tango::WAttribute &attr);
	virtual bool is_double_scalar2_allowed(Tango::AttReqType type);


	//--------------------------------------------------------
	/**
	 *	Method      : CumbiaTango::add_dynamic_attributes()
	 *	Description : Add dynamic attributes if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_attributes();




//	Command related methods
public:


	//--------------------------------------------------------
	/**
	 *	Method      : CumbiaTango::add_dynamic_commands()
	 *	Description : Add dynamic commands if any.
	 */
	//--------------------------------------------------------
	void add_dynamic_commands();

/*----- PROTECTED REGION ID(CumbiaTango::Additional Method prototypes) ENABLED START -----*/

//	Additional Method prototypes

    virtual void onProgress(int step, int total, const CuData& data);

    virtual void onResult(const CuData& data);

    virtual CuData getToken() const;

/*----- PROTECTED REGION END -----*/	//	CumbiaTango::Additional Method prototypes
};

/*----- PROTECTED REGION ID(CumbiaTango::Additional Classes Definitions) ENABLED START -----*/

//	Additional Classes Definitions

/*----- PROTECTED REGION END -----*/	//	CumbiaTango::Additional Classes Definitions

}	//	End of namespace

#endif   //	CumbiaTango_H