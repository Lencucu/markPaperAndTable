#ifndef CARD_HPP
#define CARD_HPP


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
#include <tools/physical2pixel/physical2pixel.hpp>
#include <QGraphicsView>
#include <QGraphicsScene>








class Card : public QWidget{
	Q_OBJECT
	// 1.【!just click or mouse hover over specific time then get focus!】
	// 【!when get foucs on, show a boarder and slowly disappear!】
	// 2.【#double click show textBrowser#】
	// *.一般来说右边以及下边扩大窗体内容不会动，但是左边和顶部扩大内容会跟着动，我们这里需要一致性，修了它
	// 3.【!paper auto fit content!】
	QRect rect_bg;
	QSizeF pre_wh;

private:
	qreal shrinkDelta=1/1.4;// also align to textBrowser, this will effect Card show content range
	qreal shrinkFactor;// also align to textBrowser, this will effect Card show content range
	QPointF contentAnchor;// pre中心对应的点 【!default set to top left!】

private:
	QWidget mask{this};// 【#ratio keep with textEdit#】 and 【#fill Card#】 | 【#fix#】
	// QTextEdit textEdit{this};// 【#independent size#】 | 【#postion/zoom decide by mouse middle-pos/scroll at mask#】
	// // 【!! style:(globe)
	// //      sharp border
	// //      dark/light theme follow global theme
	// // 】!!  font size/style
	// QTextBrowser textBrowser;// 【#a scale up well#】 & with 【#same ratio with textEdit and share content#】
	// // 【!an editable back!】

	// // new // //
	QGraphicsScene scene;
	QGraphicsView view{&scene,this};
	QWidget* paper=nullptr;
	// // new // //

	void textEditAboutResize()
	{	view.scale(shrinkDelta,shrinkDelta);
		shrinkFactor=view.transform().m11();
		// auto&& doc_size=papersize*shrinkDelta;
		// //   resize document
		// auto& document=*(textEdit.document());
		// auto&& new_font=document.defaultFont();
		// document.setPageSize(doc_size);
		// document.setDocumentMargin(shrinkDelta*physicalSize2pix(4));
		// new_font.setPixelSize(qRound(shrinkDelta*physicalSize2pix(4)));
		// document.setDefaultFont(new_font);

		// // auto&& t_e_size=document.pageSize().toSize();
		// auto&& t_e_size=doc_size.toSize();
		// textEdit.resize(t_e_size);
	}

public:
	Card(qreal shrink_factor=1/1.4, QWidget* _paper=nullptr, QWidget* parent=nullptr):paper(_paper),shrinkDelta(shrink_factor),QWidget(parent/*,Qt::FramelessWindowHint*/)
	{   // 1.set mask
		if(paper) scene.addWidget(paper);
		installEventFilter(this);

		mask.raise();// 置顶

		mask.setStyleSheet("background-color:rgba(180,200,255,0%);");
		view.setAlignment(Qt::AlignLeft|Qt::AlignTop);
		view.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		view.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		textEditAboutResize();

		resize(400,400);// 不resize的话就显示不了该窗体，貌似是因为前文会顶掉控件默认的resize行为，把size锁定在0，0了
	};
	~Card(){};

	// void setText(const QString& line)
	// {	textEdit.document()->setPlainText(line);
	// 	textEditAboutResize();
	// 	textBrowser.document()->setPlainText(line);
	// }

	void installEventFilter(QObject* obj)
	{	QWidget::installEventFilter(obj);
		mask.installEventFilter(obj);
		view.installEventFilter(obj);
		scene.installEventFilter(obj);
		if(paper) paper->installEventFilter(obj);
	}


protected:
	bool hasHeightForWidth() const override
	{	return true;
	}
	int heightForWidth(int w) const override
	{	return int(w*0.8);
	}
	void resizeEvent(QResizeEvent *e) override// won't change textEdit size
	{	mask.resize(e->size());
		view.resize(e->size());
		// // 同步到textEdit，中心对齐
		// QPointF center_point{mask.size().toSizeF().width()/2,mask.size().toSizeF().height()/2};
		// textEdit.move(
		// 	qRound(center_point.x()-textEdit.size().toSizeF().width()*contentAnchor.x()),
		// 	qRound(center_point.y()-textEdit.size().toSizeF().height()*contentAnchor.y())
		// );
	};
	bool eventFilter(QObject *obj, QEvent *ev) override
	{	//if(ev->type()==QEvent::Resize && obj==&textEdit)// draw textEdit to keep center
		//{	auto&& new_size_f=static_cast<QResizeEvent*>(ev)->size().toSizeF();
		//	auto&& old_size_f=static_cast<QResizeEvent*>(ev)->oldSize().toSizeF();

		//	QPointF center_point{mask.size().toSizeF().width()/2,mask.size().toSizeF().height()/2};
		//	// pos=center-size*anchor
		//	QPointF pos{
		//		center_point.x()-old_size_f.width()*contentAnchor.x(),
		//		center_point.y()-old_size_f.height()*contentAnchor.y()
		//	};
		//	auto ratio=new_size_f.width()/
		//		old_size_f.width();
		//	// (newpos-center)/(pos-center)=ratio
		//	auto newpos=((pos-center_point)*ratio+center_point).toPoint();
		//	textEdit.move(newpos);
		//}
		if(paper && ev->type()==QEvent::Paint && obj==&mask)// draw mask
		{	QPainter qp{&mask};

			// 绘制参数预算
			qreal paper_wh_ratio=qreal(paper->width())/paper->height();
			QColor color_bg{120,120,120,30};
			QColor color{180,200,255,50};

			// 绘制
			if(paper_wh_ratio>(qreal(mask.width())/mask.height()))// 画布缩放是否基于mask的宽
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
				pre_wh.setWidth(mask.width()*tinypaper_wh.height()/paper->height()/shrinkFactor);
				pre_wh.setHeight(mask.height()*tinypaper_wh.height()/paper->height()/shrinkFactor);
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
				pre_wh.setWidth(mask.width()*tinypaper_wh.width()/paper->width()/shrinkFactor);
				pre_wh.setHeight(mask.height()*tinypaper_wh.width()/paper->width()/shrinkFactor);
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
		{	if(paper && static_cast<QMouseEvent*>(ev)->buttons()&Qt::MiddleButton)
			{	qreal pre_x=static_cast<QMouseEvent*>(ev)->position().x()-rect_bg.x();
				qreal pre_y=static_cast<QMouseEvent*>(ev)->position().y()-rect_bg.y();
				if(pre_x<pre_wh.width()/2) pre_x=pre_wh.width()/2;
				else if(pre_x>rect_bg.width()-pre_wh.width()/2) pre_x=rect_bg.width()-pre_wh.width()/2;
				if(pre_y<pre_wh.height()/2) pre_y=pre_wh.height()/2;
				else if(pre_y>rect_bg.height()-pre_wh.height()/2) pre_y=rect_bg.height()-pre_wh.height()/2;
				contentAnchor.rx()=pre_x/rect_bg.width();
				contentAnchor.ry()=pre_y/rect_bg.height();
				//// 同步到textEdit
				//QPointF center_point{mask.size().toSizeF().width()/2,mask.size().toSizeF().height()/2};
				//textEdit.move(
				//	qRound(center_point.x()-textEdit.size().toSizeF().width()*contentAnchor.x()),
				//	qRound(center_point.y()-textEdit.size().toSizeF().height()*contentAnchor.y())
				//);
				view.centerOn(contentAnchor.x()*paper->width(),contentAnchor.y()*paper->height());
				mask.update();
				return true;
			}
		}
		if(ev->type()==QEvent::Wheel/* && obj!=&textBrowser*/)// zoom paper
		{	if (!static_cast<QWheelEvent*>(ev)->pixelDelta().isNull())
				shrinkDelta=(qreal(static_cast<QWheelEvent*>(ev)->pixelDelta().y())/1000+1);
			else
				shrinkDelta=(qreal(static_cast<QWheelEvent*>(ev)->angleDelta().y())/1000+1);

			textEditAboutResize();

			mask.update();
			return true;
		}
		// if(ev->type()==QEvent::MouseButtonDblClick && static_cast<QMouseEvent*>(ev)->button()==Qt::LeftButton)
		// {	textBrowser.show();
		// 	return true;
		// }
		// if(ev->type()==QEvent::Close && obj==&textBrowser)
		// {	textBrowser.hide();
		// 	return true;
		// }
		return QWidget::eventFilter(obj,ev);
	}
};








#endif // CARD_HPP