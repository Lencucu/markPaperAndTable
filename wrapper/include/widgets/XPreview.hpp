#ifndef XPREVIEW_HPP
#define XPREVIEW_HPP

#include <QWidget>
#include <QTextBrowser>
#include <QTextDocument>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QEvent>
#include <QSize>      // QSize / QSizeF
class QResizeEvent;





extern size_t physicalSize2pix(double l);
extern size_t physicalSize2pix_inch(double l);
extern qreal pix2physicalSize(size_t l);
extern qreal pix2physicalSize_inch(size_t l);





class XPreview : public QWidget{
	Q_OBJECT

	friend class XHoverPlane;
	friend class SearchBar;
private:
	QTextBrowser* textbrowser=nullptr;

public:
	XPreview(QWidget* parent=nullptr);
	~XPreview(){};

protected:
	void resizeEvent(QResizeEvent *e) override;
};








class Card : public QWidget{
	Q_OBJECT
	// 1.just click or mouse hover over specific time then get focus
	// when get foucs on, show a boarder and slowly disappear
	// 2.double click show textBrowser
	// *.一般来说右边以及下边扩大窗体内容不会动，但是左边和顶部扩大内容会跟着动，我们这里需要一致性，修了它

private:
	QSizeF textpaperbasesize{90,120};// align to textbrowser
	qreal shrinkFactor=1;// also align to textbrowser, this will effect Card show content range
	QSizeF contentAnchor;// 【!default set to top left!】

private:
	QTextDocument document;

	QHBoxLayout layout{this};
	QWidget mask{this};// 【!ratio keep with textEdit!】 and 【#fill Card#】 | 【#fix#】
	QTextEdit textEdit{this};// 【#independent size#】 | 【!postion/zoom decide by mouse middle-pos/scroll at mask!】
	// 【!! style:(globe)
	//      sharp border
	//      dark/light theme follow global theme
	// 】!!  font size/style
	QTextBrowser textBrowser;// 【!a scale up well!】 & with 【#same ratio with textEdit and share content#】
	// this can be 【!display just need front double click!】

public:
	Card(QWidget* parent=nullptr):QWidget(parent,Qt::FramelessWindowHint)
	{   // 1.set mask
		installEventFilter(this);
		mask.installEventFilter(this);
		mask.raise();// 置顶
		layout.setContentsMargins(0,0,0,0);// 填满
		setLayout(&layout);
		layout.addWidget(&mask);
		// set layout resize & show logic op method
		// *****testblock
		mask.setStyleSheet("background-color:rgba(180,200,255,90%);");
		// *****testblock

		// 2.init text & show
		textEdit.installEventFilter(this);
		textEdit.setDocument(&document);
		textEdit.resize(physicalSize2pix(90/1.4),physicalSize2pix(120/1.4));// 90x120mm
		// 字体绑定

		textBrowser.setDocument(&document);
		textBrowser.resize(physicalSize2pix(90),physicalSize2pix(120));// 90x120mm
		// 字体绑定
		textBrowser.hide();



		resize(50,50);// 不resize的话就显示不了该窗体，貌似是因为前文会顶掉控件默认的resize行为，把size锁定在0，0了
	};
	~Card(){};


protected:
	void resizeEvent(QResizeEvent *e) override{};// won't change textEdit size
	bool eventFilter(QObject *obj, QEvent *ev) override
	{	if(ev->type()==QEvent::Resize && (obj==&textEdit || obj==this))
			textEdit.move((width()-textEdit.width())/2,(height()-textEdit.height())/2);
		return QWidget::eventFilter(obj,ev);
	}
};








#endif // XPREVIEW_HPP
