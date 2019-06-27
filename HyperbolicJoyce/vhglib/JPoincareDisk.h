

#pragma once

#include "..\Common\DeviceResources.h"

#include "..\Content\HvySchlafliButtons.h"
#include "..\Content\HvySkipNumButtons.h"
#include "..\Content\HvyCheckBoxQuasiRegular.h"


#include "DefTypes.h"
#include "JPolygon.h"




namespace DJJ   //  David Joyce's Java classes and methods; 
{
    class JPoincareDisk 
    {
    public:

        JPoincareDisk(
            const std::shared_ptr<DX::DeviceResources>& p_DeviceResources,
            const std::shared_ptr<HvyDXBase::HvySchlafliButtons> & p_schlafliButtons,
            const std::shared_ptr<HvyDXBase::HvySkipNumButtons> & p_skipNumButtons,
            const std::shared_ptr<HvyDXBase::HvyCheckBoxQuasiRegular> & p_checkBoxQuasiRegular
        );


        std::wstring            getTilingDescription();



        void                    init();



        void                    RenderWithoutStars();

        void                    RenderSomeStars();


        void                    updateLikeJoyceJavaUpdate();  





    private:

        D2D1::ColorF            randomColor();

        int                     applyRule(int i, int j);

        void                    countPolygons(int p_num_layers);

        void                    determinePolygons();

        JPolygon                createNextPolygon(JPolygon& srcGon, int s);


    public:


        static std::random_device                               e_rng_seed;
        static std::mt19937                                     e_rng_gen;
        static std::uniform_real_distribution<float>            e_rng_dist;


        static bool                                         jj_AlternatingRequested;


    private:

        std::shared_ptr<DX::DeviceResources>                jj_DeviceResources;



        std::shared_ptr<HvyDXBase::HvySchlafliButtons>         jj_schlafliRadioButtons;
        uint32_t                                            jj_schlafliCurrentButton;
        uint32_t                                            jj_SchlafliN;  // number of sides of the polygon: it is an n-gon;
        uint32_t                                            jj_SchlafliK;  // vertex valence (number of n-gons meeting at each vertex);




        std::shared_ptr<HvyDXBase::HvySkipNumButtons>       jj_skipNumberRadioButtons;
        uint32_t                                            jj_skipNumberCurrentButton;
        uint32_t                                            jj_skipNumber;
        uint32_t                                            jj_skipMAXIMUM; 


        std::shared_ptr<HvyDXBase::HvyCheckBoxQuasiRegular>     jj_checkBoxQuasiRegular;
        bool                                                    jj_quasiRegularCheckBoxState;
        bool                                                    jj_quasiRegular;







        Microsoft::WRL::ComPtr<ID2D1PathGeometry>           jj_PolygonPathGeometry;

        Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>        jj_SolidColorBrush;





        std::vector<DJJ::JPolygon>                          jj_NGons;
        std::vector<int>                                    jj_Rules;
        std::vector<D2D1::ColorF>                           jj_ColorVector;



        uint32_t                                            jj_totalPolygons;
        int                                                 jj_innerPolygons;





        bool                                                jj_AlternatingGranted;



        uint32_t                                     jj_Layers;

    };

}
//  Closes namespace DJJ; 




