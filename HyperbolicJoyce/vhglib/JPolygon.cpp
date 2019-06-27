//                      
//                      
//      ghv : refer to Joyce Java file Polygon.java
//      ===========================================
//  
//   
//                      



#include "pch.h"

#include "JPolygon.h"
#include "JLine.h"
#include "JPoincareDisk.h" 




using namespace DJJ;
using namespace DirectX;
using namespace Windows::Foundation;









double const HvyDXBase::konst_pi = 3.1415926535897932384626433;




           


DJJ::JPolygon::JPolygon(uint32_t p_Taxonomy) : jpolyTaxonomy(p_Taxonomy)
{
    DJJ::HvyPlex nullVertex{ 0.00, 0.00 };
    jpolyVertices.assign(p_Taxonomy, nullVertex); 



    DJJ::EdgeODesic nullEdge{}; 
    jpolyEdges.assign(p_Taxonomy, nullEdge);
}












DJJ::HvyPlex         DJJ::ReflectPointInPoint(DJJ::HvyPlex preimage,  DJJ::HvyPlex mirror)
{
    //    ghv : refer to Joyce Java file Point.java
    //  ================================================
    //  
    //      Reflect the point A through  "this"  point B 
    //              to get the returned point C.
    //       
    // The rule for computing A thru B (as complex numbers) is:
    //      
    //      
    //                 B - t A            where t = (1+BB')/2, and
    //      A |> B = -----------               B' is the complex
    //                 t -  A B'                conjugate of B
    //                      
    //                      


    double t = (1.0 + std::norm(mirror)) / 2.00;

    // compute the numerator as  mirror - t * preimage

    
    std::complex<double> numeratr = mirror - t * preimage;


    // compute the denominator as  t - preimage * mirror' 


    std::complex<double> denomnatr = std::complex<double>{ t, 0.0 } - (preimage * std::conj(mirror));


    std::complex<double> quot = numeratr / denomnatr;


    return quot;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++







double DJJ::HyperbolicDistanceFromEuclideanDistanceFromOrigin(double p_euclidean_distance_to_origin)
{
    //                   
    //     When the disk used is the open unit disk and one of the points is the origin 
    //     and the Euclidean distance between the points is r then the hyperbolic distance is: 
    //          

    double hyperbolic_distance = std::log(
        (1.000 + p_euclidean_distance_to_origin) / (1.000 - p_euclidean_distance_to_origin) 
    );   //   NB:  std::log is the Naperian logarithm; 

    return hyperbolic_distance;
}










double                  gvTestMyInverse(double gawGai)
{

    double dawDek = gawGai + sqrt(gawGai * gawGai - 1.00);

    double circumradius_euclinos = (dawDek - 1.00) / (dawDek + 1.00); 

    return circumradius_euclinos;
}








double                  gvComputeApothem(double p_circumradiusEuc, double p_angleA)
{
    //=============================================================================
    //  ghv : 20181114 : 
    //  
    //  The Euclidean length aux_s below is the Euclidean distance 
    //  from the complex origin (aka point A) to the circlecenter 
    //  of the hyperbolic geodesic through the polygon vertex B 
    //  and the mirror image of vertex B (when vertex B is reflected 
    //  across the real axis). 
    //=============================================================================

    double aux_s = (p_circumradiusEuc * p_circumradiusEuc + 1.0) / (2.0 * p_circumradiusEuc * cos(p_angleA));

    double apothem_euclidean = aux_s - sqrt(aux_s * aux_s - 1.0);

    return apothem_euclidean;
}










        
DJJ::JPolygon         DJJ::JPolygon::constructCenterPolygon(int p_VertexCount, int p_VertexValence, bool p_quasiregular)
{
    //      
    //  Hyperbolic Tiling (Poincare Disk)
    //  =================================
    //  ...of regular hyperbolic polygons, each such hyperbolic polygon being defined by the pair of values 
    //  held in the Schlafli symbol...
    //      
    //          As the geometry has been assumed to be hyperbolic for this tiling, 
    //   there is only one size of n-gon... etc...
    //      
    //          
    //  Consider the following hyperbolic right triangle: 
    //
    //  vertex A is at the center of the polygon,
    //  vertex B is concurrent with one vertex of the polygon
    //  and vertex C  is at the intersection of angle bisector  (polygon rotated so that 
    //  vertex C is on the real axis of the complex plane) intersection of real axis 
    //  and a polygon edge as shown in the diagram: 
    //   
    //
    //      AAA: another consequence of hyperbolicity is that AAA (angle-angle-angle) 
    //  is a congruence condition for this triangle. 
    //    
    //    
    //  ...etc...having p_VertexCount vertices, 
    //  and arranged so that p_VertexValence of these polygons meet at each vertex. 
    //      
    //  




    double angleA = HvyDXBase::konst_pi / (double)p_VertexCount;

    double angleB = HvyDXBase::konst_pi / (double)p_VertexValence;

    double angleC = HvyDXBase::konst_pi / 2.0;



    //  
    //  For a regular tiling, we need to compute the distance s from A to B.
    //  
    //   (the circumradius of the polygon)
    // 

    double sinA = sin(angleA);

    double sinB = sin(angleB);


    //  For the circumradius_euclidean, distance is Euclidean distance: 

    double circumradius_euclidean =  sin(angleC - angleB - angleA) / 
        sqrt(1.0 - sinB * sinB - sinA * sinA);  // Formula for circumradius in Euclinos; 

    //    
    //  Now convert the circumradius from Euclinos to Hyperions: 
    //  For the circumradius_hyperbolic, distance is hyperbolic distance. 
    //      

    double circumradius_hyperbolic = DJJ::HyperbolicDistanceFromEuclideanDistanceFromOrigin(circumradius_euclidean);


    //          
    //  Demonstrate application of a trigonometry relation from wikipedia: 
    //      
    //      ----------------   ------------------------------------------------------- 
    //      cosh(hypotenuse) = cos(angleA) * cos(angleB) / (sin(angleA) * sin(angleB)); 
    //      ----------------   ------------------------------------------------------- 
    //
    //  This formula FAILS when "hypotenuse" is expressed in Euclinos; 
    //  The value of "hypotenuse" MUST BE EXPRESSED IN HYPERIONS. 
    //           



    bool is73tiling = false; 
    double eval_lhs_cosh = 0.00;
    double eval_rhs_quotient = 0.00;
    double error_absolute = 0.00; 
    double error_relative = 0.00;

    double eval_myEuclinos = 0.00; 

    if ((p_VertexCount == 7) && (p_VertexValence == 3))
    {
        is73tiling = true;

        //              
        //   for the {7,3} tiling, the heptagon will have 
        //   circumradius_euclidean = 0.300  742  618  746 Euclinos. 
        //              

        eval_lhs_cosh = std::cosh(circumradius_hyperbolic);
        eval_rhs_quotient = cos(angleA) * cos(angleB) / (sin(angleA) * sin(angleB)); 
        error_absolute = eval_lhs_cosh - eval_rhs_quotient;
        error_relative = error_absolute / eval_rhs_quotient;

        eval_myEuclinos = gvTestMyInverse(eval_rhs_quotient); 
    }


    double apothem_euclidean = 0.00;

    double s_Euclino = circumradius_euclidean;

    //  But for a quasiregular tiling, we need the distance s_Euclino from A to C.  

    if (p_quasiregular)
    {
        //=============================================================================
        //  ghv : 20181114 : 
        //  
        //  The Euclidean length aux_s below is the Euclidean distance 
        //  from the complex origin (aka point A) to the circlecenter 
        //  of the hyperbolic geodesic through the polygon vertex B 
        //  and the mirror image of vertex B (when vertex B is reflected 
        //  across the real axis). 
        //=============================================================================

        double aux_s = (circumradius_euclidean * circumradius_euclidean + 1.0) / (2.0 * circumradius_euclidean * cos(angleA));

        apothem_euclidean = aux_s - sqrt(aux_s * aux_s - 1.0);


        //=============================================================================
        //  ghv : Demonstrate that aux_s defined above really is the 
        //  real component of the hyperbolic geodesic's circle center: 
        //=============================================================================

        //  HvyPlex vertxUp{ circumradius_euclidean * cos(angleA), circumradius_euclidean * sin(angleA) };
        //  HvyPlex vertxDn{ circumradius_euclidean * cos(angleA), -1.00 * circumradius_euclidean * sin(angleA) };
        //   HvyDXBase::HvyGeodesic geo = HvyDXBase::hyper_geodesic_radius_center(vertxUp, vertxDn);
        //   double other_error_absolute = std::abs(aux_s - geo.e_center.real());


        s_Euclino = apothem_euclidean;
    }


    // Now determine the coordinates of the n vertices of the n-gon.
    // They're all at distance s from the center of the Poincare disk. 


    DJJ::JPolygon tmpPgon = DJJ::JPolygon(p_VertexCount);  // ctor initialization;


    for (int i = 0; i < p_VertexCount; ++i)
    {
        double tmp_x = s_Euclino * cos((3 + 2 * i) * angleA);

        double tmp_y = s_Euclino * sin((3 + 2 * i) * angleA);

        DJJ::HvyPlex tmp_zz{ tmp_x, tmp_y };

        tmpPgon.jpolyVertices.at(i) = tmp_zz;
    }


    double gv_apothemEuc = gvComputeApothem(circumradius_euclidean, angleA); // for debug only;


    return tmpPgon;
}









void        DJJ::JPolygon::constructEdges()
{

    for (uint32_t idx_one = 0; idx_one < jpolyTaxonomy; idx_one++)
    {
        DJJ::HvyPlex    vert_one = jpolyVertices.at(idx_one);


        uint32_t idx_two = (1 + idx_one) % jpolyTaxonomy;  // wraparound;
        DJJ::HvyPlex    vert_two = jpolyVertices.at(idx_two);


        //   
        //   Vertices vert_one and vert_two are a pair of 
        //   adjacent vertices of this regular polygon (aka n-gon). 
        // 
        //   These two adjacent vertices are sufficient to construct
        //   the polygon edge joining them. This polygon resides 
        //   in the Poincare disk, so the polygon edge will be 
        //   a hyperbolic geodesic: either a circular arc or a 
        //   diameter of the Poincare disk.  
        //    
        //   Invoking the JLine class constructor on these two 
        //   adjacent polygon vertices (see code below) 
        //   will construct the associated edge as a hyperbolic 
        //   geodesic. 
        //            
        //    
        //    
        //   To be used later:   
        //   the hyperbolic geodesic is "straight", i.e. is a
        //   Euclidean line segment iff the geodesic is a diameter
        //   of the Poincare disk iff the geodesic passes through
        //   the complex origin of coordinates (aka the center 
        //   of the Poincare unit disk).
        //   

        DJJ::JLine      tmpLine(vert_one, vert_two);   //  ctor initialization;

        //                  
        //   The following are all in Logical Coords: 
        //      JLine::circleCenter,
        //      JLine::circleRadius,
        //      JPolygon::jpolyVertices. 
        // 
        //   So the entities above can be used to determine 
        //   arc SWEEP_DIRECTION via vector cross product 
        //   (or the 2-dimensional junior cross product): 
        //                  

        DJJ::HvyPlex  gawgai = vert_one - tmpLine.circleCenter;  // Logical Coords;

        DJJ::HvyPlex  kawkai = vert_two - tmpLine.circleCenter;  // Logical Coords;

        double cross2d = gawgai.real() * kawkai.imag() - gawgai.imag() * kawkai.real();

        if (cross2d < 0.00)  //  Must compute cross2d from Logical Coords!!!
        {
            jpolyEdges.at(idx_one).eo_sweepDirection = D2D1_SWEEP_DIRECTION_COUNTER_CLOCKWISE;
        }
        else 
        {
            jpolyEdges.at(idx_one).eo_sweepDirection = D2D1_SWEEP_DIRECTION_CLOCKWISE;
        }

        this->jpolyEdges.at(idx_one).eo_jLine = DJJ::JLine(vert_one, vert_two);  //  copy assignment operator ???
    }
}




void DJJ::JPolygon::GeometryUseNativeArcs(
    ID2D1GeometrySink        *pSink,
    Windows::Foundation::Size  const&  renderTargetSize
)
{
    pSink->SetFillMode(D2D1_FILL_MODE_WINDING);

    //   The shape must begin at vertex zero: 

    DJJ::HvyPlex    vertex_logical = this->jpolyVertices.at(0);


    float dc_phys_x = renderTargetSize.Width / 2.0f;   //  Poincare Unit Disk Center, x-coord;
    float dc_phys_y = renderTargetSize.Height / 2.0f;
    float drad_phys = (std::min)(dc_phys_x, dc_phys_y);  // Poincare Unit Disk Radius in Physical Coords;

    float x_physical = (float)(vertex_logical.real()) * drad_phys + dc_phys_x;
    float y_physical = (float)(vertex_logical.imag()) * drad_phys + dc_phys_y;
    D2D1_POINT_2F vertex_phys2f = D2D1::Point2F(x_physical, y_physical);


    pSink->BeginFigure(vertex_phys2f, D2D1_FIGURE_BEGIN_FILLED);


    for (uint32_t idx_one = 0; idx_one < jpolyTaxonomy; idx_one++)
    {
        uint32_t idx_two = (1 + idx_one) % jpolyTaxonomy;  // wraparound;
        vertex_logical = this->jpolyVertices.at(idx_two);

        x_physical = (float)(vertex_logical.real()) * drad_phys + dc_phys_x;
        y_physical = (float)(vertex_logical.imag()) * drad_phys + dc_phys_y;
        D2D1_POINT_2F end_phys2f = D2D1::Point2F(x_physical, y_physical);

        if (jpolyEdges.at(idx_one).eo_jLine.isStraight)
        {
            pSink->AddLine(end_phys2f);
        }
        else
        {
            float geodesic_radius_phys = drad_phys * (float)(this->jpolyEdges.at(idx_one).eo_jLine.circleRadius);

            D2D1_SIZE_F ellipse_radii_phys = D2D1::SizeF(geodesic_radius_phys, geodesic_radius_phys);

            pSink->AddArc(D2D1::ArcSegment(
                end_phys2f,
                ellipse_radii_phys,
                0.0f,
                this->jpolyEdges.at(idx_one).eo_sweepDirection,
                D2D1_ARC_SIZE_SMALL
            ));
        }

    }
}




