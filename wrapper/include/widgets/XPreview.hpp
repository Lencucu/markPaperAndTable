#ifndef XPREVIEW_HPP
#define XPREVIEW_HPP

#include <QWidget>
#include <QTextBrowser>
#include <QTextDocument>
#include <QTextEdit>
#include <QHBoxLayout>
#include <QEvent>
#include <QSize>      // QSize / QSizeF
#include <QPainter>
#include <QRect>
#include <QColor>
#include <QPointF>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QResizeEvent>





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
	// 1.【!just click or mouse hover over specific time then get focus!】
	// 【!when get foucs on, show a boarder and slowly disappear!】
	// 2.【!double click show textBrowser!】
	// *.一般来说右边以及下边扩大窗体内容不会动，但是左边和顶部扩大内容会跟着动，我们这里需要一致性，修了它
	bool mask_focus=false;
	QRect rect_bg;

private:
	QSizeF papersize{qreal(physicalSize2pix(90)),qreal(physicalSize2pix(120))};// align to textbrowser
	qreal shrinkFactor=1/1.4;// also align to textbrowser, this will effect Card show content range
	QPointF contentAnchor;// pre中心对应的点 【!default set to top left!】

private:
	QTextDocument document;

	QHBoxLayout layout{this};
	QWidget mask{this};// 【#ratio keep with textEdit#】 and 【#fill Card#】 | 【#fix#】
	QTextEdit textEdit{this};// 【#independent size#】 | 【#postion/zoom decide by mouse middle-pos/scroll at mask#】
	// 【!! style:(globe)
	//      sharp border
	//      dark/light theme follow global theme
	// 】!!  font size/style
	QTextBrowser textBrowser;// 【!a scale up well!】 & with 【#same ratio with textEdit and share content#】

public:
	Card(QWidget* parent=nullptr):QWidget(parent/*,Qt::FramelessWindowHint*/)
	{   // 1.set mask
		installEventFilter(this);
		mask.installEventFilter(this);
		mask.raise();// 置顶
		layout.setContentsMargins(0,0,0,0);// 填满
		setLayout(&layout);
		layout.addWidget(&mask);
		// set layout resize & show logic op method
		// *****testblock
		mask.setStyleSheet("background-color:rgba(180,200,255,0%);");
		// *****testblock

		// 2.init text & show
		textEdit.installEventFilter(this);
		textEdit.setDocument(&document);
		textEdit.resize(papersize.width()/1.4,papersize.height()/1.4);// 90x120mm
		// 字体绑定

		textBrowser.setDocument(&document);
		textBrowser.resize(papersize.width(),papersize.height());// 90x120mm
		// 字体绑定
		textBrowser.hide();



		resize(400,400);// 不resize的话就显示不了该窗体，貌似是因为前文会顶掉控件默认的resize行为，把size锁定在0，0了
	};
	~Card(){};


protected:
	void resizeEvent(QResizeEvent *e) override// won't change textEdit size
	{	QPointF center_point{mask.size().toSizeF().width()/2,mask.size().toSizeF().height()/2};
		textEdit.move(
			qRound(center_point.x()-textEdit.size().toSizeF().width()*contentAnchor.x()),
			qRound(center_point.y()-textEdit.size().toSizeF().height()*contentAnchor.y())
		);
	};
	bool eventFilter(QObject *obj, QEvent *ev) override
	{	if(ev->type()==QEvent::Resize && obj==&textEdit)// draw textEdit
		{	QPointF center_point{mask.size().toSizeF().width()/2,mask.size().toSizeF().height()/2};
			// pos=center-size*anchor
			QPointF pos{
				center_point.x()-static_cast<QResizeEvent*>(ev)->oldSize().toSizeF().width()*contentAnchor.x(),
				center_point.y()-static_cast<QResizeEvent*>(ev)->oldSize().toSizeF().height()*contentAnchor.y()
			};
			auto ratio=static_cast<QResizeEvent*>(ev)->size().toSizeF().width()/
				static_cast<QResizeEvent*>(ev)->oldSize().toSizeF().width();
			// (newpos-center)/(pos-center)=ratio
			auto newpos=((pos-center_point)*ratio+center_point).toPoint();
			textEdit.move(newpos);
		}
		if(ev->type()==QEvent::Paint && obj==&mask)// draw mask
		{	QPainter qp{&mask};

			// 绘制参数预算
			bool tinyPaper_width_base_on_mask=false;// 画布缩放是否基于mask的宽
			qreal paper_wh_ratio=papersize.width()/papersize.height();
			if(paper_wh_ratio>(qreal(mask.width())/mask.height())) tinyPaper_width_base_on_mask=true;// paper宽高比大于mask宽高比
			QColor color_bg{120,120,120,30};
			QColor color{180,200,255,50};

			// 绘制
			if(tinyPaper_width_base_on_mask)
			{	// 纸张缩略图
				QSizeF tinypaper_wh{
					qreal(mask.width()),
					qreal(mask.width())/paper_wh_ratio
				};
				rect_bg.setRect(
					0,
					qRound((mask.height()-tinypaper_wh.height())/2),
					qRound(tinypaper_wh.width()),
					qRound(tinypaper_wh.height())
				);
				qp.fillRect(rect_bg,color_bg);
				// 预览框轮廓示意
				// size: pre/tinypaper=mask/paper=>pre=mask*(tinypaper/paper)
				QSizeF pre_wh{
					mask.width()*tinypaper_wh.height()/textEdit.height(),
					mask.height()*tinypaper_wh.height()/textEdit.height()
				};
				QRect rect{
					qRound(contentAnchor.x()*tinypaper_wh.width()-pre_wh.width()/2),
					qRound(contentAnchor.y()*tinypaper_wh.height()-pre_wh.height()/2+(mask.height()-tinypaper_wh.height())/2),
					qRound(pre_wh.width()),
					qRound(pre_wh.height())
				};
				qp.fillRect(rect,color);
			}
			else
			{	// 纸张缩略图
				QSizeF tinypaper_wh{
					qreal(mask.height())*paper_wh_ratio,
					qreal(mask.height())
				};
				rect_bg.setRect(
					qRound((mask.width()-tinypaper_wh.width())/2),
					0,
					qRound(tinypaper_wh.width()),
					qRound(tinypaper_wh.height())
				);
				qp.fillRect(rect_bg,color_bg);
				// 预览框轮廓示意
				// size: pre/tinypaper=mask/paper=>pre=mask*(tinypaper/paper)
				QSizeF pre_wh{
					mask.width()*tinypaper_wh.width()/textEdit.width(),
					mask.height()*tinypaper_wh.width()/textEdit.width()
				};
				QRect rect{
					qRound(contentAnchor.x()*tinypaper_wh.width()-pre_wh.width()/2+(mask.width()-tinypaper_wh.width())/2),
					qRound(contentAnchor.y()*tinypaper_wh.height()-pre_wh.height()/2),
					qRound(pre_wh.width()),
					qRound(pre_wh.height())
				};
				qp.fillRect(rect,color);
			}
			return true;
		}
		if(obj==&mask && (ev->type()==QEvent::MouseButtonPress || ev->type()==QEvent::MouseMove))// show-pos control
		{	if(static_cast<QMouseEvent*>(ev)->buttons()&Qt::MiddleButton)
			{	contentAnchor.rx()=(static_cast<QMouseEvent*>(ev)->position().x()-rect_bg.x())/rect_bg.width();
				contentAnchor.ry()=(static_cast<QMouseEvent*>(ev)->position().y()-rect_bg.y())/rect_bg.height();
				QPointF center_point{mask.size().toSizeF().width()/2,mask.size().toSizeF().height()/2};
				textEdit.move(
					qRound(center_point.x()-textEdit.size().toSizeF().width()*contentAnchor.x()),
					qRound(center_point.y()-textEdit.size().toSizeF().height()*contentAnchor.y())
				);
				mask.update();
				return true;
			}
		}
		if(ev->type()==QEvent::Wheel)// zoom paper
		{	if (!static_cast<QWheelEvent*>(ev)->pixelDelta().isNull())
				shrinkFactor+=qreal(static_cast<QWheelEvent*>(ev)->pixelDelta().y())/1000;
			else
				shrinkFactor+=qreal(static_cast<QWheelEvent*>(ev)->angleDelta().y())/1000;
			auto size=papersize.toSize()*shrinkFactor;
			textEdit.resize(size);
			mask.update();
			return true;
		}
		return QWidget::eventFilter(obj,ev);
	}
};








#endif // XPREVIEW_HPP
