//                      
//                      
//      ghv : refer to Joyce Java file PoincareDisk.java
//      ================================================
//  
//   
//                      


#include "pch.h"

#include "JPoincareDisk.h"



using namespace DJJ;
using namespace DirectX;
using namespace Windows::Foundation;



std::random_device                          DJJ::JPoincareDisk::e_rng_seed;
std::mt19937                                DJJ::JPoincareDisk::e_rng_gen(DJJ::JPoincareDisk::e_rng_seed());
std::uniform_real_distribution<float>       DJJ::JPoincareDisk::e_rng_dist(0.0f, 1.0f);



bool                    DJJ::JPoincareDisk::jj_AlternatingRequested = true;  // typically = false;

bool                    jj_IsForColoringBook = false;






        
DJJ::JPoincareDisk::JPoincareDisk(
    const std::shared_ptr<DX::DeviceResources> & p_DeviceResources,
    const std::shared_ptr<HvyDXBase::HvySchlafliButtons> & p_schlafliButtons,
    const std::shared_ptr<HvyDXBase::HvySkipNumButtons> & p_skipNumButtons, 
    const std::shared_ptr<HvyDXBase::HvyCheckBoxQuasiRegular> & p_checkBoxQuasiRegular
) : 
    jj_AlternatingGranted(false),

    jj_quasiRegular(true),   // sending "true" uses reflection through a point mirror;

    jj_totalPolygons(0), 

    jj_innerPolygons(0),

    jj_DeviceResources(p_DeviceResources), 

    jj_schlafliRadioButtons(p_schlafliButtons),
    jj_skipNumberRadioButtons(p_skipNumButtons),
    jj_checkBoxQuasiRegular(p_checkBoxQuasiRegular)
{


    jj_schlafliCurrentButton = jj_schlafliRadioButtons->GetDefaultButton();

    jj_skipNumberCurrentButton = jj_skipNumberRadioButtons->GetDefaultButton();

    jj_quasiRegularCheckBoxState = jj_checkBoxQuasiRegular->GetDefaultState();



    HRESULT hr_brush = jj_DeviceResources->GetD2DDeviceContext()->CreateSolidColorBrush(
        D2D1::ColorF(D2D1::ColorF::Blue),
        jj_SolidColorBrush.GetAddressOf()
    );
}







std::wstring DJJ::JPoincareDisk::getTilingDescription()
{
    std::wstring wstrDescriptive = L"Schlafli {";
    wstrDescriptive += std::to_wstring(jj_SchlafliN);
    wstrDescriptive += L", ";
    wstrDescriptive += std::to_wstring(jj_SchlafliK);
    wstrDescriptive += L"} ";

    if (jj_quasiRegular)
    {
        wstrDescriptive += L"Quasi-regular "; 
    }
    else
    {
        wstrDescriptive += L"Regular "; 
    }

    wstrDescriptive += L"Hyperbolic Tiling of the Poincare Disk ";

    return wstrDescriptive;
}







void DJJ::JPoincareDisk::init()
{
    //  Use Schlafli {5,6} or {7,3} or {7,4} or {3, 7}...
    //   
    //  Note that Schlafli {3, 7} depends upon STRAIGHT GEODESICS!!! 
    //            


    if (jj_schlafliCurrentButton == 0)
    {
        jj_SchlafliN = 7;
        jj_SchlafliK = 4;
        jj_Layers = 2;
    }
    else if (jj_schlafliCurrentButton == 1)
    {
        jj_SchlafliN = 7;
        jj_SchlafliK = 3;
        jj_Layers = 3;
    }
    else if (jj_schlafliCurrentButton == 2)
    {
        jj_SchlafliN = 5;
        jj_SchlafliK = 6;
        jj_Layers = 2;
    }
    else if (jj_schlafliCurrentButton == 3)
    {
        jj_SchlafliN = 3;
        jj_SchlafliK = 7;
        jj_Layers = 3;
    }
    else
    {
        throw;
    }


    jj_skipMAXIMUM =  (uint32_t)floor((jj_SchlafliN - 1) / 2.0f);


    //   Send value of jj_skipMAXIMUM to the skipNumber radio buttons object: 

    jj_skipNumberRadioButtons->SetSkipNumberMaximum(jj_skipMAXIMUM);



    if (jj_skipNumberCurrentButton == 0)
    {
        jj_skipNumber = 1;
    }
    else if (jj_skipNumberCurrentButton == 1)
    {
        jj_skipNumber = 2;
    }
    else if (jj_skipNumberCurrentButton == 2)
    {
        jj_skipNumber = 3;
    }
    else
    {
        throw;
    }

    assert(jj_skipNumber <= jj_skipMAXIMUM);




    jj_quasiRegular = jj_quasiRegularCheckBoxState;



    jj_AlternatingGranted = jj_AlternatingRequested && (DJJ::JPoincareDisk::jj_SchlafliK % 2 == 0);


    countPolygons(jj_Layers);


    this->determinePolygons();
}














void        DJJ::JPoincareDisk::countPolygons(int p_num_layers)
{
    //  Determine:
    //          
    //      jj_totalPolygons:  the number of polygons there are through that many layers.
    //      
    //      jj_innerPolygons:  the number through one less layer.
    //      


    jj_totalPolygons = 1;    // count the central polygon
    jj_innerPolygons = 0;


    int a = jj_SchlafliN * (jj_SchlafliK - 3);  // polygons in first layer joined by a vertex

    int b = jj_SchlafliN;        // polygons in first layer joined by an edge


    int next_a = 0; 
    int next_b = 0;


    if (jj_SchlafliK == 3)
    {
        for (int l = 1; l <= p_num_layers; ++l) 
        {
            jj_innerPolygons = jj_totalPolygons;

            next_a = a + b;


            next_b = (jj_SchlafliN - 6) * a + (jj_SchlafliN - 5) * b;


            jj_totalPolygons += a + b;

            a = next_a;
            b = next_b;

        } // for
    }
    else
    { 
        // k >= 4

        for (int l = 1; l <= p_num_layers; ++l) 
        {
            jj_innerPolygons = jj_totalPolygons;


            next_a = 
                ((jj_SchlafliN - 2) * (jj_SchlafliK - 3) - 1) * a + 
                ((jj_SchlafliN - 3) * (jj_SchlafliK - 3) - 1) * b;


            next_b = (jj_SchlafliN - 2) * a + (jj_SchlafliN - 3) * b;


            jj_totalPolygons += a + b;

            a = next_a;
            b = next_b;

        } // for


    } // if/else
}
// countPolygons








 /* rule codes
  *   0:  initial polygon.  Needs neighbors on all n sides
  *   1:  polygon already has 2 neighbors, but one less around corner needed
  *   2:  polygon already has 2 neighbors
  *   3:  polygon already has 3 neighbors
  *   4:  polygon already has 4 neighbors
  */







void        DJJ::JPoincareDisk::determinePolygons()
{
    jj_NGons.clear();  // TODO 
    DJJ::JPolygon nullPolygon{ jj_SchlafliN };
    jj_NGons.assign(jj_totalPolygons, nullPolygon);

    
    jj_Rules.clear();
    jj_Rules.assign(jj_totalPolygons, 0);


    jj_ColorVector.clear();
    jj_ColorVector.assign(jj_totalPolygons, D2D1::ColorF(0.2f, 0.2f, 0.2f, 1.f));



    jj_NGons[0] = DJJ::JPolygon::constructCenterPolygon(jj_SchlafliN, jj_SchlafliK, jj_quasiRegular);

    jj_Rules[0] = 0;

    jj_ColorVector[0] = randomColor();


    int j = 1; // index of the next polygon to create

    for (int i = 0; i < jj_innerPolygons; ++i)
    {
        j = applyRule(i, j);
    }
}











bool    gv_ColorsAreTheSame(D2D1::ColorF const& gaw, D2D1::ColorF const& kaw)
{
    double const epsilon = 1.0e-5;


    return
        (std::abs(gaw.r - kaw.r) < epsilon) &&
        (std::abs(gaw.g - kaw.g) < epsilon) &&
        (std::abs(gaw.b - kaw.b) < epsilon) &&
        (std::abs(gaw.a - kaw.a) < epsilon);
}








/* rule codes
 *   0:  initial polygon.  Needs neighbors on all n sides
 *   1:  polygon already has 2 neighbors, but one less around corner needed
 *   2:  polygon already has 2 neighbors
 *   3:  polygon already has 3 neighbors
 *   4:  polygon already has 4 neighbors
 */




int         DJJ::JPoincareDisk::applyRule(int i, int j)
{
    int r = jj_Rules[i];


    bool special = (r == 1);


    if (special)
    {
        r = 2;
    }


    int start = (r == 4) ? 3 : 2;


    int quantity = (jj_SchlafliK == 3 && r != 0) ? jj_SchlafliN - r - 1 : jj_SchlafliN - r;





    for (int s = start; s < start + quantity; ++s) 
    {
        // Create a polygon adjacent to jj_NGons[i]:  


        jj_NGons[j] = createNextPolygon(jj_NGons[i],   s % jj_SchlafliN);  



        jj_Rules[j] = (jj_SchlafliK == 3 && s == start && r != 0) ? 4 : 3;




        if (jj_AlternatingGranted && j > 1)
        {
            //   When jj_AlternatingGranted is true, the RGB color values 
            //   inside the color vector look like, 
            //   e.g.  Red ---> Yellow ---> Red ---> Yellow ---> Red ...etc...


            if (gv_ColorsAreTheSame(jj_ColorVector[i], jj_ColorVector[0]))
            {
                jj_ColorVector[j] = jj_ColorVector[1];
            }
            else
            {
                jj_ColorVector[j] = jj_ColorVector[0];
            }
        }
        else
        {
            jj_ColorVector[j] = randomColor();
        }



        j++;


        uint32_t  m;


        if (special) 
        { 
            m = 2; 
        }
        else if (s == 2 && r != 0) 
        { 
            m = 1; 
        }
        else 
        {
            m = 0;
        }



        for (; m < jj_SchlafliK - 3; ++m) 
        {

            // Create a polygon adjacent to jj_NGons[j-1]:  


            jj_NGons[j] = createNextPolygon(jj_NGons[j - 1],  1);   


            jj_Rules[j] = (jj_SchlafliN == 3 && m == jj_SchlafliK - 4) ? 1 : 2;  


            if (jj_AlternatingGranted)
            {
                if (gv_ColorsAreTheSame(jj_ColorVector[j - 1], jj_ColorVector[0]))
                {
                    jj_ColorVector[j] = jj_ColorVector[1];
                }
                else
                {
                    jj_ColorVector[j] = jj_ColorVector[0];
                }
            }
            else
            {
                jj_ColorVector[j] = randomColor();
            }

            j++;

        } // for m


    } // for r

    return j;
} 
// applyRule







JPolygon        DJJ::JPoincareDisk::createNextPolygon(JPolygon& srcGon, int s)
{
    //              
    //  reflect polygon "srcGon" through either the POINT or the EDGE 
    //  which is indicated by the side s
    //  to produce the resulting polygon Q


    DJJ::JPolygon Q{ srcGon.jpolyTaxonomy };   // braced initialization;



    if (jj_quasiRegular)
    {
        DJJ::HvyPlex   mirrorPoint = srcGon.getVertex(s);  //  quasiRegular uses reflection in a point;

        for (uint32_t i = 0; i < jj_SchlafliN; ++i) 
        {
            // reflect P[i] thru P[s] to get Q[j]

            int j = (jj_SchlafliN + i - s) % jj_SchlafliN; 

            Q.setVertex(
                j,
                DJJ::ReflectPointInPoint(srcGon.getVertex(i), mirrorPoint)
            );
        }

    }
    else
    {
        // regular tiling: 

        DJJ::JLine   C(srcGon.getVertex(s), srcGon.getVertex((s + 1) % jj_SchlafliN));


        for (uint32_t i = 0; i < jj_SchlafliN; ++i)
        {
            // reflect P[i] thru C to get Q[j]}

            int j = (jj_SchlafliN + s - i + 1) % jj_SchlafliN;


            Q.setVertex(j, C.ReflectPointInLine(srcGon.getVertex(i)));
        }
        // for

    }
    // if/else

    return Q;
}
// computeNextPolygon







int gcd(int m, int n)
{
    // greatest common divisor

    if (m < 0) m = -m;   // Make sure m and n

    if (n < 0) n = -n;  // are nonnegative.

    if (m > n)
    {
        // Make sure m <= n. }

        int temp = n;
        n = m;
        m = temp;
    }

    while (m != 0)
    {
        int temp = n;
        n = m;
        m = temp % m;
    }
    return n;
}
// gcd








void        DJJ::JPoincareDisk::updateLikeJoyceJavaUpdate()
{
    //   On every Update() call, need to check for any change of the 
    //   user interface controls, e.g. the Radio Buttons HUD: 


    uint32_t usersNewSchlafli = jj_schlafliRadioButtons->GetChosenButton();


    if (jj_schlafliCurrentButton != usersNewSchlafli)
    {
        jj_schlafliCurrentButton = usersNewSchlafli;

        jj_skipNumber = 1;
            
        jj_skipNumberCurrentButton = 0;

        jj_skipNumberRadioButtons->SetSelectedButton(0);

        //  Must call the init() method: 

        this->init();
    }
    else
    {
        //  Test for the case that the user changed value of skipNumber but didn't change the Schlafli symbol: 

        uint32_t usersNewButton = jj_skipNumberRadioButtons->GetChosenButton();

        if (jj_skipNumberCurrentButton != usersNewButton)
        {
            jj_skipNumberCurrentButton = usersNewButton;

            this->init();
        }
        else
        {
            //  Test for the case that the user changed the checkBoxQuasiRegular: 

            bool usersNewCheckBoxQuasiRegularState = jj_checkBoxQuasiRegular->GetCheckedState();

            if (jj_quasiRegularCheckBoxState != usersNewCheckBoxQuasiRegularState)
            {
                jj_quasiRegularCheckBoxState = usersNewCheckBoxQuasiRegularState;
            
                this->init();
            }

        }
    }

    RenderSomeStars();


    //  RenderWithoutStars();
}







void DJJ::JPoincareDisk::RenderSomeStars()
{
    Windows::Foundation::Size   rtarget_pixels = jj_DeviceResources->GetOutputSize();

    uint32_t numStars = gcd(jj_skipNumber, jj_SchlafliN);
    uint32_t pointsPerStar = jj_SchlafliN / numStars;


    for (uint32_t idx_ngon = 0; idx_ngon < jj_totalPolygons; idx_ngon++)
    {
        jj_SolidColorBrush->SetColor(jj_ColorVector.at(idx_ngon)); 

        for (uint32_t s = 0; s < numStars; ++s) 
        {
            HRESULT hr1 = jj_DeviceResources->GetD2DFactory()->CreatePathGeometry(jj_PolygonPathGeometry.GetAddressOf());
            assert(SUCCEEDED(hr1));

            ID2D1GeometrySink       *geometrySink = NULL;

            HRESULT hr2 = jj_PolygonPathGeometry->Open(&geometrySink);
            assert(SUCCEEDED(hr2));


            DJJ::JPolygon Q{ pointsPerStar };  // braced initialization;


            for (uint32_t j = 0; j < pointsPerStar; ++j) 
            {

                uint32_t kdA = j * jj_skipNumber % jj_SchlafliN + s;

                uint32_t kdB = j * jj_skipNumber % pointsPerStar + s;

                assert(kdA == kdB);

                Q.setVertex(j, jj_NGons.at(idx_ngon).getVertex(kdA));
            }
       
            Q.constructEdges();

            Q.GeometryUseNativeArcs(geometrySink, rtarget_pixels);

            geometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);
            HRESULT hr3 = geometrySink->Close();
            SafeRelease(&geometrySink);

            if (!jj_IsForColoringBook)  //  Must evaluate to false for Mom's Coloring Book;
            {
                jj_DeviceResources->GetD2DDeviceContext()->FillGeometry(
                    jj_PolygonPathGeometry.Get(), 
                    jj_SolidColorBrush.Get()
                );
            }

            jj_SolidColorBrush->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
            jj_DeviceResources->GetD2DDeviceContext()->DrawGeometry( jj_PolygonPathGeometry.Get(), jj_SolidColorBrush.Get(),  1.2f );
            jj_SolidColorBrush->SetColor(jj_ColorVector.at(idx_ngon)); 

            jj_PolygonPathGeometry.Reset();
        } // for s
    }
}






void DJJ::JPoincareDisk::RenderWithoutStars()
{
    Windows::Foundation::Size   rtarget_pixels = jj_DeviceResources->GetOutputSize();

    for (uint32_t idx_ngon = 0; idx_ngon < jj_totalPolygons; idx_ngon++)
    {
        jj_SolidColorBrush->SetColor(jj_ColorVector.at(idx_ngon)); 

        HRESULT hr1 = jj_DeviceResources->GetD2DFactory()->CreatePathGeometry(jj_PolygonPathGeometry.GetAddressOf());
        assert(SUCCEEDED(hr1));
        ID2D1GeometrySink       *geometrySink = NULL;
        HRESULT hr2 = jj_PolygonPathGeometry->Open(&geometrySink);
        assert(SUCCEEDED(hr2));

        jj_NGons.at(idx_ngon).constructEdges();

        jj_NGons.at(idx_ngon).GeometryUseNativeArcs(geometrySink, rtarget_pixels);

        geometrySink->EndFigure(D2D1_FIGURE_END_CLOSED);
        HRESULT hr3 = geometrySink->Close();
        SafeRelease(&geometrySink);
        jj_DeviceResources->GetD2DDeviceContext()->FillGeometry( jj_PolygonPathGeometry.Get(), jj_SolidColorBrush.Get() );
        jj_PolygonPathGeometry.Reset();
    }
}









D2D1::ColorF DJJ::JPoincareDisk::randomColor()
{
    float part_red = e_rng_dist(e_rng_gen); 

    float part_blue = e_rng_dist(e_rng_gen); 

    float part_green = e_rng_dist(e_rng_gen); 

    return D2D1::ColorF(part_red, part_green, part_blue, 1.0f);
}





