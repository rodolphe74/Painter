// Projection.cpp : Définit le point d'entrée de l'application.
//

#include "Projection.h"
#include "Chronometer.h"
#include "Painter.h"
#include "framework.h"
#include "string"
#include <gdiplus.h>
#include <objidl.h>
#include <windowsx.h>
using namespace Gdiplus;
// #pragma comment(lib, "Gdiplus.lib")

#define MAX_LOADSTRING 100
#define W 800
#define H 800

#define MAX_LOADSTRING 100

// Variables globales :
HINSTANCE hInst;                     // instance actuelle
WCHAR szTitle[MAX_LOADSTRING];       // Texte de la barre de titre
WCHAR szWindowClass[MAX_LOADSTRING]; // nom de la classe de fenêtre principale

// Déclarations anticipées des fonctions incluses dans ce module de code :
ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK About(HWND, UINT, WPARAM, LPARAM);

// User global objects
Painter painter;
RECT winRect;
bool leftMouseButton = false;
int lastMouseX = 0;
int lastMouseY = 0;
static bool firstMouseCheck = false;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                      _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine,
                      _In_ int nCmdShow) {
  UNREFERENCED_PARAMETER(hPrevInstance);
  UNREFERENCED_PARAMETER(lpCmdLine);

  // TODO: Placez le code ici.
  // Initialize GDI+.
  GdiplusStartupInput gdiplusStartupInput;
  ULONG_PTR gdiplusToken;
  GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
  painter.init();

  // Initialise les chaînes globales
  LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
  LoadStringW(hInstance, IDC_PROJECTION, szWindowClass, MAX_LOADSTRING);
  MyRegisterClass(hInstance);

  // Effectue l'initialisation de l'application :
  if (!InitInstance(hInstance, nCmdShow)) {
    return FALSE;
  }

  HACCEL hAccelTable =
      LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECTION));

  MSG msg;

  // Boucle de messages principale :
  while (GetMessage(&msg, nullptr, 0, 0)) {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
    }
  }

  painter.close();
  GdiplusShutdown(gdiplusToken);
  return (int)msg.wParam;
}

//
//  FONCTION : MyRegisterClass()
//
//  OBJECTIF : Inscrit la classe de fenêtre.
//
ATOM MyRegisterClass(HINSTANCE hInstance) {
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hInstance;
  wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECTION));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_PROJECTION);
  wcex.lpszClassName = szWindowClass;
  wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

  return RegisterClassExW(&wcex);
}

//
//   FONCTION : InitInstance(HINSTANCE, int)
//
//   OBJECTIF : enregistre le handle d'instance et crée une fenêtre principale
//
//   COMMENTAIRES :
//
//        Dans cette fonction, nous enregistrons le handle de l'instance dans
//        une variable globale, puis nous créons et affichons la fenêtre
//        principale du programme.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
  hInst = hInstance; // Stocke le handle d'instance dans la variable globale

  HWND hWnd =
      CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT,
                    0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

  if (!hWnd) {
    return FALSE;
  }

  SetWindowPos(hWnd, nullptr, 100, 100, 800, 800, 0);
  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);

  winRect.left = 0;
  winRect.top = 0;
  winRect.right = W;
  winRect.bottom = H;
  painter.w = winRect.right;
  painter.h = winRect.bottom;
  AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
  InvalidateRect(hWnd, 0, TRUE);

  return TRUE;
}

//
//  FONCTION : WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  OBJECTIF : Traite les messages pour la fenêtre principale.
//
//  WM_COMMAND  - traite le menu de l'application
//  WM_PAINT    - Dessine la fenêtre principale
//  WM_DESTROY  - génère un message d'arrêt et retourne
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {
  case WM_COMMAND: {
    int wmId = LOWORD(wParam);
    // Analyse les sélections de menu :
    switch (wmId) {
    case IDM_ABOUT:
      DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
      break;
    case IDM_EXIT:
      DestroyWindow(hWnd);
      break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
    }
  } break;
  case WM_PAINT: {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    // TODO: Ajoutez ici le code de dessin qui utilise hdc...
    painter.paint(hdc);
    EndPaint(hWnd, &ps);
  } break;

  case WM_MOUSEMOVE: {
    int xPos = GET_X_LPARAM(lParam);
    int yPos = GET_Y_LPARAM(lParam);
    if (leftMouseButton) {
      if (!firstMouseCheck) {
        lastMouseX = xPos;
        firstMouseCheck = true;
      }
      // std::string s = std::to_string(xPos) + " - " +
      // std::to_string(lastMouseX); Chronometer::write(s);

      if (std::abs(xPos - lastMouseX) > 50) {
        int p = (int)(xPos - lastMouseX);
        // std::string s = std::to_string(p);
        // Chronometer::write(s);
        //  painter.rotateY(p < 0 ? 10 : -10);
        painter.rotateObjectY(*painter.object, p < 0 ? 10 : -10);
        InvalidateRect(hWnd, 0, TRUE);
      }
    } else {
      lastMouseX = xPos;
    }

  } break;

  case WM_LBUTTONDOWN: {
    leftMouseButton = true;
  } break;
  case WM_LBUTTONUP: {
    leftMouseButton = false;
  } break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  default:
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

// Gestionnaire de messages pour la boîte de dialogue À propos de.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
  UNREFERENCED_PARAMETER(lParam);
  switch (message) {
  case WM_INITDIALOG:
    return (INT_PTR)TRUE;

  case WM_COMMAND:
    if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
      EndDialog(hDlg, LOWORD(wParam));
      return (INT_PTR)TRUE;
    }
    break;
  }
  return (INT_PTR)FALSE;
}
