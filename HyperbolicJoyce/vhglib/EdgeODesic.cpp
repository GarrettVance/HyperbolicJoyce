//                      
//                      
//      ghv : refer to anything written by Caroline Series
//      ==================================================
//  
//   
//                      


#include "pch.h"

#include "EdgeODesic.h"

using namespace DJJ;

using namespace DirectX;
using namespace Windows::Foundation;


           
DJJ::EdgeODesic::EdgeODesic() : eo_sweepDirection(D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE) 
{
    
} 







DJJ::EdgeODesic::EdgeODesic(EdgeODesic const& p_source)
{
    //  copy ctor

    this->eo_sweepDirection = p_source.eo_sweepDirection;

    this->eo_jLine = { p_source.eo_jLine };
}




//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



