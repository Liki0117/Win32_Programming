#include <Windows.h>  
#include <WindowsX.h>
#include <vector>
#include "resource.h"
#include "MathExpr.h"
#include <string>
#include <stdlib.h>
#include <sstream>
#include <math.h>
using namespace std;

#define CPINK RGB(135,38,37)
#define CRED RGB(227,23,13)
#define CBLUE RGB(65,105,225)
#define CYELLOW RGB(237,145,33)
#define CGREEN RGB(46,139,87)

double f(double x)
{
	//输入所求函数并取反
	return -1*(5*x);
}

double g(double x)
{
	//输入所求函数并取反
	return -1*(x*x);
}



TCHAR *chr2wch(const char *buffer) {
	size_t len = strlen(buffer);
	size_t wlen = MultiByteToWideChar(CP_ACP, 0, (const char*)buffer, int(len), NULL, 0);
	wchar_t *wBuf  = new wchar_t[wlen+1];
	MultiByteToWideChar(CP_ACP, 0, (const char*)buffer, int(len), wBuf, int(wlen));
	return wBuf;
}

string TCHAR2STRING(TCHAR *STR)
{
int iLen = WideCharToMultiByte(CP_ACP, 0,STR, -1, NULL, 0, NULL, NULL);
char* chRtn = new char[iLen*sizeof(char)];
WideCharToMultiByte(CP_ACP, 0, STR, -1, chRtn, iLen, NULL, NULL);
std::string str(chRtn);
return str;
}


float process_func(TCHAR buf[],float i){
	string expr = TCHAR2STRING(buf);
	double value;
	if(expr.find("sin")!=(-1)){
		size_t p_sin = expr.find("sin");
		size_t p_l = expr.find_first_of("(");
		size_t p_r = expr.find_last_of(")");
		string content = expr.substr(p_l+1,p_r-p_l-1);
		while((content.find("x"))!=-1){
			size_t pos = content.find("x");
			content.insert(pos,"(");
			content.insert(pos+2,")");
			char *str=new char[];
			sprintf(str,"%f",i);
			content.replace(pos+1,1,str);
		}
		char *cstr = new char[content.length()+1];
		strcpy(cstr,content.c_str());
		double val_content = MathExprParser::parse(cstr);
		value =sin(val_content);
		delete[] cstr;
	}else if(expr.find("cos")!=(-1)){
			size_t p_sin = expr.find("cos");
			size_t p_l = expr.find_first_of("(");
			size_t p_r = expr.find_last_of(")");
			string content = expr.substr(p_l+1,p_r-p_l-1);
			while((content.find("x"))!=-1){
				size_t pos = content.find("x");
				content.insert(pos,"(");
				content.insert(pos+2,")");
				char *str=new char[];
				sprintf(str,"%f",i);
				content.replace(pos+1,1,str);
			}
			char *cstr = new char[content.length()+1];
			strcpy(cstr,content.c_str());
			double val_content = MathExprParser::parse(cstr);
			value =cos(val_content);
			delete[] cstr;
	}else if(expr.find("tan")!=(-1)){
			size_t p_sin = expr.find("tan");
			size_t p_l = expr.find_first_of("(");
			size_t p_r = expr.find_last_of(")");
			string content = expr.substr(p_l+1,p_r-p_l-1);
			while((content.find("x"))!=-1){
				size_t pos = content.find("x");
				content.insert(pos,"(");
				content.insert(pos+2,")");
				char *str=new char[];
				sprintf(str,"%f",i);
				content.replace(pos+1,1,str);
			}
			char *cstr = new char[content.length()+1];
			strcpy(cstr,content.c_str());
			double val_content = MathExprParser::parse(cstr);
			value =tan(val_content);
			delete[] cstr;
	}else if(expr.find("log")!=(-1)){
			size_t p_sin = expr.find("log");
			size_t p_l = expr.find_first_of("(");
			size_t p_r = expr.find_last_of(")");
			string content = expr.substr(p_l+1,p_r-p_l-1);
			while((content.find("x"))!=-1){
				size_t pos = content.find("x");
				content.insert(pos,"(");
				content.insert(pos+2,")");
				char *str=new char[];
				sprintf(str,"%f",i);
				content.replace(pos+1,1,str);
			}
			char *cstr = new char[content.length()+1];
			strcpy(cstr,content.c_str());
			double val_content = MathExprParser::parse(cstr);
			value =log(val_content);
			delete[] cstr;
	}else{
		while((expr.find("x"))!=-1){
			size_t pos = expr.find("x");
			expr.insert(pos,"(");
			expr.insert(pos+2,")");
			char *str=new char[];
			sprintf(str,"%f",i);
			expr.replace(pos+1,1,str);
		}
		char *cstr = new char[expr.length()+1];
		strcpy(cstr,expr.c_str());
		value = MathExprParser::parse(cstr);
		delete[] cstr;
	}
	return value;
}

void parser(HDC &hdc,HWND hwnd,int cxClient,int cyClient, TCHAR *buf1, TCHAR *buf2, TCHAR *buf3, TCHAR *buf4, TCHAR *buf5,TCHAR *buf6,TCHAR *buf7,int zoom_factor,HBRUSH &bgbrush){
	string gst =  TCHAR2STRING(buf6);
	string tst =  TCHAR2STRING(buf7);
	const char *gstr=gst.c_str();
	const char *tstr = tst.c_str();
		static float g_space;  //单位像素
		static float t_dis;    //每隔代表的实际长度
		if(buf6[0]!=NULL){
			g_space = atof(gstr);
		}else{
			g_space = 50;
		}
		g_space = g_space*zoom_factor;
		if(buf7[0]!=NULL){
			t_dis = atof(tstr);
		}else{
			t_dis = 1;
		}
		RECT rect;
		int x,y;
		float xp,yp;
		GetClientRect(hwnd,&rect);
		//绘制屏幕背景颜色
		//HBRUSH BackBrush_l = CreateSolidBrush(RGB(192,192,192));
		SelectObject(hdc,bgbrush);
		Rectangle(hdc,100,0,cxClient,cyClient);
		//DeleteObject(BackBrush_l);
		//绘制文字说明
		TextOut(hdc,10,50,L"grid spacing",12);
		TextOut(hdc,10,110,L"tick distance",12);
		TextOut(hdc,0,180,L"function 1",10);
		TextOut(hdc,0,290,L"function 2",10);
		TextOut(hdc,0,400,L"function 3",10);
		TextOut(hdc,0,510,L"function 4",10);
		TextOut(hdc,0,620,L"function 5",10);
		//绘制x轴变量
	//	TextOut(hdc,10,40,L"x axis range:",13);
		MoveToEx(hdc,100,cyClient/2,NULL);
		LineTo(hdc,cxClient,cyClient/2);
		MoveToEx(hdc,cxClient/2,0,NULL);
		LineTo(hdc,cxClient/2,cyClient);
		//绘制坐标轴
		HPEN hPen,hOldPen;
		hPen = CreatePen(PS_DASHDOT,1,RGB(192,192,192));
		hOldPen = (HPEN)SelectObject(hdc,hPen);
		SelectObject(hdc,hPen);
		TextOut(hdc,cxClient/2+5,cyClient/2+5,L"0",1);
		for(x =cxClient/2-g_space;x>=100;x-=g_space){
			int cnt= floor((x-cxClient/2)/(g_space))*t_dis ;
			TCHAR buff[5];
			TextOut(hdc,x+5,cyClient/2+5,buff,wsprintf(buff,TEXT("%d"),cnt));
		}
		for(x =cxClient/2+g_space;x<cxClient;x+=g_space){
			int cnt=floor((x-cxClient/2)/(g_space))*t_dis ;
			TCHAR buff[5];
			TextOut(hdc,x+5,cyClient/2+5,buff,wsprintf(buff,TEXT("%d"),cnt));
		}
		for(y=cyClient/2-g_space;y>0;y-=g_space){
			int cnt= floor((cyClient/2-y)/(g_space))*t_dis;
			TCHAR buff[5];
			TextOut(hdc,cxClient/2-5,y,buff,wsprintf(buff,TEXT("%d"),cnt));
		}
		for(y=cyClient/2+g_space;y<cyClient;y+=g_space){
			int cnt=floor((cyClient/2-y)/(g_space))*t_dis ;
			TCHAR buff[5];
			TextOut(hdc,cxClient/2-5,y,buff,wsprintf(buff,TEXT("%d"),cnt));
		}
		//绘制网格
		for (x = cxClient/2+g_space;x<rect.right;x+=g_space){
			MoveToEx(hdc,x,0,NULL);
			LineTo(hdc,x,rect.bottom);
		}
		for (x = cxClient/2-g_space;x>100;x-=g_space){
			MoveToEx(hdc,x,0,NULL);
			LineTo(hdc,x,rect.bottom);
		}
		for (y = cyClient/2+g_space;y<rect.bottom;y+=g_space){
			MoveToEx(hdc,100,y,NULL);
			LineTo(hdc,rect.right,y);
		}
		for (y = cyClient/2-g_space;y>rect.top;y-=g_space){
			MoveToEx(hdc,100,y,NULL);
			LineTo(hdc,rect.right,y);
		}
		SelectObject(hdc,hOldPen);


		//函数1 蓝色
		if(buf1[0]!=NULL){
		HPEN hBluePen,hOldBluePen;
		hBluePen = CreatePen(PS_SOLID,5,CBLUE);
		hOldBluePen = (HPEN)SelectObject(hdc,hBluePen);
		SelectObject(hdc,hBluePen);	
		for (float i = -cxClient/2; i <cxClient/2; i += 0.05) // x每增长0.05绘制1个点
		{
			xp = cxClient/2 + i;
			yp = cyClient/2 + (-1*(process_func(buf1,i/(g_space/t_dis))*g_space/t_dis));
		// 在 (x,y) 处绘制一个点
			SetPixel(hdc, xp, yp, CBLUE);
		}
		SelectObject(hdc,hOldBluePen);
		}

		//函数2 红色
		if(buf2[0]!=NULL){
		HPEN hRedPen,hOldRedPen;
		hRedPen = CreatePen(PS_SOLID,5,CRED);
		hOldRedPen = (HPEN)SelectObject(hdc,hRedPen);
		SelectObject(hdc,hRedPen);
		for (float i = -cxClient/2; i <cxClient/2; i += 0.05) // x每增长0.05绘制1个点
		{
			xp = cxClient/2 + i;
			//yp = cyClient/2 + g(i/g_space)*g_space;
			yp = cyClient/2 + (-1*(process_func(buf2,i/(g_space/t_dis))*g_space/t_dis));
		// 在 (x,y) 处绘制一个点
			SetPixel(hdc, xp, yp, CRED);
		}
		SelectObject(hdc,hOldRedPen);
		}


		//函数3 粉色
		if(buf3[0]!=NULL){
		HPEN hPinkPen,hOldPinkPen;
		hPinkPen = CreatePen(PS_SOLID,5,CPINK);
		hOldPinkPen = (HPEN)SelectObject(hdc,hPinkPen);
		SelectObject(hdc,hPinkPen);
		for (float i = -cxClient/2; i <cxClient/2; i += 0.05) // x每增长0.05绘制1个点
		{
			xp = cxClient/2 + i;
			//yp = cyClient/2 + g(i/g_space)*g_space;
			yp = cyClient/2 + (-1*(process_func(buf3,i/(g_space/t_dis))*g_space/t_dis));
		// 在 (x,y) 处绘制一个点
			SetPixel(hdc, xp, yp,CPINK);
		}
		SelectObject(hdc,hOldPinkPen);
		}


	    //函数4 黄色
		if(buf4[0]!=NULL){
		HPEN hYELPen,hOldYELPen;
		hYELPen = CreatePen(PS_SOLID,5,CYELLOW);
		hOldYELPen = (HPEN)SelectObject(hdc,hYELPen);
		SelectObject(hdc,hYELPen);
		for (float i = -cxClient/2; i <cxClient/2; i += 0.05) // x每增长0.05绘制1个点
		{
			xp = cxClient/2 + i;
			//yp = cyClient/2 + g(i/g_space)*g_space;
			yp = cyClient/2 + (-1*(process_func(buf4,i/(g_space/t_dis))*g_space/t_dis));
		// 在 (x,y) 处绘制一个点
			SetPixel(hdc, xp, yp,CYELLOW);
		}
		SelectObject(hdc,hOldYELPen);
		}


		 //函数5 绿色
		if(buf5[0]!=NULL){
		HPEN hGREENPen,hOldGREENPen;
		hGREENPen = CreatePen(PS_SOLID,5,CGREEN);
		hOldGREENPen = (HPEN)SelectObject(hdc,hGREENPen);
		SelectObject(hdc,hGREENPen);
		for (float i = -cxClient/2; i <cxClient/2; i += 0.05) // x每增长0.05绘制1个点
		{
			xp = cxClient/2 + i;
			//yp = cyClient/2 + g(i/g_space)*g_space;
			yp = cyClient/2 + (-1*(process_func(buf5,i/(g_space/t_dis))*g_space/t_dis));
		// 在 (x,y) 处绘制一个
			SetPixel(hdc, xp, yp,CGREEN);
		}
		SelectObject(hdc,hOldGREENPen);
		}



 
}