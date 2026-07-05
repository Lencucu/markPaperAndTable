#include "physical2pixel.hpp"


#include <QGuiApplication>
#include <QScreen>
size_t physicalSize2pix(double l){
    return (QGuiApplication::primaryScreen()->physicalDotsPerInch() * l / 25.4);
}
size_t physicalSize2pix_inch(double l){
    return (QGuiApplication::primaryScreen()->physicalDotsPerInch() * l);
}
qreal pix2physicalSize(size_t l){
    return qreal(l)*25.4/QGuiApplication::primaryScreen()->physicalDotsPerInch();
}
qreal pix2physicalSize_inch(size_t l){
    return qreal(l)/QGuiApplication::primaryScreen()->physicalDotsPerInch();
}


void enableMousePassthrough(HWND WID){
    LONG exStyle = GetWindowLong(WID, GWL_EXSTYLE); // 读取当前扩展样式
    SetWindowLong(WID, GWL_EXSTYLE, exStyle | WS_EX_TRANSPARENT); // 添加穿透
}
// 撤销鼠标穿透
void disableMousePassthrough(HWND WID){
    LONG exStyle = GetWindowLong(WID, GWL_EXSTYLE); // 读取当前扩展样式
    exStyle &= ~WS_EX_TRANSPARENT;               // 移除穿透标志
    SetWindowLong(WID, GWL_EXSTYLE, exStyle);   // 应用修改
}