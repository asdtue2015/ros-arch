//******************************************************************
// 
//  Generated by IDL to C++ Translator
//  
//  File name: AddTwoInts_Request_.h
//  Source: /home/rameez/ADAS_overlay_ws/build/example_interfaces/rosidl_generator_dds_idl/example_interfaces/srv/dds_opensplice/AddTwoInts_Request_.idl
//  Generated: Tue Feb  2 10:39:17 2016
//  OpenSplice V6.4.140407OSS
//  
//******************************************************************
#ifndef _ADDTWOINTS_REQUEST__H_
#define _ADDTWOINTS_REQUEST__H_

#include "sacpp_mapping.h"
#include "sacpp_DDS_DCPS.h"


namespace example_interfaces
{
   namespace srv
   {
      namespace dds_
      {
         struct AddTwoInts_Request_;

         struct AddTwoInts_Request_
         {
               DDS::LongLong a_;
               DDS::LongLong b_;
         };

         typedef DDS_DCPSStruct_var < AddTwoInts_Request_> AddTwoInts_Request__var;
         typedef AddTwoInts_Request_&AddTwoInts_Request__out;
      }
   }
}




#endif 