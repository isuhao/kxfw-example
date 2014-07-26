#include "stdafx.h"
#include "kkslidervolume.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KKSliderVolume)

KKSliderVolume::KKSliderVolume( QGraphicsItem *parent /*= NULL*/ )
: KFrameItem(parent)
, m_timerid(0)
{
	KWidget *itemParent = qobject_cast<KWidget*>(parent->toGraphicsObject());
	QGraphicsView *itemView = itemParent->view();
	m_pWindow = new KBubbleTip(itemView);
	m_pWindow->setSceneMargins(QMargins(0,0,0,0));
	KWidget *root = m_pWindow->rootWidget();
	m_sliderPannel = new KFrameItem(root);
	root->addItem(m_sliderPannel);

	m_sliderPannel->setFrameType(KFrameItem::GridFour);
	m_sliderPannel->setLayoutType(KWidget::VBox);

	m_sliderFrame = new KSpacer(m_sliderPannel);
	m_sliderFrame->setLayoutType(KWidget::VBox);
	m_sliderFrame->setGrayToDisable(true);
	m_slider = new KSlider(m_sliderFrame);
	m_slider->setOrientation(Qt::Vertical);
	m_slider->setRange(0, 1.0);
	m_mute = new KFrameItem(m_sliderPannel);
	m_sliderFrame->addItem(m_slider);
	m_sliderPannel->addItem(m_sliderFrame);
	m_sliderPannel->addItem(m_mute);
	setAcceptHover(true);

	m_popupSize = QSize(20,100);
	m_mute->setFixHeight(20);
	m_mute->setAcceptClick(true);
	m_sliderPannel->setMargins(QMargins(0,0,0,0));
	m_sliderPannel->setDefaultFrame(KFrameItem::RectWhite);
	m_sliderPannel->setFrameType(KFrameItem::GridFour);
	m_sliderPannel->setZValue(100);
	m_mute->setFrameSwitch(false);
	setFrameSwitch(false);

	m_pWindow->setTarget(this);
	m_pWindow->setDirection(KFollowWindow::TopCenter);

	m_pWindow->hide();

	QObject::connect(m_mute, SIGNAL(clicked()), this, SLOT(on_mute_clicked()));
	QObject::connect(m_slider, SIGNAL(valueChanged(qreal)), this, SIGNAL(valueChanged(qreal)));
}

KKSliderVolume::~KKSliderVolume()
{
	
}

void KKSliderVolume::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	updateLayout();	
}

void KKSliderVolume::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
	__super::hoverEnterEvent(event);
	
	updateLayout();
	m_pWindow->show();
	m_timerid = startTimer(100);
}

void KKSliderVolume::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	__super::hoverLeaveEvent(event);
}

void KKSliderVolume::timerEvent( QTimerEvent * event )
{
	if(event->timerId() == m_timerid)
	{
		if(!m_sliderPannel->isUnderMouseEx() && !m_sliderPannel->isUnderMouse())
		{
			m_pWindow->hide();
			killTimer(m_timerid);
		}
	}
}

void KKSliderVolume::updateLayout()
{
	m_sliderPannel->setFixSize(m_popupSize);
	m_pWindow->setWindowSize(m_popupSize);
	QPointF offsetPt(0, height());
	m_pWindow->setPosOffset(offsetPt);
	m_pWindow->adjustGeometry();
	m_pWindow->setFixedWidth(m_popupSize.width());
}

QSize KKSliderVolume::popupSize() const
{
	return m_popupSize;
}

void KKSliderVolume::setPopupSize( const QSize& s )
{
	m_popupSize = s;
}

QObject * KKSliderVolume::popupFrameObject() const
{
	return m_sliderPannel;
}

QObject * KKSliderVolume::statusObject() const
{
	return m_mute;
}

QObject * KKSliderVolume::sliderObject() const
{
	return m_slider;
}

QString KKSliderVolume::imagePath() const
{
	return "";
}

void KKSliderVolume::setImagePath( const QString& s )
{
	m_mute->setImagePath(s);
	__super::setImagePath(s);
}

QObject * KKSliderVolume::sliderFrameObject() const
{
	return m_sliderFrame;
}

void KKSliderVolume::setVolume( float val, bool mute )
{
	if(!mute)
	{
		m_slider->setValue(val);
	}
	int idx = mute ? 1 : 0;
	m_mute->setFrameIndex(idx);
	setFrameIndex(idx);
	m_sliderFrame->setEnabled(!mute);
}

void KKSliderVolume::on_mute_clicked()
{
	int index = m_mute->frameIndex();
	index = index == 1 ? 0 : 1;
	m_mute->setFrameIndex(index);
	setFrameIndex(index);
	m_sliderFrame->setEnabled(!index);
	emit muteChanged(index == 1);
}

bool KKSliderVolume::mute()
{
	int index = m_mute->frameIndex();
	return index==1;
}