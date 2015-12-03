#include <Windows.h>  
#include <WindowsX.h>
#include <vector>
#include "resource.h"
#include "parser.h"
#include "MathExpr.h"
#include "shlobj.h"
#include <fstream>
//#include "math.h"

//先声明一下消息处理函数  
LRESULT CALLBACK MyWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);  

HINSTANCE hg_app;
#define POS_WIDTH 100	// 坐标系宽度
HWND hEdit1,hEdit2,hEdit3,hEdit4,hEdit5,hButton_plot,hEditx_1,hEditx_2,hButton_save,hButton_color,hButton_in,hButton_out;
int pflag=0;

PAINTSTRUCT ps;
HDC hdc = NULL;
HDC hMemDC = NULL;
HBITMAP hMemBM = NULL; 
HBRUSH hbrush;
UINT uMessage;
int wScrollNotify;
void CaptureScreen(HWND window, const char* filename);
float zoom_factor=1;

// 入口点  
int CALLBACK WinMain(  
    HINSTANCE hInstance,  
    HINSTANCE hPrvInstance,  
    LPSTR lpCommandLine,  
    int cmdShow)
	{  
    WCHAR* cln = L"MyApp";  
    //设计窗口类  
    WNDCLASS wc = {};  
    wc.hInstance = hInstance;  
    wc.lpszClassName = cln;  
    wc.lpfnWndProc = MyWindowProc;  
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAIN);

    //注册窗口类  
    RegisterClass(&wc);  
    // 创建窗口  
	HWND hMainwind = CreateWindow(cln,L"绘制窗口", WS_OVERLAPPEDWINDOW|WS_BORDER|WS_VSCROLL|WS_HSCROLL,0,0,1300,1000,NULL,NULL,hInstance,NULL);  
   // 显示窗口  
    if(hMainwind == NULL)  
        return 0;  
    ShowWindow(hMainwind,SW_NORMAL);  
    // 消息循环  
    MSG msg;  
    while(GetMessage(&msg,NULL,0,0))  
   {  
        TranslateMessage(&msg);  
        DispatchMessage(&msg);  
    }  
    return 0;  
}  

static int cxClient = 1300,cyClient = 1000;	
HBRUSH bgbrush = CreateSolidBrush(RGB(192, 192, 192)); 
//HBRUSH bgbrush;

void handle111(HWND &hwnd)
{
				static TCHAR buf1[10];
				static TCHAR buf2[10];
				static TCHAR buf3[10];
				static TCHAR buf4[10];
				static TCHAR buf5[10];
				static TCHAR buf6[10];//grid spacing
				static TCHAR buf7[10];//tick distance
				::SendMessage(hEdit1, WM_GETTEXT,10,(LPARAM)buf1);
				::SendMessage(hEdit2, WM_GETTEXT,10,(LPARAM)buf2);
				::SendMessage(hEdit3, WM_GETTEXT,10,(LPARAM)buf3);
				::SendMessage(hEdit4, WM_GETTEXT,10,(LPARAM)buf4);
				::SendMessage(hEdit5, WM_GETTEXT,10,(LPARAM)buf5);
				::SendMessage(hEditx_1 , WM_GETTEXT,10,(LPARAM)buf6);
				::SendMessage(hEditx_2 , WM_GETTEXT,10,(LPARAM)buf7);
				::wsprintf(buf1,L"%s",buf1);
				::wsprintf(buf2,L"%s",buf2);
				::wsprintf(buf3,L"%s",buf3);
				::wsprintf(buf4,L"%s",buf4);
				::wsprintf(buf5,L"%s",buf5);
				::wsprintf(buf6,L"%s",buf6);
				::wsprintf(buf7,L"%s",buf7);
				parser(hMemDC,hwnd,cxClient,cyClient,buf1,buf2,buf3,buf4,buf5,buf6,buf7,zoom_factor,bgbrush);
}


                 // brush handle  
// 窗口消息处理程序  
LRESULT CALLBACK MyWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)  
{  
	static char szFileName[MAX_PATH]="";
	CHOOSECOLOR cc; 
	static HWND hfdlg;
	PIDLIST_ABSOLUTE pIDLIST; //存放函数返回值
	BROWSEINFO brsInfo;
	
	WCHAR fdpath[MAX_PATH];//选择接受目录的字符数组
	WCHAR fullPath[MAX_PATH];//完整路径
	//POINT pt1,pt2;
	SCROLLINFO si;
    switch(msg)  
    {  
	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		return 0;

	case WM_DESTROY:  
		PostQuitMessage(0);//退出程序  
		return 0;  

	case WM_COMMAND:
		{
			
			int wmID = LOWORD(wParam);
			switch(wmID){
			case 111:
				//handle111(hwnd);
				RECT rect;
				GetClientRect(hwnd, &rect);
				InvalidateRect(hwnd, &rect, false);
				break;

			case IDM_SAVE:
				 CaptureScreen(hwnd, "plot.bmp");
				 
			case 222:
				{
				OPENFILENAME ofn;
				//char szFileName[MAX_PATH]="";
				ZeroMemory(&ofn, sizeof(ofn));
				ofn.lStructSize = sizeof(ofn); 
				ofn.hwndOwner = NULL;
				ofn.lpstrFilter = (LPCWSTR)L"Image Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0";
				ofn.lpstrFile = (LPWSTR)szFileName;
				ofn.nMaxFile = MAX_PATH;
				ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
				ofn.lpstrDefExt = (LPCWSTR)L"bmp";
				GetSaveFileName(&ofn);
				char buffer[256];
				wcstombs(buffer,ofn.lpstrFile, 256);
				CaptureScreen(hwnd,buffer);
				MessageBox(hwnd,L"image has been saved.",L"note",MB_OK);
				break;
				}

			case 333:
				CHOOSECOLOR cc;                 // common dialog box structure 
				static COLORREF acrCustClr[16]; // array of custom colors 
				static DWORD rgbCurrent;        // initial color selection
				// Initialize CHOOSECOLOR 
				ZeroMemory(&cc, sizeof(cc));
				cc.lStructSize = sizeof(cc);
				cc.hwndOwner = hwnd;
				cc.lpCustColors = (LPDWORD) acrCustClr;
				cc.rgbResult = rgbCurrent;
				cc.Flags = CC_FULLOPEN | CC_RGBINIT;
				if (ChooseColor(&cc)==TRUE) 
				{
					bgbrush = CreateSolidBrush(cc.rgbResult);
					rgbCurrent = cc.rgbResult; 
					//MessageBox(hwnd,L"image has been saved.",L"note",MB_OK);
				}
				GetClientRect(hwnd,&rect);
				InvalidateRect(hwnd, &rect, FALSE);
				break;
			case 444:
				zoom_factor = zoom_factor*2;
				break;
			case 555:
				zoom_factor = zoom_factor*0.5;
				break;

			case IDM_ABOUT:
				 MessageBox(hwnd,L"About.",L"note",MB_OK);

			case IDM_EXIT:
				DestroyWindow(hwnd);
				
			case IDM_BROWSFOLDER:  
				brsInfo.hwndOwner = hwnd;
				brsInfo.pidlRoot = NULL;
				brsInfo.pszDisplayName = fdpath;
				brsInfo.lpszTitle = L"请选择目录";
				brsInfo.ulFlags  = BIF_RETURNONLYFSDIRS | BIF_DONTGOBELOWDOMAIN | BIF_NEWDIALOGSTYLE;
				brsInfo.lpfn = NULL;
				//初始化com组件
				CoInitialize(NULL);
				pIDLIST = SHBrowseForFolder(&brsInfo);
				if(pIDLIST){
					//得到完整路径
					SHGetPathFromIDList(pIDLIST,fullPath);
					//取出对话框中用来显示结果的控件
					HWND hStatic  = GetDlgItem(hfdlg,IDC_DISPLAY);
					SendMessage(hStatic,WM_SETTEXT,NULL,(LPARAM)fullPath);
				}

			//end of copy BMP
			}
		}
		return 0;

	case WM_PAINT:
		{
			hdc = BeginPaint(hwnd, &ps);
			
			hMemDC = CreateCompatibleDC(hdc);
			hMemBM = CreateCompatibleBitmap(hdc, cxClient, cyClient);

			SelectObject(hMemDC, hMemBM);

			hbrush = CreateSolidBrush(GetSysColor(COLOR_WINDOW));
			RECT rect;
			GetClientRect(hwnd, &rect);
			FillRect(hMemDC, &rect, hbrush);
			DeleteObject(hbrush);

			handle111(hwnd);
			BitBlt(hdc, 0, 0, cxClient, cyClient, hMemDC, 0, 0, SRCCOPY);
			EndPaint(hwnd, &ps);

			return 0;
		}
	case WM_CREATE: 
		hfdlg = CreateDialog(hg_app,MAKEINTRESOURCE(IDD_FORMVIEW),hwnd,NULL);
		hEdit1 = CreateWindow(L"Edit",L"",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON|WS_BORDER,0,200,100,40,hwnd,NULL,hg_app,NULL);
		hEdit2 = CreateWindow(L"Edit",L"",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON|WS_BORDER,0,310,100,40,hwnd,NULL,hg_app,NULL);
		hEdit3 = CreateWindow(L"Edit",L"",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON|WS_BORDER,0,420,100,40,hwnd,NULL,hg_app,NULL);
		hEdit4 = CreateWindow(L"Edit",L"",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON|WS_BORDER,0,530,100,40,hwnd,NULL,hg_app,NULL);
		hEdit5 = CreateWindow(L"Edit",L"",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON|WS_BORDER,0,640,100,40,hwnd,NULL,hg_app,NULL);
		hButton_plot = CreateWindow(L"Button",L"plot",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,0,700,70,30,hwnd,(HMENU)111,hg_app,NULL);
		hButton_save = CreateWindow(L"Button",L"save",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,0,750,70,30,hwnd,(HMENU)222,hg_app,NULL);
		hButton_color = CreateWindow(L"Button",L"bg color",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,0,800,70,30,hwnd,(HMENU)333,hg_app,NULL);
	    hEditx_1 =  CreateWindow(L"Edit",L"",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON|WS_BORDER,10,80,60,20,hwnd,NULL,hg_app,NULL);
		hEditx_2  = CreateWindow(L"Edit",L"",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON|WS_BORDER,10,140,60,20,hwnd,NULL,hg_app,NULL);
		hButton_in = CreateWindow(L"Button",L"IN",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,0,850,30,30,hwnd,(HMENU)444,hg_app,NULL);
		hButton_out = CreateWindow(L"Button",L"OUT",WS_VISIBLE|WS_CHILD|BS_PUSHBUTTON,40,850,30,30,hwnd,(HMENU)555,hg_app,NULL);
		break;

	case WM_MOUSEWHEEL:
		if(HIWORD(wParam)>WHEEL_DELTA){  //scrollDown()
			zoom_factor = zoom_factor*0.5;

		}
		else if(HIWORD(wParam) == WHEEL_DELTA){  //scrollUp()
			zoom_factor = zoom_factor*2;
		}
	default:
		return DefWindowProc(hwnd,msg,wParam,lParam); 
   }  
	
	return 0;
}  

//保存bitmap图片
void CaptureScreen(HWND window, const char* filename)
{
 // get screen rectangle
 RECT windowRect;
 GetWindowRect(window, &windowRect);

 // bitmap dimensions
 int bitmap_dx = windowRect.right - windowRect.left;
 int bitmap_dy = windowRect.bottom - windowRect.top;

 // create file
 ofstream file(filename, ios::binary);
 if(!file) return;

 // save bitmap file headers
 BITMAPFILEHEADER fileHeader;
 BITMAPINFOHEADER infoHeader;

 fileHeader.bfType      = 0x4d42;
 fileHeader.bfSize      = 0;
 fileHeader.bfReserved1 = 0;
 fileHeader.bfReserved2 = 0;
 fileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

 infoHeader.biSize          = sizeof(infoHeader);
 infoHeader.biWidth         = bitmap_dx;
 infoHeader.biHeight        = bitmap_dy;
 infoHeader.biPlanes        = 1;
 infoHeader.biBitCount      = 24;
 infoHeader.biCompression   = BI_RGB;
 infoHeader.biSizeImage     = 0;
 infoHeader.biXPelsPerMeter = 0;
 infoHeader.biYPelsPerMeter = 0;
 infoHeader.biClrUsed       = 0;
 infoHeader.biClrImportant  = 0;

 file.write((char*)&fileHeader, sizeof(fileHeader));
 file.write((char*)&infoHeader, sizeof(infoHeader));

 // dibsection information
 BITMAPINFO info;
 info.bmiHeader = infoHeader; 
 // create a dibsection and blit the window contents to the bitmap
 HDC winDC = GetWindowDC(window);
 HDC memDC = CreateCompatibleDC(winDC);
 BYTE* memory = 0;
 HBITMAP bitmap = CreateDIBSection(winDC, &info, DIB_RGB_COLORS, (void**)&memory, 0, 0);
 SelectObject(memDC, bitmap);
 BitBlt(memDC, 0, 0, bitmap_dx, bitmap_dy, winDC, 0, 0, SRCCOPY);
 DeleteDC(memDC);
 ReleaseDC(window, winDC);

 // save dibsection data
 int bytes = (((24*bitmap_dx + 31) & (~31))/8)*bitmap_dy;
 file.write((char *)memory, bytes);

 DeleteObject(bitmap);
}