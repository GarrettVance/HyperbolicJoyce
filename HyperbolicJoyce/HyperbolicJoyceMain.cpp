


#include "pch.h"


#include "HyperbolicJoyceMain.h"


#include "Common\DirectXHelper.h"



using namespace HyperbolicJoyce;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;


using namespace HvyDXBase;








double const HvyDXBase::MedievalFadeDuration = 6;  //  GOLD : 11.00; It goes to eleven!!!







HyperbolicJoyceMain::HyperbolicJoyceMain(
    const std::shared_ptr<DX::DeviceResources>& deviceResources
) : m_deviceResources(deviceResources)
{



	// Register to be notified if the Device is lost or recreated

	m_deviceResources->RegisterDeviceNotify(this);







    e_mouse = std::make_unique<DirectX::Mouse>();
    e_mouse->SetWindow(Windows::UI::Core::CoreWindow::GetForCurrentThread());






	hudZero = std::unique_ptr<Hvy2DHUD>(new Hvy2DHUD(m_deviceResources));




    schlafliButtons = std::shared_ptr<HvySchlafliButtons>(new HvySchlafliButtons(
        m_deviceResources,
        4,
        0.75f,
        0.15f,
        D2D1::ColorF::LightBlue
        ) 
    );

    skipNumberButtons = std::shared_ptr<HvySkipNumButtons>(new HvySkipNumButtons( 
        m_deviceResources,
        3,
        0.15f,
        0.85f,
        D2D1::ColorF::DarkOrange
        ) 
    );

    checkBoxQuasiRegular = std::shared_ptr<HvyCheckBoxQuasiRegular>(new HvyCheckBoxQuasiRegular( 
        m_deviceResources,
        0.75f,
        0.85f,
        D2D1::ColorF::YellowGreen
        ) 
    );


	m_sceneRenderer = std::unique_ptr<Hvy3DScene>(
        new Hvy3DScene(
            m_deviceResources, 
            schlafliButtons, 
            skipNumberButtons,
            checkBoxQuasiRegular
        )
    );

}









HyperbolicJoyceMain::~HyperbolicJoyceMain()
{
	// Deregister device notification

	m_deviceResources->RegisterDeviceNotify(nullptr);
}














void HyperbolicJoyceMain::CreateWindowSizeDependentResources() 
{
    // Updates application state when the window size changes (e.g. device orientation change)


	m_sceneRenderer->CreateWindowSizeDependentResources();
}









void HyperbolicJoyceMain::Update() 
{
    
    DirectX::Mouse::State    maus_state = e_mouse->GetState();

    tracker.Update(maus_state);


    if (maus_state.leftButton == true)
    {
        std::complex<double> mouse_click_physical = std::complex<double>(1.000 * maus_state.x, 1.000 * maus_state.y);
    
   
        this->schlafliButtons->MouseButtonLeft(mouse_click_physical);

        this->skipNumberButtons->MouseButtonLeft(mouse_click_physical);

        if (tracker.leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED)
        {
            this->checkBoxQuasiRegular->MouseButtonLeft(mouse_click_physical);
        }
    }




	m_timer.Tick([&]()
	{

		m_sceneRenderer->Update(m_timer);


        hudZero->Update(
            m_timer, 
            m_sceneRenderer->getTilingDescription()
        );


		this->schlafliButtons->Update(m_timer, std::wstring(L"NYI"));

        this->skipNumberButtons->Update(m_timer, std::wstring(L"NYI"));

        this->checkBoxQuasiRegular->Update(m_timer, std::wstring(L"NYI"));
	});
}











bool HyperbolicJoyceMain::Render() 
{
    // Renders the current frame according to the current application state.

    // Returns true if the frame was rendered and is ready to be displayed.


	// Don't try to render anything before the first Update.

	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}



	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);



	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());



	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);



	m_sceneRenderer->Render();


    hudZero->Render();

    schlafliButtons->Render();

    skipNumberButtons->Render();

    checkBoxQuasiRegular->Render();


	return true;
}















void HyperbolicJoyceMain::OnDeviceLost()
{
    // Notifies renderers that device resources need to be released.


	m_sceneRenderer->ReleaseDeviceDependentResources();

	hudZero->ReleaseDeviceDependentResources();

	schlafliButtons->ReleaseDeviceDependentResources();

	skipNumberButtons->ReleaseDeviceDependentResources();

	checkBoxQuasiRegular->ReleaseDeviceDependentResources();
}











void HyperbolicJoyceMain::OnDeviceRestored()
{
    // Notifies renderers that device resources may now be recreated.

	m_sceneRenderer->CreateDeviceDependentResources();

	hudZero->CreateDeviceDependentResources();

	schlafliButtons->CreateDeviceDependentResources();

	skipNumberButtons->CreateDeviceDependentResources();

	checkBoxQuasiRegular->CreateDeviceDependentResources();

	CreateWindowSizeDependentResources();
}




