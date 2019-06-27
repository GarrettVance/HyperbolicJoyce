//      
//              
//              
//                  VhgSystem.cpp    
//              
//              



#include "pch.h"

#include "..\Common\DirectXHelper.h"

#include "VhgSystem.h"

#include "BitmapUtil.h"



using namespace HvyDXBase;
using namespace DirectX;
using namespace Windows::Foundation;




unsigned char               HvyDXBase::VhgSystem::e_OpMode_Joyce        = 0x04;


unsigned char               HvyDXBase::VhgSystem::e_OpMode_bits = e_OpMode_Joyce; 







VhgSystem::VhgSystem(
    std::shared_ptr<DX::DeviceResources>const & deviceResources,
    const std::shared_ptr<HvyDXBase::HvySchlafliButtons> & p_schlafliButtons,
    const std::shared_ptr<HvyDXBase::HvySkipNumButtons> & p_skipNumButtons, 
    const std::shared_ptr<HvyDXBase::HvyCheckBoxQuasiRegular> & p_checkBoxQuasiRegular
) :
    sy_ShowMedieval(true), 
    sy_deviceResources(deviceResources),
    sy_schlafliButtons(p_schlafliButtons), 
    sy_skipNumButtons(p_skipNumButtons),
    sy_checkBoxQuasiRegular(p_checkBoxQuasiRegular)
{

    DX::ThrowIfFailed(
        sy_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            sy_d2d1_brush.GetAddressOf()
        )
    );


    HvyDXBase::CreateSimpleD2D1BitmapFromImage(
        deviceResources,
        sy_D2D1BitmapMedieval,
        L"Assets\\Pacioli.png"
    );


    sy_JoyceJavaDisk = std::make_unique<DJJ::JPoincareDisk>(
        deviceResources, 
        p_schlafliButtons, 
        p_skipNumButtons, 
        p_checkBoxQuasiRegular
    );


    sy_JoyceJavaDisk->init();
}







std::wstring        HvyDXBase::VhgSystem::getTilingDescription()
{

    return sy_JoyceJavaDisk.get()->getTilingDescription();

}








void HvyDXBase::VhgSystem::RenderByMode()
{
    auto d2d_context_2d = sy_deviceResources->GetD2DDeviceContext();


    d2d_context_2d->BeginDraw();

    
    d2d_context_2d->Clear(D2D1::ColorF(17.f/255.f, 19.f/255.f, 32.f/255.f, 1.0f));  // clearcolor
   

    sy_JoyceJavaDisk->updateLikeJoyceJavaUpdate();
        

    DX::ThrowIfFailed(d2d_context_2d->EndDraw());
}






