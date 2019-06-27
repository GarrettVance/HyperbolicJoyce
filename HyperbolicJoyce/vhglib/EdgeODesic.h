


#pragma once

#include "..\Common\DeviceResources.h"

#include "DefTypes.h"
#include "JLine.h"




namespace DJJ   //  David Joyce's Java classes and methods; 
{
    class EdgeODesic  
    {
    public:

        EdgeODesic();


        EdgeODesic(EdgeODesic const&  p_source);  //  copy ctor;


        // EdgeODesic::~EdgeODesic();  


    public: 

        
        D2D1_SWEEP_DIRECTION            eo_sweepDirection;

        DJJ::JLine                      eo_jLine;
    };

}
//  Closes namespace DJJ; 




