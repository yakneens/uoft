// generated by Fast Light User Interface Designer (fluid) version 1.0100

#ifndef ScissorUI_h
#define ScissorUI_h
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include "stdafx.h"
#include "c-code/imdraw.h"
#include <iostream.h>
#include <FL/Fl_Menu_Bar.H>
#include <stdlib.h>
#include <FL/Fl_File_Chooser.H>
#include <FL/fl_ask.H>
#include <math.h>
#include <FL/fl_file_chooser.h>
#include <FL/Fl_Group.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Color_Chooser.h>
#include "c-code/color.h"
#include <FL/Fl_Value_Slider.H>

class ScissorUI {
  VImage *im;
  VAttrList attrs;
public:
  ScissorUI();
  Fl_Window *mainWindow;
  ImDraw *imagePanel;
  Fl_Menu_Bar *TopMenu;
  static Fl_Menu_Item menu_TopMenu[];
  static Fl_Menu_Item *FileSubmenu;
  static Fl_Menu_Item *OpenImageItem;
private:
  inline void cb_OpenImageItem_i(Fl_Menu_*, void*);
  static void cb_OpenImageItem(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *OpenCommandItem;
private:
  inline void cb_OpenCommandItem_i(Fl_Menu_*, void*);
  static void cb_OpenCommandItem(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *SaveCommandItem;
private:
  inline void cb_SaveCommandItem_i(Fl_Menu_*, void*);
  static void cb_SaveCommandItem(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *QuitItem;
private:
  inline void cb_QuitItem_i(Fl_Menu_*, void*);
  static void cb_QuitItem(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *ScissorSubmenu;
  static Fl_Menu_Item *SaveBoundary;
private:
  inline void cb_SaveBoundary_i(Fl_Menu_*, void*);
  static void cb_SaveBoundary(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *SaveMatte;
private:
  inline void cb_SaveMatte_i(Fl_Menu_*, void*);
  static void cb_SaveMatte(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *SaveGradient;
private:
  inline void cb_SaveGradient_i(Fl_Menu_*, void*);
  static void cb_SaveGradient(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *SaveZeroCrossings;
private:
  inline void cb_SaveZeroCrossings_i(Fl_Menu_*, void*);
  static void cb_SaveZeroCrossings(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *SaveLinkCosts;
private:
  inline void cb_SaveLinkCosts_i(Fl_Menu_*, void*);
  static void cb_SaveLinkCosts(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *SaveSeedPoints;
private:
  inline void cb_SaveSeedPoints_i(Fl_Menu_*, void*);
  static void cb_SaveSeedPoints(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *LoadSeedPoints;
private:
  inline void cb_LoadSeedPoints_i(Fl_Menu_*, void*);
  static void cb_LoadSeedPoints(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *BSpline;
  static Fl_Menu_Item *ConvertPointsToBSpline;
private:
  inline void cb_ConvertPointsToBSpline_i(Fl_Menu_*, void*);
  static void cb_ConvertPointsToBSpline(Fl_Menu_*, void*);
public:
  static Fl_Menu_Item *LoadSplineFromFile;
private:
  inline void cb_LoadSplineFromFile_i(Fl_Menu_*, void*);
  static void cb_LoadSplineFromFile(Fl_Menu_*, void*);
public:
  Fl_Group *Tools;
private:
  Fl_Button *DrawPoint;
  inline void cb_DrawPoint_i(Fl_Button*, void*);
  static void cb_DrawPoint(Fl_Button*, void*);
  Fl_Button *DrawLine;
  inline void cb_DrawLine_i(Fl_Button*, void*);
  static void cb_DrawLine(Fl_Button*, void*);
  Fl_Button *DrawRect;
  inline void cb_DrawRect_i(Fl_Button*, void*);
  static void cb_DrawRect(Fl_Button*, void*);
  Fl_Button *DrawCircle;
  inline void cb_DrawCircle_i(Fl_Button*, void*);
  static void cb_DrawCircle(Fl_Button*, void*);
  Fl_Button *DrawPoly;
  inline void cb_DrawPoly_i(Fl_Button*, void*);
  static void cb_DrawPoly(Fl_Button*, void*);
  Fl_Button *ChooseColor;
  inline void cb_ChooseColor_i(Fl_Button*, void*);
  static void cb_ChooseColor(Fl_Button*, void*);
  Fl_Button *ClearAll;
  inline void cb_ClearAll_i(Fl_Button*, void*);
  static void cb_ClearAll(Fl_Button*, void*);
  Fl_Button *Undo;
  inline void cb_Undo_i(Fl_Button*, void*);
  static void cb_Undo(Fl_Button*, void*);
  Fl_Button *ScissorTool;
  inline void cb_ScissorTool_i(Fl_Button*, void*);
  static void cb_ScissorTool(Fl_Button*, void*);
  Fl_Button *Bspline;
  inline void cb_Bspline_i(Fl_Button*, void*);
  static void cb_Bspline(Fl_Button*, void*);
public:
  Fl_Value_Slider *levelOfDetail;
private:
  inline void cb_levelOfDetail_i(Fl_Value_Slider*, void*);
  static void cb_levelOfDetail(Fl_Value_Slider*, void*);
public:
  void show();
  void set(VImage new_im);
  void setLevelOfDetail(double l);
};
#endif