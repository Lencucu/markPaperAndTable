#include <QtTypes>
extern size_t physicalSize2pix(double l);
extern size_t physicalSize2pix_inch(double l);
extern qreal pix2physicalSize(size_t l);
extern qreal pix2physicalSize_inch(size_t l);


#include <windows.h>
// 让一个 Qt 窗口鼠标穿透（贯穿）
extern void enableMousePassthrough(HWND WID);
extern void disableMousePassthrough(HWND WID);
