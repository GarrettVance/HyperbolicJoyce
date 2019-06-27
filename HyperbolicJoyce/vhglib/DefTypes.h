

#pragma once

#include <complex>



#define GHV_OPTION_USE_DJJ_CHOPPER






namespace DJJ   //  David Joyce's Java classes and methods; 
{
    using HvyPlex = std::complex<double>;
}
//  Closes namespace DJJ; 





namespace HvyDXBase
{
    using HvyPlex = std::complex<double>;

    struct HvyGeodesic
    {
    public:
        bool                            e_infinite_radius;
        double                          e_radius;
        std::complex<double>            e_center;
    };

    struct Hv3D_ViewTransform
    {
        double                          scale;
        double                          rotation;
        double                          translationX;
        double                          translationY;
    };



    struct conbuf_Klein_struct
    {
        DirectX::XMFLOAT2    obs_z;
        DirectX::XMFLOAT2    obs_dir;
        DirectX::XMFLOAT2    e_pixel_size;   // Store pixel size in x; 
        DirectX::XMFLOAT2    e_alpha_channel;
    };





}
//  Closes namespace HvyDXBase; 



