#include <widgets/XPreview.hpp>
#include <QTextBrowser>
#include <QResizeEvent>


preview::preview(QWidget* parent=nullptr):QWidget(parent)
{	textbrowser = new QTextBrowser{this};
}


void preview::resizeEvent(QResizeEvent *e){
    if(textbrowser) textbrowser->resize(e->size());
}