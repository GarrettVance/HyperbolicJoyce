


#include "pch.h"

#include "HvySkipNumButtons.h"
#include "Common/DirectXHelper.h"

using namespace HvyDXBase;
using namespace Microsoft::WRL;







HvyDXBase::HvySkipNumButtons::HvySkipNumButtons(
    const std::shared_ptr<DX::DeviceResources>&         p_deviceResources,
    const uint32_t                                      p_number_of_radiettes,
    const float&                                        p_pos_horizontal,
    const float&                                        p_pos_vertical,
    D2D1::ColorF::Enum                                  p_D2D1_ColorF_Enum
)
  : rb_numberOfRadiettes(p_number_of_radiettes), 
    snb_theSelectedButton(0),    //  aka Default Selection is zero;
    m_text(std::wstring(L"NYI")),
    m_panelCornerRadius(17.f),     //  corner radius = 17.f nominal;
    m_panelOutlineThickness(2.f),   //  thickness = 2.f;

    snb_skipMAXIMUM(1),

    HvyWidgets(p_deviceResources, p_pos_horizontal, p_pos_vertical, p_D2D1_ColorF_Enum)
{
        
    Microsoft::WRL::ComPtr<ID2D1EllipseGeometry>        nullGeometry;
    m_ellipseGeometries.assign(rb_numberOfRadiettes, nullGeometry);
}








void HvyDXBase::HvySkipNumButtons::MouseButtonLeft(std::complex<double>   p_mouseClickPhysical)
{
    BOOL containsPointM;
    HRESULT hrM = S_OK;
    D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(m_refDatumX, m_refDatumY); 




    //  GOLD : for (uint32_t idx_geometry = 0; idx_geometry < rb_numberOfRadiettes; idx_geometry++)

    for (uint32_t idx_geometry = 0; idx_geometry < snb_skipMAXIMUM; idx_geometry++)  //  TODO : problem ???
    {
        hrM = m_ellipseGeometries.at(idx_geometry).Get()->FillContainsPoint(
            D2D1::Point2F((float)(p_mouseClickPhysical.real()), (float)(p_mouseClickPhysical.imag())),
            screenTranslation,
            &containsPointM
        );

        if (containsPointM)
        {
            snb_theSelectedButton = idx_geometry;
            break;
        }
    }
}





















void HvyDXBase::HvySkipNumButtons::Render()
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



    m_refDatumX = (m_screen_pos_horz < 0.5f) ?  30.f : (logicalSize.Width - m_textMetrics.layoutWidth - 60);

    //  m_refDatumY = (m_screen_pos_vert < 0.5f) ?  30.f : (logicalSize.Height - m_textMetrics.height - 30);

    m_refDatumY = (m_screen_pos_vert < 0.5f) ?  30.f : (logicalSize.Height - rb_numberOfRadiettes * (m_textMetrics.height + m_buttonDeltaY) - 30);




    D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(m_refDatumX, m_refDatumY); 

    context->SetTransform(screenTranslation * m_deviceResources->GetOrientationTransform2D());











    //  DWRITE_TEXT_ALIGNMENT   v_lateral = (m_screen_pos_horz < 0.5f) ?  DWRITE_TEXT_ALIGNMENT_LEADING : DWRITE_TEXT_ALIGNMENT_TRAILING;

    DWRITE_TEXT_ALIGNMENT   v_lateral = DWRITE_TEXT_ALIGNMENT_TRAILING;

    DX::ThrowIfFailed(m_textFormat->SetTextAlignment(v_lateral));










    //   
    //    Need to query the value of skipMAXIMUM: 
    //        





    OnDemandText(std::wstring(L"Skip 1"));
    context->DrawTextLayout(D2D1::Point2F(0.f, 6.f), m_textLayout.Get(), m_text_SolidColorBrush.Get());

    if (snb_skipMAXIMUM > 1)
    {
        OnDemandText(std::wstring(L"Skip 2"));
        context->DrawTextLayout(D2D1::Point2F(0.f, 6.f + m_buttonDeltaY), m_textLayout.Get(), m_text_SolidColorBrush.Get());

        if (snb_skipMAXIMUM > 2)
        {
            OnDemandText(std::wstring(L"Skip 3"));
            context->DrawTextLayout(D2D1::Point2F(0.f, 6.f + 2.f * m_buttonDeltaY), m_textLayout.Get(), m_text_SolidColorBrush.Get());
        }
    }








    //   The values of "Left" and "Top" in the following D2D1_RECT_F struct 
    //   must be set to zero because the prior call to context->SetTransform() 
    //   is still exerting its influence over all D2D1 drawing: 

        
    D2D1_RECT_F buttonRect = D2D1::RectF(
        0.f, 
        0.f, 
        1.2f * m_textMetrics.layoutWidth, 
        1.5f * m_textMetrics.height
    );


    //  GOLD : for (uint32_t idx_wgt = 0; idx_wgt < rb_numberOfRadiettes; idx_wgt++)

    for (uint32_t idx_wgt = 0; idx_wgt < snb_skipMAXIMUM; idx_wgt++)
    {
        buttonRect.top = 0.f + idx_wgt * m_buttonDeltaY;
        buttonRect.bottom = buttonRect.top + 1.5f * m_textMetrics.height;

        D2D1_ROUNDED_RECT buttonRoundedRect{ buttonRect, m_panelCornerRadius, m_panelCornerRadius };
        context->DrawRoundedRectangle(&buttonRoundedRect, m_text_SolidColorBrush.Get(), m_panelOutlineThickness, NULL);
    }








    /************************************************************************************/
    /************************************************************************************/
    //              
    //      Draw Square Check Boxes: 
    //              
#if 1 == 2
    float edge_rr = 12; float x_rr = 20.f; 
    float y_rr = (1.5f * m_textMetrics.height / 2.f)  -  (edge_rr / 2.f);
    D2D1_RECT_F checkBox = D2D1::RectF( x_rr, y_rr, edge_rr + x_rr, edge_rr + y_rr );
    context->DrawRectangle(checkBox, m_text_SolidColorBrush.Get());
#endif 
    /************************************************************************************/
    /************************************************************************************/








    //              
    //      Draw Circular Radio Buttons: 
    //              
    float outer_circle_radius = 18.f;
    float x_indent = 23.f;
    float y_centerline = (1.5f * m_textMetrics.height / 2.f);
    D2D1_ELLIPSE myEllipse;


    myEllipse.radiusX = 1 + outer_circle_radius / 2.f;
    myEllipse.radiusY = 1 + outer_circle_radius / 2.f;

    static bool buttonsInvalidated = true;

    if (buttonsInvalidated)
    {
        buttonsInvalidated = false; 

        for (uint32_t idxB = 0; idxB < rb_numberOfRadiettes; idxB++)
        {
            myEllipse.point = D2D1::Point2F(
                x_indent, 
                y_centerline + ((float)idxB * m_buttonDeltaY)
            );

            HRESULT hr0 = m_deviceResources->GetD2DFactory()->CreateEllipseGeometry(
                myEllipse, 
                m_ellipseGeometries.at(idxB).GetAddressOf()
            );

            assert(SUCCEEDED(hr0));
        }
    }




    //  GOLD : for (uint32_t idxDraw = 0; idxDraw < rb_numberOfRadiettes; idxDraw++)

    for (uint32_t idxDraw = 0; idxDraw < snb_skipMAXIMUM; idxDraw++)
    {
        context->DrawGeometry(m_ellipseGeometries.at(idxDraw).Get(), m_text_SolidColorBrush.Get());
    }



            
    //  Need some visual cue for recognition of the selected button...
            
    float ctr_y = y_centerline + (float)snb_theSelectedButton * m_buttonDeltaY;

    D2D1_ELLIPSE centr{ D2D1::Point2F(x_indent, ctr_y), 5.f, 5.f };

    context->FillEllipse(centr, m_text_SolidColorBrush.Get());




    // Ignore D2DERR_RECREATE_TARGET here. This error indicates that the device
    // is lost. It will be handled during the next call to Present.   





    HRESULT hr = context->EndDraw();    //   <-----   D2D1 EndDraw;




    if (hr != D2DERR_RECREATE_TARGET)
    {
        DX::ThrowIfFailed(hr);
    }

    context->RestoreDrawingState(m_stateBlock.Get());
}








