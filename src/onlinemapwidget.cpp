#include "onlinemapwidget.h"
#include <QWebChannel>
#include <QFile>

#pragma execution_character_set("utf-8")

OnLineMapWidget::OnLineMapWidget(QWidget* parent)
	: QWebEngineView(parent)
{}

OnLineMapWidget::~OnLineMapWidget()
{
}

bool OnLineMapWidget::setUrlFile(const QString& url_file)
{
	/* 创建一个与网页交互的通道 */
	QWebChannel* webChannel = new QWebChannel(this->page());
	this->page()->setWebChannel(webChannel);
	/* 注册通道，ID 为 JSInterface，其将在JS文件这引用 */
	webChannel->registerObject(QString("JSInterface"), this);
	qDebug() << url_file; /* 获取你要显示网页的路径 */
	QFile file(url_file);
	if (!file.exists())
	{
		qDebug() << "html file is not exist";
		return false;
	}
	this->page()->load(QUrl("file:///" + url_file));
	return true;
}

static int g_count = 0;
QFile* g_file = nullptr;
std::vector<double> g_lons;
std::vector<double> g_lats;
void OnLineMapWidget::renderTraceIns(double lon, double lat, uint64_t time, double height
	, double roll, double pitch, double yaw, double v, bool follow_ins)
{
	QString script = QString("renderTraceIns(%1,%2,%3,%4,%5,%6,%7,%8,%9)")
		.arg(lon, 0, 'f', 10).arg(lat, 0, 'f', 10).arg(time).arg(height).arg(roll)
		.arg(pitch).arg(yaw).arg(v).arg(follow_ins);
	//QString script = QString("renderTraceGPS(%1,%2)")
	//	.arg(lon, 0, 'f', 10).arg(lat, 0, 'f', 10);
	runJavaScript(script);
}

void OnLineMapWidget::renderTraceGps(double lon, double lat, uint64_t time
	, double height, double roll, double pitch, double yaw, double v)
{
	QString script = QString("renderTraceGPS(%1,%2,%3,%4,%5,%6,%7,%8)")
		.arg(lon, 0, 'f', 10).arg(lat, 0, 'f', 10).arg(time).arg(height).arg(roll)
		.arg(pitch).arg(yaw).arg(v);
	// 	QString script = QString("renderTraceGPS(%1,%2)")
	// 		.arg(lon, 0, 'f', 10).arg(lat, 0, 'f', 10);
	runJavaScript(script);
}

void OnLineMapWidget::renderTraceGps(double lon, double lat)
{
	QString script = QString("renderTraceGPS(%1,%2)")
		.arg(lon, 0, 'f', 6).arg(lat, 0, 'f', 6);
	runJavaScript(script);
}

void OnLineMapWidget::clearGpsPath()
{
	QString script = QString("ClearPathGPS()");
	runJavaScript(script);
}

void OnLineMapWidget::clearInsPath()
{
	QString script = QString("ClearPathIns()");
	runJavaScript(script);
}

void OnLineMapWidget::measureDistance(int type)
{
	QString script = QString("MeasureCtrl(%1)").arg(type);
	runJavaScript(script);
}

void OnLineMapWidget::setInsVisible(bool is_visible)
{
	QString script = QString("ShowPathIns(%1)").arg(is_visible ? "1" : "0");
	runJavaScript(script);
}

void OnLineMapWidget::setGpsVisible(bool is_visible)
{
	QString script = QString("ShowPathGPS(%1)").arg(is_visible ? "1" : "0");
	runJavaScript(script);
}

void OnLineMapWidget::locate(double lon, double lat)
{
	if (fabs(lon) < 1e-7 || fabs(lat) < 1e-7)
	{
		return;
	}
	QString script = QString("locate(%1,%2)").arg(lon, 0, 'f', 10).arg(lat, 0, 'f', 10);
	runJavaScript(script);
}

void OnLineMapWidget::runJavaScript(const QString& script)
{
	if (nullptr != this->page())
	{
		//qDebug() << script;
		this->page()->runJavaScript(script, [this](const QVariant& v) {
			qDebug() << v.toString();
			});
	}
}

void OnLineMapWidget::setMapMode(QString mode)
{
	QString script = QString("setMapMode('%1')").arg(mode);
	runJavaScript(script);
}

//void OnLineMapWidget::setMapLang(QString lang)
//{
//	QString script = QString("setMapLang('%1')").arg(lang);
//	runJavaScript(script);
//}