


#pragma once

#include "..\Common\DeviceResources.h"
#include "..\Common\StepTimer.h"

#include "HvySchlafliButtons.h"
#include "HvySkipNumButtons.h"
#include "HvyCheckBoxQuasiRegular.h"


#include "..\vhglib\VhgSystem.h"




namespace HvyDXBase
{

    struct ModelViewProjectionConstantBuffer
    {
        DirectX::XMFLOAT4X4 model;
        DirectX::XMFLOAT4X4 view;
        DirectX::XMFLOAT4X4 projection;
    };



    // Used to send per-vertex data to the vertex shader.

    struct VertexPositionColor
    {
        DirectX::XMFLOAT3 pos;
        DirectX::XMFLOAT3 color;
    };




    class Hvy3DScene
    {
    public:

        Hvy3DScene(
            const std::shared_ptr<DX::DeviceResources>& deviceResources,
            const std::shared_ptr<HvyDXBase::HvySchlafliButtons> & p_schlafliButtons,
            const std::shared_ptr<HvyDXBase::HvySkipNumButtons> & p_skipNumButtons,
            const std::shared_ptr<HvyDXBase::HvyCheckBoxQuasiRegular> & p_checkBoxQuasiRegular
        );



        void CreateDeviceDependentResources();
        void CreateWindowSizeDependentResources();
        void ReleaseDeviceDependentResources();



        bool                    displayModeIsMedieval() { return e_ghv_system->displayModeIsMedieval(); }

        std::wstring            getTilingDescription() { return e_ghv_system->getTilingDescription(); }



        void Hvy3DScene::debouncer_for_mouse(void);
        void Hvy3DScene::debouncer_for_keyboard(void);



        void Update(DX::StepTimer const& timer);

        void Hvy3DScene::Render_D2D1(void);

        void Hvy3DScene::Render_3D_Shaders(void);

        void Render();



        void                    MeshMonoQuad(void);



        HvyDXBase::VhgSystem                                  *e_ghv_system;



    private:

        std::shared_ptr<DX::DeviceResources>                    m_deviceResources;


        std::shared_ptr<HvyDXBase::HvySchlafliButtons>             sc_schlafliButtons;

        std::shared_ptr<HvyDXBase::HvySkipNumButtons>           sc_skipNumButtons;

		std::shared_ptr<HvyDXBase::HvyCheckBoxQuasiRegular>     sc_checkBoxQuasiRegular;




        std::unique_ptr<DirectX::Keyboard>                      e_keyboard;
        bool                                                    g0_debouncer_processed_mouse;
        bool                                                    g0_debouncer_processed_keyboard;


        Microsoft::WRL::ComPtr<ID3D11Buffer>                    s3d_conbuf_Klein_buffer;
        conbuf_Klein_struct                                     s3d_conbuf_Klein_data;
        Microsoft::WRL::ComPtr<ID3D11BlendState>                s3d_blend_state;
        Microsoft::WRL::ComPtr<ID3D11SamplerState>              s3d_texture_sampler_state;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>        s3d_texture_srv_1;



        Microsoft::WRL::ComPtr<ID3D11InputLayout>               m_inputLayout;
        Microsoft::WRL::ComPtr<ID3D11Buffer>                    m_vertexBuffer;
        Microsoft::WRL::ComPtr<ID3D11Buffer>                    m_indexBuffer;
        uint32                                                  m_indexCount;


        Microsoft::WRL::ComPtr<ID3D11VertexShader>              m_vertexShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader>               m_pixelShader;


        Microsoft::WRL::ComPtr<ID3D11Buffer>                    m_constantBuffer;
        ModelViewProjectionConstantBuffer                       m_constantBufferData;

        bool                                                    m_loadingComplete;
        float                                                   m_degreesPerSecond;
        bool                                                    m_tracking;
    };


}
//  Closes namespace HvyDXBase; 




