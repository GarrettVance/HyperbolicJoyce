




#include "pch.h"


#include "BitmapUtil.h"






using namespace HvyDXBase;
using namespace DirectX;
using namespace Windows::Foundation;









//      
//              
//              
//                generic method to create D2D1Bitmap from an Image File:
//              
//              




void HvyDXBase::CreateSimpleD2D1BitmapFromImage(
    std::shared_ptr<DX::DeviceResources>const & p_DeviceResources, 
    Microsoft::WRL::ComPtr<ID2D1Bitmap>&     p_comptr_D2D1Bitmap,  // ghv : call by reference or fail and crash;
    std::wstring                             p_file_path 
)
{
    // Create a decoder

    IWICBitmapDecoder *pDecoder = NULL;



    HRESULT hr = p_DeviceResources->GetWicImagingFactory()->CreateDecoderFromFilename(
        //  L"Assets\\a_png_bitmap.png",     // Image to be decoded
        p_file_path.c_str(),             // Image to be decoded
        NULL,                            // Do not prefer a particular vendor
        GENERIC_READ,                    // Desired read access to the file
        WICDecodeMetadataCacheOnDemand,  // Cache metadata when needed
        &pDecoder                        // Pointer to the decoder
    );



    // Retrieve the first frame of the image from the decoder

    IWICBitmapFrameDecode *pFrame = NULL;

    if (SUCCEEDED(hr))
    {
        hr = pDecoder->GetFrame(0, &pFrame);
    }



    //    
    //    ghv : A peek inside of wincodec.h reveals that 
    //    IWICFormatConverter inherits from IWICBitmapSource: 
    //    
    //    
    //    wincodec.h  ===>     IWICFormatConverter : public IWICBitmapSource
    //    
    //    
    //     Here's another revealing quote:   
    //        
    //     The IWICFormatConverter interface represents an IWICBitmapSource that converts 
    //     the image data from one pixel format to another, handling dithering ...etc...
    //                 



    IWICFormatConverter   *gvFConverter = NULL;




    // Format convert the frame to 32bppPBGRA


    if (SUCCEEDED(hr))
    {
        SafeRelease(&gvFConverter);


        hr = p_DeviceResources->GetWicImagingFactory()->CreateFormatConverter(&gvFConverter);
    }


    if (SUCCEEDED(hr))
    {
        hr = gvFConverter->Initialize(
            pFrame,                          // Input bitmap to convert
            GUID_WICPixelFormat32bppPBGRA,   // Destination pixel format
            WICBitmapDitherTypeNone,         // Specified dither pattern
            NULL,                            // Specify a particular palette 
            0.f,                             // Alpha threshold
            WICBitmapPaletteTypeCustom       // Palette translation type
        );
    }


    // d2d_context


    hr = p_DeviceResources->GetD2DDeviceContext()->CreateBitmapFromWicBitmap(
        gvFConverter,
        p_comptr_D2D1Bitmap.GetAddressOf()
    );
}
//  Closes HvyDXBase::CreateD2D1BitmapFromImage();












