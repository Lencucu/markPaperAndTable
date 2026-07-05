// // can switch between write & read
// class Paper:public QTextEdit{
// 	QTextBrowser browser{this};

// public:
// 	Paper(xxx):browser(xxx)
// 	{	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
// 		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

// 		textBrowser.setWindowsFlags(Qt::Window);
// 		textBrowser.setAttribute(Qt::WA_QuitOnClose, false);
// 		textBrowser.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
// 		textBrowser.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
// 		textBrowser.resize(papersize.width(),papersize.height());// 90x120mm
// 		auto& browser_document=*(textBrowser.document());
// 		auto&& new_font=browser_document.defaultFont();
// 		browser_document.setPageSize(papersize);
// 		browser_document.setDocumentMargin(qreal(physicalSize2pix(4)));
// 		new_font.setPixelSize(int(physicalSize2pix(4)));
// 		browser_document.setDefaultFont(new_font);
// 		// 字体绑定
// 		textBrowser.hide();
// 	}
// };