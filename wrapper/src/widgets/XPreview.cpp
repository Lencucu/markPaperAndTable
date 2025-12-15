#include <widgets/XPreview.hpp>
#include <QTextBrowser>
#include <QResizeEvent>


XPreview::XPreview(QWidget* parent):QWidget(parent)
{	textbrowser = new QTextBrowser{this};
}


void XPreview::resizeEvent(QResizeEvent *e){
    if(textbrowser) textbrowser->resize(e->size());
}