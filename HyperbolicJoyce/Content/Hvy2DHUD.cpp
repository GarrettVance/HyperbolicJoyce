





#include "pch.h"

#include "Hvy2DHUD.h"
#include "Common/DirectXHelper.h"


using namespace HvyDXBase;
using namespace Microsoft::WRL;






Hvy2DHUD::Hvy2DHUD(const std::shared_ptr<DX::DeviceResources>& deviceResources) : 
    m_text(L""),
    m_deviceResources(deviceResources)
{
    ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

    // Create device independent resources
    ComPtr<IDWriteTextFormat> textFormat;
    DX::ThrowIfFailed(
        m_deviceResources->GetDWriteFactory()->CreateTextFormat(
            L"Segoe UI",
            nullptr,
            //  DWRITE_FONT_WEIGHT_LIGHT,
            DWRITE_FONT_WEIGHT_BOLD,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            32.0f,
            L"en-US",
            &textFormat
            )
        );

    DX::ThrowIfFailed(
        textFormat.As(&m_textFormat)
        );

    DX::ThrowIfFailed(
        m_textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR)
        );

    DX::ThrowIfFailed(
        m_deviceResources->GetD2DFactory()->CreateDrawingStateBlock(&m_stateBlock)
        );

    CreateDeviceDependentResources();
}








void Hvy2DHUD::Update(
    DX::StepTimer       const&  p_timer,
    std::wstring        const&  p_wchar_message
)
{
 
    double alphaChannel = 1.0;
    if (p_timer.GetTotalSeconds() < HvyDXBase::MedievalFadeDuration - 0.1)  //  subtract 0.1f to prevent taking square root of negative number...
    {
        alphaChannel = sqrt(HvyDXBase::MedievalFadeDuration - (float)p_timer.GetTotalSeconds()) / sqrt(HvyDXBase::MedievalFadeDuration);
    }
    else
    {
        alphaChannel = 0.0;
    }
    m_text_SolidColorBrush->SetOpacity(1.f - (float)alphaChannel);



    m_text = p_wchar_message;


    //  orig : uint32 fps = p_timer.GetFramesPerSecond();
    //  orig : m_text = (fps > 0) ? std::to_wstring(fps) + L" FPS" : L" - FPS";


    ComPtr<IDWriteTextLayout> textLayout;

    DX::ThrowIfFailed(
        m_deviceResources->GetDWriteFactory()->CreateTextLayout(
            m_text.c_str(),
            (uint32) m_text.length(),
            m_textFormat.Get(),
            340.0f, // Max width of the input text.
            50.0f, // Max height of the input text.
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








void Hvy2DHUD::Render()
{
    ID2D1DeviceContext* context = m_deviceResources->GetD2DDeviceContext();
    Windows::Foundation::Size logicalSize = m_deviceResources->GetLogicalSize();

    context->SaveDrawingState(m_stateBlock.Get());
    context->BeginDraw();


    //  
    //      m_screen_pos_horz == 0.000 implies "position at left edge of screen";       
    //      m_screen_pos_horz == 0.999 implies "position at right edge of screen";       
    // 
    //      m_screen_pos_vert == 0.000 implies "position along the top edge of screen";       
    //      m_screen_pos_vert == 0.999 implies "position along the bottom edge of screen";    
    // 



    float location_x = (m_screen_pos_horz < 0.5f) ?
        30.f : (logicalSize.Width - m_textMetrics.layoutWidth - 30);

    float location_y = (m_screen_pos_vert < 0.5f) ?
        30.f : (logicalSize.Height - m_textMetrics.height - 30);


    D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(location_x, location_y); 

    context->SetTransform(screenTranslation * m_deviceResources->GetOrientationTransform2D());











    DWRITE_TEXT_ALIGNMENT   v_lateral = (m_screen_pos_horz < 0.5f) ?
        DWRITE_TEXT_ALIGNMENT_LEADING : DWRITE_TEXT_ALIGNMENT_TRAILING;


    DX::ThrowIfFailed(m_textFormat->SetTextAlignment(v_lateral));

    //   ???   i wonder if this call to SetTextAlignment() invalidates some of the data held in m_textMetrics ??? 

    //   maybe should re-calculate text metrics now ??? 



    context->DrawTextLayout(D2D1::Point2F(0.f, 0.f), m_textLayout.Get(), m_text_SolidColorBrush.Get());




    // Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
    // is lost. It will be handled during the next call to Present.   





    HRESULT hr = context->EndDraw();    //   <-----   D2D1 EndDraw;




    if (hr != D2DERR_RECREATE_TARGET)
    {
        DX::ThrowIfFailed(hr);
    }

    context->RestoreDrawingState(m_stateBlock.Get());
}









void Hvy2DHUD::CreateDeviceDependentResources()
{
    DX::ThrowIfFailed(
        m_deviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(
            //  GOLD : D2D1::ColorF(D2D1::ColorF::Black), 
            //  D2D1::ColorF(D2D1::ColorF::YellowGreen), 
            //  D2D1::ColorF(0.f, 0.8f, 0.f, 0.07f), 
            D2D1::ColorF(0.f, 0.8f, 0.f, 1.f), 
            &m_text_SolidColorBrush
        )
    );
}





void Hvy2DHUD::ReleaseDeviceDependentResources()
{
    m_text_SolidColorBrush.Reset();
}




