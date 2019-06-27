






#pragma once

#include "..\Common\DeviceResources.h"

#include "DefTypes.h"




namespace HvyDXBase
{

//              
//                generic method to create D2D1Bitmap from an Image File:
//              


void CreateSimpleD2D1BitmapFromImage(
    std::shared_ptr<DX::DeviceResources>const & p_DeviceResources, 
    Microsoft::WRL::ComPtr<ID2D1Bitmap>&     p_comptr_D2D1Bitmap,  // ghv : call by reference or fail and crash;
    std::wstring                             p_file_path 
);




}
//  Closes namespace HvyDXBase; 








