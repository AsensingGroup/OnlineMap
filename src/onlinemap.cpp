#include "onlinemap.h"
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include "paramsetting.h"

#pragma execution_character_set("utf-8")

OnlineMap::OnlineMap(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	this->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
	readConfig();
	connect(ui._online_map, &QWebEngineView::loadFinished, this, &OnlineMap::loadHtmlFinined);
	connect(ui._button_setting, &QPushButton::clicked, this, &OnlineMap::settings);
	connect(ui._button_load, &QPushButton::clicked, this, &OnlineMap::loadData);
	connect(ui._button_clear, &QPushButton::clicked, this, &OnlineMap::clearData);
	connect(ui._button_convert, &QPushButton::clicked, this, &OnlineMap::dataConvert);
	connect(ui._check_base, &QCheckBox::stateChanged, this, &OnlineMap::showBase);
	connect(ui._check_5711, &QCheckBox::stateChanged, this, &OnlineMap::show5711);
	connect(ui._check_merge, &QCheckBox::stateChanged, this, &OnlineMap::showOther);
	_map_file = QString("%1/index.html").arg(QApplication::applicationDirPath());
	_render_functions.emplace_back("renderBase");
	_render_functions.emplace_back("render5711");
	_render_functions.emplace_back("renderOther");
	_clear_functions.emplace_back("clearBase");
	_clear_functions.emplace_back("clear5711");
	_clear_functions.emplace_back("clearOther");
	_visible_functions.emplace_back("showBase");
	_visible_functions.emplace_back("show5711");
	_visible_functions.emplace_back("showOther");
	_data_files.emplace_back("D:/base.csv");
	_data_files.emplace_back("D:/5711.csv");
	_data_files.emplace_back("D:/Other.csv");
	ui._online_map->setUrlFile(_map_file);
}

OnlineMap::~OnlineMap()
{}

struct Gps
{
	Gps(double b, double l)
	{
		lon = l;
		lat = b;
	}
	double lon;
	double lat;
};

static double pi = 3.1415926535897932384626;
//椭球长半径，依据克拉索索夫斯基椭球系数计算
static double a = 6378245.0;
//第一偏心率的平方
static double ee = 0.00669342162296594323;

static double transformLat(double x, double y) {
	double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y
		+ 0.2 * sqrt(abs(x));
	ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
	ret += (20.0 * sin(y * pi) + 40.0 * sin(y / 3.0 * pi)) * 2.0 / 3.0;
	ret += (160.0 * sin(y / 12.0 * pi) + 320 * sin(y * pi / 30.0)) * 2.0 / 3.0;
	return ret;
}

static double transformLon(double x, double y) {
	double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1
		* sqrt(abs(x));
	ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
	ret += (20.0 * sin(x * pi) + 40.0 * sin(x / 3.0 * pi)) * 2.0 / 3.0;
	ret += (150.0 * sin(x / 12.0 * pi) + 300.0 * sin(x / 30.0
		* pi)) * 2.0 / 3.0;
	return ret;
}

/***
 * 判断是否在中国范围之内
 * @param lat
 * @param lon
 * @return
 */
static bool outOfChina(double lat, double lon) {
	if (lon < 72.004 || lon > 137.8347)
		return true;
	if (lat < 0.8293 || lat > 55.8271)
		return true;
	return false;
}

static Gps transform(double lat, double lon) {
	if (outOfChina(lat, lon)) {
		return Gps(lat, lon);
	}
	double dLat = transformLat(lon - 105.0, lat - 35.0);
	double dLon = transformLon(lon - 105.0, lat - 35.0);
	double radLat = lat / 180.0 * pi;
	double magic = sin(radLat);
	magic = 1 - ee * magic * magic;
	double sqrtMagic = sqrt(magic);
	dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
	dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
	double mgLat = lat + dLat;
	double mgLon = lon + dLon;
	return Gps(mgLat, mgLon);
}

static Gps wgs84_To_Gcj02(double lat, double lon) {
	return transform(lat, lon);
}

static Gps gcj02_To_Bd09(double gg_lat, double gg_lon) {
	double x = gg_lon, y = gg_lat;
	double z = sqrt(x * x + y * y) + 0.00002 * sin(y * pi);
	double theta = atan2(y, x) + 0.000003 * cos(x * pi);
	double bd_lon = z * cos(theta) + 0.0065;
	double bd_lat = z * sin(theta) + 0.006;
	return Gps(bd_lat, bd_lon);
}

void OnlineMap::settings()
{
	ParamSetting setting;
	setting.initDialog(_data_files, _lon_precise, _lat_precise);
	if (QDialog::Accepted == setting.exec())
	{
		_data_files = setting.dataFiles();
		_lon_precise = setting.lonPrecise();
		_lat_precise = setting.latPrecise();
		saveConfig();
	}
}

void OnlineMap::loadData()
{
	ui._online_map->setUrlFile(_map_file);
}

void OnlineMap::clearData()
{
	for (size_t i = 0; i < _clear_functions.size(); ++i)
	{
		QString script = QString("%1()")
			.arg(_clear_functions.at(i));
		ui._online_map->runJavaScript(script);
	}
}

void OnlineMap::loadHtmlFinined()
{
	//读取数据
	std::vector<std::vector<QPointF>> load_points;
	int max_point = 0;
	for (size_t i = 0; i < _data_files.size(); ++i)
	{
		QFile file(_data_files.at(i));
		std::vector<QPointF> file_data;
		if (file.open(QIODevice::ReadOnly))
		{
			while (!file.atEnd())
			{
				QStringList string_list = QString::fromLocal8Bit(file.readLine()).split(",");
				if (string_list.size() > 1)
				{
					QPointF pos = QPointF(string_list.at(0).toDouble(), string_list.at(1).toDouble());
					if (pos.x() > 0 && pos.y() > 0)
					{
						file_data.emplace_back(pos);
					}
					else
					{
						int m = 0;
					}
				}
			}
			file.close();
		}

		if (0 == max_point || file_data.size() < max_point)
		{
			max_point = file_data.size();
		}
		load_points.emplace_back(file_data);
	}

	//加载数据
	for (size_t i = 0; i < load_points.size(); ++i)
	{
		Gps last_gps = Gps(0, 0);
		for (size_t j = 0; j < load_points.at(i).size(); ++j)
		{
			Gps gps = wgs84_To_Gcj02(load_points.at(i).at(j).x(), load_points.at(i).at(j).y());
			if (fabs(gps.lon - last_gps.lon) < _lon_precise || fabs(gps.lat - last_gps.lat) < _lat_precise)
			{
				continue;
			}
			last_gps = gps;

			QString script = QString("%1(%2,%3)")
				.arg(_render_functions.at(i))
				.arg(gps.lon, 0, 'f', 10)
				.arg(gps.lat, 0, 'f', 10);
			ui._online_map->runJavaScript(script);
		}
	}
}

void OnlineMap::dataConvert()
{
	QString file_name = QFileDialog::getOpenFileName(this, tr("Open"), "", "*(*.*)");
	if (file_name.isEmpty())
	{
		return;
	}

	QFile file(file_name);
	if (!file.open(QIODevice::ReadOnly))
	{
		return;
	}
	QString data = file.readAll();
	file.close();
	data = data.replace("	", ",");
	data = data.replace(" ", ",");
	QString save_file_name = QString("%1.csv").arg(file_name);
	QFile save_file(save_file_name);
	if (!save_file.open(QIODevice::WriteOnly))
	{
		return;
	}
	save_file.write(data.toLocal8Bit());
	save_file.close();
}

void OnlineMap::showBase(int is_visible)
{
	QString script = QString("%1(%2)").arg(_visible_functions.at(0)).arg(0 != is_visible ? "1" : "0");
	ui._online_map->runJavaScript(script);
}

void OnlineMap::show5711(int is_visible)
{
	QString script = QString("%1(%2)").arg(_visible_functions.at(1)).arg(0 != is_visible ? "1" : "0");
	ui._online_map->runJavaScript(script);
}

void OnlineMap::showOther(int is_visible)
{
	QString script = QString("%1(%2)").arg(_visible_functions.at(2)).arg(0 != is_visible ? "1" : "0");
	ui._online_map->runJavaScript(script);
}

void OnlineMap::saveConfig()
{
	QSettings setting("config.ini", QSettings::IniFormat);
	setting.setValue("LatPrecise", _lat_precise);
	setting.setValue("LonPrecise", _lon_precise);
	setting.beginWriteArray("Files");
	for (size_t i = 0; i < _data_files.size(); ++i)
	{
		setting.setArrayIndex(i);
		setting.setValue("File", _data_files.at(i));
	}
	setting.endArray();
}

void OnlineMap::readConfig()
{
	QSettings setting("config.ini", QSettings::IniFormat);
	_lat_precise = setting.value("LatPrecise", _lat_precise).toDouble();
	_lon_precise = setting.value("LonPrecise", _lon_precise).toDouble();
	_data_files.clear();
	int size = setting.beginReadArray("Files");
	for (size_t i = 0; i < size; ++i)
	{
		setting.setArrayIndex(i);
		_data_files.emplace_back(setting.value("File").toString());
	}
	setting.endArray();
}