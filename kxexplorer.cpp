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
