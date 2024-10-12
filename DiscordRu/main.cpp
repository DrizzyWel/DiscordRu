#include <Windows.h>
#include <iostream>
#include <d3d9.h>
using namespace std;
#pragma comment(linker, "/entry:WinMainCRTStartup")
#pragma comment(lib,"d3d9.lib")
struct CUSTOMVERTEX
{
	float X, Y, Z; // оординаты в трехмерном пространстве
	float rhw;//параметр преобразовани€ вершины
	DWORD color;//цвет
	 
};
#define D3DFVF_CUSTOMVERTEX ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE )
LPDIRECT3D9	pDirect3D = NULL;
LPDIRECT3DDEVICE9 pDirect3DDevice = NULL;
LPDIRECT3DVERTEXBUFFER9 pBufferVershin = NULL;


HRESULT InitialDirect3D(HWND hWnd)
{
	pDirect3D = Direct3DCreate9(D3D_SDK_VERSION); //создаем директ3дкриате
	if (pDirect3D == NULL)
	{
		return E_FAIL;
	}
	D3DDISPLAYMODE Display;
	if (FAILED(pDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &Display)))
	{
		return E_FAIL;
	}
	D3DPRESENT_PARAMETERS Direct3DParametr;
	ZeroMemory(&Direct3DParametr, sizeof(Direct3DParametr));
	Direct3DParametr.Windowed = TRUE;
	Direct3DParametr.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Direct3DParametr.BackBufferFormat = Display.Format;
if(FAILED(	pDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &Direct3DParametr, &pDirect3DDevice)))
	return E_FAIL;

}
void RenderingDirect3D()
{
	if (pDirect3DDevice == 0)return;
	pDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(50,255, 0, 255), 1.0f, 0);
	pDirect3DDevice->BeginScene();

	pDirect3DDevice->SetStreamSource(0, pBufferVershin, 0, sizeof(CUSTOMVERTEX));
	pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);

	pDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
	pDirect3DDevice->EndScene();
	pDirect3DDevice->Present(NULL, NULL, NULL, NULL);
}
void DeleteDirect3D()
{
	if (pBufferVershin != 0)
		pBufferVershin->Release();
	if (pDirect3DDevice != 0)
		pDirect3DDevice->Release();
	if (pDirect3D != 0)
		pDirect3D->Release(); 
}
HRESULT InitialBufferVershin()
{
	CUSTOMVERTEX Vershin[] =
	{
	{ 300.0f, 300.0f, 0.5f, 1.0f, 0x00000fff },
	 { 150.0f, 300.0f, 0.5f, 1.0f, 0x00000fff },
	{150.0f, 150.0f, 0.5f, 1.0f, 0x00000fff },
	};
	if (FAILED(pDirect3DDevice->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX), 0, D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT, &pBufferVershin, NULL)))
		return E_FAIL;
	VOID* pBV;
	if (FAILED(pBufferVershin->Lock(0, sizeof(Vershin), (void**)&pBV, 0)))
		return E_FAIL;
	memcpy(pBV, Vershin, sizeof(Vershin));
	pBufferVershin->Unlock();
	return S_OK;
}
LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		DeleteDirect3D();
		PostQuitMessage(0);
		break;
	default:return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASS wc{ 0 };
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = "name";
	wc.lpszMenuName = "Fack";
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.style = CS_VREDRAW | CS_HREDRAW;
	if (!RegisterClass(&wc))
	{
		return -5;
	}
	HWND hwnd = CreateWindow(wc.lpszClassName, wc.lpszMenuName, WS_OVERLAPPEDWINDOW|WS_VISIBLE, 0, 0, 800, 600, 0, 0, hInstance, 0);

	MSG msg{ 0 };
	if (SUCCEEDED(InitialDirect3D(hwnd))) {
		ShowWindow(hwnd, SW_SHOWDEFAULT);
		UpdateWindow(hwnd);
		ZeroMemory(&msg, sizeof(msg));

		while (msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
				RenderingDirect3D();
			InitialBufferVershin();
		}

	}
	return 0;
}
