


#pragma once


#include "..\Common\DeviceResources.h"
#include "..\Common\StepTimer.h"

#include "HvyWidgets.h"





namespace HvyDXBase
{
    class HvySkipNumButtons : public HvyWidgets
    {
    public:

        HvySkipNumButtons::HvySkipNumButtons(
            const std::shared_ptr<DX::DeviceResources>& p_deviceResources,
            const uint32_t                              p_number_of_radiettes,
            const float&                                p_horizontal_percent,
            const float&                                p_vertical_percent,
            D2D1::ColorF::Enum                          p_D2D1_ColorF_Enum
        );


        virtual void                        Render();

        virtual void                        MouseButtonLeft(std::complex<double>   p_mouseClickPhysical);





        void                        SetSkipNumberMaximum(uint32_t p_skipMAXIMUM) { snb_skipMAXIMUM = p_skipMAXIMUM; }

        void                        SetSelectedButton(uint32_t p_selectedButton) { snb_theSelectedButton = p_selectedButton; }

        uint32_t                    GetChosenButton() { return snb_theSelectedButton; }

        uint32_t                    GetDefaultButton() { return 0; }


    private:

        std::vector<Microsoft::WRL::ComPtr<ID2D1EllipseGeometry>>           m_ellipseGeometries;
        uint32_t                                                            rb_numberOfRadiettes;
        uint32_t                                                            snb_theSelectedButton;
        std::wstring                                                        m_text;


        uint32_t                                                            snb_skipMAXIMUM;

        float                                                               m_panelCornerRadius;
        float                                                               m_panelOutlineThickness;
    };

}
//  Closes namespace HvyDXBase; 



