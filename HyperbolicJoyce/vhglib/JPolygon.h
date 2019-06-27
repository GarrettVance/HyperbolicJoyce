
#pragma once

#include "..\Common\DeviceResources.h"
#include "DefTypes.h"
#include "EdgeODesic.h"





namespace DJJ   //  David Joyce's Java classes and methods; 
{


    DJJ::HvyPlex         ReflectPointInPoint(DJJ::HvyPlex preimage,  DJJ::HvyPlex mirror);



    double HyperbolicDistanceFromEuclideanDistanceFromOrigin( double p_euclidean_distance_to_origin );




    class JPolygon
    {
    public:

        JPolygon(uint32_t p_Taxonomy);


        DJJ::HvyPlex            getVertex(int i) { return jpolyVertices[i]; }

       
        void                    setVertex(int i, DJJ::HvyPlex P) { jpolyVertices.at(i) = P; }


        static JPolygon         constructCenterPolygon(int p_VertexCount, int p_VertexValence, bool p_quasiregular);


        void                    constructEdges();


        void                    GeometryUseNativeArcs(
            ID2D1GeometrySink        *pSink,
            Windows::Foundation::Size  const&  renderTargetSize
        );


    public:  

        uint32_t                            jpolyTaxonomy;  //  5 for Pentagon, 6 for hexagon, ...

        std::vector<DJJ::HvyPlex>           jpolyVertices;

        std::vector<DJJ::EdgeODesic>        jpolyEdges;
    };

}
//  Closes namespace DJJ; 







