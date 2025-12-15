#ifndef XPREVIEW_HPP
#define XPREVIEW_HPP

#include <QWidget>
class QTextBrowser;
class QResizeEvent;


class XPreview : public QWidget{
    Q_OBJECT

    friend class XHoverPlane;
private:
    QTextBrowser* textbrowser=nullptr;

public:
    XPreview(QWidget* parent=nullptr);
    ~XPreview(){};

protected:
    void resizeEvent(QResizeEvent *e) override;
};

#endif // XPREVIEW_HPP
