

#pragma once

#include "..\Common\DeviceResources.h"
#include "..\Common\StepTimer.h"


namespace HvyDXBase
{
    class HvyWidgets
    {
    public:

        HvyWidgets::HvyWidgets(
            const std::shared_ptr<DX::DeviceResources>&     p_deviceResources,
            const float&                                    p_pos_horizontal,
            const float&                                    p_pos_vertical,
            D2D1::ColorF::Enum                              p_D2D1_ColorF_Enum
        );



        //====================================================================
        //====================================================================
        //          
        //          Non-virtual methods: these methods 
        //   are implemented completely in this base class: 
        //          
        //====================================================================
        //====================================================================

        void CreateDeviceDependentResources(); // Intentionally not virtual;

        void ReleaseDeviceDependentResources(); // Intentionally not virtual;


        //====================================================================
        //====================================================================
        //          
        //          Virtual but not pure: 
        //          
        //====================================================================
        //====================================================================

        virtual void                    Update(
                                    DX::StepTimer       const&  p_timer,
                                    std::wstring        const&  p_wchar_message
        );


        virtual void                    OnDemandText(std::wstring    p_message);


        //====================================================================
        //====================================================================
        //          
        //          Methods that every derived class must implement
        //         in code that is unique to each derived class: 
        //          
        //====================================================================
        //====================================================================



        virtual void                    Render() = 0;


        virtual void                    MouseButtonLeft(std::complex<double>   p_mouseClickPhysical) = 0;





    //=================================================================
    //          
    //      My HvyWidgets class has no public data members.
    //          
    //=================================================================


    protected:

        std::shared_ptr<DX::DeviceResources>                m_deviceResources;
        Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>        m_text_SolidColorBrush;
        D2D1::ColorF::Enum                                  m_color_enum;
        float                                               m_screen_pos_vert;
        float                                               m_screen_pos_horz;


        DWRITE_TEXT_METRICS                                 m_textMetrics;
        Microsoft::WRL::ComPtr<ID2D1DrawingStateBlock1>     m_stateBlock;
        Microsoft::WRL::ComPtr<IDWriteTextLayout3>          m_textLayout;
        Microsoft::WRL::ComPtr<IDWriteTextFormat2>          m_textFormat;


        float                                               m_textMaxWidth;
        float                                               m_textMaxHeight;

        float                                               m_refDatumX;
        float                                               m_refDatumY;
        float                                               m_buttonDeltaY;
    };
}
//  Closes namespace HvyDXBase; 



