#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"



#include "Content\Hvy3DScene.h"
#include "Content\Hvy2DHUD.h"
#include "Content\HvySchlafliButtons.h"
#include "Content\HvySkipNumButtons.h"
#include "Content\HvyCheckBoxQuasiRegular.h"





namespace HyperbolicJoyce
{
	class HyperbolicJoyceMain : public DX::IDeviceNotify
	{
	public:
		HyperbolicJoyceMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~HyperbolicJoyceMain();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:

		std::shared_ptr<DX::DeviceResources>                        m_deviceResources;
		DX::StepTimer                                               m_timer;


        std::unique_ptr<DirectX::Mouse>                             e_mouse;
        DirectX::Mouse::ButtonStateTracker                          tracker;


		std::unique_ptr<HvyDXBase::Hvy3DScene>                      m_sceneRenderer;
		std::unique_ptr<HvyDXBase::Hvy2DHUD>                        hudZero;




		std::shared_ptr<HvyDXBase::HvySchlafliButtons>              schlafliButtons;
		std::shared_ptr<HvyDXBase::HvySkipNumButtons>               skipNumberButtons;
		std::shared_ptr<HvyDXBase::HvyCheckBoxQuasiRegular>         checkBoxQuasiRegular;



	};
}
