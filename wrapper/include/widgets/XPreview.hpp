#ifndef XPREVIEW_HPP
#define XPREVIEW_HPP

class QWidget;
class QTextBrowser;
class QResizeEvent;
namespace LccToolbox{
	template<class T> class GmarkSetAuto;
}

class preview : public QWidget{
    Q_OBJECT

private:
    QTextBrowser* textbrowser=nullptr;

public:
    preview(QWidget* parent=nullptr);
    ~preview(){};

protected:
    void resizeEvent(QResizeEvent *e) override;
};

#endif // XPREVIEW_HPP
