#include "paramsetting.h"
#include <QFileDialog>
#include <QMessageBox>

#pragma execution_character_set("utf-8")

ParamSetting::ParamSetting(QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	ui._lat_precise->setToolTip("��ֵԽС,��Ⱦ���صĵ���Խ��,��ʱԽ��");
	ui._lon_precise->setToolTip("��ֵԽС,��Ⱦ���صĵ���Խ��,��ʱԽ��");
	connect(ui._button_base, &QPushButton::clicked, this, &ParamSetting::selectBase);
	connect(ui._button_5711, &QPushButton::clicked, this, &ParamSetting::select5711);
	connect(ui._button_merge, &QPushButton::clicked, this, &ParamSetting::selectMerge);
	connect(ui._button_ok, &QPushButton::clicked, this, &ParamSetting::saveData);
	connect(ui._button_cancel, &QPushButton::clicked, this, &ParamSetting::reject);
}

ParamSetting::~ParamSetting()
{}

void ParamSetting::initDialog(const std::vector<QString>& files, double lon_precise, double lat_precise)
{
	if (files.size() > 0)
	{
		ui._line_base_file->setText(files.at(0));
	}
	if (files.size() > 1)
	{
		ui._line_5711_file->setText(files.at(1));
	}

	if (files.size() > 2)
	{
		ui._line_merge_file->setText(files.at(2));
	}
	ui._lat_precise->setText(QString("%1").arg(lat_precise));
	ui._lon_precise->setText(QString("%1").arg(lon_precise));
}

void ParamSetting::selectBase()
{
	QString file_name = QFileDialog::getOpenFileName(this
		, tr("ѡ���׼����"), ui._line_base_file->text(), "EXCEL(*.csv)");
	if (!file_name.isEmpty())
	{
		ui._line_base_file->setText(file_name);
	}
}

void ParamSetting::select5711()
{
	QString file_name = QFileDialog::getOpenFileName(this
		, tr("ѡ��5711����"), ui._line_5711_file->text(), "EXCEL(*.csv)");
	if (!file_name.isEmpty())
	{
		ui._line_5711_file->setText(file_name);
	}
}

void ParamSetting::selectMerge()
{
	QString file_name = QFileDialog::getOpenFileName(this
		, tr("ѡ���ں�����"), ui._line_merge_file->text(), "EXCEL(*.csv)");
	if (!file_name.isEmpty())
	{
		ui._line_merge_file->setText(file_name);
	}
}

void ParamSetting::saveData()
{
	bool is_right = false;
	_lon_precise = ui._lon_precise->text().toDouble(&is_right);
	if (!is_right)
	{
		QMessageBox::information(this, tr("��ʾ"), tr("���Ⱦ�����д����ȷ"));
		return;
	}
	_lat_precise = ui._lat_precise->text().toDouble(&is_right);
	if (!is_right)
	{
		QMessageBox::information(this, tr("��ʾ"), tr("γ�Ⱦ�����д����ȷ"));
		return;
	}

	_data_files.clear();
	_data_files.emplace_back(ui._line_base_file->text());
	_data_files.emplace_back(ui._line_5711_file->text());
	_data_files.emplace_back(ui._line_merge_file->text());
	accept();
}