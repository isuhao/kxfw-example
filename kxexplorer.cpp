#include "stdafx.h"
#include "kxexplorer.h"
#include "kkembedwindowwidget.h"

Q_GLOBAL_STATIC(KxExplorer, _KxExplorerInstance)

KxExplorer* KxExplorer::instance()
{
	return _KxExplorerInstance();
}


KxExplorer::KxExplorer( QObject *parent /*= NULL*/ )
: QObject(parent)
{
	m_pMainWindow = qobject_cast<KPopupWindow*>(KXmlUI::windowFromFile("res:/Resources/layout/kkmainwindow.xml"));
}

KxExplorer::~KxExplorer()
{
	
}



void KxExplorer::execute()
{
	m_pViewport = m_pMainWindow->viewport();
	m_pViewport->installEventFilter(this);

	m_widgetToolTip = new KToolTip(KToolTip::CustomWidget);
	m_widgetToolTip->setAutoDeleteWhenAppQuit(true);
	m_widgetToolTip->setDirection(KToolTip::LeftTop);
	m_widgetToolTip->setTrigger(KToolTip::CursorInTarget);
	QObject::connect(m_widgetToolTip, SIGNAL(aboutToShow()), this, SLOT(on_tooltip_aboutToShow()));
	
	KWidget *root = m_widgetToolTip->rootWidget();
	m_widgetTip = new KWidget(root);
	root->addItem(m_widgetTip);
	m_widgetTip->setLayoutType(KWidget::VBox);
	m_widgetTip->setMargins(QMargins(2,2,2,2));
	m_widgetTip->setFixSize(-1,-1);
	bool bok1 = KXmlUI::widgetFromFile("res:/Resources/layout/gifttip.xml", m_widgetTip);

	KTabWidget *userWizard = m_pMainWindow->findChild<KTabWidget*>("userWizard");
	KStackedWidget *userStacked = m_pMainWindow->findChild<KStackedWidget*>("userStacked");
	QObject::connect(userWizard, SIGNAL(indexSelected(int)), userStacked, SLOT(setCurrentIndex(int)));
	KCommonListWidget *list = m_pMainWindow->findChild<KCommonListWidget*>("userList");
	list->setProperty("myToolTip", QVariant::fromValue<QObject*>(m_widgetToolTip));
	list->setItemCreator(new KListItemCreatorT<KKListItem>());
	KCommonListModel *model = qobject_cast<KCommonListModel*>(list->model());
	model->batchInsert();
	for(int i = 0; i < 50; i++)
	{
		model->addItem(QString("items%1").arg(i+1), (rand() % 100000) + 100);
	}
	model->batchCommit();


	KSpacer *captionArea = m_pMainWindow->findChild<KSpacer*>("captionArea");
	captionArea->setDragPolicy(KWidget::WindowMove);
	bool bOk4 = QObject::connect(captionArea, SIGNAL(mouseEvent(QEvent*)), this, SLOT(on_captionArea_mouseEvent(QEvent*)));


	m_pRoomInput = m_pMainWindow->findChild<KWidget*>("inputArea");
	KPushButton *btnFace = m_pRoomInput->findChild<KPushButton*>("btnFace");
	QObject::connect(btnFace, SIGNAL(clicked()), this, SLOT(on_btnFace_clicked()));
	m_pFacePage = m_pRoomInput->findChild<KWidget*>("facePage");
	m_pFaceFrame = m_pFacePage->findChild<KWidget*>("faceFrame");
	QObject::connect(m_pFacePage, SIGNAL(visibleChanged()), this, SLOT(on_facePage_visibleChanged()));

	KTabWidget *faceTab = m_pRoomInput->findChild<KTabWidget*>("faceTab");
	KStackedWidget *faceStacked = m_pRoomInput->findChild<KStackedWidget*>("faceStacked");
	QObject::connect(faceTab, SIGNAL(indexSelected(int)), faceStacked, SLOT(setCurrentIndex(int)));

	m_pBtnColorWord = m_pRoomInput->findChild<KPushButton*>("btnColorWord");
	QObject::connect(m_pBtnColorWord, SIGNAL(clicked()), this, SLOT(on_colorWord_clicked()));



	KPushButton *btnMin = m_pMainWindow->findChild<KPushButton*>("titleMin");
	bool bOk = QObject::connect(btnMin, SIGNAL(clicked()), m_pMainWindow, SLOT(showMinimized()));

	KCheckButton *btnMaxRestore = m_pMainWindow->findChild<KCheckButton*>("titleMaxRestore");
	bool bOk1 = QObject::connect(btnMaxRestore, SIGNAL(clicked()), m_pMainWindow, SLOT(showMaximizeOrRestore()));

	KPushButton *btnClose = m_pMainWindow->findChild<KPushButton*>("titleClose");
	bool bOK2 = QObject::connect(btnClose, SIGNAL(clicked()), m_pMainWindow, SLOT(close()));

	btnClose->setToolTip("这只是一个测试\r\n关闭的按钮\r\n你点击它，真的可以关闭");
	KToolTip::textTipInstance()->addTarget(btnClose);
	btnMaxRestore->setToolTip("最大化或恢复");
	KToolTip::textTipInstance()->addTarget(btnMaxRestore);
	btnMin->setToolTip("最小化");
	KToolTip::textTipInstance()->addTarget(btnMin);

	m_pRoom = m_pMainWindow->findChild<KWidget*>("oneRoom");
	for(int i = 0; i < 3; i++)
	{
		KKEmbedWindowWidget *topmic = m_pMainWindow->findChild<KKEmbedWindowWidget*>(QString("topmic%1").arg(i));
		KWidget* root = topmic->rootWidget();
		root->setFixSize(-1,-1);
		KXmlUI::widgetFromFile("res:/Resources/layout/videoport.xml", root);
		root->invalidateLayout();
		QSizeF ms = root->minimumSize();
		topmic->setFixSize(ms);

		KLabel *name = root->findChild<KLabel*>("nickName");
		name->setText(QString("VideoName%1").arg(i+1));
	}

	
	KTextEdit *edit = m_pMainWindow->findChild<KTextEdit*>("publicChat");
	KTextCursor txtCursor = edit->textCursor();
	txtCursor.insertText("默认就可以打开我的主页：开心&努力[http://www.kxtry.com]点击试试", Qt::blue, "http://www.kxtry.com","", true);
	txtCursor.insertText("\r\n\r\n");
	txtCursor.insertText("还可以打开我的开源GitHub主页：开心&努力[https://github.com/kxtry]点击试试", Qt::blue, "https://github.com/kxtry","", true);


	KTextEdit *edit2 = m_pMainWindow->findChild<KTextEdit*>("privateChat");
	txtCursor = edit2->textCursor();
	QString txt = "以前创业开发的一个产品界面，创业失败后，再重新打工，这么好的代码如果只是放在硬盘上，那是一种浪费，"\
		"再者本人已经转战服务端开发，这KXFW界面库，对我来说已经不再重要，重要的是有人能够发挥它的价值。"\
		"利用它做为一个学习的例子又或作为一个产品开发库，做什么用途都好，至少比占用硬盘浪费空间的好。"\
		"感恩曾经工作过和现在正在工作的公司和同事，也感恩过去的我、现在的我还有将来的我---【心路xinlu】";

	txtCursor.insertText(txt);


	m_pMainWindow->moveCenter(NULL);
	KWidget *areaRegister = m_pMainWindow->findChild<KWidget*>("titleLogin");
	m_tipRegister = new KTextBubbleTip(m_pMainWindow);
	m_tipRegister->setTarget(areaRegister);
	m_tipRegister->setText("注册或登录或切换帐号请点击这里");
	m_tipRegister->setBackgroundColor(QColor(244,249,252));
	m_tipRegister->setPenColor(QColor(184,197,210));
	m_tipRegister->setElapseTime(3000);
	KTextItem *text = qobject_cast<KTextItem*>(m_tipRegister->textObject());
	text->setTextColor(QColor(72,72,72));
	m_tipRegister->setPosOffset(QPointF(0,-10));
	QSizeF s = m_tipRegister->calcWindowPreferSize();
	m_tipRegister->setWindowSize(s.toSize());
	m_tipRegister->show();

	m_pMainWindow->setWindowIcon(GetModuleHandle(NULL), IDI_APPICO);
	m_pMainWindow->setWindowTitle("可可视频社区");


	m_pMainWindow->show();
}

void KxExplorer::on_btnFace_clicked()
{
	if(m_pFacePage->isVisible())
	{
		m_pFacePage->hide();
	}
	else
	{
		m_pFacePage->show();
		m_pFacePage->setActive(false);
	}
}

void KxExplorer::on_facePage_visibleChanged()
{
	if(m_pFace[0] == NULL || m_pFace[1] == NULL || m_pFace[2] == NULL || m_pFace[3] == NULL)
	{
		m_pFace[0] = m_pRoomInput->findChild<KTableWidget*>("tableFace1");
		m_pFace[1] = m_pRoomInput->findChild<KTableWidget*>("tableFace2");
		m_pFace[2] = m_pRoomInput->findChild<KTableWidget*>("tableFace3");
		m_pFace[3] = m_pRoomInput->findChild<KTableWidget*>("tableFace4");

		QString faceDefault = "res:/Resources/face/default";
		QString faceCustom1= "res:/Resources/face/custom1";
		QString faceCustom2 = "res:/Resources/face/custom2";
		QString faceCustom3 = "res:/Resources/face/custom3";
		initFaceTable(m_pFace[0], QSizeF(28,28), faceDefault, "default");
		initFaceTable(m_pFace[1], QSizeF(50,50), faceCustom1, "custom1");
		initFaceTable(m_pFace[2], QSizeF(50,50), faceCustom2, "custom2");
		initFaceTable(m_pFace[3], QSizeF(50,50), faceCustom3, "custom3");
	}
}

void KxExplorer::initFaceTable( KTableWidget *table, QSizeF cellSize, const QString& gifpath, const QString& gifkey )
{
	if(table == NULL)
		return;
	QString faceDefault = gifpath;
	QDir dir1(faceDefault);
	QStringList files1 = dir1.entryList(QDir::Files);
	for(int i = 0; i < files1.count(); i++)
	{
		KFrameItem *frame = new KFrameItem();
		frame->setLayoutType(KWidget::VBox);
		frame->setFrameType(KFrameItem::GridFour);
		frame->setImagePath("res:/resources/image/facecell_bk.png");
		frame->setFixSize(cellSize);
		frame->setMargins(QMargins(3,3,3,3));
		frame->setAcceptClick(true);
		QObject::connect(frame, SIGNAL(clicked()), this, SLOT(on_facePage_item_clicked()));
		KAnimationItem *gifani = new KAnimationItem(frame);
		QString face = faceDefault + "\\" + files1.at(i);
		gifani->setImagePath(face);
		gifani->setPlayLoop(true);
		gifani->setStretch(true);
		gifani->setPlayPolicy(KAnimationItem::PlayOnHover);
		frame->addItem(gifani);
		frame->setProperty("page", gifkey);
		frame->setProperty("name", files1.at(i));
		frame->setProperty("path", face);
		static_cast<KWidget*>(table)->addItem(frame);
	}
	int minHeight = table->calcMinimumHeight();
	table->setFixHeight(minHeight+10);
}

bool KxExplorer::eventFilter( QObject *w, QEvent *e )
{
	if(m_pMainWindow == NULL)
		return false;
	if(w == m_pViewport)
	{
		switch(e->type())
		{
		case QEvent::MouseButtonPress:
			{
				if(m_pFaceFrame && m_pFaceFrame->isVisible())
				{
					if(!m_pFaceFrame->isUnderMouse())
					{
						m_pFacePage->hide();
					}
				}
			}
			break;
		case QEvent::WindowDeactivate:
			{
				if(m_pFaceFrame && m_pFaceFrame->isVisible())
				{
					m_pFacePage->hide();
				}
			}
			break;
		}
	}

	return false;
}

void KxExplorer::on_facePage_item_clicked()
{
	KFrameItem *frame = qobject_cast<KFrameItem*>(sender());
	if(frame == NULL)
		return;
	m_pFacePage->hide();
}

void KxExplorer::on_colorWord_clicked()
{
	KMenu *pMenu = KMenu::createPopupMenu();

	for(int i = 0; i < 10; i++)
	{
		QAction *pAction = pMenu->addAction(QString("ColorWord%1").arg(i+1));
		pAction->setData(i+1);
	}
	KWidget *w = qobject_cast<KWidget*>(sender());
	pMenu->exec(w, KMenu::TopLeft);	
}

void KxExplorer::on_captionArea_mouseEvent( QEvent *e )
{
	if(e->type() == QEvent::GraphicsSceneMouseDoubleClick)
	{
		QVariant var = m_pMainWindow->property("resizable");
		if(var.isValid())
		{
			if(!var.toBool())
				return;
		}
		if(m_pMainWindow->isMaximized())
		{
			m_pMainWindow->showNormal();
		}
		else
		{
			m_pMainWindow->showMaximized();
		}
		e->setAccepted(true);
		return ;
	}
	e->setAccepted(false);
}

void KxExplorer::on_tooltip_aboutToShow()
{
	KToolTip *tip = qobject_cast<KToolTip*>(sender());
	KKListItem *item = qobject_cast<KKListItem*>(tip->target());
	QVariant other = item->itemData((Qt::ItemDataRole)KCommonListModel::UserRoleOther);

	int val = other.toInt();
	static int index = 0;
	index = index++ % 3;
	switch(index)
	{
	case 0:
		val = 1;
		break;
	case 1:
		val = 1000;
		break;
	default:
		val = 10000000;
		break;
	}

	KLabel *price = m_widgetTip->findChild<KLabel*>("price");
	price->setText(QString("%1").arg(val));
	price->setAutoAdjust(true);
	/*显示提示*/
	m_widgetTip->invalidateLayout();

	QSizeF s = m_widgetTip->minimumSize();
	m_widgetToolTip->setWindowSize(s.toSize());
}

KKListItem::KKListItem( KListWidget *view )
: KListItem(view)
{
	KToolTip *toolTip = qobject_cast<KToolTip*>(view->property("myToolTip").value<QObject*>());
	toolTip->addTarget(this);
}

KKListItem::~KKListItem()
{
	
}
