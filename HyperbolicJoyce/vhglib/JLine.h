

#pragma once

#include "..\Common\DeviceResources.h"
#include "DefTypes.h"





namespace DJJ   //  David Joyce's Java classes and methods; 
{
    class JLine  //  Class to realize hyperbolic geodesics, be they circular or be they straight.
    {
    public:

        JLine();  // default ctor doesn't do much...

        JLine(DJJ::HvyPlex const& pA, DJJ::HvyPlex const& pB);  // useful ctor;

        JLine(JLine const&  p_source);  //  copy ctor;


        DJJ::HvyPlex        ReflectPointInLine(DJJ::HvyPlex const & jpt_preimage);


    public:

        DJJ::HvyPlex                endptA_logical;  //  "this" is the line segment with end points endptA and endptB;
        DJJ::HvyPlex                endptB_logical;  //  "this" is the line segment with end points endptA and endptB;

        bool                        isStraight;  // is this geodesic a diameter of the Poincare disk???

        // if it's a circle, then a center C and radius r are needed:

        DJJ::HvyPlex                circleCenter;  // typical hyperbolic geodesic is circular arc;
        double                      circleRadius;  // typical hyperbolic geodesic is circular arc;

        // if it's is a straight line, then a point P and a direction D are needed:

        DJJ::HvyPlex                 P_logical;
        DJJ::HvyPlex                 D_logical;
    };

}
//  Closes namespace DJJ; 




