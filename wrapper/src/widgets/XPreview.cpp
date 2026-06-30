#include <widgets/XPreview.hpp>
#include <QResizeEvent>

// default:16x12cm
XPreview::XPreview(QWidget* parent):QWidget(parent)
{	textbrowser = new QTextBrowser{this};
}


void XPreview::resizeEvent(QResizeEvent *e){
    if(textbrowser) textbrowser->resize(e->size());
}