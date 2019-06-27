



#include "pch.h"


#include "Hvy3DScene.h"
#include "..\Common\DirectXHelper.h"



using namespace HvyDXBase; 
using namespace DirectX;
using namespace Windows::Foundation;







Hvy3DScene::Hvy3DScene(
    const std::shared_ptr<DX::DeviceResources>& deviceResources,
    const std::shared_ptr<HvyDXBase::HvySchlafliButtons> & p_schlafliButtons,
    const std::shared_ptr<HvyDXBase::HvySkipNumButtons> & p_skipNumButtons, 
    const std::shared_ptr<HvyDXBase::HvyCheckBoxQuasiRegular> & p_checkBoxQuasiRegular
) :
    m_loadingComplete(false),
    m_degreesPerSecond(45),
    m_indexCount(0),
    m_tracking(false),


    g0_debouncer_processed_mouse(false),
    g0_debouncer_processed_keyboard(false),


    m_deviceResources(deviceResources),
    sc_schlafliButtons(p_schlafliButtons), 
    sc_skipNumButtons(p_skipNumButtons),
    sc_checkBoxQuasiRegular(p_checkBoxQuasiRegular)
{

    e_ghv_system = new VhgSystem(deviceResources, p_schlafliButtons, p_skipNumButtons, p_checkBoxQuasiRegular);


    e_keyboard = std::make_unique<DirectX::Keyboard>();
    e_keyboard->SetWindow(Windows::UI::Core::CoreWindow::GetForCurrentThread());



    CreateDeviceDependentResources();
    CreateWindowSizeDependentResources();
}






bool approx_equal(double pr, double ps)
{
    double const konst_zero = 0.000000001; 
    bool retval = false; 

    if (abs(pr - ps) < konst_zero)
    {
        retval = true;
    }

    return retval;
}








void Hvy3DScene::CreateWindowSizeDependentResources()
{
    // Initializes view parameters when the window size changes.


    Size outputSize = m_deviceResources->GetOutputSize();
    float aspectRatio = outputSize.Width / outputSize.Height;
    float fovAngleY = 70.0f * XM_PI / 180.0f;
    if (aspectRatio < 1.0f) { fovAngleY *= 2.0f; }




    XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
        fovAngleY,
        aspectRatio,
        0.01f,
        100.0f
        );

    XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

    XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

    XMStoreFloat4x4(
        &m_constantBufferData.projection,
        XMMatrixTranspose(perspectiveMatrix * orientationMatrix)
        );


    XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixIdentity());
}























void Hvy3DScene::Update(DX::StepTimer const& timer)
{




    double alphaChannel = 1.f;

    if (timer.GetTotalSeconds() < HvyDXBase::MedievalFadeDuration - 0.1f)  //  subtract 0.1f to prevent taking square root of negative number...
    {
        alphaChannel = sqrt(HvyDXBase::MedievalFadeDuration - (float)timer.GetTotalSeconds()) / sqrt(HvyDXBase::MedievalFadeDuration);
    }
    else
    {
        alphaChannel = 0.f;
    }


    if (timer.GetTotalSeconds() > HvyDXBase::MedievalFadeDuration)
    {
        e_ghv_system->hideMedieval();
    }



    //  store the new values of obs_dir, obs_z, pix in conbuf_Klein_data: 

    s3d_conbuf_Klein_data.obs_dir.x = 0.f;
    s3d_conbuf_Klein_data.obs_dir.y = 0.f;

    s3d_conbuf_Klein_data.obs_z.x = 0.f;
    s3d_conbuf_Klein_data.obs_z.y = 0.f;

    s3d_conbuf_Klein_data.e_pixel_size.x = 2.f;
    s3d_conbuf_Klein_data.e_pixel_size.y = 0.0f;  //  unused;

    s3d_conbuf_Klein_data.e_alpha_channel.x = (float)alphaChannel;
    s3d_conbuf_Klein_data.e_alpha_channel.y = 0.0f; //  unused; 






    DirectX::Keyboard::State           kb = e_keyboard->GetState();










    if (!m_tracking)
    {
        
        //   GOLD : XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixIdentity());

        float alpha = 1.2f;
        alpha = 1.1f;

        XMStoreFloat4x4(
            &m_constantBufferData.model, 
            XMMatrixTranspose(
                // XMMatrixScaling(0.6f, 0.6f, 1.0f)
                XMMatrixScaling(alpha * 0.6f, alpha * 0.5f, 1.0f)
                // XMMatrixScaling(0.6f, 0.4f, 1.0f)
            )
        );
    }
}















void Hvy3DScene::Render_D2D1(void)
{

    this->e_ghv_system->RenderByMode();

}





















void Hvy3DScene::Render_3D_Shaders(void)
{
    auto d3d_context_3d = m_deviceResources->GetD3DDeviceContext();


    //   ghv : prepare the Constant Buffers: 

    d3d_context_3d->UpdateSubresource1( m_constantBuffer.Get(), 0, NULL, &m_constantBufferData, 0, 0, 0 );

    d3d_context_3d->UpdateSubresource1(s3d_conbuf_Klein_buffer.Get(), 0, NULL, &s3d_conbuf_Klein_data, 0, 0, 0);


    //      ghv : vertexbuffer and indexbuffer: 
    //      ===================================
    //  Each index is a 16-bit unsigned integer (short), thus DXGI_FORMAT_R16_UINT:  
    //      

    UINT stride = sizeof(VertexPositionColor);
    UINT offset = 0;
    d3d_context_3d->IASetVertexBuffers( 0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset );
    d3d_context_3d->IASetIndexBuffer( m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0 );


    d3d_context_3d->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    d3d_context_3d->IASetInputLayout(m_inputLayout.Get());


    d3d_context_3d->VSSetShader( m_vertexShader.Get(), nullptr, 0 );
    d3d_context_3d->VSSetConstantBuffers1( 0, 1, m_constantBuffer.GetAddressOf(), nullptr, nullptr );


    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    const float gv_blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };  //  result 03 : pure black&white slices;

    d3d_context_3d->OMSetBlendState(s3d_blend_state.Get(), gv_blend_factor, 0xFFFFFFFF);  //  keywords: blendstate, alphablending;

    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


    d3d_context_3d->PSSetShader( m_pixelShader.Get(), nullptr, 0 );
    d3d_context_3d->PSSetConstantBuffers1(0, 1, s3d_conbuf_Klein_buffer.GetAddressOf(), nullptr, nullptr);
    d3d_context_3d->PSSetShaderResources(0, 1, s3d_texture_srv_1.GetAddressOf());
    d3d_context_3d->PSSetSamplers(0, 1, s3d_texture_sampler_state.GetAddressOf());


    d3d_context_3d->DrawIndexed( m_indexCount, 0, 0 );
}
//  Closes Hvy3DScene::Render_3D_Shaders();  










void Hvy3DScene::debouncer_for_mouse(void)
{
    static uint32_t idx_waiting = 0;
    if (g0_debouncer_processed_mouse)
    {
        idx_waiting++;
        if (idx_waiting > 30)
        {
            idx_waiting = 0; 
            g0_debouncer_processed_mouse = false;
        }
    }
}









void Hvy3DScene::debouncer_for_keyboard(void)
{
    static uint32_t idx_waiting = 0;
    if (g0_debouncer_processed_keyboard)
    {
        idx_waiting++;
        if (idx_waiting > 30)
        {
            idx_waiting = 0; 
            g0_debouncer_processed_keyboard = false;
        }
    }
}























void Hvy3DScene::Render()
{

    if (!m_loadingComplete)  // Loading is asynchronous. Only draw geometry after it's loaded.
    {
        return;
    }


    this->debouncer_for_mouse();

    this->debouncer_for_keyboard();


    this->Render_D2D1();
   
   
    if (displayModeIsMedieval())
    {
        this->Render_3D_Shaders();
    }
}



















void Hvy3DScene::MeshMonoQuad(void)
{
    // ghv : Values of texco texture coordinates will be 
    // set inside the VS vertex shader. 


    static const VertexPositionColor monoQuadVertices[] =
    {
        //   increasing negative values of z push the mesh deeper into the background: 

        {XMFLOAT3(-1.0f, -1.0f,  -0.75f), XMFLOAT3(0.0f, 0.0f, 0.0f)},
        {XMFLOAT3(+1.0f, -1.0f,  -0.75f), XMFLOAT3(0.0f, 0.0f, 1.0f)},
        {XMFLOAT3(-1.0f, +1.0f,  -0.75f), XMFLOAT3(0.0f, 1.0f, 0.0f)},

        {XMFLOAT3(-1.0f, +1.0f,  -0.75f), XMFLOAT3(0.0f, 1.0f, 1.0f)},
        {XMFLOAT3(+1.0f, -1.0f,  -0.75f), XMFLOAT3(1.0f, 0.0f, 0.0f)},
        {XMFLOAT3(+1.0f, +1.0f,  -0.75f), XMFLOAT3(1.0f, 0.0f, 1.0f)},
    };

    D3D11_SUBRESOURCE_DATA quad_vb_data = { 0 };
    quad_vb_data.pSysMem = monoQuadVertices;
    quad_vb_data.SysMemPitch = 0;
    quad_vb_data.SysMemSlicePitch = 0;

    CD3D11_BUFFER_DESC quad_vb_descr(sizeof(monoQuadVertices), D3D11_BIND_VERTEX_BUFFER);

    DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(
        &quad_vb_descr, &quad_vb_data, &m_vertexBuffer
    )
    );



    //===============================================
    //  Each triangle below is FRONT_FACE_CLOCKWISE: 
    //          
    //  (cf D3D11_RASTERIZER_DESC.FrontCounterClockwise);
    //      
    //  Each trio of index entries represents one triangle. 
    //===============================================

    static const unsigned short quadIndices[] =
    {
        0,2,1,
        5,4,3,
    };

    m_indexCount = ARRAYSIZE(quadIndices);


    D3D11_SUBRESOURCE_DATA quad_ib_data = { 0 };
    quad_ib_data.pSysMem = quadIndices;
    quad_ib_data.SysMemPitch = 0;
    quad_ib_data.SysMemSlicePitch = 0;

    CD3D11_BUFFER_DESC quad_ib_descr(sizeof(quadIndices), D3D11_BIND_INDEX_BUFFER);

    DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(
        &quad_ib_descr, &quad_ib_data, &m_indexBuffer
    )
    );
}










void Hvy3DScene::CreateDeviceDependentResources()
{

    D3D11_RENDER_TARGET_BLEND_DESC  rt_blend_descr = { 0 };
    rt_blend_descr.BlendEnable = TRUE;

    rt_blend_descr.SrcBlend = D3D11_BLEND_SRC_ALPHA;
    rt_blend_descr.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
    rt_blend_descr.BlendOp = D3D11_BLEND_OP_ADD;

    rt_blend_descr.SrcBlendAlpha = D3D11_BLEND_ONE;
    rt_blend_descr.DestBlendAlpha = D3D11_BLEND_ZERO;
    rt_blend_descr.BlendOpAlpha = D3D11_BLEND_OP_ADD;

    rt_blend_descr.RenderTargetWriteMask = 0x0F;

    D3D11_BLEND_DESC  d3d11_blend_descr = { 0 };

    d3d11_blend_descr.AlphaToCoverageEnable = FALSE;
    d3d11_blend_descr.IndependentBlendEnable = FALSE;

    d3d11_blend_descr.RenderTarget[0] = { rt_blend_descr };

    DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBlendState(
        &d3d11_blend_descr,
        s3d_blend_state.GetAddressOf()
    )
    );




    DirectX::CreateWICTextureFromFile(
        m_deviceResources->GetD3DDevice(),
        L"Assets\\Pacioli.png",
        nullptr,    //  temp_resource.ReleaseAndGetAddressOf(), 
        s3d_texture_srv_1.ReleaseAndGetAddressOf(),
        0
    );




    D3D11_SAMPLER_DESC samplerStateDescr;
    ZeroMemory(&samplerStateDescr, sizeof(samplerStateDescr));
    samplerStateDescr.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;


    samplerStateDescr.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerStateDescr.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerStateDescr.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerStateDescr.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samplerStateDescr.MinLOD = 0;
    samplerStateDescr.MaxLOD = D3D11_FLOAT32_MAX;

    DX::ThrowIfFailed(
        m_deviceResources->GetD3DDevice()->CreateSamplerState(
            &samplerStateDescr,
            s3d_texture_sampler_state.ReleaseAndGetAddressOf()
        )
    );













    // Load shaders asynchronously.

    auto loadVSTask = DX::ReadDataAsync(L"generic_vs.cso");
    auto loadPSTask = DX::ReadDataAsync(L"generic_ps.cso");





    // After the vertex shader file is loaded, create the shader and input layout.
    auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
        DX::ThrowIfFailed(
            m_deviceResources->GetD3DDevice()->CreateVertexShader(
                &fileData[0],
                fileData.size(),
                nullptr,
                &m_vertexShader
                )
            );

        static const D3D11_INPUT_ELEMENT_DESC vertexDesc [] =
        {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };

        DX::ThrowIfFailed(
            m_deviceResources->GetD3DDevice()->CreateInputLayout(
                vertexDesc,
                ARRAYSIZE(vertexDesc),
                &fileData[0],
                fileData.size(),
                &m_inputLayout
                )
            );
    });








    auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) 
    {
        DX::ThrowIfFailed( m_deviceResources->GetD3DDevice()->CreatePixelShader(
                &fileData[0], fileData.size(), nullptr, &m_pixelShader )
        );


        CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer) , D3D11_BIND_CONSTANT_BUFFER);
        DX::ThrowIfFailed( m_deviceResources->GetD3DDevice()->CreateBuffer( &constantBufferDesc, nullptr, &m_constantBuffer ) );


        {
            UINT const bytewidth_Klein_struct = sizeof(conbuf_Klein_struct);
            static_assert(bytewidth_Klein_struct % 16 == 0,
                "Constant Buffer must be 16-byte aligned.");

            CD3D11_BUFFER_DESC conbuf_Klein_descr(bytewidth_Klein_struct, D3D11_BIND_CONSTANT_BUFFER);

            DX::ThrowIfFailed(m_deviceResources->GetD3DDevice()->CreateBuffer(
                &conbuf_Klein_descr, nullptr, &s3d_conbuf_Klein_buffer)
            );
        }

    });













    auto createCubeTask = (createPSTask && createVSTask).then([this] () 
    {

        this->MeshMonoQuad();  // ghv : one quad composed of two triangles;

    });










   

    createCubeTask.then([this] () 
    {
        m_loadingComplete = true;
    });

}















void Hvy3DScene::ReleaseDeviceDependentResources()
{
    m_loadingComplete = false;
    m_vertexShader.Reset();
    m_inputLayout.Reset();
    m_pixelShader.Reset();
    m_constantBuffer.Reset();
    m_vertexBuffer.Reset();
    m_indexBuffer.Reset();


    s3d_conbuf_Klein_buffer.Reset();
}



