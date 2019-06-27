//                      
//                      
//      ghv : refer to Joyce Java file Line.java
//      ========================================
//  
//   
//                      



#include "pch.h"

#include "JLine.h"




using namespace DJJ;

using namespace DirectX;
using namespace Windows::Foundation;





DJJ::JLine::JLine()
{
    this->endptA_logical = 0;
    this->endptB_logical = 0;

    this->isStraight = true;

    this->circleCenter = 0;
    this->circleRadius = 1;

    this->P_logical = 0;
    this->D_logical = 0;
}








//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


DJJ::JLine::JLine(              //  class ctor for JLine class;
    DJJ::HvyPlex const& pA, 
    DJJ::HvyPlex const& pB
)  :  
    endptA_logical(pA), 
    endptB_logical(pB)
{

    // first determine if its a line or a circle
    
    double den = endptA_logical.real() * endptB_logical.imag() - endptB_logical.real() * endptA_logical.imag();



    isStraight = (std::abs(den) < 1.0e-10);  //  hazard : 1e-14 is on the edge of viability;
   


    if (isStraight) 
    {
        this->P_logical = endptA_logical;   //  a point on the line;
      

        // find a unit vector D in the direction of the line
      
        den = sqrt(
            (endptA_logical.real() - endptB_logical.real()) * (endptA_logical.real() - endptB_logical.real()) + 
            (endptA_logical.imag() - endptB_logical.imag()) * (endptA_logical.imag() - endptB_logical.imag())
        );


        D_logical = DJJ::HvyPlex((endptB_logical.real() - endptA_logical.real()) / den, (endptB_logical.imag() - endptA_logical.imag()) / den);
    }
    else 
    {
        // it's a circle
        // find the center of the circle thru these points
        
        double s1 = (1.0 + endptA_logical.real() * endptA_logical.real() + endptA_logical.imag() * endptA_logical.imag()) / 2.0;
      
        double s2 = (1.0 + endptB_logical.real() * endptB_logical.real() + endptB_logical.imag() * endptB_logical.imag()) / 2.0;


        circleCenter = DJJ::HvyPlex(
            (s1 * endptB_logical.imag() - s2 * endptA_logical.imag()) / den, 
            (endptA_logical.real() * s2 - endptB_logical.real() * s1) / den
        );
      

        //   the formula below: 
        //   1 + circleRadius * circleRadius = normSquared( circleCenter );
        //  

        circleRadius = sqrt(circleCenter.real() * circleCenter.real() + circleCenter.imag() * circleCenter.imag() - 1.0);
    }
}
//  Closes class ctor JLine::JLine();














DJJ::JLine::JLine(JLine const&  p_source)  //  copy ctor;
{
    //  copy ctor

    this->endptA_logical = p_source.endptA_logical;
    this->endptB_logical = p_source.endptB_logical;

    this->isStraight = p_source.isStraight;

    this->circleCenter = p_source.circleCenter;
    this->circleRadius = p_source.circleRadius;

    this->P_logical = p_source.P_logical;
    this->D_logical = p_source.D_logical;
}






DJJ::HvyPlex      DJJ::JLine::ReflectPointInLine(DJJ::HvyPlex const & jpt_preimage)
{
    //  Reflect the point jpt_preimage in this line (in this mirror) 
    //  to get the_reflection(reflected_x, reflected_y). 


    double reflected_x = 0.00;
    double reflected_y = 0.00;

    if (this->isStraight)
    {
        double factor = 2.0 * (
            (jpt_preimage.real() - P_logical.real()) * D_logical.real() + 
            (jpt_preimage.imag() - P_logical.imag()) * D_logical.imag()
        );

        reflected_x = 2.0 * P_logical.real() + factor * D_logical.real() - jpt_preimage.real();

        reflected_y = 2.0 * P_logical.imag() + factor * D_logical.imag() - jpt_preimage.imag();
    }
    else
    {  
        // it's a circle

        double factor = 
            circleRadius * circleRadius / 
            (
                (jpt_preimage.real() - circleCenter.real()) * (jpt_preimage.real() - circleCenter.real()) + 
                (jpt_preimage.imag() - circleCenter.imag()) * (jpt_preimage.imag() - circleCenter.imag())
            );


        reflected_x = circleCenter.real() + factor * (jpt_preimage.real() - circleCenter.real());

        reflected_y = circleCenter.imag() + factor * (jpt_preimage.imag() - circleCenter.imag());
    }

    HvyPlex the_reflection{ reflected_x, reflected_y };

    return the_reflection;
}
//  Closes method ReflectPointInLine();





//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

