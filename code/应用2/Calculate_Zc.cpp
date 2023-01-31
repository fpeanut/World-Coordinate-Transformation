#include "Calculate_Zc.h"

Calculate_Zc::Calculate_Zc(QWidget *parent)
	: QWidget(parent),
	ui(new Ui::Calculate_ZcClass)
{
	ui->setupUi(this);
	ui->closebtn->setEnabled(false);

}

Calculate_Zc::~Calculate_Zc() {
	delete ui;
}
//config��������
void Calculate_Zc::ConfigGet() {
	QString config_path_ = "config.ini";
	QSettings *configRead_ = new QSettings(config_path_, QSettings::IniFormat);
	u0 = configRead_->value("/param/u0").toDouble();
	v0 = configRead_->value("/param/v0").toDouble();
	fx = configRead_->value("/param/fx").toDouble();
	fy = configRead_->value("/param/fy").toDouble();
	deta_d = configRead_->value("/param/d").toInt();
	//qDebug() << u0 << v0;
}
//Zc����궨����
void Calculate_Zc::ZcInit(Mat &img, double &zc) {
	Mat img_gray;
	cvtColor(img, img_gray, COLOR_BGR2GRAY);
	vector<cv::Point2f> corners;//�洢�ǵ�
	//���ǵ㣬ret=1��⵽
	bool ret = cv::findChessboardCorners(img_gray,
		cv::Size(3, 3),
		corners,
		cv::CALIB_CB_ADAPTIVE_THRESH |
		cv::CALIB_CB_NORMALIZE_IMAGE);
	//�������z
	float deta_u, deta_v;
	if (ret) {
		Point2f pix1 = corners.at(0);
		Point2f pix2 = corners.at(1);
		deta_u = pix1.x - pix2.x;
		deta_v = pix1.y - pix2.y;
		cv::circle(img, pix1, 5, cv::Scalar(255, 0, 0), -1);
		cv::circle(img, pix2, 5, cv::Scalar(255, 0, 0), -1);
		zc = deta_d / sqrtf(((deta_u*deta_u) / (fx*fx)) + ((deta_v*deta_v) / (fy*fy)));

		if (zc > 2000) {
			zc = 850;
		}
	}
	else if (!ret)
	{
		zc = 850;
	}
}
//�ؼ�����
//�����
void Calculate_Zc::opencam_clicked() {
	ui->closebtn->setEnabled(true);
	//��ȡ�ڲ�
	fx = ui->fxedt->toPlainText().toDouble();
	fy = ui->fyedt->toPlainText().toDouble();
	u0 = ui->u0edt->toPlainText().toDouble();
	v0 = ui->v0edt->toPlainText().toDouble();
	deta_d = ui->detadedt->toPlainText().toFloat();
	camid = ui->Idedt->toPlainText().toInt();
	fps = ui->waitedt->toPlainText().toInt();

	if (camid == 0) {
		cap.open(camid);
	}
	else if (camid == 1) {
		cap.open(camid);
	}
	else if (camid==-1) {
		cap.open(cam_id_s);
	}
	while (1) {
		cap >> frame;
		if (frame.empty())
			break;
		ZcInit(frame, Zc);
		ui->zcedt->setText(QString::number(Zc));
		cv::Mat img;
		frame.copyTo(img);
		cv::resize(img, img, cv::Size(1280, 720));
		cv::imshow("frame", img);
		cv::waitKey(fps);
	}
}
//�ر����
void Calculate_Zc::closecam_clicked() {
	cap.release();
	destroyAllWindows();
}
//�رճ���
void Calculate_Zc::closeui_clicked() {
	QString exeFilePath = QCoreApplication::applicationFilePath(); //��ȡexe����ȫ·��
	QString exeFileName = QFileInfo(exeFilePath).fileName(); //��ȡexe������

	QProcess p;
	QString c = "taskkill /im " + exeFileName + " /f";    //exeFileNameΪҪɱ���Ľ�����
	p.execute(c);
	p.close();
}
//������Ƶ�ļ�
void Calculate_Zc::getdir_clicked() {
	
	if (ui->Idedt->toPlainText().toInt()==-1) {
		QString directory = QFileDialog::getOpenFileName(this, tr(""), "", tr("*.mp4;;*.avi;;All Files (*.*)"));//�ļ�·��
		if (!directory.isEmpty())
			cam_id_s = directory.toStdString();
		
	}
}
//config��ʼ��
void Calculate_Zc::Config_init() {
	ConfigGet();
	ui->fxedt->setText(QString::number(fx));
	ui->fyedt->setText(QString::number(fy));
	ui->u0edt->setText(QString::number(u0));
	ui->v0edt->setText(QString::number(v0));
	ui->detadedt->setText(QString::number(deta_d));
}
