

#pragma once

#include "..\Common\DeviceResources.h"
#include "..\Common\StepTimer.h"

#include "HvyWidgets.h"





namespace HvyDXBase
{
    class HvyCheckBoxQuasiRegular : public HvyWidgets
    {
    public:

        HvyCheckBoxQuasiRegular::HvyCheckBoxQuasiRegular(
            const std::shared_ptr<DX::DeviceResources>& p_deviceResources,
            const float&                                p_horizontal_percent,
            const float&                                p_vertical_percent,
            D2D1::ColorF::Enum                          p_D2D1_ColorF_Enum
        );


        virtual void                        Render();

        virtual void                        MouseButtonLeft(std::complex<double>   p_mouseClickPhysical);



        void                    SetCheckedState(bool p_buttonState) { m_buttonState = p_buttonState; }

        bool                    GetCheckedState() { return m_buttonState; }

        bool                    GetDefaultState() { return false; }

    private:

        std::vector<Microsoft::WRL::ComPtr<ID2D1EllipseGeometry>>           m_ellipseGeometries;

        bool                                                                m_buttonState;

        std::wstring                                                        m_text;

        float                                                               m_panelCornerRadius;
        float                                                               m_panelOutlineThickness;
    };

}
//  Closes namespace HvyDXBase; 



