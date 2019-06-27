

#include "pch.h"

#include "HvyWidgets.h"
#include "Common/DirectXHelper.h"

using namespace HvyDXBase;
using namespace Microsoft::WRL;







void HvyDXBase::HvyWidgets::CreateDeviceDependentResources()
{
    DX::ThrowIfFailed(
        m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(
            D2D1::ColorF(m_color_enum), 
            &m_text_SolidColorBrush
        )
    );
}







void HvyDXBase::HvyWidgets::ReleaseDeviceDependentResources()
{
    m_text_SolidColorBrush.Reset();
}










HvyDXBase::HvyWidgets::HvyWidgets(
    const std::shared_ptr<DX::DeviceResources>&         p_deviceResources,
    const float&                                        p_pos_horizontal,
    const float&                                        p_pos_vertical,
    D2D1::ColorF::Enum                                  p_D2D1_ColorF_Enum
)
  : m_deviceResources(p_deviceResources),
    m_color_enum(p_D2D1_ColorF_Enum),
    m_screen_pos_vert(p_pos_vertical),
    m_screen_pos_horz(p_pos_horizontal),


    m_textMaxWidth(200.f),
    m_textMaxHeight(80.f), 

    m_refDatumX(0.f),
    m_refDatumY(0.f),
    m_buttonDeltaY(85.f)
{
        

    //  Microsoft::WRL::ComPtr<ID2D1EllipseGeometry>        nullGeometry;
    //  m_ellipseGeometries.assign(rb_numberOfRadiettes, nullGeometry);





    ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));


    //   First part:  Create device independent resources

    ComPtr<IDWriteTextFormat> textFormat;
    DX::ThrowIfFailed(
        m_deviceResources->GetDWriteFactory()->CreateTextFormat(
            L"Segoe UI",
            nullptr,
            DWRITE_FONT_WEIGHT_SEMI_BOLD,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            22.0f,    //   Font Size;
            L"en-US", 
            &textFormat
        )
    );
    DX::ThrowIfFailed(textFormat.As(&m_textFormat));




    DX::ThrowIfFailed(m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR));

    DX::ThrowIfFailed(m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock));



    //   
    //    must create a colored brush:
    //  

    CreateDeviceDependentResources();
}







void HvyDXBase::HvyWidgets::Update(
    DX::StepTimer       const&  p_timer,
    std::wstring        const&  p_wchar_message
)
{
    double alphaChannel = 1.0;

    //   
    //  subtract 0.1f to prevent taking square root of negative number: 
    //   

    if (p_timer.GetTotalSeconds() < HvyDXBase::MedievalFadeDuration - 0.1)  
    {
        alphaChannel = 
            sqrt(HvyDXBase::MedievalFadeDuration - (float)p_timer.GetTotalSeconds()) / 
            sqrt(HvyDXBase::MedievalFadeDuration);
    }
    else
    {
        alphaChannel = 0.0;
    }


    m_text_SolidColorBrush->SetOpacity(1.f - (float)alphaChannel);
}






void HvyDXBase::HvyWidgets::OnDemandText(std::wstring p_message)
{
    ComPtr<IDWriteTextLayout> textLayout;

    DX::ThrowIfFailed(
        m_deviceResources->GetDWriteFactory()->CreateTextLayout(
            p_message.c_str(),
            (uint32) p_message.length(),
            m_textFormat.Get(),
            m_textMaxWidth, // Max width of the input text.
            m_textMaxHeight, // Max height of the input text.
            &textLayout
            )
        );

    DX::ThrowIfFailed(
        textLayout.As(&m_textLayout)
        );

    DX::ThrowIfFailed(
        m_textLayout->GetMetrics(&m_textMetrics)
        );
}



