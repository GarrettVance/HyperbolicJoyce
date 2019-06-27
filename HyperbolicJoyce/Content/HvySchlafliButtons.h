


#pragma once


#include "..\Common\DeviceResources.h"
#include "..\Common\StepTimer.h"

#include "HvyWidgets.h"




namespace HvyDXBase
{
    class HvySchlafliButtons : public HvyDXBase::HvyWidgets
    {
    public:

        HvySchlafliButtons::HvySchlafliButtons(
            const std::shared_ptr<DX::DeviceResources>& p_deviceResources,
            const uint32_t                              p_number_of_radiettes,
            const float&                                p_horizontal_percent,
            const float&                                p_vertical_percent,
            D2D1::ColorF::Enum                          p_D2D1_ColorF_Enum
        );



        virtual void                        Render();

        virtual void                        MouseButtonLeft(std::complex<double>   p_mouseClickPhysical);





        uint32_t                            GetChosenButton() { return m_theSelectedButton; }

        uint32_t                            GetDefaultButton() { return 0; }


    private:

        std::vector<Microsoft::WRL::ComPtr<ID2D1EllipseGeometry>>           m_ellipseGeometries;

        uint32_t                                                            rb_numberOfRadiettes;

        uint32_t                                                            m_theSelectedButton;

        std::wstring                                                        m_text;


        float                                                               m_panelCornerRadius;
        float                                                               m_panelOutlineThickness;
    };

}
//  Closes namespace HvyDXBase; 



