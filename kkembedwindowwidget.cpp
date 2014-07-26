#include "stdafx.h"
#include "kkembedwindowwidget.h"

#include "kwidget_p.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KKEmbedWindowWidget)

class KKEmbedWindowWidgetPrivate : public KWidgetPrivate
{
	Q_DECLARE_PUBLIC(KKEmbedWindowWidget)
public:
	QPointer<KWindow> m_pWindow;
public:
	KKEmbedWindowWidgetPrivate()
	{

	}

	~KKEmbedWindowWidgetPrivate()
	{

	}
};

KKEmbedWindowWidget::KKEmbedWindowWidget( QGraphicsItem *parent/*=NULL*/ )
: KWidget(*new KKEmbedWindowWidgetPrivate(), parent)
{
	Q_D(KKEmbedWindowWidget);
	KWidget *itemParent = qobject_cast<KWidget*>(parent->toGraphicsObject());
	QGraphicsView *view = itemParent->view();
	KWindow *window = new KWindow(view);
	d->m_pWindow = window;
	setFlag(QGraphicsItem::ItemHasNoContents);
	setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
	setAttribute(Qt::WA_NativeWindow);
	window->winId();
	bool bok = QObject::connect(this, SIGNAL(scenePosition(QPointF)), this, SLOT(on_scenePosition(QPointF)));
}

KKEmbedWindowWidget::~KKEmbedWindowWidget()
{
	Q_D(KKEmbedWindowWidget);
	if(d->m_pWindow)
	{
		delete d->m_pWindow;
	}
}

KWindow * KKEmbedWindowWidget::realWindow()
{
	Q_D(KKEmbedWindowWidget);
	return d->m_pWindow;
}

KWidget * KKEmbedWindowWidget::rootWidget()
{
	Q_D(KKEmbedWindowWidget);
	return d->m_pWindow->rootWidget();
}

KWidget * KKEmbedWindowWidget::layoutWidget()
{
	Q_D(KKEmbedWindowWidget);
	return d->m_pWindow->rootWidget();
}

void KKEmbedWindowWidget::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	resetGeometry();
}

void KKEmbedWindowWidget::construct()
{
	Q_D(KKEmbedWindowWidget);
	d->m_pWindow->construct();
	bool isvis = isVisible();
	d->m_pWindow->setVisible(isvis);
	resetGeometry();
}

void KKEmbedWindowWidget::visibleEvent( bool bvis )
{
	Q_D(KKEmbedWindowWidget);
	if(d->m_pWindow)
	{
		d->m_pWindow->setVisible(bvis);
	}
}

void KKEmbedWindowWidget::on_scenePosition( const QPointF& pt )
{
	resetGeometry();
}

void KKEmbedWindowWidget::resetGeometry()
{
	Q_D(KKEmbedWindowWidget);
	if(d->m_pWindow)
	{
		QRectF geo = rectToView();
		QMargins m = margins();
		geo.adjust(m.left(), m.top(), -m.right(), -m.bottom());
		d->m_pWindow->setGeometry(geo.toRect());
	}
}
