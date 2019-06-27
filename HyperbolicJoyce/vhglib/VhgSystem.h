


#pragma once

#include "..\Common\DeviceResources.h"
#include "..\Content\HvySchlafliButtons.h"
#include "..\Content\HvySkipNumButtons.h"
#include "..\Content\HvyCheckBoxQuasiRegular.h"
#include "DefTypes.h"
#include "JPoincareDisk.h"








namespace HvyDXBase
{

    class VhgSystem
    {
    public:

        VhgSystem(
            std::shared_ptr<DX::DeviceResources>const & deviceResources,
            const std::shared_ptr<HvyDXBase::HvySchlafliButtons> & p_schlafliButtons,
            const std::shared_ptr<HvyDXBase::HvySkipNumButtons> & p_skipNumButtons,
            const std::shared_ptr<HvyDXBase::HvyCheckBoxQuasiRegular> & p_checkBoxQuasiRegular
        );




        void                    hideMedieval() { sy_ShowMedieval = false; }


        bool                    displayModeIsMedieval() { return sy_ShowMedieval; }




        std::wstring                                            getTilingDescription();

        static std::random_device                               e_rng_seed;
        static std::mt19937                                     e_rng_gen;
        static std::uniform_int_distribution<int>               e_rng_dist;



        static unsigned char                                    e_OpMode_Joyce;
        static unsigned char                                    e_OpMode_bits;



        void                                                    RenderByMode();



    private: 

        std::shared_ptr<DX::DeviceResources>                    sy_deviceResources;
        std::shared_ptr<HvyDXBase::HvySchlafliButtons>          sy_schlafliButtons;
        std::shared_ptr<HvyDXBase::HvySkipNumButtons>           sy_skipNumButtons;
        std::shared_ptr<HvyDXBase::HvyCheckBoxQuasiRegular>     sy_checkBoxQuasiRegular;
        bool                                                    sy_ShowMedieval;
        Microsoft::WRL::ComPtr<ID2D1Bitmap>                     sy_D2D1BitmapMedieval;
        Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>            sy_d2d1_brush;
        std::unique_ptr<DJJ::JPoincareDisk>                     sy_JoyceJavaDisk;
    };


}
//  Closes namespace HvyDXBase; 




