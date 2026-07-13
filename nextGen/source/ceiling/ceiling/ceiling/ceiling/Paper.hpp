#ifndef MPT_PAPER_HEADER
#define MPT_PAPER_HEADER


#define SET_AUTO_HIDE_ENABLE true


#include <QDebug>
#include <QTextEdit>
#include <QTextBrowser>
#include <QVBoxLayout>
#include <QPainter>
#include <QResizeEvent>
#include <QUrl>
#include <QCoreApplication>
#include <QFile>
#include <QPushButton>
#include <QPolygon>
#include <QRegion>
#include <functional>
#include <QTextFrame>


/*
读写双模式，可以做子控件
同时保持一套显示逻辑，即在做子控件时有种特殊激发方式可以让此控件保持父级关系的同时显示方式变成独立控件的显示方式
所以此控件需要记录两套显示逻辑对应的参数设置，以方便切换
独立显示方式拥有完全全量的编辑、浏览逻辑
子控件可以依据情况定制

切换方法就是界面右下角的墨水状态槽◢/◿  对应编辑/只读
*/








extern void lc_setTextDocumentMargin(QTextDocument*, qreal margin);
extern void lc_setDocument(QTextDocument* target, QTextDocument* from);


// can switch between write & read
class Paper:public QWidget{
	// default size
	QRect preGeo{30,30,400,500};
	QPoint default_pos{30,30};
	QWidget* parent;
	Qt::WindowFlags preFlags;
	QFont default_font;

	// format
	qreal lc_margin=9;
	qreal font_sizef=10;
	bool _detached=true;// 默认是非收纳状态
	bool bt_hide_enable;
	bool auto_undetach_enable;

	QVBoxLayout layout{this};
	QTextEdit textEdit;
	QTextBrowser textBrowser;
	QPushButton flatDragBt{this};
	std::function<void()> flatDragBtFunc;
	QPushButton triangleBt{this};

private:
	void modeSwitch()
	{	if(textEdit.isVisible())
		{	if(!true) return;// 判断是不是要保存更改，待完善 ***************
			if(textEdit.document()->isModified())
				lc_setDocument(textBrowser.document(),textEdit.document());
			textEdit.hide();
			textBrowser.show();
		}
		else
		{	// 缺少判断browser是否跳转的逻辑 ******************
			textEdit.document()->setModified(false);
			textEdit.show();
			textBrowser.hide();
		}
	}

public:
	// initUI & installEventFilter
	Paper(bool bt_hide_enable=false, bool auto_undetach_enable=true, QWidget* parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags()|Qt::FramelessWindowHint):auto_undetach_enable(auto_undetach_enable),bt_hide_enable(bt_hide_enable),parent(parent),QWidget(parent,f),preFlags(f)
	{	connect(&flatDragBt, &QPushButton::clicked, this,
			[this]()
			{	if(flatDragBtFunc) flatDragBtFunc();
				undetach();
			}
		);
		connect(&triangleBt, &QPushButton::clicked, this, &Paper::modeSwitch);

		textBrowser.document()->setBaseUrl(QUrl::fromLocalFile(QCoreApplication::applicationDirPath()+"/notes/"));
		lc_setDocument(textEdit.document(),textBrowser.document());
		connect(&textBrowser, &QTextBrowser::anchorClicked,
			[this](const QUrl& url)
			{	auto realUrl=textBrowser.document()->baseUrl().resolved(url);

				QFile file(realUrl.toLocalFile());

				if(file.open(QIODevice::ReadOnly|QIODevice::Text))
				{
					textBrowser.setMarkdown(file.readAll());
					lc_setTextDocumentMargin(textBrowser.document(),lc_margin);
					textBrowser.document()->setBaseUrl(realUrl.adjusted(QUrl::RemoveFilename));
					// 添加对应的textEdit ********
				}
			}
		);//////////////////////////////////////

		setObjectName("paper");
		setStyleSheet("#paper{background-color:black}");
		// textEdit以及textBrowser格式设置
		textEdit   .setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		textEdit   .setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		textBrowser.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		textBrowser.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		textEdit   .setFrameStyle(QFrame::NoFrame);
		textBrowser.setFrameStyle(QFrame::NoFrame);
		textEdit   .setMinimumSize(1,1);
		textBrowser.setMinimumSize(1,1);
		default_font.setPointSizeF(font_sizef);
		textEdit   .document()->setDefaultFont(default_font);
		textBrowser.document()->setDefaultFont(default_font);

		textEdit.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
		textBrowser.setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

		// 布局初始化
		textEdit.hide();
		layout.setContentsMargins(1,1,1,1);// l u r b
		layout.addWidget(&textEdit);
		layout.addWidget(&textBrowser);
		flatDragBt.raise();
		triangleBt.raise();

		// 启动
		setGeometry(preGeo);
		installEventFilter(this);
	}

	void installEventFilter(QObject* filterObj)
	{	QObject::installEventFilter(filterObj);
		textEdit.installEventFilter(filterObj);
		textBrowser.installEventFilter(filterObj);
		textEdit.viewport()->installEventFilter(filterObj);
		textBrowser.viewport()->installEventFilter(filterObj);
	}

	void setText(const QString& line)
	{	textBrowser.document()->setMarkdown(line);
		lc_setTextDocumentMargin(textBrowser.document(),lc_margin);
		lc_setDocument(textEdit.document(),textBrowser.document());
	}

	const bool detached() const{return _detached;}
	void detach()
	{	if(_detached) return;
		_detached=true;
		if(bt_hide_enable)
		{	flatDragBt.show();
			triangleBt.show();
		}
		preFlags=windowFlags();
		setWindowFlags(Qt::Window|Qt::WindowStaysOnTopHint|Qt::FramelessWindowHint);
		show();

		auto tempGeo=geometry();
		setGeometry(preGeo);
		preGeo=tempGeo;
	}

	/**
	 * 收回
	 * 信号量更新
	 * 控件显示更新
	 * 标志复原
	 * 位置复原
	 * */
	void undetach()
	{	if(!_detached) return;
		_detached=false;
		if(bt_hide_enable)
		{	flatDragBt.hide();
			triangleBt.hide();
		}
		if(textEdit.isVisible()) modeSwitch();
		setWindowFlags(preFlags);
		show();

		auto tempGeo=geometry();
		setGeometry(preGeo);
		preGeo=tempGeo;
	}

	const QSize realSize() const
	{	return preGeo.size();
	}

	void signFlatDragBtFunc(std::function<void()> func)
	{	flatDragBtFunc=func;
	}

protected:
	void resizeEvent(QResizeEvent* event) override
	{	// move flat button
		flatDragBt.setGeometry(
			qRound(width()-lc_margin*0.85),
			0,
			qRound(lc_margin*0.85),
			qRound(lc_margin*3.3)
		);

		// move mode button
		triangleBt.setGeometry(
			qRound(width()-lc_margin*2),
			qRound(height()-lc_margin*2),
			qRound(lc_margin*2),
			qRound(lc_margin*2)
		);
		triangleBt.setMask(QRegion{QPolygon{
			QPoint(triangleBt.width(),0),                   // 顶部
			QPoint(triangleBt.width(),triangleBt.height()), // 右下
			QPoint(0,triangleBt.height())                   // 左下
		}});

		// layout.setContentsMargins(qRound(lc_margin/2),0,qRound(lc_margin/10),0);// l u r b
		QWidget::resizeEvent(event);
	}
	bool eventFilter(QObject *obj, QEvent *ev) override
	{	// if((obj==textEdit.viewport()||obj==textBrowser.viewport()) && ev->type()==QEvent::Paint)
		// {	auto w=static_cast<QWidget*>(obj);
		// 	QPainter painter(w);

		// 	painter.setPen(layout.itemAt(0)->widget()==&textBrowser?Qt::lightGray:Qt::black);
		// 	painter.setBrush(layout.itemAt(0)->widget()==&textBrowser?Qt::white:Qt::black);


		// 	// 定义三角形的三个顶点
		// 	QPoint points[3] = {
		// 	};
		// QPolygon polygon;
		// polygon
		//  << QPoint(w->width(), qRound(w->height()-lc_margin*1.5)) // 顶部
		//  << QPoint(w->width(), w->height())                       // 右下
		//  << QPoint(qRound(w->width()-lc_margin*1.5), w->height()) // 左下
		// ;

		// 	painter.drawPolygon(points, 3);


		// 	QPen pen(Qt::gray);          // 边框颜色
		// 	pen.setWidth(qMax(qRound(lc_margin/6),3));            // 边框宽度
		// 	painter.setPen(pen);

		// 	// 2. 设置画刷为不填充（关键步骤）
		// 	painter.setBrush(Qt::NoBrush);  // 或者 painter.setBrush(Qt::transparent);

		// 	// 3. 绘制矩形
		// 	painter.drawRect(0, 0, w->width(), w->height());
		// }
		if(static_cast<QMouseEvent*>(ev)->buttons()==Qt::MiddleButton)
		{	static QPointF cornerDis;
			// newMousePos-newThisPos=oldMousePos-oldThisPos
			// newThisPos=oldThisPos-oldMousePos+newMousePos
			if(ev->type()==QEvent::MouseButtonPress)
			{	cornerDis=mapToGlobal(QPointF{0,0})-static_cast<QMouseEvent*>(ev)->globalPosition();
				return true;
			}
			else if(ev->type()==QEvent::MouseMove)
			{	auto tempDis=cornerDis+static_cast<QMouseEvent*>(ev)->globalPosition();
				if(parentWidget() && !isWindow()) tempDis-=parentWidget()->mapToGlobal(QPointF{0,0});
				move(tempDis.toPoint());
				return true;
			}
		}
		if(auto_undetach_enable && obj==this)
		{	if(ev->type()==QEvent::ParentChange && !isWindow())
			{	if(parentWidget()) undetach();// 父级调换或新加
				else detach();// 清除父级
				return true;
			}
			else if(ev->type() == QEvent::WindowStateChange && parentWidget())
			{	if(windowFlags()&Qt::Window) detach();// 修改后包含独立窗体标志
				else undetach();// 修改后不包含独立窗体标志
				return true;
			}
		}
		return QWidget::eventFilter(obj,ev);
	}
};


#endif