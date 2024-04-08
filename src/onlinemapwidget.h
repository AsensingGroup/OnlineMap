#pragma once

#include <QWebEngineView>

class OnLineMapWidget : public QWebEngineView
{
	Q_OBJECT

public:

	enum Type {
		Default = 0,
		GaoDe = 1,
		Bing = 2
	};

	explicit OnLineMapWidget(QWidget* parent = nullptr);
	~OnLineMapWidget();

	/// <summary>
	/// 设置加载的网页文件
	/// </summary>
	/// <param name="url_file"></param>
	/// <returns></returns>
	bool setUrlFile(const QString& url_file);

	/// <summary>
	/// INS轨迹刷新
	/// </summary>
	/// <param name="lon">经度</param>
	/// <param name="lat">纬度</param>
	/// <param name="time">时间</param>
	/// <param name="height">高度</param>
	/// <param name="roll">横滚角</param>
	/// <param name="pitch">俯仰角</param>
	/// <param name="yaw">航向角</param>
	/// <param name="v">速度</param>
	/// <param name="follow_ins">是否ins跟随</param>
	void renderTraceIns(double lon, double lat, uint64_t time, double height
		, double roll, double pitch, double yaw, double v, bool follow_ins);

	/// <summary>
	/// GPS轨迹刷新
	/// </summary>
	/// <param name="lon">经度</param>
	/// <param name="lat">纬度</param>
	/// <param name="time">时间</param>
	/// <param name="height">高度</param>
	/// <param name="roll">横滚角</param>
	/// <param name="pitch">俯仰角</param>
	/// <param name="yaw">航向角</param>
	/// <param name="v">速度</param>
	void renderTraceGps(double lon, double lat, uint64_t time, double height
		, double roll, double pitch, double yaw, double v);

	void renderTraceGps(double lon, double lat);

	/// <summary>
	/// 清空GPS的路径
	/// </summary>
	void clearGpsPath();

	/// <summary>
	/// 清空INS的路径
	/// </summary>
	void clearInsPath();

	/// <summary>
	/// 测距
	/// </summary>
	/// <param name="type">1:开始测控i；2:关闭测距</param>
	void measureDistance(int type);

	/// <summary>
	/// 设置INS显隐
	/// </summary>
	/// <param name="is_visible"></param>
	void setInsVisible(bool is_visible);

	/// <summary>
	/// 设置GPS显隐
	/// </summary>
	/// <param name="is_visible"></param>
	void setGpsVisible(bool is_visible);

	/// <summary>
	/// 回到指定位置
	/// </summary>
	/// <param name="lon"></param>
	/// <param name="lat"></param>
	void locate(double lon, double lat);

	/// <summary>
	/// 设置地图背景样式
	/// </summary>
	/// <param name="name"></param>
	void setMapMode(QString name);

	/// <summary>
	/// 设置地图语言
	/// </summary>
	/// <param name="lang"></param>
	//void setMapLang(QString lang);

	/// <summary>
	/// 执行javascript脚本
	/// </summary>
	/// <param name="script"></param>
	void runJavaScript(const QString& script);
};