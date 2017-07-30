#define D3D_DEBUG_INFO
#define WIN32_LEAN_AND_MEAN
#define STRICT
#include <d3d9.h>
#include <dinput.h>
#include <vector>
#include "naves.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Explosion.h"


LPDIRECT3D9             g_pD3D       = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB        = NULL; // Buffer to hold dxtest
LPDIRECTINPUT8 din;    // the pointer to our DirectInput interface
LPDIRECTINPUTDEVICE8 dinkeyboard;    // the pointer to the keyboard device

struct VERTEX
{
    float x, y, z;
	float rhw;
    DWORD colour;
};

struct MYPOINT { float x, y; };

struct MYLINE { float x1, y1, x2, y2; };


#define D3DFVF_VERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE)

std::vector<CEnemy*> vEnemies;
std::vector<CProjectile*> vProjectiles;
std::vector<CExplosion*> vExplosions;

CMe Me;


void Render()
{
	g_pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0 );	

    if( SUCCEEDED( g_pd3dDevice->BeginScene() ) )
    {
		g_pd3dDevice->SetStreamSource( 0, g_pVB, 0, sizeof(VERTEX) );
		g_pd3dDevice->SetFVF( D3DFVF_VERTEX );
		
		//DRAW ENEMIES
		for(UINT i = 0; i < vEnemies.size(); i++)
		{
			VERTEX triangle[] = 
			{
				{ vEnemies[i]->PosX,                        vEnemies[i]->PosY,                       0.0f, 0.0f, 0xFFFF0000 },
				{ vEnemies[i]->PosX-(vEnemies[i]->Width/2), vEnemies[i]->PosY - vEnemies[i]->Height, 0.0f, 0.0f, 0xFFFFFFFF },
				{ vEnemies[i]->PosX+(vEnemies[i]->Width/2), vEnemies[i]->PosY - vEnemies[i]->Height, 0.0f, 0.0f, 0xFFFFCCCC },
			};
			VOID* ptriangle;
			g_pVB->Lock( 0, sizeof(triangle), (void**)&ptriangle, 0 );
			memcpy( ptriangle, triangle, sizeof(triangle) );
			g_pVB->Unlock();
			g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
		}

		//DRAW PROJECTILES
		for(UINT i = 0; i < vProjectiles.size(); i++)
		{
			VERTEX triangle[] = 
			{
				{ vProjectiles[i]->PosX-(vProjectiles[i]->Width/2), vProjectiles[i]->PosY + vProjectiles[i]->Height, 0.0f, 0.0f, 0xFFFFCC55 },
				{ vProjectiles[i]->PosX,                            vProjectiles[i]->PosY,                           0.0f, 0.0f, 0xFFFFCC55 },
				{ vProjectiles[i]->PosX+(vProjectiles[i]->Width/2), vProjectiles[i]->PosY + vProjectiles[i]->Height, 0.0f, 0.0f, 0xFFFFCC55 },
			};
			VOID* ptriangle;
			g_pVB->Lock( 0, sizeof(triangle), (void**)&ptriangle, 0 );
			memcpy( ptriangle, triangle, sizeof(triangle) );
			g_pVB->Unlock();
			g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
		}

		//DRAW MOTHERSHIP
		VERTEX triangle[] = 
		{ 
			{ Me.PosX,              Me.PosY,             0.0f, 0.0f, 0xFF999999 },
			{ Me.PosX+(Me.Width/2), Me.PosY + Me.Height, 0.0f, 0.0f, 0xFFAAAAAA },
			{ Me.PosX-(Me.Width/2), Me.PosY + Me.Height, 0.0f, 0.0f, 0xFFFFFFFF },

		};
		VOID* ptriangle;
		g_pVB->Lock( 0, sizeof(triangle), (void**)&ptriangle, 0 );
		memcpy( ptriangle, triangle, sizeof(triangle) );
		g_pVB->Unlock();
		g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );

		//DRAW EXPLOSIONS
		for(UINT i = 0; i < vExplosions.size(); i++)
		{
			VERTEX triangle[] = 
			{
				{ vExplosions[i]->PosX,                        vExplosions[i]->PosY,                       0.0f, 0.0f, 0xFFFFCC55 },
				{ vExplosions[i]->PosX-(vExplosions[i]->Width/2), vExplosions[i]->PosY - vExplosions[i]->Height, 0.0f, 0.0f, 0xFFFFCC55 },
				{ vExplosions[i]->PosX+(vExplosions[i]->Width/2), vExplosions[i]->PosY - vExplosions[i]->Height, 0.0f, 0.0f, 0xFFFFCC55 },
			};
			VOID* ptriangle;
			g_pVB->Lock( 0, sizeof(triangle), (void**)&ptriangle, 0 );
			memcpy( ptriangle, triangle, sizeof(triangle) );
			g_pVB->Unlock();
			g_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, 1 );
		}
		
		g_pd3dDevice->EndScene();
    }

    g_pd3dDevice->Present( NULL, NULL, NULL, NULL );
}

void GameProcess()
{
	static unsigned int maxEnemies = 10;
	static unsigned int maxProjectiles = 5;
    static BYTE keystate[256];
	static long lastshot;

    dinkeyboard->GetDeviceState(256, (LPVOID)keystate);

	if(keystate[DIK_LEFTARROW] & 0x80)
		Me.PosX -= 15;

	if(keystate[DIK_RIGHTARROW] & 0x80)
		Me.PosX += 15;

	if(keystate[DIK_LCONTROL] & 0x80 && (GetTickCount() - lastshot > 100))
	{
		if(vProjectiles.size() < maxProjectiles)
		{
			vProjectiles.push_back( new CProjectile(Me.PosX, Me.PosY) );
			lastshot = GetTickCount();
		}
	}

	if( vEnemies.size() < maxEnemies )
		if( rand() % 10 == 0 )
			vEnemies.push_back( new CEnemy() );

	for(UINT i = 0; i < vEnemies.size(); i++)
	{
		vEnemies[i]->Move();
		if( vEnemies[i]->PosY > SCREENY )
			vEnemies.erase( vEnemies.begin()+i );
	}

	for(UINT i = 0; i < vProjectiles.size(); i++)
	{
		vProjectiles[i]->Move();
		if( vProjectiles[i]->PosY < 1 )
			vProjectiles.erase( vProjectiles.begin()+i );
	}

	for( UINT i = 0; i < vExplosions.size(); i++ )
	{
		if(  GetTickCount() - vExplosions[i]->TimeStarted > 1000)
			vExplosions.erase( vExplosions.begin()+i );
	}

	//CHECK COLLISIONS
	static const MYPOINT pointOff = { SCREENX + 1, SCREENY + 1 };
	UINT nInter = 0;
	for(UINT i = 0; i < vEnemies.size(); i++)
	{


		if( Me.PosY < vEnemies[i]->PosY-10 &&
			Me.PosX > vEnemies[i]->PosX-vEnemies[i]->Width/2 &&
			Me.PosX < vEnemies[i]->PosX+vEnemies[i]->Width/2 )
		{
			vExplosions.push_back( new CExplosion( vEnemies[i]->PosX, vEnemies[i]->PosY ) );
			vEnemies.erase(vEnemies.begin()+i);
			break;
		}

		for(UINT j = 0; j < vProjectiles.size(); j++)
		{
			if( vProjectiles[j]->PosY < vEnemies[i]->PosY-10 &&
				vProjectiles[j]->PosX > vEnemies[i]->PosX-vEnemies[i]->Width/2 &&
				vProjectiles[j]->PosX < vEnemies[i]->PosX+vEnemies[i]->Width/2 )
			{
				vExplosions.push_back( new CExplosion( vEnemies[i]->PosX, vEnemies[i]->PosY ) );
				vEnemies.erase(vEnemies.begin()+i);
				break;
			}
		}

					
	}


}

HRESULT InitD3D( HWND hWnd )
{

	//find a better place?
	srand(GetTickCount());
	
	// Create the D3D object.
    if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

    // Create the D3DDevice
    if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_SOFTWARE_VERTEXPROCESSING,
                                      &d3dpp, &g_pd3dDevice ) ) )
    {
        return E_FAIL;
    }
    if( FAILED( g_pd3dDevice->CreateVertexBuffer( 300*sizeof(VERTEX),
                                                  0, D3DFVF_VERTEX,
                                                  D3DPOOL_DEFAULT, &g_pVB, NULL ) ) )
    {
        return E_FAIL;
    }
    return S_OK;

}

HRESULT InitDInput(HINSTANCE hInst, HWND hWnd)
{
	DirectInput8Create( hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&din, NULL );
	din->CreateDevice( GUID_SysKeyboard, &dinkeyboard, NULL );
	dinkeyboard->SetDataFormat(&c_dfDIKeyboard);
    dinkeyboard->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	dinkeyboard->Acquire();
	return(S_OK);
}

void Cleanup()
{
    if( g_pVB != NULL )        
        g_pVB->Release();
	
    if( g_pd3dDevice != NULL ) 
        g_pd3dDevice->Release();

    if( g_pD3D != NULL )       
        g_pD3D->Release();
}

LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
    switch( msg )
    {
        case WM_DESTROY:
            Cleanup();
            PostQuitMessage( 0 );
            return 0;
    }

    return DefWindowProc( hWnd, msg, wParam, lParam );
}

INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
    WNDCLASSEX wc = { sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L,
                      GetModuleHandle(NULL), NULL, NULL, NULL, NULL,
                      L"Naves", NULL };
    RegisterClassEx( &wc );
    HWND hWnd = CreateWindow( L"Naves", L"Naves Game",
                              WS_OVERLAPPEDWINDOW, 100, 100, SCREENX, SCREENY,
                              NULL, NULL, wc.hInstance, NULL );

    if( SUCCEEDED( InitD3D( hWnd ) ) && SUCCEEDED( InitDInput( hInst, hWnd ) ) )
    {
        ShowWindow( hWnd, SW_SHOWDEFAULT );
        UpdateWindow( hWnd );

        MSG msg;
        ZeroMemory( &msg, sizeof(msg) );
        while( msg.message != WM_QUIT )
        {
            if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
            {
                TranslateMessage( &msg );
                DispatchMessage( &msg );
            }
            else
			{
				long start_time = GetTickCount();
				GameProcess();
				Render();
				while( (GetTickCount() - start_time) < 33 )
					Sleep(5);
			}
        }
    }

    UnregisterClass( L"Naves", wc.hInstance );
    return 0;
}