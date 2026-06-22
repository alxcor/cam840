///////////////////////////////////////////////////////////////////////////////
/*! \file   cam840aform.cpp
 *  \author
 *  \date
 *  \brief  Implementation file for class cam840aform
 *
 *  This file is part of the HMI Solutionline.
 *
 */
///////////////////////////////////////////////////////////////////////////////
#include "cam840aform.h"

///////////////////////////////////////////////////////////////////////////////
// DEFINES / CONSTS
///////////////////////////////////////////////////////////////////////////////
//const QString SUCCESS_READ	   = "command read successful";
//const QString SUCCESS_WRITE	   = "command write successful";
//const QString SUCCESS_ADVISE   = "command advise successful";
//const QString SUCCESS_UNADVISE = "command unadvise successful";
const QString ACCESS_ERROR	   = "errorNr: %1";

///////////////////////////////////////////////////////////////////////////////
// CREATORS
///////////////////////////////////////////////////////////////////////////////

cam840aform::cam840aform(QWidget* pParent, const QString& rszName)
				: SlGfwDialogForm(pParent, rszName),
				  m_capNameSpaceNC("/NC/_N_NC_TEA_ACX"),
				  m_capNameSpaceCH("/NC/_N_CH_TEA_ACX"),
				  m_capNameSpaceAX("/NC/_N_AX_TEA_ACX")
{
	b_TargetOK = false;
	int nIndex1 = 0;
	int i;

	//*************************************************************************
	//Initializare dimens. fereastra
	i_dispwidth	= 0;
	//Initializare "status bar"
	setStatusBarElementCount(1);    
	setStatusBarVisible(false);
	setStatusBarElementAlignment(Qt::AlignLeft, 0);
	//Initializare "caption bar"
	setCaptionBarElementCount(2);
	setCaptionBarElementLength(75,0);
	setCaptionBarElementStartPosition(75, 1);
	setCaptionBarElementAlignment(Qt::AlignRight, 1);
	setCaptionBarElementText(tr("alxcor@yahoo.com v220906"), 0);
	setCaptionBarElementText(tr("--:--:--"),1);
	//*************************************************************************
	//Layout general
	QVBoxLayout* MainLayout = new QVBoxLayout(this);
	MainLayout->setMargin(0);
	MainLayout->setSpacing(0);
	QString* strGroupB1Style = new QString(   "QGroupBox { background-color: rgb(245,245,245); color: rgb(0,0,0); border-top: 1px solid white; border-left: 1px solid white; border-right: 1px solid gray; border-bottom: 1px solid gray }");
	QString* strGroupB2Style = new QString(   "QGroupBox { background-color: rgb(245,245,245); color: rgb(0,0,0); border-top:10px solid white; border-left: 1px solid white; border-right: 1px solid gray; border-bottom: 1px solid gray }");
	QString* strAxLabelStyle = new QString(      "QLabel { background-color: rgb(211,211,211); color: rgb(0,0,0); border-top: 1px solid white; border-left: 1px solid white; border-right: 1px solid gray; border-bottom: 1px solid gray }");
	QString* strProgBarStyle = new QString("QProgressBar { background-color: rgb(211,211,211); color: rgb(0,0,0); border-top: 1px solid white; border-left: 1px solid white; border-right: 1px solid gray; border-bottom: 1px solid gray } QProgressBar::chunk { background-color: rgb( 70,130,180); width: 1px;}");
	//*************************************************************************
	//Ecran Setari -> GroupBox
	gbSetari = new QGroupBox();
	gbSetari->setStyleSheet(*strGroupB1Style);
	QVBoxLayout* SetariLayout = new QVBoxLayout(this);
	gbSetari->setLayout(SetariLayout);
	MainLayout->addWidget(gbSetari);
	//Layout pentru Liste
	QGridLayout* axListLayout = new QGridLayout(this);
	//Listele de actiuni
	pListaLog1	= new QListWidget(this);
	pListaLog1->setHmiFonts(true);
	pListaLog1->addItem(tr("NCK: ..."));						//line 0
	pListaLog1->item(0)->setBackgroundColor(QColor(211,211,211));		//line 0
	pListaLog1->addItem(tr("Target..."));						//line 1
	pListaLog1->item(1)->setBackgroundColor(QColor(211,211,211));		//line 1
	pListaLog1->addItem(tr(">channels.../axes..."));			//line 2
	pListaLog1->addItem(tr(">synchr1..."));						//line 3
	pListaLog1->addItem(tr(">synchr2..."));						//line 4
	pListaLog1->addItem(tr(">aeevnt..."));						//line 5
	pListaLog1->addItem(tr(">ini..."));							//line 6
	pListaLog1->addItem(tr("Scouting status..."));				//line 7
	pListaLog2	= new QListWidget(this);
	pListaLog2->setHmiFonts(true);
	pListaLog2->addItem(tr(">Log:"));							//line 0/4
	pListaLog2->item(0)->setBackgroundColor(QColor(211,211,211));		//line 0/4
	iListaLog2 = 1;
	//Liste in Layout Liste
	axListLayout->addWidget(pListaLog1,0, 0);
	axListLayout->addWidget(pListaLog2,0, 1);
	//Layout Liste in MainLayout
	SetariLayout->addLayout(axListLayout);
	//*************************************************************************
	//Ecran Sinoptic -> GroupBox
	gbSinoptic = new QGroupBox();
	gbSinoptic->setStyleSheet(*strGroupB1Style);
	QVBoxLayout* SinopticLayout = new QVBoxLayout(this);
	gbSinoptic->setLayout(SinopticLayout);
	MainLayout->addWidget(gbSinoptic);
	//Layout stare canale
	QGridLayout* chGridLayout  = new QGridLayout(this);
	for (nIndex1 = 0; nIndex1 < 10; nIndex1++){
		pChChNo[nIndex1] = new QLabel(QString("*Ch.%1").arg(nIndex1 + 1), this);
		pChChNo[nIndex1]->setVisible(false);
		pChChNo[nIndex1]->setHmiFonts(true);
		pChChNo[nIndex1]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		pChChNo[nIndex1]->setStyleSheet(*strAxLabelStyle);
		pChChNo[nIndex1]->setStyleSheet("QLabel { background-color: rgb(211,211,211); color: rgb(0,0,0); border-top: 1px solid white; border-left: 1px solid white; border-right: 1px solid gray; border-bottom: 1px solid gray }");
		pChState[nIndex1] = new QLabel(QString("#"), this);
		pChState[nIndex1]->setVisible(false);
		pChState[nIndex1]->setHmiFonts(true);
		pChState[nIndex1]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		pChState[nIndex1]->setStyleSheet("QLabel { background-color: rgb(128,128,128); color: rgb(255,255,255); border-top: 1px solid white; border-left: 1px solid white; border-right: 1px solid gray; border-bottom: 1px solid gray }");
		pChParts[nIndex1] = new QLabel(QString("#"), this);
		pChParts[nIndex1]->setVisible(false);
		pChParts[nIndex1]->setHmiFonts(true);
		pChParts[nIndex1]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		pChParts[nIndex1]->setStyleSheet("QLabel { background-color: rgb( 70,130,180); color: rgb(255,255,255); border-top: 1px solid white; border-left: 1px solid white; border-right: 1px solid gray; border-bottom: 1px solid gray }");
		pChSelWP[nIndex1] = new QLabel(QString("#"), this);
		pChSelWP[nIndex1]->setVisible(false);
		pChSelWP[nIndex1]->setHmiFonts(true);
		pChSelWP[nIndex1]->setToolTip(QString(""));
		pChSelWP[nIndex1]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		pChSelWP[nIndex1]->setStyleSheet("QLabel { background-color: rgb(211,211,211); color: rgb(0,0,0); border-top: 1px solid white; border-left: 1px solid white; border-right: 1px solid gray; border-bottom: 1px solid gray }");
		pChProgN[nIndex1] = new QLabel(QString("#"), this);
		pChProgN[nIndex1]->setToolTip(QString(""));
		pChProgN[nIndex1]->setVisible(false);
		pChProgN[nIndex1]->setHmiFonts(true);
		pChProgN[nIndex1]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		pChProgN[nIndex1]->setStyleSheet("QLabel { background-color: rgb(128,128,128); color: rgb(255,255,255); border-top: 1px solid white; border-left: 1px solid white; border-right: 1px solid gray; border-bottom: 1px solid gray }");
		pChActBl[nIndex1] = new QLabel(QString("#"), this);
		pChActBl[nIndex1]->setVisible(false);
		pChActBl[nIndex1]->setHmiFonts(true);
		pChActBl[nIndex1]->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		pChActBl[nIndex1]->setStyleSheet("QLabel { background-color: rgb(0,0,0); color: rgb(255,255,255); border-top: 1px solid white; border-left: 1px solid white; border-right: 1px solid gray; border-bottom: 1px solid gray }");
		chGridLayout->addWidget(pChChNo[nIndex1] , (nIndex1 * 2), 0);
		chGridLayout->addWidget(pChSelWP[nIndex1], (nIndex1 * 2), 1, 1, 3);
		chGridLayout->addWidget(pChState[nIndex1], (nIndex1 * 2) + 1, 0);
		chGridLayout->addWidget(pChParts[nIndex1], (nIndex1 * 2) + 1, 1);
		chGridLayout->addWidget(pChProgN[nIndex1], (nIndex1 * 2) + 1, 2);
		chGridLayout->addWidget(pChActBl[nIndex1], (nIndex1 * 2) + 1, 3);
		chGridLayout->setSpacing(0);
		chGridLayout->setColumnStretch(0, 1);
		chGridLayout->setColumnStretch(1, 1);
		chGridLayout->setColumnStretch(2, 2);
		chGridLayout->setColumnStretch(3, 5);
	}
	SinopticLayout->addLayout(chGridLayout);
	//Layout Axe
	axGridLayout = new QGridLayout(this);
	for (nIndex1 = 0; nIndex1 < 31; nIndex1++){
		pAxNameLbl1[nIndex1] = new QLabel(QString(" %1").arg(nIndex1 + 1), this);
		pAxNameLbl1[nIndex1]->setVisible(false);
		pAxNameLbl1[nIndex1]->setHmiFonts(true);
		pAxNameLbl1[nIndex1]->setHmiUseBigFont(false);
		pAxNameLbl1[nIndex1]->setStyleSheet(*strAxLabelStyle);
		pAxNameLbl2[nIndex1] = new QLabel(QString("g"), this);
		pAxNameLbl2[nIndex1]->setVisible(false);
		pAxNameLbl2[nIndex1]->setHmiFonts(true);
		pAxNameLbl2[nIndex1]->setHmiUseBigFont(false);
		pAxNameLbl2[nIndex1]->setStyleSheet(*strAxLabelStyle);
		pAxNameLbl3[nIndex1] = new QLabel(QString("G"), this);
		pAxNameLbl3[nIndex1]->setVisible(false);
		pAxNameLbl3[nIndex1]->setHmiFonts(true);
		pAxNameLbl3[nIndex1]->setHmiUseBigFont(false);
		pAxNameLbl3[nIndex1]->setStyleSheet(*strAxLabelStyle);
		pAxNameLbl4[nIndex1] = new QLabel(QString("c"), this);
		pAxNameLbl4[nIndex1]->setVisible(false);
		pAxNameLbl4[nIndex1]->setHmiFonts(true);
		pAxNameLbl4[nIndex1]->setHmiUseBigFont(false);
		pAxNameLbl4[nIndex1]->setStyleSheet(*strAxLabelStyle);
		pAxNameLbl5[nIndex1] = new QLabel(QString("C"), this);
		pAxNameLbl5[nIndex1]->setVisible(false);
		pAxNameLbl5[nIndex1]->setHmiFonts(true);
		pAxNameLbl5[nIndex1]->setHmiUseBigFont(false);
		pAxNameLbl5[nIndex1]->setStyleSheet(*strAxLabelStyle);
		pAxNameLbl6[nIndex1] = new QLabel(QString("#"), this);
		pAxNameLbl6[nIndex1]->setVisible(false);
		pAxNameLbl6[nIndex1]->setHmiFonts(true);
		pAxNameLbl6[nIndex1]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		pAxNameLbl6[nIndex1]->setHmiUseBigFont(false);
		pAxNameLbl6[nIndex1]->setStyleSheet(*strAxLabelStyle);
		bAxNameLbl61[nIndex1] = false;
		pAxNameLbl7[nIndex1] = new QLabel(QString("m"), this);
		pAxNameLbl7[nIndex1]->setVisible(false);
		pAxNameLbl7[nIndex1]->setHmiFonts(true);
		pAxNameLbl7[nIndex1]->setHmiUseBigFont(false);
		pAxNameLbl7[nIndex1]->setStyleSheet(*strAxLabelStyle);
		pAxNameLbl8[nIndex1] = new QLabel(QString("M"), this);
		pAxNameLbl8[nIndex1]->setVisible(false);
		pAxNameLbl8[nIndex1]->setHmiFonts(true);
		pAxNameLbl8[nIndex1]->setHmiUseBigFont(false);
		pAxNameLbl8[nIndex1]->setStyleSheet(*strAxLabelStyle);
		pAxNameLbl9[nIndex1] = new QLabel(QString("#"), this);
		pAxNameLbl9[nIndex1]->setVisible(false);
		pAxNameLbl9[nIndex1]->setHmiFonts(true);
		pAxNameLbl9[nIndex1]->setHmiUseBigFont(false);
		pAxNameLbl9[nIndex1]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		pAxNameLbl9[nIndex1]->setStyleSheet(*strAxLabelStyle);
		pAxNameLbla[nIndex1] = new QLabel(QString("#"), this);
		pAxNameLbla[nIndex1]->setVisible(false);
		pAxNameLbla[nIndex1]->setHmiFonts(true);
		pAxNameLbla[nIndex1]->setHmiUseBigFont(false);
		pAxNameLbla[nIndex1]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		pAxNameLbla[nIndex1]->setStyleSheet(*strAxLabelStyle);
		//Aranjare in Layout Axe
		axGridLayout->addWidget(pAxNameLbl1[nIndex1] ,nIndex1 ,0);
		axGridLayout->addWidget(pAxNameLbl2[nIndex1] ,nIndex1 ,1);
		axGridLayout->addWidget(pAxNameLbl3[nIndex1] ,nIndex1 ,2);
		axGridLayout->addWidget(pAxNameLbl4[nIndex1] ,nIndex1 ,3);
		axGridLayout->addWidget(pAxNameLbl5[nIndex1] ,nIndex1 ,4);
		axGridLayout->addWidget(pAxNameLbl6[nIndex1] ,nIndex1 ,5);
		axGridLayout->addWidget(pAxNameLbl7[nIndex1] ,nIndex1 ,6);
		axGridLayout->addWidget(pAxNameLbl8[nIndex1] ,nIndex1 ,7);
		axGridLayout->addWidget(pAxNameLbl9[nIndex1] ,nIndex1 ,8);
		axGridLayout->addWidget(pAxNameLbla[nIndex1] ,nIndex1 ,9);
	}
	SinopticLayout->addLayout(axGridLayout);
	//Layout Arbori
	spGridLayout = new QGridLayout(this);
	for (nIndex1 = 0; nIndex1 < 10; nIndex1++){
		pSpNameLbl1[nIndex1] = new QLabel(QString(" %1").arg(nIndex1 + 1), this); //canal no.
		pSpNameLbl1[nIndex1]->setVisible(false);
		pSpNameLbl1[nIndex1]->setHmiFonts(true);
		pSpNameLbl1[nIndex1]->setHmiUseBigFont(false);
		pSpNameLbl1[nIndex1]->setStyleSheet(*strAxLabelStyle);
		pSpNameLbl2[nIndex1] = new QLabel(QString(".%1").arg(nIndex1 + 1), this); //spindle no.
		pSpNameLbl2[nIndex1]->setVisible(false);
		pSpNameLbl2[nIndex1]->setHmiFonts(true);
		pSpNameLbl2[nIndex1]->setHmiUseBigFont(false);
		pSpNameLbl2[nIndex1]->setStyleSheet(*strAxLabelStyle);
		pSpNameLbl3[nIndex1] = new QLabel(QString("S"), this); //MCS ax. name
		pSpNameLbl3[nIndex1]->setVisible(false);
		pSpNameLbl3[nIndex1]->setHmiFonts(true);
		pSpNameLbl3[nIndex1]->setHmiUseBigFont(false);
		pSpNameLbl3[nIndex1]->setStyleSheet(*strAxLabelStyle);
		pSpNameLbl4[nIndex1] = new QLabel(QString("s"), this); //pos [deg]
		pSpNameLbl4[nIndex1]->setVisible(false);
		pSpNameLbl4[nIndex1]->setHmiFonts(true);
		pSpNameLbl4[nIndex1]->setHmiUseBigFont(false);
		pSpNameLbl4[nIndex1]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		pSpNameLbl4[nIndex1]->setStyleSheet(*strAxLabelStyle);
		pSpNameLbl5[nIndex1] = new QLabel(QString("s"), this); //speed [rpm]
		pSpNameLbl5[nIndex1]->setVisible(false);
		pSpNameLbl5[nIndex1]->setHmiFonts(true);
		pSpNameLbl5[nIndex1]->setHmiUseBigFont(false);
		pSpNameLbl5[nIndex1]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		pSpNameLbl5[nIndex1]->setStyleSheet(*strAxLabelStyle);
		pSpLoad[nIndex1] = new QProgressBar(this);
		pSpLoad[nIndex1]->setMinimum(0);
		pSpLoad[nIndex1]->setMaximum(100);
		pSpLoad[nIndex1]->setTextVisible(true);
		pSpLoad[nIndex1]->setHmiFonts(true);
		pSpLoad[nIndex1]->setAlignment(Qt::AlignCenter);
		pSpLoad[nIndex1]->setVisible(false);
		pSpLoad[nIndex1]->setStyleSheet(*strProgBarStyle);
		spGridLayout->addWidget(pSpNameLbl1[nIndex1] ,nIndex1 ,0);
		spGridLayout->addWidget(pSpNameLbl2[nIndex1] ,nIndex1 ,1);
		spGridLayout->addWidget(pSpNameLbl3[nIndex1] ,nIndex1 ,2);
		spGridLayout->addWidget(pSpNameLbl4[nIndex1] ,nIndex1 ,3);
		spGridLayout->addWidget(pSpNameLbl5[nIndex1] ,nIndex1 ,4);
		spGridLayout->addWidget(pSpLoad[nIndex1] ,nIndex1 ,5);
	}
	spGridLayout->setColumnMinimumWidth(0, 20);
	spGridLayout->setColumnMinimumWidth(1, 20);
	spGridLayout->setColumnMinimumWidth(2, 20);	
	spGridLayout->setColumnMinimumWidth(3, 150);
	spGridLayout->setColumnMinimumWidth(4, 200);
	spGridLayout->setColumnMinimumWidth(5, 300);
	SinopticLayout->addLayout(spGridLayout);
	//Ecran Setari -> GroupBox Setari
	gbSinopticSett = new QGroupBox(this);
	gbSinopticSett->setStyleSheet(*strGroupB1Style);
	gbSinopticSett->setHmiFonts(true);
	QGridLayout* SinopticSettLayout = new QGridLayout(this);
	SinopticSettLayout->setSpacing(0);
	gbSinopticSett->setLayout(SinopticSettLayout);
	MainLayout->addWidget(gbSinopticSett);
	bSinopticSett = false;
	bSinopticChart = false;
	chk_afis_chan	= new QCheckBox(tr("Channels"), this);
	chk_afis_chan->setHmiFonts(true);
	chk_afis_nosp	= new QCheckBox(tr("Compr."), this);
	chk_afis_nosp->setHmiFonts(true);
	chk_afis_nosp->setToolTip(tr("No spaces..."));
	//chk_afis_log	= new QCheckBox(tr("Jurnal"), this);
	chk_afis_axes	= new QCheckBox(tr("Axes"), this);
	chk_afis_axes->setHmiFonts(true);
	spn_afis_axno	= new QSpinBox(this);
	spn_afis_axno->setRange(-1, 31);
	spn_afis_axno->setHmiFonts(true);
	chk_afis_zoom	= new QCheckBox(tr("Zoom"), this);
	chk_afis_zoom->setHmiFonts(true);
	chk_afis_spin	= new QCheckBox(tr("Spindles"), this);
	chk_afis_spin->setHmiFonts(true);
	spn_afis_spno	= new QSpinBox(this);
	spn_afis_spno->setRange(-1, 31);
	spn_afis_spno->setHmiFonts(true);
	led_afis_axes	= new QLineEdit("", this);
	led_afis_axes->setAlignment(Qt::AlignRight);
	led_afis_axes->setHmiFonts(true);
	led_afis_time	= new QLineEdit("", this);
	led_afis_time->setAlignment(Qt::AlignRight);
	led_afis_time->setHmiFonts(true);
	led_afis_time->setDisabled(true);
	led_afis_time->setFixedWidth(50);
	SinopticSettLayout->addWidget(chk_afis_chan,  0, 0);
	SinopticSettLayout->addWidget(chk_afis_axes,  0, 1);
	SinopticSettLayout->addWidget(spn_afis_axno,  0, 2);
	SinopticSettLayout->addWidget(led_afis_axes,  0, 3);
	SinopticSettLayout->addWidget(chk_afis_spin,  0, 4);
	SinopticSettLayout->addWidget(spn_afis_spno,  0, 5);
	SinopticSettLayout->addWidget(chk_afis_nosp,  0, 6);
	SinopticSettLayout->addWidget(chk_afis_zoom,  0, 7);
	SinopticSettLayout->addWidget(led_afis_time,  0, 8);
	//*************************************************************************
	//Ecran Monitor -> GroupBox
	gbMonitor = new QGroupBox();
	gbMonitor->setStyleSheet(*strGroupB1Style);
	gbMonitor->setHmiFonts(true);
	QVBoxLayout* MonitorLayout = new QVBoxLayout(this);
	MonitorLayout->setSpacing(0);
	gbMonitor->setLayout(MonitorLayout);
	MainLayout->addWidget(gbMonitor);
	//Layout Tabel si Grafic
	QHBoxLayout* axMonitorLayout = new QHBoxLayout(this);
	table_ch2 = new QTableView(this);
	chart_ch2 = new ChartXY(this);
	chart_ch2->setHmiFonts(true);
	table_ch2->setHmiColors(true);
	table_ch2->setVisible(false);
	//model pt. grafic
	model_ch2=new QStandardItemModel(1,3,this);
	model_ch2->setHeaderData(0,Qt::Horizontal,tr("0"));
	model_ch2->setHeaderData(1,Qt::Horizontal,tr("1"));
	model_ch2->setHeaderData(2,Qt::Horizontal,tr("2"));
	i_monitorpos = 0;
	//Initializare Model2 cu date din burta
    for(i=0;i<model_ch2->rowCount();i++)
    {
		model_ch2->setData(model_ch2->index(i,0,QModelIndex()), i);
		model_ch2->setData(model_ch2->index(i,1,QModelIndex()), 0.0);
		model_ch2->setData(model_ch2->index(i,1,QModelIndex()),QColor(212, 45, 47),Qt::DecorationRole);
		model_ch2->setData(model_ch2->index(i,1,QModelIndex()), 0.0);
		model_ch2->setData(model_ch2->index(i,1,QModelIndex()),QColor(119,160, 71),Qt::DecorationRole);
    }
	//Alocare Model la tabel si grafic
	table_ch2->setModel(model_ch2);
	chart_ch2->setModel(model_ch2);
	//Layout Monitor
	axMonitorLayout->addWidget(table_ch2);
	axMonitorLayout->addWidget(chart_ch2);
	//Layout Monitor in MainLayout
	MonitorLayout->addLayout(axMonitorLayout);
	//Ecran Monitor -> GroupBox Variabile
	gbMonitorVar = new QGroupBox();
	gbMonitorVar->setStyleSheet(*strGroupB1Style);
	gbMonitorVar->setHmiFonts(true);
	QHBoxLayout* MonitorVarLayout = new QHBoxLayout(this);
	MonitorVarLayout->setSpacing(0);
	gbMonitorVar->setLayout(MonitorVarLayout);
	MainLayout->addWidget(gbMonitorVar);
	for (nIndex1 = 0; nIndex1 < 2; nIndex1++){
		pLblMonVar[nIndex1] = new QLabel(QString("#"), this);
		pLblMonVar[nIndex1]->setVisible(true);
		pLblMonVar[nIndex1]->setHmiFonts(true);
		pLblMonVar[nIndex1]->setHmiUseBigFont(false);
		pLblMonVar[nIndex1]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		pLblMonVar[nIndex1]->setFrameStyle(QFrame::Panel | QFrame::Raised);
		pVarMonVar[nIndex1] = new QVariant();
		MonitorVarLayout->addWidget(pLblMonVar[nIndex1]);
	}
	pLblMonVar[0]->setStyleSheet("QLabel { background-color: rgb(212, 45, 47); color: rgb(255,255,255)}");
	pLblMonVar[1]->setStyleSheet("QLabel { background-color: rgb(119,160, 71); color: rgb(255,255,255)}");

	//*************************************************************************
	//Ecran Chart -> GroupBox
	gbChart = new QGroupBox();
	gbChart->setStyleSheet(*strGroupB1Style);
	gbChart->setHmiFonts(true);
	QVBoxLayout* ChartLayout = new QVBoxLayout(this);
	ChartLayout->setSpacing(0);
	gbChart->setLayout(ChartLayout);
	MainLayout->addWidget(gbChart);
	//Layout Tabel si Grafic
	QHBoxLayout* axChartLayout = new QHBoxLayout(this);
	table_ch1 = new QTableView(this);
	chart_ch1 = new ChartXY(this);
	chart_ch1->setHmiFonts(true);
	table_ch1->setHmiColors(true);
	table_ch1->setVisible(false);
	//model pt. grafic
	model_ch1=new QStandardItemModel(51,5,this);
	model_ch1->setHeaderData(0,Qt::Horizontal,tr("0"));
	model_ch1->setHeaderData(1,Qt::Horizontal,tr("1"));
	model_ch1->setHeaderData(2,Qt::Horizontal,tr("2"));
	model_ch1->setHeaderData(3,Qt::Horizontal,tr("3"));
	model_ch1->setHeaderData(3,Qt::Horizontal,tr("4"));
	i_chartpos = 0;
	//Initializare Model cu date din burta
    for(i=0;i<model_ch1->rowCount();i++)
    {
		model_ch1->setData(model_ch1->index(i,0,QModelIndex()), i);
		model_ch1->setData(model_ch1->index(i,1,QModelIndex()), 0.0);
		model_ch1->setData(model_ch1->index(i,1,QModelIndex()),QColor(212, 45, 47),Qt::DecorationRole);
		model_ch1->setData(model_ch1->index(i,2,QModelIndex()), 0.0);
		model_ch1->setData(model_ch1->index(i,2,QModelIndex()),QColor(119,160, 71),Qt::DecorationRole);
		model_ch1->setData(model_ch1->index(i,3,QModelIndex()), 0.0);
		model_ch1->setData(model_ch1->index(i,3,QModelIndex()),QColor( 70,130,180),Qt::DecorationRole);
		model_ch1->setData(model_ch1->index(i,4,QModelIndex()), 0.0);
		model_ch1->setData(model_ch1->index(i,4,QModelIndex()),QColor(239,246,123),Qt::DecorationRole);
    }
	//Alocare Model la tabel si grafic
	table_ch1->setModel(model_ch1);
	chart_ch1->setModel(model_ch1);
	//Layout Chart
	axChartLayout->addWidget(table_ch1);
	axChartLayout->addWidget(chart_ch1);
	//Layout Chart in MainLayout
	ChartLayout->addLayout(axChartLayout);
	//Ecran Chart -> GroupBox Variabile
	gbChartVar = new QGroupBox();
	gbChartVar->setStyleSheet(*strGroupB1Style);
	gbChartVar->setHmiFonts(true);
	QHBoxLayout* ChartVarLayout = new QHBoxLayout(this);
	ChartVarLayout->setSpacing(0);
	gbChartVar->setLayout(ChartVarLayout);
	MainLayout->addWidget(gbChartVar);
	for (nIndex1 = 0; nIndex1 < 4; nIndex1++){
		pLblNcVar[nIndex1] = new QLabel(QString("#"), this);
		pLblNcVar[nIndex1]->setVisible(true);
		pLblNcVar[nIndex1]->setHmiFonts(true);
		pLblNcVar[nIndex1]->setHmiUseBigFont(false);
		pLblNcVar[nIndex1]->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
		pLblNcVar[nIndex1]->setFrameStyle(QFrame::Panel | QFrame::Raised);
		pVarNcVar[nIndex1] = new QVariant();
		ChartVarLayout->addWidget(pLblNcVar[nIndex1]);
	}
	pLblNcVar[0]->setStyleSheet("QLabel { background-color: rgb(212, 45, 47); color: rgb(255,255,255)}");
	pLblNcVar[1]->setStyleSheet("QLabel { background-color: rgb(119,160, 71); color: rgb(255,255,255)}");
	pLblNcVar[2]->setStyleSheet("QLabel { background-color: rgb( 70,130,180); color: rgb(255,255,255)}");
	pLblNcVar[3]->setStyleSheet("QLabel { background-color: rgb(239,246,123); color: rgb(255,255,255)}");
	//Layout Variabile in MainLayout
	//ChartLayout->addLayout(axVarLayout);

	//Ecran Chart -> GroupBox Setari
	gbChartSett = new QGroupBox(this);
	gbChartSett->setStyleSheet(*strGroupB1Style);
	gbChartSett->setHmiFonts(true);
	QGridLayout* ChartSettLayout = new QGridLayout(this);
	ChartSettLayout->setSpacing(0);
	gbChartSett->setLayout(ChartSettLayout);
	MainLayout->addWidget(gbChartSett);
	bChartSett = false;
	chk_ch1_afisv1	= new QCheckBox(tr("NC_Val"), this);
	chk_ch1_afisv1->setHmiFonts(true);
	chk_ch1_afchart	= new QCheckBox(tr("Graphic"), this);
	chk_ch1_afchart->setHmiFonts(true);
	chk_ch1_aftable	= new QCheckBox(tr("Table"), this);
	chk_ch1_aftable->setHmiFonts(true);
	QLabel *lbl_ch1_varM0	= new QLabel(tr("M Start"), this);
	lbl_ch1_varM0->setHmiFonts(true);
	led_ch1_varM[0]	= new QLineEdit("", this);
	led_ch1_varM[0]->setHmiFonts(true);
	QLabel *lbl_ch1_varM1	= new QLabel(tr("M Stop"), this);
	lbl_ch1_varM1->setHmiFonts(true);
	led_ch1_varM[1]	= new QLineEdit("", this);
	led_ch1_varM[1]->setHmiFonts(true);
	led_ch1_valM = new QLabel();
	led_ch1_valM->setHmiFonts(true);
	b_ch1_varM[0] = false;
	b_ch1_varM[1] = false;
	b_ch1_varMold[0] = false;
	b_ch1_varMold[1] = false;
	led_ch1_var[0]	= new QLineEdit("", this);
	led_ch1_var[0]->setHmiFonts(true);
	led_ch1_var[0]->setMinimumWidth(300);
	led_ch1_var[1]	= new QLineEdit("", this);
	led_ch1_var[1]->setHmiFonts(true);
	led_ch1_var[1]->setMinimumWidth(300);
	led_ch1_var[2]	= new QLineEdit("", this);
	led_ch1_var[2]->setHmiFonts(true);
	led_ch1_var[2]->setMinimumWidth(300);
	led_ch1_var[3]	= new QLineEdit("", this);
	led_ch1_var[3]->setHmiFonts(true);
	led_ch1_var[3]->setMinimumWidth(300);
	spn_ch1_fx_min_y	= new QSpinBox(this);
	spn_ch1_fx_min_y->setRange(-1000,1000);
	spn_ch1_fx_min_y->setSingleStep(10);
	spn_ch1_fx_min_y->setHmiFonts(true);
	spn_ch1_fx_max_y	= new QSpinBox(this);
	spn_ch1_fx_max_y->setRange(-1000,1000);
	spn_ch1_fx_max_y->setSingleStep(10);
	spn_ch1_fx_max_y->setHmiFonts(true);
	spn_ch1_trd	= new QSpinBox(this);
	spn_ch1_trd->setRange(1,1000);
	spn_ch1_trd->setSingleStep(10);
	spn_ch1_trd->setHmiFonts(true);
	QLabel *lbl_treadch1	= new QLabel(tr("ms"), this);
	lbl_treadch1->setHmiFonts(true);
	spn_ch1_size	= new QSpinBox(this);
	spn_ch1_size->setRange(10, 1000);
	spn_ch1_size->setSingleStep(10);
	spn_ch1_size->setHmiFonts(true);
	QLabel *lbl_ch1_size	= new QLabel(tr("points(*)"), this);
	lbl_ch1_size->setHmiFonts(true);
	for (nIndex1 = 0; nIndex1 < 4; nIndex1++){
		led_ch1_time[nIndex1] = new QLineEdit("", this);
		led_ch1_time[nIndex1]->setAlignment(Qt::AlignRight);
		led_ch1_time[nIndex1]->setHmiFonts(true);
		led_ch1_time[nIndex1]->setDisabled(true);
		led_ch1_time[nIndex1]->setFixedWidth(50);
	}
	ChartSettLayout->addWidget(chk_ch1_afisv1  , 0, 0, 1, 1); //NC_Val On/Off
	ChartSettLayout->addWidget(lbl_ch1_varM0   , 0, 1, 1, 1); //Label M Start
	ChartSettLayout->addWidget(led_ch1_varM[0] , 0, 2, 1, 2); //LineEdit M Start
	ChartSettLayout->addWidget(led_ch1_var[0]  , 0, 4, 1, 1); //Variabila 0
	ChartSettLayout->addWidget(led_ch1_time[0] , 0, 5, 1, 1); //Timp Refresh multiadvice2
	ChartSettLayout->addWidget(chk_ch1_afchart , 1, 0, 1, 1); //Chart On/Off
	ChartSettLayout->addWidget(lbl_ch1_varM1   , 1, 1, 1, 1); //Label M Stop
	ChartSettLayout->addWidget(led_ch1_varM[1] , 1, 2, 1, 2); //LineEdit M Stop
	ChartSettLayout->addWidget(led_ch1_var[1]  , 1, 4, 1, 1); //Variabila 1
	ChartSettLayout->addWidget(led_ch1_time[1] , 1, 5, 1, 1); //Timp refresh chart
	ChartSettLayout->addWidget(chk_ch1_aftable , 2, 0, 1, 1); //Tabel On/Off
	ChartSettLayout->addWidget(led_ch1_valM    , 2, 1, 1, 1); //Ultima Functie M
	ChartSettLayout->addWidget(spn_ch1_size    , 2, 2, 1, 1); //Nr.Valori
	ChartSettLayout->addWidget(lbl_ch1_size    , 2, 3, 1, 1); //Nr.Valori
	ChartSettLayout->addWidget(led_ch1_var[2]  , 2, 4, 1, 1); //Variabila 2
	ChartSettLayout->addWidget(led_ch1_time[2] , 2, 5, 1, 1); //N.U.
	ChartSettLayout->addWidget(spn_ch1_fx_min_y, 3, 0, 1, 1); //Minim Chart
	ChartSettLayout->addWidget(spn_ch1_fx_max_y, 3, 1, 1, 1); //Maxim Chart
	ChartSettLayout->addWidget(spn_ch1_trd     , 3, 2, 1, 1); //Time Chart
	ChartSettLayout->addWidget(lbl_treadch1    , 3, 3, 1, 1); //Time Chart (ms)
	ChartSettLayout->addWidget(led_ch1_var[3]  , 3, 4, 1, 1); //Variabila 3
	ChartSettLayout->addWidget(led_ch1_time[3] , 3, 5, 1, 1); //N.U.

	//*************************************************************************
	//Ecran Alarme -> GroupBox
	gbAlarmLog = new QGroupBox();
	gbAlarmLog->setStyleSheet(*strGroupB1Style);
	QVBoxLayout* AlarmLogLayout = new QVBoxLayout(this);
	gbAlarmLog->setLayout(AlarmLogLayout);
	MainLayout->addWidget(gbAlarmLog);
	//Layout Jurnal Alarme
	lbAlarmStat = new QLabel("AlarmLog: ...");
	lbAlarmStat->setHmiFonts(true);
	lbAlarmStat->setStyleSheet("QLabel { background-color: rgb( 70,130,180); color: rgb(255,255,255)}");
	pteAlarmLog = new QTextEdit();
	pteAlarmLog->setHmiFonts(true);
	AlarmLogLayout->addWidget(lbAlarmStat);
	AlarmLogLayout->addWidget(pteAlarmLog);

	//Ecran Alarme -> GroupBox Setari
	gbAlarmSett = new QGroupBox(this);
	gbAlarmSett->setStyleSheet(*strGroupB1Style);
	gbAlarmSett->setHmiFonts(true);
	QGridLayout* AlarmSettLayout = new QGridLayout(this);
	QHBoxLayout* AlarmSet2Layout = new QHBoxLayout(this);
	QHBoxLayout* AlarmSet3Layout = new QHBoxLayout(this);
	AlarmSettLayout->setSpacing(0);
	gbAlarmSett->setLayout(AlarmSettLayout);
	MainLayout->addWidget(gbAlarmSett);
	bAlarmSett = false;
	chk_alm_typ[0] = new QCheckBox(tr("Msg/M"), this);
	chk_alm_typ[0]->setHmiFonts(true);
	chk_alm_typ[1] = new QCheckBox(tr("Alarm"), this);
	chk_alm_typ[1]->setHmiFonts(true);
	chk_alm_typ[2] = new QCheckBox(tr("Message"), this);
	chk_alm_typ[2]->setHmiFonts(true);
	chk_alm_typ[3] = new QCheckBox(tr("Oper."), this);
	chk_alm_typ[3]->setHmiFonts(true);
	chk_alm_typ[0]->setDisabled(true);
	chk_alm_typ[1]->setDisabled(true);
	chk_alm_typ[2]->setDisabled(true);
	chk_alm_typ[3]->setDisabled(true);
	QLabel *lbl_alm_varM0 = new QLabel(tr("M Start"), this);
	QLabel *lbl_alm_varM1 = new QLabel(tr("M Stop"), this);
	QLabel *lbl_alm_varM2 = new QLabel(tr("M Clear"), this);
	QLabel *lbl_alm_varM3 = new QLabel(tr("M Save"), this);
	lbl_alm_varM0->setHmiFonts(true);
	lbl_alm_varM1->setHmiFonts(true);
	lbl_alm_varM2->setHmiFonts(true);
	lbl_alm_varM3->setHmiFonts(true);
	led_alm_varM[0] = new QLineEdit("", this);
	led_alm_varM[0]->setHmiFonts(true);
	led_alm_varM[1] = new QLineEdit("", this);
	led_alm_varM[1]->setHmiFonts(true);
	led_alm_varM[2] = new QLineEdit("", this);
	led_alm_varM[2]->setHmiFonts(true);
	led_alm_varM[3] = new QLineEdit("", this);
	led_alm_varM[3]->setHmiFonts(true);
	led_alm_file = new QLineEdit("", this);
	led_alm_file->setHmiFonts(true);
	led_alm_file->setMinimumWidth(200);
	lbl_alm_file = new QLabel("", this);
	lbl_alm_file->setHmiFonts(true);
	chk_alm_fincr = new QCheckBox(tr("Incr."), this);
	chk_alm_fincr->setHmiFonts(true);
	QLabel *lbl_alm_fincr = new QLabel(tr("max."), this);
	lbl_alm_fincr->setHmiFonts(true);
	spn_alm_fincr = new QSpinBox(this);
	spn_alm_fincr->setRange(0, 99999);
	spn_alm_fincr->setSingleStep(1);
	spn_alm_fincr->setHmiFonts(true);
	QLabel *lbl_alm_fincd = new QLabel(tr("def."), this);
	lbl_alm_fincd->setHmiFonts(true);
	spn_alm_fincd = new QSpinBox(this);
	spn_alm_fincd->setRange(0, 99999);
	spn_alm_fincd->setSingleStep(1);
	spn_alm_fincd->setHmiFonts(true);
	QLabel *lbl_alm_finca = new QLabel(tr("act."), this);
	lbl_alm_finca->setHmiFonts(true);
	spn_alm_finca = new QSpinBox(this);
	spn_alm_finca->setRange(0, 99999);
	spn_alm_finca->setSingleStep(1);
	spn_alm_finca->setHmiFonts(true);
	chk_alm_finca = new QCheckBox(tr("AutoSave"), this);
	chk_alm_finca->setHmiFonts(true);
	chk_alm_simpl = new QCheckBox(tr("Extend."), this);
	chk_alm_simpl->setHmiFonts(true);
	chk_alm_clrstart = new QCheckBox(tr("Clear/Start"), this);
	chk_alm_clrstart->setHmiFonts(true);
	chk_alm_stopsave = new QCheckBox(tr("Stop/Save"), this);
	chk_alm_stopsave->setHmiFonts(true);
	
	b_alm_varM[0] = false;
	b_alm_varM[1] = false;
	b_alm_varM[2] = false;
	b_alm_varM[3] = false;
	b_alm_varMold[0] = false;
	b_alm_varMold[1] = false;
	b_alm_varMold[2] = false;
	b_alm_varMold[3] = false;

	AlarmSettLayout->addWidget(chk_alm_typ[0] , 0, 0, 1, 1);
	AlarmSettLayout->addWidget(chk_alm_typ[1] , 1, 0, 1, 1);
	AlarmSettLayout->addWidget(chk_alm_typ[2] , 2, 0, 1, 1);
	AlarmSettLayout->addWidget(chk_alm_typ[3] , 3, 0, 1, 1);
	AlarmSettLayout->addWidget(lbl_alm_varM0  , 0, 1, 1, 1);
	AlarmSettLayout->addWidget(lbl_alm_varM1  , 1, 1, 1, 1);
	AlarmSettLayout->addWidget(lbl_alm_varM2  , 2, 1, 1, 1);
	AlarmSettLayout->addWidget(lbl_alm_varM3  , 3, 1, 1, 1);
	AlarmSettLayout->addWidget(led_alm_varM[0], 0, 2, 1, 1);
	AlarmSettLayout->addWidget(led_alm_varM[1], 1, 2, 1, 1);
	AlarmSettLayout->addWidget(led_alm_varM[2], 2, 2, 1, 1);
	AlarmSettLayout->addWidget(led_alm_varM[3], 3, 2, 1, 1);
	//De inserat: Selectare separator: ; sau tab sau ...
	AlarmSettLayout->addWidget(led_alm_file   , 0, 3, 1, 1);
	AlarmSet2Layout->addWidget(chk_alm_fincr);
	AlarmSet2Layout->addWidget(lbl_alm_fincr);
	AlarmSet2Layout->addWidget(spn_alm_fincr);
	AlarmSet2Layout->addWidget(lbl_alm_fincd);
	AlarmSet2Layout->addWidget(spn_alm_fincd);
	AlarmSet2Layout->addWidget(lbl_alm_finca);
	AlarmSet2Layout->addWidget(spn_alm_finca);
	AlarmSet2Layout->addWidget(chk_alm_finca);
	AlarmSet2Layout->addStretch();
	AlarmSet3Layout->addWidget(chk_alm_simpl);
	AlarmSet3Layout->addWidget(chk_alm_clrstart);
	AlarmSet3Layout->addWidget(chk_alm_stopsave);
	AlarmSet3Layout->addStretch();
	AlarmSettLayout->addLayout(AlarmSet2Layout, 1, 3, 1, 1);
	AlarmSettLayout->addLayout(AlarmSet3Layout, 2, 3, 1, 1);
	AlarmSettLayout->addWidget(lbl_alm_file   , 3, 3, 1, 1);

	MainLayout->addStretch();
	//Sfarsit Layout Ecrane

	//Activare Server Evenimente (Citire Alarme)
	//Instanta la "alarm list class"
	m_pQEventSink = new SlAeQEventSink(this);
	//Initializare instanta Alarme
	long lError = 0;
	lError = m_pQEventSink->init("eng");
	if(lError == 0){
		pListaLog1->item(5)->setText(QString(">aeevt init OK"));
	}
	else{
		pListaLog1->item(5)->setText(QString(">aeevt init Err.%1").arg(lError));
	}
	//Stabilire Atribute
	SlAeIdsList attributes2Subscribe;
	attributes2Subscribe.push_back(SLAE_EV_ATTR_MSGTEXT);
	SlAeCategoryInfoArray rCategories;
	rCategories.clear();
	lError = m_pQEventSink->queryCategories(rCategories);
	SlAeCategoryInfoArray::Iterator it;
	for(it = rCategories.begin(); it != rCategories.end(); ++it){
		lError = m_pQEventSink->setReturnAttributes((*it).m_nId, attributes2Subscribe);
	}
	QObject::connect(m_pQEventSink, SIGNAL(newEvent(const QList<SlAeEvent*>&)),
					this,SLOT(onNewEvent(const QList<SlAeEvent*>&)));
	//m_pQEventSink->deactivate();
	m_pQEventSink->activate();
	b_alm_active = false;
	//Sfarsit Activare Server Evenimente (Citire Alarme)

	//Activare Timer Citire ciclica Date Chart
	t_readch1 = new QTimer(this);
	connect(t_readch1, SIGNAL(timeout()), this, SLOT(readncvar()));
	//Activare Timer Citire ciclica Date Monitor
	t_readch2 = new QTimer(this);
	connect(t_readch2, SIGNAL(timeout()), this, SLOT(readmonvar()));
	//afisare ceas in titlebar
	t_status = new QTimer(this);
	connect(t_status, SIGNAL(timeout()), this, SLOT(show_caption()));
	t_status->start(1000);
	//cronometrare multi1
	t_multi1 = new QTime();
	t_multi1->start();
	t_multi2 = new QTime();
	t_multi2->start();
	t_chart1 = new QTime();
	t_chart1->start();
	//generare nume fisier alarme
	connect(chk_alm_fincr, SIGNAL(toggled(bool)), this, SLOT(almlog_fileinc1(bool)));
	connect(spn_alm_fincr, SIGNAL(valueChanged(int)), this, SLOT(almlog_fileinc2(int)));
	connect(spn_alm_finca, SIGNAL(valueChanged(int)), this, SLOT(almlog_fileinc2(int)));
	connect(led_alm_file,  SIGNAL(textChanged(const QString &)), this, SLOT(almlog_fileinc3(const QString &)));
	getinifil();
	readinifil();
	readstatus(false);
	set_ecran(1);

} // <-- cam840aform::cam840aform ()


cam840aform::~cam840aform (void)
{

} // <-- cam840aform::~cam840aform ()



///////////////////////////////////////////////////////////////////////////////
// Overrides
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// public ACCESSORS
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// MANIPULATORS - system overrides
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
// public SLOTS - system overrides
///////////////////////////////////////////////////////////////////////////////
void cam840aform::onFunction (const QString& rsFunction,
                                    const QString& rsArgs,
                                    bool& rbHandled)
{
	//onFunction: Activat de HMI, de ex. apasare taste soft...
	bool b_temp;
	if ( "mod_ecran1" == rsFunction )
	{ //Afisare Ecran1
		if ((! b_settingon) && (! b_readstaon)){set_ecran(1);}
		// set that the event was handled by this function
		rbHandled = true;
	}
	if ( "mod_ecran2" == rsFunction )
	{ //Afisare Ecran2
		if ((! b_settingon) && (! b_readstaon)){set_ecran(2);}
		rbHandled = true;
	}
	if ( "mod_ecran3" == rsFunction )
	{ //Afisare Ecran3
		if ((! b_settingon) && (! b_readstaon)){set_ecran(3);}
		rbHandled = true;
	}
	if ( "mod_ecran4" == rsFunction )
	{ //Afisare Ecran4
		if ((! b_settingon) && (! b_readstaon)){set_ecran(4);}
		rbHandled = true;
	}
	if ( "mod_ecran8" == rsFunction )
	{ //Afisare Ecran8
		if ((! b_settingon) && (! b_readstaon)){set_ecran(8);}
		rbHandled = true;
	}

	if ( "mod_screenchart" == rsFunction )
	{ //Screen Chart
		if(bSinopticChart == true){bSinopticChart = false;}
		else{bSinopticChart = true;}
		gbChart->setVisible(bSinopticChart);
		rbHandled = true;
	}
	if ( "mod_screenzoom" == rsFunction )
	{ //Screen Set/Rst Zoom
		b_temp = chk_afis_zoom->isChecked();
		if(b_temp == true){
			chk_afis_zoom->setChecked(false);
			this->setZoomContent(false);
		}
		else{
			chk_afis_zoom->setChecked(true);
			this->setZoomContent(true);
		}
		rbHandled = true;
	}
	if ( "mod_screencompr" == rsFunction )
	{ //Screen Set/Rst Compression
		b_temp = chk_afis_nosp->isChecked();
		if(b_temp == true){
			chk_afis_nosp->setChecked(false);
			axGridLayout->setSpacing(6);
			spGridLayout->setSpacing(6);
		}
		else{
			chk_afis_nosp->setChecked(true);
			axGridLayout->setSpacing(0);
			spGridLayout->setSpacing(0);
		}
		rbHandled = true;
	}
	if ( "mod_screensett" == rsFunction )
	{ //Screen Setttings
		if(bSinopticSett == true){bSinopticSett = false;}
		else{bSinopticSett = true;}
		gbSinopticSett->setVisible(bSinopticSett);
		rbHandled = true;
	}

	if ( "mod_monstartrec" == rsFunction)
	{ // Start Timer Citire Automata Monitor
		monitor_start();
		rbHandled = true;
	}
	if ( "mod_monstoprec" == rsFunction)
	{ // Stop Timer Citire Automata Monitor
		monitor_stop();
		rbHandled = true;
	}
	if ( "mod_monstart" == rsFunction)
	{ // Start Timer Citire Automata Monitor
		monitor_start();
		rbHandled = true;
	}
	if ( "mod_monstop" == rsFunction)
	{ // Stop Timer Citire Automata Monitor
		monitor_stop();
		rbHandled = true;
	}

	if ( "mod_chartread" == rsFunction)
	{ // Citire Valoare Unica pt. Chart1
		readncvar();
		rbHandled = true;
	}
	if ( "mod_chartstart" == rsFunction)
	{ // Start Timer Citire Automata Chart1
		timer_start();
		rbHandled = true;
	}
	if ( "mod_chartstop" == rsFunction )
	{ // Stop Timer Citire Automata Chart1
		timer_stop();
		rbHandled = true;
	}
	if ( "mod_chartsett" == rsFunction )
	{ //Screen Setttings
		if(bChartSett == true){bChartSett = false;}
		else{bChartSett = true;}
		gbChartSett->setVisible(bChartSett);
		rbHandled = true;
	}

	if ( "mod_almlogclear" == rsFunction )
	{ // Stergere Inregistrari Alarme in Log
		pteAlarmLog->setText("");
		rbHandled = true;
	}
	if ( "mod_almlogstart" == rsFunction )
	{ // Start Inregistrare Alarme in Log
		//m_pQEventSink->activate();
		b_alm_active = true;
		if(chk_alm_clrstart->isChecked()){
			//Automatic clear of log on Start
			pteAlarmLog->clear();
		}
		lbAlarmStat->setText("AlarmLog: ON");
		rbHandled = true;
	}
	if ( "mod_almlogstop" == rsFunction )
	{ // Stop Inregistrare Alarme in Log
		//m_pQEventSink->deactivate();
		b_alm_active = false;
		lbAlarmStat->setText("AlarmLog: OFF");
		rbHandled = true;
	}
	if ( "mod_almlogsave" == rsFunction )
	{ // Salvare Jurnal Alarme
		almlog_save();
		rbHandled = true;
	}
	if ( "mod_almlogsett" == rsFunction )
	{ //Screen Setttings
		if(bAlarmSett == true){bAlarmSett = false;}
		else{bAlarmSett = true;}
		gbAlarmSett->setVisible(bAlarmSett);
		rbHandled = true;
	}

	if ( "mod_readini" == rsFunction )
	{ // citesc fisier INI (fara refresh)
		readinifil();
		rbHandled = true;
	}
	if ( "mod_saveini" == rsFunction )
	{ // salvez fisier INI
		saveinifil();
		rbHandled = true;
	}
	if ( "mod_reconnect" == rsFunction )
	{ //Reconectare NCK
		readstatus(false);
		rbHandled = true;
	}

	//call base class
	SlGfwDialogForm::onFunction(rsFunction, rsArgs, rbHandled);
} // <-- cam840aform::onFunction ()


///////////////////////////////////////////////////////////////////////////////
// private methods
///////////////////////////////////////////////////////////////////////////////
// write success or error to the status bar
void cam840aform::handleRetVal(SlCapErrorEnum eError, QString szSuccess)
{
	// evaluate return value
	if( SL_CAP_OK != eError )
	{
		// write error in status bar
		QString szHexErrorNr = QString("%1").arg(eError,0,16).right(8);
		setStatusBarElementText(ACCESS_ERROR.arg(szHexErrorNr), 0); 
	}
	else
	{
		// write success in status bar
		setStatusBarElementText(" ", 0);  
	}
}

void cam840aform::timer_start()
{
//start timer chart
	int i_temp;
	i_temp = spn_ch1_trd->value();
	if (t_readch1->isActive() == 0){
		t_readch1->start(i_temp);
		chart_ch1->b_ison = true;
		chart_ch1->updateChart();
	}
}

void cam840aform::timer_stop()
{
//stop timer chart
	t_readch1->stop();
	chart_ch1->b_ison = false;
	chart_ch1->updateChart();
}

void cam840aform::monitor_start()
{
//start timer chart
	int i_temp;
	i_temp = spn_ch1_trd->value();
	if (t_readch2->isActive() == 0){
		t_readch2->start(i_temp);
		chart_ch2->b_ison = true;
		chart_ch2->updateChart();
	}
}

void cam840aform::monitor_stop()
{
//stop timer chart
	t_readch2->stop();
	chart_ch2->b_ison = false;
	chart_ch2->updateChart();
}

void cam840aform::add_log2(QString textlog)
{
	//Actualizare lista evenimente : Log2
	int iMaxLog2 = 10;
	int i_temp1, i_temp2;
	QTime tActTime = QTime::currentTime();
	QString sActText;
	//Verificare numar linii din ListaLog2
	i_temp1 = pListaLog2->count();
	//Umplere lista cu ... max... linii
	for (i_temp2 = i_temp1; i_temp2 <= iMaxLog2; i_temp2++){
		pListaLog2->addItem("");
	}
	//Mutare linii mai jos cu una
	for (i_temp2 = iMaxLog2; i_temp2 > 1; i_temp2--){
		sActText = pListaLog2->item(i_temp2 - 1)->text();
		pListaLog2->item(i_temp2)->setText(sActText);
	}
	//Inserare eveniment nou in linia 1
	sActText = tActTime.toString();
	pListaLog2->item(1)->setText(sActText + "> " + textlog);
}

void cam840aform::almlog_save()
{
	QString s_nume;
	//Nume Fisier AlarmLog
	s_nume = lbl_alm_file->text().trimmed();
    QFile f_AlmFile(s_nume);
	if (!f_AlmFile.open(QIODevice::WriteOnly | QIODevice::Text)){
		add_log2("Error: on save AlarmLog to file " + s_nume);
        return;
	}
	QTextStream out(&f_AlmFile);
    out << pteAlarmLog->toPlainText();
	f_AlmFile.close();
	if(chk_alm_fincr->isChecked()){
		if(spn_alm_finca->value() < spn_alm_fincr->value()){
			spn_alm_finca->setValue(spn_alm_finca->value() + 1);
		}
		else{
			spn_alm_finca->setValue(0);
		}
	}
	almlog_file();
	if(chk_alm_stopsave->isChecked()){
		//Automatic stop of log on Save
		//m_pQEventSink->deactivate();
		b_alm_active = false;
	}
}

void cam840aform::almlog_file()
{
	QString s_nume, continut, s_index;
	int i_temp, i_index, i_numlead;
	//Generare Nume Fisier AlarmLog
	if(spn_alm_finca->value() > spn_alm_fincr->value()){
		spn_alm_finca->setValue(spn_alm_fincr->value());
	}
	s_nume = led_alm_file->text().trimmed();
	if(chk_alm_fincr->isChecked()){
		//verifc indexul actual
		i_index = spn_alm_finca->value();
		i_numlead = 0;
		i_temp = spn_alm_fincr->value();
		//calculez numarul de zerouri in fata
		while(i_temp > 0){
			i_temp = i_temp / 10;
			i_numlead++;
		}
		//index in format text
		s_index = QString("%1").arg(i_index, i_numlead, 10, QChar('0'));
		i_temp = s_nume.lastIndexOf(".");
		if(i_temp > 0){
			//nume = nume.left(nume.lastIndexOf(".")) + QString("%1.").arg(i_alm_fincr) + nume.right(nume.length() - nume.lastIndexOf(".") - 1);
			s_nume = s_nume.left(s_nume.lastIndexOf(".")) + s_index + "." + s_nume.right(s_nume.length() - s_nume.lastIndexOf(".") - 1);
		}
		else{
			//nume = nume + QString("%1").arg(i_alm_fincr);
			s_nume = s_nume + s_index;
		}
	}
	lbl_alm_file->setText(s_nume);
	lbl_alm_file->setToolTip(s_nume);

	if((chk_alm_fincr->isChecked()) && (chk_alm_finca->isChecked())){
		savealmindex();
	}
}

void cam840aform::almlog_fileinc1(bool b_checked)
{
	almlog_file();	
}

void cam840aform::almlog_fileinc2(int i_value)
{
	almlog_file();	
}

void cam840aform::almlog_fileinc3(const QString &)
{
	almlog_file();	
}
///////////////////////////////////////////////////////////////////////////
// PRIVATE SLOTS
///////////////////////////////////////////////////////////////////////////
// varianta cu o multi - advice hotlink
// is send, if asynchron advise request is complete
void cam840aform::multiAdviseDataSlot1(SlCapErrorEnum eError, const QVector<SlCapAdviseResultType>& rvData)
{
	if ((b_settingon) || (b_readstaon)){return;}

	double	rTemp;
	int		nIndex1 = 0;
	int		nIndex2 = 0;
	int		uTemp;
	QString	nTempS = QString();
	QChar	ch_Deg(0xB0);

	if (b_TargetOK == true){
		if( SL_CAP_OK  != eError )
		{
			pListaLog1->item(3)->setText(QString(tr(">multiadv.1 err: %1")).arg(eError));
			for (nIndex1 = 0; nIndex1 < nNrDatChan; nIndex1++){
				pChState[nIndex1]->setText("");
				pChParts[nIndex1]->setText("");
				pChSelWP[nIndex1]->setText("");
				pChProgN[nIndex1]->setText("");
				pChProgN[nIndex1]->setToolTip("");
				pChActBl[nIndex1]->setText("");
				pChActBl[nIndex1]->setToolTip("");
			}
			for (nIndex1 = 0; nIndex1 < nNrDatAxes; nIndex1++){
				pAxNameLbl6[nIndex1]->setText("");
				pAxNameLbl9[nIndex1]->setText("");
				pAxNameLbla[nIndex1]->setText("");
			}
			for (nIndex1 = 0; nIndex1 < nNrDatSpindle; nIndex1++){
				pSpNameLbl4[nIndex1]->setText("");
				pSpNameLbl5[nIndex1]->setText("");
				pSpLoad[nIndex1]->setValue(0);
			}
		}
		else
		{
			nIndex2 = rvData.count();
			//verific consistenta datelor
			if (nIndex2 == ((nNrDatChan * 5) + (nNrDatAxes * 3) + (nNrDatSpindle * 3))){
				//nIndex2 = int(nIndex2 / 2);
				//actualizez tabelul canalelor
				nIndex2 = nNrDatChan;
				for (nIndex1 = 0; nIndex1 < nNrDatChan; nIndex1++){
					//citesc starea canalului [0=Reset, 1=Interrupted, 2=Active]
					uTemp = rvData[(nIndex1 * 5)].m_vValue.toUInt();
					nTempS = QString("#");
					if (uTemp == 0){
						nTempS = QString("Reset");
						pChState[nIndex1]->setStyleSheet("QLabel { background-color: rgb(128,128,128); color: rgb(255,255,255); border-top: 1px solid white; border-left: 1px solid white; border-right: 1px solid gray; border-bottom: 1px solid gray }");
					}
					if (uTemp == 1){
						nTempS = QString("Interrupted");
						pChState[nIndex1]->setStyleSheet("QLabel { background-color: rgb(212, 45, 47); color: rgb(255,255,255); border-top: 1px solid white; border-left: 1px solid white; border-right: 1px solid gray; border-bottom: 1px solid gray }");
					}
					if (uTemp == 2){
						nTempS = QString("Active");
						pChState[nIndex1]->setStyleSheet("QLabel { background-color: rgb(119,160, 71); color: rgb(255,255,255); border-top: 1px solid white; border-left: 1px solid white; border-right: 1px solid gray; border-bottom: 1px solid gray }");
					}
					pChState[nIndex1]->setText(nTempS);
					//citesc numarul de piese
					rTemp = rvData[(nIndex1 * 5) + 1].m_vValue.toDouble();
					nTempS = QString("%1").arg(rTemp,0, 'f', 3) + QString("pcs");
					pChParts[nIndex1]->setText(nTempS);
					pChParts[nIndex1]->setToolTip(QString("Parts"));
					//citesc programul selectat in canal
					nTempS = rvData[(nIndex1 * 5) + 2].m_vValue.toString();
					pChSelWP[nIndex1]->setText(nTempS);
					pChSelWP[nIndex1]->setToolTip(nTempS);
					//citesc programul curent in canal
					nTempS = rvData[(nIndex1 * 5) + 3].m_vValue.toString();
					pChProgN[nIndex1]->setText(nTempS);
					pChProgN[nIndex1]->setToolTip(nTempS);
					//citesc fraza curenta
					nTempS = rvData[(nIndex1 * 5) + 4].m_vValue.toString();
					pChActBl[nIndex1]->setText(nTempS);
					pChActBl[nIndex1]->setToolTip(nTempS);
				}
				//actualizez tabelul cu axe
				nIndex2 = nNrDatAxes;
				for (nIndex1 = 0; nIndex1 < nNrDatAxes; nIndex1++){
					rTemp = rvData[(nNrDatChan * 5) + (nIndex1 * 3)].m_vValue.toDouble();
					if (bAxNameLbl61[nIndex1] == true){
						nTempS = QString("%1").arg(rTemp,0, 'f', 4) + QString(ch_Deg);}
					else{
						nTempS = QString("%1").arg(rTemp,0, 'f', 4) + QString(" ");}
					pAxNameLbl6[nIndex1]->setText(nTempS);
					rTemp = rvData[(nNrDatChan * 5) + (nIndex1 * 3) + 1].m_vValue.toDouble();
					if (bAxNameLbl61[nIndex1] == true){
						nTempS = QString("%1").arg(rTemp,0, 'f', 4) + QString(ch_Deg);}
					else{
						nTempS = QString("%1").arg(rTemp,0, 'f', 4) + QString(" ");}
					pAxNameLbl9[nIndex1]->setText(nTempS);
					rTemp = rvData[(nNrDatChan * 5) + (nIndex1 * 3) + 2].m_vValue.toDouble();
					nTempS = QString("%1").arg(rTemp,0, 'f', 0) + QString(" ");
					pAxNameLbla[nIndex1]->setText(nTempS);
				}
				//actualizez tabelul cu arbori
				for (nIndex1 = 0; nIndex1 < nNrDatSpindle; nIndex1++){
					rTemp = rvData[(nNrDatChan * 5) + (nNrDatAxes * 3) + (nIndex1 * 3)].m_vValue.toDouble();
					nTempS = QString("%1").arg(rTemp,0, 'f', 1) + QString(ch_Deg);
					pSpNameLbl4[nIndex1]->setText(nTempS);
					rTemp = rvData[(nNrDatChan * 5) + (nNrDatAxes * 3) + (nIndex1 * 3) + 1].m_vValue.toDouble();
					nTempS = QString("%1").arg(rTemp,0, 'f', 1) + " rpm";
					pSpNameLbl5[nIndex1]->setText(nTempS);
					rTemp = rvData[(nNrDatChan * 5) + (nNrDatAxes * 3) + (nIndex1 * 3) + 2].m_vValue.toDouble();
					nTempS = QString("%1").arg(rTemp,0, 'f', 1) + "%";
					pSpLoad[nIndex1]->setValue(int(rTemp));
				}
				pListaLog1->item(3)->setText(QString(tr(">multiadv.1 OK: %1")).arg(nNrDatChan) + QString("ch / %1").arg(nNrDatAxes) + QString("ax / %1").arg(nNrDatSpindle) + QString("sp"));
			}
			else{
				pListaLog1->item(3)->setText(QString(tr(">multiadv.1 Err: data no. = %1")).arg(nIndex2));
			}
		}
	}

	int i_time1 = t_multi1->elapsed();
	led_afis_time->setText(QString("%1ms").arg(i_time1));
	t_multi1->restart();
}

void cam840aform::multiAdviseDataSlot2(SlCapErrorEnum eError, const QVector<SlCapAdviseResultType>& rvData)
{
	if ((b_settingon) || (b_readstaon)){return;}

	double	rTemp;
	int		iTemp, iTemp2;
	int		nIndex2 = 0;
	QString	nTempS = QString();
	QChar	ch_Deg(0xB0);

	if (b_TargetOK == true){
		if( SL_CAP_OK  != eError )
		{
			pListaLog1->item(4)->setText(QString(tr(">multiadv.2 Err: %1")).arg(eError));
			pLblNcVar[0]->setText(QString(tr("Err.- %1")).arg(eError));
			pLblNcVar[1]->setText(QString(tr("Err.- %1")).arg(eError));
			pLblNcVar[2]->setText(QString(tr("Err.- %1")).arg(eError));
			pLblNcVar[3]->setText(QString(tr("Err.- %1")).arg(eError));
			pLblMonVar[0]->setText(QString(tr("Err.- %1")).arg(eError));
		}
		else
		{
			nIndex2 = rvData.count();
			//citesc valorile din Multiread2
			if (nIndex2 > 0){ //daca am citit functia M
				iTemp = rvData[0].m_vValue.toInt();
				//Afisare Functie M actuala...
				led_ch1_valM->setText(QString("M%1").arg(iTemp));
				//verific M start chart
				iTemp2 = led_ch1_varM[0]->text().toInt();
				if (iTemp == iTemp2){	b_ch1_varM[0] = true;}
				else{					b_ch1_varM[0] = false;}
				if(b_ch1_varM[0] && !b_ch1_varMold[0]){timer_start();}
				b_ch1_varMold[0] = b_ch1_varM[0];
				//verific M stop chart
				iTemp2 = led_ch1_varM[1]->text().toInt();
				if (iTemp == iTemp2){	b_ch1_varM[1] = true;}
				else{					b_ch1_varM[1] = false;}
				if(b_ch1_varM[1] && !b_ch1_varMold[1]){timer_stop();}
				b_ch1_varMold[1] = b_ch1_varM[1];
				//verific M start AlarmLog
				iTemp2 = led_alm_varM[0]->text().toInt();
				if (iTemp == iTemp2){	b_alm_varM[0] = true;}
				else{					b_alm_varM[0] = false;}
				if(b_alm_varM[0] && !b_alm_varMold[0]){
					if(chk_alm_clrstart->isChecked()){
						//Automatic clear of log on Start
						pteAlarmLog->clear();
					}
					//m_pQEventSink->activate();
					b_alm_active = true;
				}
				b_alm_varMold[0] = b_alm_varM[0];
				//verific M stop AlarmLog
				iTemp2 = led_alm_varM[1]->text().toInt();
				if (iTemp == iTemp2){	b_alm_varM[1] = true;}
				else{					b_alm_varM[1] = false;}
				if(b_alm_varM[1] && !b_alm_varMold[1]){
					//m_pQEventSink->deactivate();
					b_alm_active = false;
				}
				b_alm_varMold[1] = b_alm_varM[1];
				//verific M clear AlarmLog
				iTemp2 = led_alm_varM[2]->text().toInt();
				if (iTemp == iTemp2){	b_alm_varM[2] = true;}
				else{					b_alm_varM[2] = false;}
				if(b_alm_varM[2] && !b_alm_varMold[2]){pteAlarmLog->clear();}
				b_alm_varMold[2] = b_alm_varM[2];
				//verific M Save AlarmLog
				iTemp2 = led_alm_varM[3]->text().toInt();
				if (iTemp == iTemp2){	b_alm_varM[3] = true;}
				else{					b_alm_varM[3] = false;}
				if(b_alm_varM[3] && !b_alm_varMold[3]){almlog_save();}
				b_alm_varMold[3] = b_alm_varM[3];
			}
			//actualizez tabelul cu axe
			if (nIndex2 > 1){
				rTemp = rvData[1].m_vValue.toDouble();
				nTempS = QString("%1").arg(rTemp,0, 'f', 4) + QString(" ");
				pLblNcVar[0]->setText(nTempS);
				pVarNcVar[0]->setValue(rTemp);
				pLblMonVar[0]->setText(nTempS);
				pVarMonVar[0]->setValue(rTemp);
			}
			else{
				pLblNcVar[0]->setText(QString("#"));
				pLblMonVar[0]->setText(QString("#"));
			}
			if (nIndex2 > 2){
				rTemp = rvData[2].m_vValue.toDouble();
				nTempS = QString("%1").arg(rTemp,0, 'f', 4) + QString(" ");
				pLblNcVar[1]->setText(nTempS);
				pVarNcVar[1]->setValue(rTemp);
			}
			else{
				pLblNcVar[1]->setText(QString("#"));
			}
			if (nIndex2 > 3){
				rTemp = rvData[3].m_vValue.toDouble();
				nTempS = QString("%1").arg(rTemp,0, 'f', 4) + QString(" ");
				pLblNcVar[2]->setText(nTempS);
				pVarNcVar[2]->setValue(rTemp);
			}
			else{
				pLblNcVar[2]->setText(QString("#"));
			}
			if (nIndex2 > 4){
				rTemp = rvData[4].m_vValue.toDouble();
				nTempS = QString("%1").arg(rTemp,0, 'f', 4) + QString(" ");
				pLblNcVar[3]->setText(nTempS);
				pVarNcVar[3]->setValue(rTemp);
			}
			else{
				pLblNcVar[3]->setText(QString("#"));
			}
			pListaLog1->item(4)->setText(QString(tr(">multiadv.2 OK: %1")).arg(nIndex2) + QString("val."));
		}
	}

	int i_time2 = t_multi2->elapsed();
	led_ch1_time[0]->setText(QString("%1ms").arg(i_time2));
	t_multi2->restart();
}

void cam840aform::readstatus(bool b_silent)
{
	b_readstaon = true;
	int	nIndex1	= 0;
	int	nIndex2 = 0;
	nNrDatChan = 0;
	nNrDatAxes = 0;
	nNrDatSpindle = 0;
	QString nTempS = QString();
	bool b_isSpindle = false;

	b_TargetOK = false;
	SlCapErrorEnum eError;

	//Dezactivare HotLink pana la refacerea ecranului
	eError = m_capServerHotlink1.unadvise(m_capHandleHotlink1);
	eError = m_capServerHotlink2.unadvise(m_capHandleHotlink2);

	//Curatare fereastra Axe
	for (nIndex1 = 0; nIndex1 < 31; nIndex1++){
		pAxNameLbl1[nIndex1]->setVisible(false);
		pAxNameLbl1[nIndex1]->setText("");
		pAxNameLbl2[nIndex1]->setVisible(false);
		pAxNameLbl2[nIndex1]->setText("");
		pAxNameLbl3[nIndex1]->setVisible(false);
		pAxNameLbl3[nIndex1]->setText("");
		pAxNameLbl4[nIndex1]->setVisible(false);
		pAxNameLbl4[nIndex1]->setText("");
		pAxNameLbl5[nIndex1]->setVisible(false);
		pAxNameLbl5[nIndex1]->setText("");
		pAxNameLbl6[nIndex1]->setVisible(false);
		pAxNameLbl6[nIndex1]->setText("");
		pAxNameLbl7[nIndex1]->setVisible(false);
		pAxNameLbl7[nIndex1]->setText("");
		pAxNameLbl8[nIndex1]->setVisible(false);
		pAxNameLbl8[nIndex1]->setText("");
		pAxNameLbl9[nIndex1]->setVisible(false);
		pAxNameLbl9[nIndex1]->setText("");
		pAxNameLbla[nIndex1]->setVisible(false);
		pAxNameLbla[nIndex1]->setText("");
	}

	//Curatare fereastra Arbori
	for (nIndex1 = 0; nIndex1 < 10; nIndex1++){
		pSpNameLbl1[nIndex1]->setText("");
		pSpNameLbl1[nIndex1]->setVisible(false);
		pSpNameLbl2[nIndex1]->setText("");
		pSpNameLbl2[nIndex1]->setVisible(false);
		pSpNameLbl3[nIndex1]->setText("");
		pSpNameLbl3[nIndex1]->setVisible(false);
		pSpNameLbl4[nIndex1]->setText("");
		pSpNameLbl4[nIndex1]->setVisible(false);
		pSpNameLbl5[nIndex1]->setText("");
		pSpNameLbl5[nIndex1]->setVisible(false);
		pSpLoad[nIndex1]->setVisible(false);
	}

	//Stergere Variabile
	QString szReadData1 = QString();
	QString szReadData2 = QString();
	QVariant vData(QVariant::Invalid);
	int i_MaxNumChan = 0;
	int i_MaxNumAxes = 0;
	//Citire Tip NCK
	if (b_silent == false){
		pListaLog1->item(0)->setText(tr("NCK: Setting connection to NC ..."));}
	//Conectare la NCK: Citire tip NC / Cod Eroare conectare
	eError = m_capServerReadWrite.read("/Nck/Configuration/nckType", vData);
	if( SL_CAP_OK != eError )
	{
		szReadData1= QString("%1").arg(eError,0,16).right(8);
		pListaLog1->item(0)->setText(tr("NCK: connection error:") + szReadData1);
		if (b_silent == false){add_log2("Error: NCK connection error: " + szReadData1);}
		b_TargetOK = false;
	}
	else
	{
		szReadData1= vData.toString();
		if (szReadData1== "0"	){	szReadData1= "0: 840D pl";}
		if (szReadData1== "1000"){	szReadData1= "1000: FM-NC sw <= 6";	}
		if (szReadData1== "2000"){	szReadData1= "2000: 810D pl";}
		if (szReadData1== "3000"){	szReadData1= "3000: 802S";}
		if (szReadData1== "4000"){	szReadData1= "4000: 802D pl sw <=6";}
		if (szReadData1== "5000"){	szReadData1= "5000: 840Di pl sw <=6";}
		if (szReadData1== "6000"){	szReadData1= "6000: SOLUTIONLINE";}
		if (szReadData1== "10700"){	szReadData1= "10700: 840D sl";}
		if (szReadData1== "14000"){	szReadData1= "14000: 802D sl T/M or N/G or C/U";}
		if (szReadData1== "15000"){	szReadData1= "15000: 840Di sl";}
		pListaLog1->item(0)->setText(tr("NCK: type ") + szReadData1);
		add_log2(tr("NCK connection OK: ") + szReadData1);
		b_TargetOK = true;
	}
	//Citire Numar Canale
	eError = m_capServerReadWrite.read("/Nck/Configuration/numChannels", vData);
	szReadData1= "#";
	if( SL_CAP_OK == eError ){
		szReadData1= vData.toString();}
	szReadData2 = tr(">chan.: ") + szReadData1;
	//Citire Numar Maxim Canale
	szReadData1= "#";
	eError = m_capServerReadWrite.read("/Nck/Configuration/maxnumChannels", vData);
	if( SL_CAP_OK == eError ){
		i_MaxNumChan = vData.toInt();
		szReadData1= QString("%1").arg(i_MaxNumChan);
	}
	szReadData2 = szReadData2 + "/" + szReadData1;
	//Citire Numar Axe
	szReadData1= "#";
	eError = m_capServerReadWrite.read("/Nck/Configuration/numGlobMachAxes", vData);
	if( SL_CAP_OK == eError ){
		szReadData1= vData.toString();}
	szReadData2 = szReadData2 + "; axes: " + szReadData1;
	//Citire Numar Maxim Axe
	szReadData1= "#";
	eError = m_capServerReadWrite.read("/Nck/Configuration/maxnumGlobMachAxes", vData);
	if( SL_CAP_OK == eError ){
		i_MaxNumAxes = vData.toInt();
		szReadData1= QString("%1").arg(i_MaxNumAxes);
	}
	szReadData2 = szReadData2 + "/" + szReadData1+ ";";
	//Actualizare Log1
	pListaLog1->item(2)->setText(szReadData2);

	//Mapare Variabile
	QString			szItem;
	QVariant		vLinkItems;
	QList<QVariant> varList;
	QList<QVariant>	dataList;
    QList<int>		i_ChanActiv;
	int				nTemp1	= 0;
	int				nTemp2	= 0;
	QString			s_temp1;
	QString			s_temp2;

	//Pregatire lista de canale, axe si arbori pt. mapare
	QStringList szItemCh, szItemAx, szItemSp;
	int		nNrAxeMap = 0;
	int		nNrSpiMap = 0;
	int		nNrSpiChan = 0;
	QChar	ch_Deg(0xB0);

	int		i_ChanAxis[10][31];
	//Initializare cu zero a tabelului de alocare a axelor
	for (nIndex1 = 0; nIndex1 < 10; nIndex1++){
		for (nIndex2 = 0; nIndex2 <31; nIndex2++){
			i_ChanAxis[nIndex1][nIndex2] = 0;
		}
	}
	int		i_ChanAxisGeo[10][3];
	//Initializare cu zero a tabelului de alocare a axelor GEO
	for (nIndex1 = 0; nIndex1 < 10; nIndex1++){
		for (nIndex2 = 0; nIndex2 <3; nIndex2++){
			i_ChanAxisGeo[nIndex1][nIndex2] = 0;
		}
	}

	//RUN map command
	m_sTargetSyst = "#";
	if (b_TargetOK == true)
	{
		//Tentativa Mapare
		eError = m_capNameSpaceNC.map();
		//Daca Mapare OK
		if( SL_CAP_OK == eError )
		{
			//RUN lookup command		
			eError = m_capNameSpaceNC.lookup(vLinkItems);
			//Daca Lookup OK
			if( SL_CAP_OK == eError )
			{
				//Scrie toate variabilele ce pot fi accesate din namespace intr-o lista
				//Write all reachable variables from namespace in a list
				varList = vLinkItems.toList();
				dataList = varList[0].toList();
				szReadData1= dataList[0].toString();
				nIndex1 = szReadData1.indexOf("/", 0);
				if (nIndex1 > 0){
					m_sTargetSyst = szReadData1.left(nIndex1);}
				pListaLog1->item(1)->setText("Target: " + m_sTargetSyst);
				//Citire a numerelor canalelor folosite in sistem
				for( nIndex1 = 1 ; nIndex1 <= i_MaxNumChan ; nIndex1++ ){
					nNrSpiChan = 1;
					//Initializare variabila
					szItem = m_sTargetSyst + QString("/NC/_N_NC_TEA_ACX/$MN_ASSIGN_CHAN_TO_MODE_GROUP[u1,%1]").arg(nIndex1);
					//Citire Variabila
					eError = m_capServerReadWrite.read(szItem, vData);
					nTemp1 = vData.toInt();
					i_ChanActiv.append(nTemp1);
					//Pentru fiecare Canal activ: Citire date Canal
					if (nTemp1 > 0){
						//Citire Nume Canal
						szItem = m_sTargetSyst + QString("/NC/_N_CH_TEA_ACX/$MC_CHAN_NAME[u%1").arg(nIndex1) + QString("]");
						eError = m_capServerReadWrite.read(szItem, vData);
						s_temp1 = vData.toString();
						pChChNo[nNrDatChan]->setText(QString("%1:").arg(nIndex1) + s_temp1);
						szItemCh.append(QString("/Channel/State/acStat[u%1").arg(nIndex1) + QString(", 1]"));
						szItemCh.append(QString("/Channel/State/totalParts[u%1").arg(nIndex1) + QString("]"));
						szItemCh.append(QString("/Channel/ProgramInfo/selectedWorkPProg[u%1").arg(nIndex1) + QString(", 1]"));
						szItemCh.append(QString("/Channel/ProgramInfo/progName[u%1").arg(nIndex1) + QString(", 1]"));
						szItemCh.append(QString("/Channel/ProgramInfo/actBlock[u%1").arg(nIndex1) + QString(", 1]"));
						nNrDatChan++;
					}
					//Citire date despre axele din canal
					if (nTemp1 > 0){
						//Citire: prima axa GEO
						szItem = m_sTargetSyst + QString("/NC/_N_CH_TEA_ACX/$MC_AXCONF_GEOAX_ASSIGN_TAB[u%1").arg(nIndex1) + ",1]";
						eError = m_capServerReadWrite.read(szItem, vData);
						nTemp1 = vData.toInt();
						i_ChanAxisGeo[nIndex1 - 1][0] = nTemp1;
						//Citire: a doua axa GEO
						szItem = m_sTargetSyst + QString("/NC/_N_CH_TEA_ACX/$MC_AXCONF_GEOAX_ASSIGN_TAB[u%1").arg(nIndex1) + ",2]";
						eError = m_capServerReadWrite.read(szItem, vData);
						nTemp1 = vData.toInt();
						i_ChanAxisGeo[nIndex1 - 1][1] = nTemp1;
						//Citire: a treia axa GEO
						szItem = m_sTargetSyst + QString("/NC/_N_CH_TEA_ACX/$MC_AXCONF_GEOAX_ASSIGN_TAB[u%1").arg(nIndex1) + ",3]";
						eError = m_capServerReadWrite.read(szItem, vData);
						nTemp1 = vData.toInt();
						i_ChanAxisGeo[nIndex1 - 1][2] = nTemp1;
						//Completare date axe: axe alocate la axele GEO
						for (nIndex2 = 0; nIndex2 < 3; nIndex2++){
							if (i_ChanAxisGeo[nIndex1 - 1][nIndex2] > 0){
								//scriu pe ecran nr. canal
								pAxNameLbl1[nNrDatAxes]->setText(QString("Ch.%1 ").arg(nIndex1));
								//citesc numele axei geometrice
								szItem = m_sTargetSyst + QString("/NC/_N_CH_TEA_ACX/$MC_AXCONF_GEOAX_NAME_TAB[u%1").arg(nIndex1) + QString(",%1]").arg(nIndex2 + 1);
								eError = m_capServerReadWrite.read(szItem, vData);
								s_temp1 = vData.toString();
								//citesc numele axei in canal
								szItem = m_sTargetSyst + QString("/NC/_N_CH_TEA_ACX/$MC_AXCONF_CHANAX_NAME_TAB[u%1").arg(nIndex1) + QString(",%1]").arg(i_ChanAxisGeo[nIndex1 - 1][nIndex2]);
								eError = m_capServerReadWrite.read(szItem, vData);
								s_temp2 = vData.toString();
								//scriu pe ecran nr. axa Geo.
								pAxNameLbl2[nNrDatAxes]->setText(QString(" %1").arg(nIndex2 + 1));
								//scriu pe ecran nume axa Geo.
								pAxNameLbl3[nNrDatAxes]->setText(s_temp1);
								pAxNameLbl3[nNrDatAxes]->setHmiUseBigFont(true);
								//scriu pe ecran nr. axa canal
								pAxNameLbl4[nNrDatAxes]->setText(QString(" %1").arg(i_ChanAxisGeo[nIndex1 - 1][nIndex2]));
								//scriu pe ecran nume axa canal
								pAxNameLbl5[nNrDatAxes]->setText(s_temp2);
								//afisez si cota WCS
								pAxNameLbl6[nNrDatAxes]->setHmiUseBigFont(true);
								//citesc numarul axei masina aferent axei canal
								szItem = m_sTargetSyst + QString("/NC/_N_CH_TEA_ACX/$MC_AXCONF_MACHAX_USED[u%1").arg(nIndex1) + QString(",%1]").arg(i_ChanAxisGeo[nIndex1 - 1][nIndex2]);
								eError = m_capServerReadWrite.read(szItem, vData);
								nTemp1 = vData.toInt();
								//citesc numele axei masina aferent axei canal
								szItem = m_sTargetSyst + QString("/NC/_N_NC_TEA_ACX/$MN_AXCONF_MACHAX_NAME_TAB[u1,%1]").arg(nTemp1);
								eError = m_capServerReadWrite.read(szItem, vData);
								s_temp1 = vData.toString();
								//scriu pe ecran nr. axa masina
								pAxNameLbl7[nNrDatAxes]->setText(QString(" %1").arg(nTemp1));
								//scriu pe ecran nume axa masina
								pAxNameLbl8[nNrDatAxes]->setText(s_temp1);
								//afisez si cota MCS
								//afisez si viteza
								szItemAx.append(QString("/Channel/GeometricAxis/actToolEdgeCenterPos[u%1").arg(nIndex1) + QString(", %1]").arg(i_ChanAxisGeo[nIndex1 - 1][nIndex2]));
								szItemAx.append(QString("/Channel/MachineAxis/actToolBasePos[u%1").arg(nIndex1) + QString(", %1]").arg(i_ChanAxisGeo[nIndex1 - 1][nIndex2]));
								szItemAx.append(QString("/Channel/MachineAxis/aaVactM[u%1").arg(nIndex1) + QString(", %1]").arg(i_ChanAxisGeo[nIndex1 - 1][nIndex2]));
								nNrAxeMap++;
								nNrAxeMap++;
								nNrDatAxes++;
							}
						}
						//citesc numerele axelor masina folosite [CHAN]
						for (nIndex2 = 1 ; nIndex2 <= i_MaxNumAxes ; nIndex2++){
							//verific sa nu fie deja in lista ca axe Geo
							if (( nIndex2 != i_ChanAxisGeo[nIndex1-1][0])&&( nIndex2 != i_ChanAxisGeo[nIndex1-1][1]) && ( nIndex2 != i_ChanAxisGeo[nIndex1-1][2])){
								//citesc numele axei masina aferent axei canal (0 daca nu e folosita)
								szItem = m_sTargetSyst + QString("/NC/_N_CH_TEA_ACX/$MC_AXCONF_MACHAX_USED[u%1").arg(nIndex1) + QString(",%1]").arg(nIndex2);
								eError = m_capServerReadWrite.read(szItem, vData);
								nTemp1 = vData.toInt();
								//daca axa e folosita completez tabelul
								if (nTemp1 > 0){
									//scriu pe ecran nr. canal
									pAxNameLbl1[nNrDatAxes]->setText(QString("Ch.%1 ").arg(nIndex1));
									//citesc numarul arborelui aferent
									szItem = m_sTargetSyst + QString("/NC/_N_AX_TEA_ACX/$MA_SPIND_ASSIGN_TO_MACHAX[u%1]").arg(nTemp1);
									eError = m_capServerReadWrite.read(szItem, vData);
									nTemp2 = vData.toInt();
									if (nTemp2 > 0){
										b_isSpindle = true;
										pSpNameLbl1[nNrDatSpindle]->setText(QString("Ch.%1 ").arg(nIndex1));
										pSpNameLbl2[nNrDatSpindle]->setText(QString("S%1 ").arg(nTemp2));
									}
									else{
										b_isSpindle = false;
									}
									//citesc numele axei in canal
									szItem = m_sTargetSyst + QString("/NC/_N_CH_TEA_ACX/$MC_AXCONF_CHANAX_NAME_TAB[u%1").arg(nIndex1) + QString(",%1]").arg(nIndex2);
									eError = m_capServerReadWrite.read(szItem, vData);
									s_temp2 = vData.toString();
									//scriu pe ecran nr. axa Geo. [adica nimic]
									pAxNameLbl2[nNrDatAxes]->setText(" ");
									//scriu pe ecran nume axa Geo. [adica nimic]
									pAxNameLbl3[nNrDatAxes]->setText(" ");
									//scriu pe ecran nr. axa canal
									pAxNameLbl4[nNrDatAxes]->setText(QString(" %1").arg(nIndex2));
									//scriu pe ecran nume axa canal
									pAxNameLbl5[nNrDatAxes]->setText(s_temp2);
									pAxNameLbl5[nNrDatAxes]->setHmiFonts(true);
									if (b_isSpindle == false){
										pAxNameLbl5[nNrDatAxes]->setHmiUseBigFont(true);}
									//citesc numele axei masina aferent axei canal
									szItem = m_sTargetSyst + QString("/NC/_N_NC_TEA_ACX/$MN_AXCONF_MACHAX_NAME_TAB[u1,%1]").arg(nTemp1);
									eError = m_capServerReadWrite.read(szItem, vData);
									s_temp1 = vData.toString();
									//afisez si cota WCS
									if (b_isSpindle == false){
										pAxNameLbl6[nNrDatAxes]->setHmiUseBigFont(true);}
									//scriu pe ecran nr. axa masina
									pAxNameLbl7[nNrDatAxes]->setText(QString(" %1").arg(nTemp1));
									//scriu pe ecran nume axa masina
									pAxNameLbl8[nNrDatAxes]->setText(s_temp1);
									pAxNameLbl8[nNrDatAxes]->setHmiFonts(true);
									if (b_isSpindle == true){
										//scriu pe ecran numele axei
										pSpNameLbl3[nNrDatSpindle]->setText(s_temp1);
									}
									//citesc unitatea de masura pt. cota
									szItem = m_sTargetSyst + QString("/NC/_N_AX_TEA_ACX/$MA_IS_ROT_AX[u%1]").arg(nTemp1);
									eError = m_capServerReadWrite.read(szItem, vData);
									nTemp1 = vData.toInt();
									if (nTemp1 == 1){
										bAxNameLbl61[nNrDatAxes] = true;}
									//afisez si cota MCS
									//gata. Trec la urmatoarea axa
									if (b_isSpindle == true){
										szItemSp.append(QString("/Channel/MachineAxis/actToolBasePos[u%1").arg(nIndex1) + QString(", %1]").arg(nIndex2));
										szItemSp.append(QString("/Channel/Spindle/actSpeed[u%1").arg(nIndex1)   + QString(", %1]").arg(nTemp2));
										szItemSp.append(QString("/Channel/Spindle/driveLoad[u%1").arg(nIndex1) + QString(", %1]").arg(nTemp2));
										nNrDatSpindle++;
										nNrSpiMap++;
										nNrSpiMap++;
										nNrSpiChan++;
									}
									nNrDatAxes++;
									//cer citirea pozitiei MCS
									szItemAx.append(QString("/Channel/GeometricAxis/actToolEdgeCenterPos[u%1").arg(nIndex1) + QString(", %1]").arg(nIndex2));
									//cer citirea pozitiei WCS
									szItemAx.append(QString("/Channel/MachineAxis/actToolBasePos[u%1").arg(nIndex1) + QString(", %1]").arg(nIndex2));
									//cer citirea vitezei MCS
									szItemAx.append(QString("/Channel/MachineAxis/aaVactM[u%1").arg(nIndex1) + QString(", %1]").arg(nIndex2));
									nNrAxeMap++;
									nNrAxeMap++;
								}//end => daca axa e folosita completez tabelul
							}//end => verific sa nu fie deja in lista ca axe Geo
						}//end => citesc numerele axelor masina folosite [CHAN]
					}//end => Pentru Fiecare canal activ
				}//end => citesc numerele canalelor folosite in sistem
			}//end => daca lookup a reusit
			else
			{
				//lookup nu a reusit
				pListaLog1->item(1)->setText("Target: Lookup error: " + eError);
				add_log2("Error: Target lookup error: " + eError);
			}
		}//end => daca maparea a reusit
		else
		{
			//maparea nu a reusit
			pListaLog1->item(1)->setText("Target: Map error: " + eError);
			add_log2("Error: Target map error: " + eError);
		}
	}//end => target OK

	if (b_TargetOK == true)
	{
		QObject::connect(&m_capServerHotlink1,
	 					 SIGNAL(multiAdviseData(SlCapErrorEnum, const QVector<SlCapAdviseResultType>&)),
						 this,
						 SLOT(multiAdviseDataSlot1(SlCapErrorEnum, const QVector<SlCapAdviseResultType>&)));
		// completare specificatie "multi read"
		QVector<SlCapAdviseSpecType> vecAdviseSpec1;
		//adaug info despre canal
		nIndex2 = szItemCh.count();
		for (nIndex1 = 0; nIndex1 < nIndex2; nIndex1++){
			vecAdviseSpec1.append(SlCapAdviseSpecType(szItemCh.at(nIndex1)));
		}
		//adaug info despre axe
		nIndex2 = szItemAx.count();
		for (nIndex1 = 0; nIndex1 < nIndex2; nIndex1++){
			vecAdviseSpec1.append(SlCapAdviseSpecType(szItemAx.at(nIndex1)));
		}
		//adaug info despre arbori
		nIndex2 = szItemSp.count();
		for (nIndex1 = 0; nIndex1 < nIndex2; nIndex1++){
			vecAdviseSpec1.append(SlCapAdviseSpecType(szItemSp.at(nIndex1)));
		}
		// advise data
		eError = m_capServerHotlink1.multiAdvise(vecAdviseSpec1, m_capHandleHotlink1);
		if( SL_CAP_OK == eError ){
			pListaLog1->item(3)->setText(">multiadv.1 OK");
		}
		else{
			pListaLog1->item(3)->setText(">multiadv.1 Err. " + eError);
		}
		//INCERC SA CITESC SI VARIABILELE
		QObject::connect(&m_capServerHotlink2,
	 					 SIGNAL(multiAdviseData(SlCapErrorEnum, const QVector<SlCapAdviseResultType>&)),
						 this,
						 SLOT(multiAdviseDataSlot2(SlCapErrorEnum, const QVector<SlCapAdviseResultType>&)));
		QVector<SlCapAdviseSpecType> vecAdviseSpec2;
		//citesc o functie M in multiread
		nTempS = QString("/Channel/SelectedFunctions/Mval[u1, 1]");
		vecAdviseSpec2.append(SlCapAdviseSpecType(nTempS));
		// completare specificatie "multi read"
		nTempS = led_ch1_var[0]->text();
		if (nTempS.length() > 0){
			vecAdviseSpec2.append(SlCapAdviseSpecType(nTempS));}
		pLblNcVar[0]->setToolTip(nTempS);
		pLblMonVar[0]->setToolTip(nTempS);
		nTempS = led_ch1_var[1]->text();
		if (nTempS.length() > 0){
			vecAdviseSpec2.append(SlCapAdviseSpecType(nTempS));}
		pLblNcVar[1]->setToolTip(nTempS);
		nTempS = led_ch1_var[2]->text();
		if (nTempS.length() > 0){
			vecAdviseSpec2.append(SlCapAdviseSpecType(nTempS));}
		pLblNcVar[2]->setToolTip(nTempS);
		nTempS = led_ch1_var[3]->text();
		if (nTempS.length() > 0){
			vecAdviseSpec2.append(SlCapAdviseSpecType(nTempS));}
		pLblNcVar[3]->setToolTip(nTempS);
		// advise data
		eError = m_capServerHotlink2.multiAdvise(vecAdviseSpec2, m_capHandleHotlink2);
		if( SL_CAP_OK == eError ){
			pListaLog1->item(4)->setText(">multiadv.2 OK");
		}
		else{
			pListaLog1->item(4)->setText(">multiadv.2 Err." + eError);
		}
	}
	pListaLog1->item(7)->setText("Scouting on ...");
	b_readstaon = false;
}

void cam840aform::getinifil()
{
	QString s_temp, s_appbase;
	int i_linux = 0;
	int i_temp = 0;

	s_temp = QApplication::applicationDirPath();
	add_log2("App. path: " + s_temp);
	i_linux = QString::compare(s_temp.left(5), "/card", Qt::CaseSensitive);
	//Verific daca structura este de forma: [Windows Path]/siemens/sinumerik/hmi/base
	s_appbase = "/siemens/sinumerik/hmi/base";
	i_temp = s_appbase.length();
	if((i_temp < s_temp.length()) && (QString::compare(s_temp.right(i_temp), s_appbase, Qt::CaseInsensitive) == 0)){
		//Structura corespunde: generez calea
		m_sFilePath = s_temp.left(s_temp.length() - i_temp) + "/oem/sinumerik/hmi/appl/";
	}
	else{
		if	(i_linux == 0){
			//LINUX: caut in card...oem fisierul INI cu setarile aplicatiei
			m_sFilePath = "/card/oem/sinumerik/hmi/appl/";
		}
		else{
			//WINDOWS: caut in radacina! fisierul INI cu setarile aplicatiei
			m_sFilePath = QApplication::applicationDirPath().left(1) + ":/Siemens/";
		}
	}
	m_sSettingsFile = m_sFilePath + "cam840a.ini";
}

void cam840aform::readinifil()
{
	bool b_temp = false;
	int i_temp = 0;
	QString s_temp;
	SlCapErrorEnum eError;

	//dezactivez orice eventual hotlink pana cand refac ecranul
	eError = m_capServerHotlink1.unadvise(m_capHandleHotlink1);
	eError = m_capServerHotlink2.unadvise(m_capHandleHotlink2);
		
	QFile *f_SetFile = new QFile();

	if (f_SetFile->exists(m_sSettingsFile))
    {
        //fisierul de setari exista... trec mai departe
		pListaLog1->item(6)->setText(">ini: OK >" + m_sSettingsFile);
		add_log2("INI file connected: " + m_sSettingsFile);
        QSettings settings(m_sSettingsFile, QSettings::IniFormat, this);
		//citesc setarile pt. display
		b_temp = settings.value("display/zoom", false).toBool();
		chk_afis_zoom->setChecked(b_temp);
		b_temp = settings.value("display/show_no_space", true).toBool();
		chk_afis_nosp->setChecked(b_temp);
		b_temp = settings.value("display/show_channels", true).toBool();
		chk_afis_chan->setChecked(b_temp);
		b_temp = settings.value("display/show_axes", true).toBool();
		chk_afis_axes->setChecked(b_temp);
		i_temp = settings.value("display/show_axno", -1).toInt();
		spn_afis_axno->setValue(i_temp);
		b_temp = settings.value("display/show_spindles", true).toBool();
		chk_afis_spin->setChecked(b_temp);
		i_temp = settings.value("display/show_spno", -1).toInt();
		spn_afis_spno->setValue(i_temp);
		s_temp = settings.value("display/show_axes_width", "").toString();
		led_afis_axes->setText(s_temp);
		led_afis_axes->setToolTip(QString("layout axe: ") + s_temp);
		//citesc setarile pt. chart
		b_temp = settings.value("chart1/show_ncvar", true).toBool();
		chk_ch1_afisv1->setChecked(b_temp);
		b_temp = settings.value("chart1/show_chart", true).toBool();
		chk_ch1_afchart->setChecked(b_temp);
		b_temp = settings.value("chart1/show_table", true).toBool();
		chk_ch1_aftable->setChecked(b_temp);
		i_temp = settings.value("chart1/min_y", 0).toInt();
		spn_ch1_fx_min_y->setValue(i_temp);
		i_temp = settings.value("chart1/max_y", 0).toInt();
		spn_ch1_fx_max_y->setValue(i_temp);
		i_temp = settings.value("chart1/interval", 1000).toInt();
		spn_ch1_trd->setValue(i_temp);
		i_temp = settings.value("chart1/size", 501).toInt();
		spn_ch1_size->setValue(i_temp);
		s_temp = settings.value("chart1/m_start", "410").toString();
		led_ch1_varM[0]->setText(s_temp);
		s_temp = settings.value("chart1/m_stop", "411").toString();
		led_ch1_varM[1]->setText(s_temp);
		s_temp = settings.value("chart1/ncvar1", "").toString();
		led_ch1_var[0]->setText(s_temp);
		s_temp = settings.value("chart1/ncvar2", "").toString();
		led_ch1_var[1]->setText(s_temp);
		s_temp = settings.value("chart1/ncvar3", "").toString();
		led_ch1_var[2]->setText(s_temp);
		s_temp = settings.value("chart1/ncvar4", "").toString();
		led_ch1_var[3]->setText(s_temp);
		//citesc setarile pt. AlarmLog
		b_temp = settings.value("almlog/type1", false).toBool();
		chk_alm_typ[0]->setChecked(b_temp);
		b_temp = settings.value("almlog/type2", true).toBool();
		chk_alm_typ[1]->setChecked(b_temp);
		b_temp = settings.value("almlog/type3", true).toBool();
		chk_alm_typ[2]->setChecked(b_temp);
		b_temp = settings.value("almlog/type4", true).toBool();
		chk_alm_typ[3]->setChecked(b_temp);
		s_temp = settings.value("almlog/m_start", "420").toString();
		led_alm_varM[0]->setText(s_temp);
		s_temp = settings.value("almlog/m_stop", "421").toString();
		led_alm_varM[1]->setText(s_temp);
		s_temp = settings.value("almlog/m_clear", "422").toString();
		led_alm_varM[2]->setText(s_temp);
		s_temp = settings.value("almlog/m_save", "423").toString();
		led_alm_varM[3]->setText(s_temp);
		s_temp = settings.value("almlog/logfile", m_sFilePath + "almlog.csv").toString();
		led_alm_file->setText(s_temp);
		b_temp = settings.value("almlog/logfile_incr", false).toBool();
		chk_alm_fincr->setChecked(b_temp);
		i_temp = settings.value("almlog/logfile_modulo", 10).toInt();
		spn_alm_fincr->setValue(i_temp);
		i_temp = settings.value("almlog/logfile_index_def", 0).toInt();
		spn_alm_fincd->setValue(i_temp);
		b_temp = settings.value("almlog/logfile_save_index", false).toBool();
		chk_alm_finca->setChecked(b_temp);
		if(b_temp){
			i_temp = settings.value("almlog/logfile_index_act", 0).toInt();
			spn_alm_finca->setValue(i_temp);
		}
		else{
			i_temp = settings.value("almlog/logfile_index_def", 0).toInt();
			spn_alm_finca->setValue(i_temp);
		}
		b_temp = settings.value("almlog/logfile_extend", false).toBool();
		chk_alm_simpl->setChecked(b_temp);
		b_temp = settings.value("almlog/logfile_clrstart", false).toBool();
		chk_alm_clrstart->setChecked(b_temp);
		b_temp = settings.value("almlog/logfile_stopsave", false).toBool();
		chk_alm_stopsave->setChecked(b_temp);
    }
    else
    {
		pListaLog1->item(6)->setText(">ini: INI not found: " + m_sSettingsFile);
		add_log2("Error: error on connection to INI file: " + m_sSettingsFile);
		//citesc setarile pt. display
		chk_afis_zoom->setChecked(false);
		chk_afis_nosp->setChecked(true);
		chk_afis_chan->setChecked(true);
		chk_afis_axes->setChecked(true);
		spn_afis_axno->setValue(-1);
		chk_afis_spin->setChecked(true);
		spn_afis_spno->setValue(-1);
		led_afis_axes->setText("");
		led_afis_axes->setToolTip("");
		chk_ch1_afisv1->setChecked(true);
		chk_ch1_afchart->setChecked(true);
		chk_ch1_aftable->setChecked(false);
		spn_ch1_fx_min_y->setValue(0);
		spn_ch1_fx_max_y->setValue(0);
		spn_ch1_trd->setValue(100);
		spn_ch1_size->setValue(501);
		led_ch1_varM[0]->setText("410");
		led_ch1_varM[1]->setText("411");
		led_ch1_var[0]->setText("/Channel/MachineAxis/actToolBasePos[u1,1]");
		led_ch1_var[1]->setText("");
		led_ch1_var[2]->setText("");
		led_ch1_var[3]->setText("/Channel/Spindle/driveLoad[u1,1]");
		//citesc setarile pt. AlarmLog
		chk_alm_typ[0]->setChecked(false);
		chk_alm_typ[1]->setChecked(true);
		chk_alm_typ[2]->setChecked(true);
		chk_alm_typ[3]->setChecked(true);
		led_alm_varM[0]->setText("420");
		led_alm_varM[1]->setText("421");
		led_alm_varM[2]->setText("422");
		led_alm_varM[3]->setText("423");
		led_alm_file->setText(m_sFilePath + "almlog.csv");
		chk_alm_fincr->setChecked(false);
		spn_alm_fincr->setValue(10);
		spn_alm_finca->setValue(0);
		spn_alm_fincd->setValue(0);
		chk_alm_finca->setChecked(false);
		chk_alm_simpl->setChecked(false);
		chk_alm_clrstart->setChecked(false);
		chk_alm_stopsave->setChecked(false);
	}
	almlog_file();
}

void cam840aform::saveinifil()
{
	bool b_temp;
	int i_temp;
	QString s_temp;

    //fisierul de setari exista... trec mai departe
	QSettings settings(m_sSettingsFile, QSettings::IniFormat);
	//scriu setarile pt. display
	b_temp  = chk_afis_zoom->checkState();
	settings.setValue("display/zoom", b_temp);
	b_temp  = chk_afis_nosp->checkState();
	settings.setValue("display/show_no_space", b_temp);
	b_temp  = chk_afis_chan->checkState();
	settings.setValue("display/show_channels", b_temp);
	b_temp = chk_afis_axes->checkState();
	settings.setValue("display/show_axes", b_temp);
	i_temp = spn_afis_axno->value();
	settings.setValue("display/show_axno", i_temp);
	b_temp = chk_afis_spin->checkState();
	settings.setValue("display/show_spindles", b_temp);
	i_temp = spn_afis_spno->value();
	settings.setValue("display/show_spno", i_temp);
	s_temp = led_afis_axes->text().trimmed();
	settings.setValue("display/show_axes_width", s_temp);
	//scriu variabilele de monitorizat pt chart
	b_temp = chk_ch1_afisv1->checkState();
	settings.setValue("chart1/show_ncvar", b_temp);
	b_temp = chk_ch1_afchart->checkState();
	settings.setValue("chart1/show_chart", b_temp);
	b_temp = chk_ch1_aftable->checkState();
	settings.setValue("chart1/show_table", b_temp);
	i_temp = spn_ch1_fx_min_y->value();
	settings.setValue("chart1/min_y", i_temp);
	i_temp = spn_ch1_fx_max_y->value();
	settings.setValue("chart1/max_y", i_temp);
	i_temp = spn_ch1_trd->value();
	settings.setValue("chart1/interval", i_temp);
	i_temp = spn_ch1_size->value();
	settings.setValue("chart1/size", i_temp);
	s_temp = led_ch1_varM[0]->text().trimmed();
	settings.setValue("chart1/m_start", s_temp);
	s_temp = led_ch1_varM[1]->text().trimmed();
	settings.setValue("chart1/m_stop", s_temp);
	s_temp = led_ch1_var[0]->text().trimmed();
	settings.setValue("chart1/ncvar1", s_temp);
	s_temp = led_ch1_var[1]->text().trimmed();
	settings.setValue("chart1/ncvar2", s_temp);
	s_temp = led_ch1_var[2]->text().trimmed();
	settings.setValue("chart1/ncvar3", s_temp);
	s_temp = led_ch1_var[3]->text().trimmed();
	settings.setValue("chart1/ncvar4", s_temp);
	//salvez setarile pt. AlarmLog
	b_temp = chk_alm_typ[0]->checkState();
	settings.setValue("almlog/type1", b_temp);
	b_temp = chk_alm_typ[1]->checkState();
	settings.setValue("almlog/type2", b_temp);
	b_temp = chk_alm_typ[2]->checkState();
	settings.setValue("almlog/type3", b_temp);
	b_temp = chk_alm_typ[3]->checkState();
	settings.setValue("almlog/type4", b_temp);
	s_temp = led_alm_varM[0]->text().trimmed();
	settings.setValue("almlog/m_start", s_temp);
	s_temp = led_alm_varM[1]->text().trimmed();
	settings.setValue("almlog/m_stop", s_temp);
	s_temp = led_alm_varM[2]->text().trimmed();
	settings.setValue("almlog/m_clear", s_temp);
	s_temp = led_alm_varM[3]->text().trimmed();
	settings.setValue("almlog/m_save", s_temp);
	s_temp = led_alm_file->text().trimmed();
	settings.setValue("almlog/logfile", s_temp);
	b_temp = chk_alm_fincr->checkState();
	settings.setValue("almlog/logfile_incr", b_temp);
	i_temp = spn_alm_fincr->value();
	settings.setValue("almlog/logfile_modulo", i_temp);
	i_temp = spn_alm_finca->value();
	settings.setValue("almlog/logfile_index_act", i_temp);
	i_temp = spn_alm_fincd->value();
	settings.setValue("almlog/logfile_index_def", i_temp);
	b_temp = chk_alm_finca->checkState();
	settings.setValue("almlog/logfile_save_index", b_temp);
	b_temp = chk_alm_simpl->checkState();
	settings.setValue("almlog/logfile_extend", b_temp);
	b_temp = chk_alm_clrstart->checkState();
	settings.setValue("almlog/logfile_clrstart", b_temp);
	b_temp = chk_alm_stopsave->checkState();
	settings.setValue("almlog/logfile_stopsave", b_temp);

	add_log2("Save INI file: " + m_sSettingsFile);

	almlog_file();
}

void cam840aform::savealmindex()
{
	int i_temp;
    //fisierul de setari exista... trec mai departe
	QSettings settings(m_sSettingsFile, QSettings::IniFormat);

	i_temp = spn_alm_finca->value();
	settings.setValue("almlog/logfile_index_act", i_temp);

	add_log2("Save AlarmLogIndex in INI file: " + m_sSettingsFile);
}

void cam840aform::set_ecran(int i_scr_no)
{
	b_settingon = true;
	bool b_temp = false;
	int i_temp1 = 0;
	int i_temp2 = 0;
	int nIndex1 = 0;

	setStatusBarVisible(false);

	if (i_scr_no == 0){ //ecran0 = neinitializat
		i_scr_no = 8;
	}

	if (i_scr_no == 1){ //ecran0 Sinoptic
		//Afisare canale
		b_temp = chk_afis_chan->checkState();
		if (b_temp){i_temp1 = nNrDatChan;} //afisez doar canalele active, doar daca trebuie
		else{i_temp1 = 0;}
		for (nIndex1 = 0; nIndex1 < i_temp1; nIndex1++){
			pChChNo[nIndex1]->setVisible(true);
			pChState[nIndex1]->setVisible(true);
			pChParts[nIndex1]->setVisible(true);
			pChActBl[nIndex1]->setVisible(true);
			pChSelWP[nIndex1]->setVisible(true);
			pChProgN[nIndex1]->setVisible(true);
		}
		for (nIndex1 = i_temp1; nIndex1 < 10; nIndex1++){
			pChChNo[nIndex1]->setVisible(false);
			pChState[nIndex1]->setVisible(false);
			pChParts[nIndex1]->setVisible(false);
			pChActBl[nIndex1]->setVisible(false);
			pChSelWP[nIndex1]->setVisible(false);
			pChProgN[nIndex1]->setVisible(false);
		}
		//Afisare axe
		b_temp = chk_afis_axes->checkState();
		if(b_temp){
			//citesc numarul de axe de afisat
			i_temp1 = spn_afis_axno->value();
			//afisez doar axele active, doar daca trebuie
			if ((i_temp1 > 31) || (i_temp1 < 0)){i_temp1 = nNrDatAxes;}
		}
		else{i_temp1 = 0;}
		for (nIndex1 = 0; nIndex1 < i_temp1; nIndex1++){
			pAxNameLbl1[nIndex1]->setVisible(true);
			pAxNameLbl2[nIndex1]->setVisible(true);
			pAxNameLbl3[nIndex1]->setVisible(true);
			pAxNameLbl4[nIndex1]->setVisible(true);
			pAxNameLbl5[nIndex1]->setVisible(true);
			pAxNameLbl6[nIndex1]->setVisible(true);
			pAxNameLbl7[nIndex1]->setVisible(true);
			pAxNameLbl8[nIndex1]->setVisible(true);
			pAxNameLbl9[nIndex1]->setVisible(true);
			pAxNameLbla[nIndex1]->setVisible(true);
		}
		for (nIndex1 = i_temp1; nIndex1 < 31; nIndex1++){
			pAxNameLbl1[nIndex1]->setVisible(false);
			pAxNameLbl2[nIndex1]->setVisible(false);
			pAxNameLbl3[nIndex1]->setVisible(false);
			pAxNameLbl4[nIndex1]->setVisible(false);
			pAxNameLbl5[nIndex1]->setVisible(false);
			pAxNameLbl6[nIndex1]->setVisible(false);
			pAxNameLbl7[nIndex1]->setVisible(false);
			pAxNameLbl8[nIndex1]->setVisible(false);
			pAxNameLbl9[nIndex1]->setVisible(false);
			pAxNameLbla[nIndex1]->setVisible(false);
		}
		//Afisare arbori
		b_temp = chk_afis_spin->checkState();
		if(b_temp){
			//citesc numarul de arbori de afisat
			i_temp1 = spn_afis_spno->value();
			//afisez doar arborii activi, doar daca trebuie
			if ((i_temp1 > 10) || (i_temp1 < 0)){
				i_temp1 = nNrDatSpindle;
			}
		}
		else{i_temp1 = 0;}
		for (nIndex1 = 0; nIndex1 < i_temp1; nIndex1++){
			pSpNameLbl1[nIndex1]->setVisible(true);
			pSpNameLbl2[nIndex1]->setVisible(true);
			pSpNameLbl3[nIndex1]->setVisible(true);
			pSpNameLbl4[nIndex1]->setVisible(true);
			pSpNameLbl5[nIndex1]->setVisible(true);
			pSpLoad[nIndex1]->setVisible(true);
		}
		for (nIndex1 = i_temp1; nIndex1 < 10; nIndex1++){
			pSpNameLbl1[nIndex1]->setVisible(false);
			pSpNameLbl2[nIndex1]->setVisible(false);
			pSpNameLbl3[nIndex1]->setVisible(false);
			pSpNameLbl4[nIndex1]->setVisible(false);
			pSpNameLbl5[nIndex1]->setVisible(false);
			pSpLoad[nIndex1]->setVisible(false);
		}
		gbSinoptic->setVisible(true);
	}
	else{
		gbSinoptic->setVisible(false);
	}

	if (i_scr_no == 2){ //ecran2 Monitor
		//Screen2: Monitor
		//Afisare Variabile, Model, Chart, etc
		chart_ch2->fx_min_y = 0;		//Setari Minim / Maxim
		chart_ch2->fx_max_y = 0;
		gbMonitorVar->setVisible(true);	//Afisare variabile
		chart_ch2->setVisible(true);
		table_ch2->setVisible(false);
		gbMonitor->setVisible(true);
	}
	else{
		gbMonitorVar->setVisible(false);
		gbMonitor->setVisible(false);
	}

	if (i_scr_no == 3){ //ecran3 Chart
		//Screen3: Chart
		//Afisare Variabile, Model, Chart, etc
		chart_ch1->fx_min_y = spn_ch1_fx_min_y->value();
		chart_ch1->fx_max_y = spn_ch1_fx_max_y->value();
		b_temp = chk_ch1_afisv1->checkState();
		gbChartVar->setVisible(b_temp);
		//redimensionare model
		i_temp1 = model_ch1->rowCount();
		i_temp2 = spn_ch1_size->value();
		if (i_temp1 < i_temp2){	//adaugare puncte in grafic
			model_ch1->insertRows(i_temp1, i_temp2 - i_temp1,QModelIndex());
			add_log2(QString("chart_ch1: add %1 points").arg(i_temp2 - i_temp1));
			for(i_temp2 = i_temp1; i_temp2 < model_ch1->rowCount(); i_temp2++)
			{
				model_ch1->setData(model_ch1->index(i_temp2,0,QModelIndex()), i_temp2);
				model_ch1->setData(model_ch1->index(i_temp2,1,QModelIndex()), 0.0);
				model_ch1->setData(model_ch1->index(i_temp2,1,QModelIndex()),QColor(212, 45, 47),Qt::DecorationRole);
				model_ch1->setData(model_ch1->index(i_temp2,2,QModelIndex()), 0.0);
				model_ch1->setData(model_ch1->index(i_temp2,2,QModelIndex()),QColor(119,160, 71),Qt::DecorationRole);
				model_ch1->setData(model_ch1->index(i_temp2,3,QModelIndex()), 0.0);
				model_ch1->setData(model_ch1->index(i_temp2,3,QModelIndex()),QColor( 70,130,180),Qt::DecorationRole);
				model_ch1->setData(model_ch1->index(i_temp2,4,QModelIndex()), 0.0);
				model_ch1->setData(model_ch1->index(i_temp2,4,QModelIndex()),QColor(239,246,123),Qt::DecorationRole);
			}
		}
		if (i_temp1 > i_temp2){	//scoatere puncte din grafic
			model_ch1->removeRows(i_temp2, i_temp1 - i_temp2,QModelIndex());
			add_log2(QString("chart_ch1: removed %1 points").arg(i_temp1 - i_temp2));
		}
		b_temp = chk_ch1_afchart->checkState();
		chart_ch1->setVisible(b_temp);
		b_temp = chk_ch1_aftable->checkState();
		table_ch1->setVisible(b_temp);
		gbChart->setVisible(true);
	}
	else{
		gbChartVar->setVisible(false);
		if((i_scr_no == 1) && bSinopticChart){
			gbChart->setVisible(true);
		}
		else{
			gbChart->setVisible(false);
		}
	}

	if (i_scr_no == 4){ //ecran3 AlarmLog
		gbAlarmLog->setVisible(true);
	}
	else{
		gbAlarmLog->setVisible(false);
	}

	if (i_scr_no == 8){ //ecran1 Settings
		//Screen2: Setari
		gbSetari->setVisible(true);
		gbSinopticSett->setVisible(true);
		gbChartSett->setVisible(true);
		gbAlarmSett->setVisible(true);
	}
	else{
		gbSetari->setVisible(false);
		if((i_scr_no == 1) && bSinopticSett){
			gbSinopticSett->setVisible(true);
		}
		else{
			gbSinopticSett->setVisible(false);
		}
		if((i_scr_no == 3) && bChartSett){
			gbChartSett->setVisible(true);
		}
		else{
			gbChartSett->setVisible(false);
		}
		if((i_scr_no == 4) && bAlarmSett){
			gbAlarmSett->setVisible(true);
		}
		else{
			gbAlarmSett->setVisible(false);
		}
	}

	//afisare cu Zoom
	i_dispwidth = 0;
	b_temp = chk_afis_zoom->checkState();
	if (b_temp == true){ this->setZoomContent(true);}
	else{	this->setZoomContent(false);}
	//afisare comprimata axe si arbori
	b_temp = chk_afis_nosp->checkState();
	if (b_temp == true) {
		axGridLayout->setSpacing(0);
		spGridLayout->setSpacing(0);
	}
	else{
		axGridLayout->setSpacing(6);
		spGridLayout->setSpacing(6);
	}

	b_settingon = false;
}

void cam840aform::readncvar()
{
	//Transfer valori din pVarNcVar in Model tabel si chart
	int nIndex1;
	double d_temp;
	int nChartMod = 1;
	if ((b_settingon) || (b_readstaon)){return;}
	if (nChartMod == 0){
		//Varianta Clasica (umplere Chart)
		if (i_chartpos < model_ch1->rowCount()){
			model_ch1->setData(model_ch1->index(i_chartpos,0,QModelIndex()), i_chartpos*10);
			model_ch1->setData(model_ch1->index(i_chartpos,1,QModelIndex()), pVarNcVar[0]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,1,QModelIndex()),QColor(212, 45, 47),Qt::DecorationRole);
			model_ch1->setData(model_ch1->index(i_chartpos,2,QModelIndex()), pVarNcVar[1]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,2,QModelIndex()),QColor(119,160, 71),Qt::DecorationRole);
			model_ch1->setData(model_ch1->index(i_chartpos,3,QModelIndex()), pVarNcVar[2]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,3,QModelIndex()),QColor( 70,130,180),Qt::DecorationRole);
			model_ch1->setData(model_ch1->index(i_chartpos,4,QModelIndex()), pVarNcVar[3]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,4,QModelIndex()),QColor(239,246,123),Qt::DecorationRole);
			i_chartpos++;
		}
		else{
			i_chartpos = 0;
			model_ch1->setData(model_ch1->index(i_chartpos,0,QModelIndex()), i_chartpos*10);
			model_ch1->setData(model_ch1->index(i_chartpos,1,QModelIndex()), pVarNcVar[0]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,1,QModelIndex()),QColor(212, 45, 47),Qt::DecorationRole);
			model_ch1->setData(model_ch1->index(i_chartpos,2,QModelIndex()), pVarNcVar[1]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,2,QModelIndex()),QColor(119,160, 71),Qt::DecorationRole);
			model_ch1->setData(model_ch1->index(i_chartpos,3,QModelIndex()), pVarNcVar[2]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,3,QModelIndex()),QColor( 70,130,180),Qt::DecorationRole);
			model_ch1->setData(model_ch1->index(i_chartpos,4,QModelIndex()), pVarNcVar[3]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,4,QModelIndex()),QColor(239,246,123),Qt::DecorationRole);
			i_chartpos++;
		}
	}
	if (nChartMod == 1){
		//Varianta Osci...
		i_chartpos = model_ch1->rowCount() - 1;
		if (i_chartpos < model_ch1->rowCount()-1){
			model_ch1->setData(model_ch1->index(i_chartpos,0,QModelIndex()), i_chartpos*10);
			model_ch1->setData(model_ch1->index(i_chartpos,1,QModelIndex()), pVarNcVar[0]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,1,QModelIndex()),QColor(212, 45, 47),Qt::DecorationRole);
			model_ch1->setData(model_ch1->index(i_chartpos,2,QModelIndex()), pVarNcVar[1]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,2,QModelIndex()),QColor(119,160, 71),Qt::DecorationRole);
			model_ch1->setData(model_ch1->index(i_chartpos,3,QModelIndex()), pVarNcVar[2]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,3,QModelIndex()),QColor( 70,130,180),Qt::DecorationRole);
			model_ch1->setData(model_ch1->index(i_chartpos,4,QModelIndex()), pVarNcVar[3]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,4,QModelIndex()),QColor(239,246,123),Qt::DecorationRole);
			i_chartpos++;
		}
		else{
			for (nIndex1 = 0; nIndex1 < i_chartpos; nIndex1++){
				d_temp = model_ch1->index(nIndex1 + 1, 1, QModelIndex()).data().toDouble();
				model_ch1->setData(model_ch1->index(nIndex1,1,QModelIndex()), d_temp);
				d_temp = model_ch1->index(nIndex1 + 1, 2, QModelIndex()).data().toDouble();
				model_ch1->setData(model_ch1->index(nIndex1,2,QModelIndex()), d_temp);
				d_temp = model_ch1->index(nIndex1 + 1, 3, QModelIndex()).data().toDouble();
				model_ch1->setData(model_ch1->index(nIndex1,3,QModelIndex()), d_temp);
				d_temp = model_ch1->index(nIndex1 + 1, 4, QModelIndex()).data().toDouble();
				model_ch1->setData(model_ch1->index(nIndex1,4,QModelIndex()), d_temp);
			}
			model_ch1->setData(model_ch1->index(i_chartpos,1,QModelIndex()), pVarNcVar[0]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,2,QModelIndex()), pVarNcVar[1]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,3,QModelIndex()), pVarNcVar[2]->toDouble());
			model_ch1->setData(model_ch1->index(i_chartpos,4,QModelIndex()), pVarNcVar[3]->toDouble());
		}
	}

	int i_time2 = t_chart1->elapsed();
	led_ch1_time[1]->setText(QString("%1ms").arg(i_time2));
	t_chart1->restart();
}

void cam840aform::readmonvar(void)
{
	//Transfer valori din pVarMonVar in Model tabel si chart
	int i_temp1;
	if ((b_settingon) || (b_readstaon)){return;}

	//De adaugat inregistrare Trace referinta, inregistrare Trace act, comparare Trace...

	//Adaugare punct in model2
	//i_temp1 = model_ch2->rowCount();
	//model_ch2->insertRows(i_temp1, 1,QModelIndex());
	//model_ch2->setData(model_ch2->index(i_temp1,0,QModelIndex()), i_temp1);
	//model_ch2->setData(model_ch2->index(i_temp1,1,QModelIndex()), 0.0);
	//model_ch2->setData(model_ch2->index(i_temp1,1,QModelIndex()),QColor(212, 45, 47),Qt::DecorationRole);
	//model_ch2->setData(model_ch2->index(i_temp1,2,QModelIndex()), 0.0);
	//model_ch2->setData(model_ch2->index(i_temp1,2,QModelIndex()),QColor(119,160, 71),Qt::DecorationRole);

	//i_monitorpos = model_ch2->rowCount() - 1;
	//model_ch2->setData(model_ch2->index(i_monitorpos,0,QModelIndex()), i_monitorpos*10);
	//model_ch2->setData(model_ch2->index(i_monitorpos,1,QModelIndex()), pVarMonVar[0]->toDouble());
	//model_ch2->setData(model_ch2->index(i_monitorpos,1,QModelIndex()),QColor(212, 45, 47),Qt::DecorationRole);
	//model_ch2->setData(model_ch2->index(i_monitorpos,2,QModelIndex()), pVarMonVar[1]->toDouble());
	//model_ch2->setData(model_ch2->index(i_monitorpos,2,QModelIndex()),QColor(119,160, 71),Qt::DecorationRole);
	//i_monitorpos++;
}

void cam840aform::show_caption()
{
	if (b_TargetOK == false){
		readstatus(true);
		//if (b_TargetOK == true){set_ecran(1);}
	}

	int i_tempwidth = this->width();
	int i_temp2 = 0;
	int i_temp3 = 0;
	QFont s_font;
	//afisez ora in Caption Bar
	QTime t_temp = QTime::currentTime();
	QString s_temp = t_temp.toString();
	s_temp = QString(s_temp);
	setCaptionBarElementText(s_temp,1);
	//incerc sa aranjez afisarea pe ecrane
	if (i_tempwidth != i_dispwidth){
		//reglez fonturile in Setari
		s_font = led_afis_axes->font();
		i_temp2 = s_font.pixelSize();
		spn_afis_axno->setStyleSheet(QString("QSpinBox { font: %1 px;}").arg(i_temp2));
		spn_afis_spno->setStyleSheet(QString("QSpinBox { font: %1 px;}").arg(i_temp2));
		spn_ch1_fx_min_y->setStyleSheet(QString("QSpinBox { font: %1 px;}").arg(i_temp2));
		spn_ch1_fx_max_y->setStyleSheet(QString("QSpinBox { font: %1 px;}").arg(i_temp2));
		spn_ch1_trd->setStyleSheet(QString("QSpinBox { font: %1 px;}").arg(i_temp2));
		spn_ch1_size->setStyleSheet(QString("QSpinBox { font: %1 px;}").arg(i_temp2));
		//citesc dimensiunile minime pt. axe
		s_temp = led_afis_axes->text();
		led_afis_axes->setToolTip(QString(tr("layout axe: ")) + s_temp);
		QStringList s_temp1 = s_temp.split(",");
		i_temp2 = s_temp1.size();
		//add_log2(QString("Latime Ecran: %1; Font axe: %2").arg(i_tempwidth).arg(led_afis_axes->font().weight()));
		//setez ceva dimensiuni implicite
		if ((i_tempwidth < 700) && (i_tempwidth >= 650) && (i_temp2 <= 1)){
			axGridLayout->setColumnMinimumWidth(5, 171);
			axGridLayout->setColumnMinimumWidth(8, 120);
			axGridLayout->setColumnMinimumWidth(9,  80);
		}
		if ((i_tempwidth < 650) && (i_temp2 <= 1)){
			axGridLayout->setColumnMinimumWidth(5, 130);
			axGridLayout->setColumnMinimumWidth(8,  80);
			axGridLayout->setColumnMinimumWidth(9,  60);
		}
		//tin cont si de setarile referitoare la layout
		if (i_temp2 > 0){axGridLayout->setColumnMinimumWidth(0,s_temp1[0].toInt());}
		if (i_temp2 > 1){axGridLayout->setColumnMinimumWidth(1,s_temp1[1].toInt());}
		if (i_temp2 > 2){axGridLayout->setColumnMinimumWidth(2,s_temp1[2].toInt());}
		if (i_temp2 > 3){axGridLayout->setColumnMinimumWidth(3,s_temp1[3].toInt());}
		if (i_temp2 > 4){axGridLayout->setColumnMinimumWidth(4,s_temp1[4].toInt());}
		if (i_temp2 > 5){axGridLayout->setColumnMinimumWidth(5,s_temp1[5].toInt());}
		if (i_temp2 > 6){axGridLayout->setColumnMinimumWidth(6,s_temp1[6].toInt());}
		if (i_temp2 > 7){axGridLayout->setColumnMinimumWidth(7,s_temp1[7].toInt());}
		if (i_temp2 > 8){axGridLayout->setColumnMinimumWidth(8,s_temp1[8].toInt());}
		if (i_temp2 > 9){axGridLayout->setColumnMinimumWidth(9,s_temp1[9].toInt());}
		i_dispwidth = i_tempwidth;
	}
	//Stare AlarmLog
	//if(m_pQEventSink->isActive()){
	if(b_alm_active){
		lbAlarmStat->setText("AlarmLog: ON");
	}
	else{
		lbAlarmStat->setText("AlarmLog: OFF");
	}
}

void cam840aform::onNewEvent(const QList<SlAeEvent*>& rEventList)
{
    QDateTime				dt;
    QString					szTmp, szAlarmLine;
    QVariant				vTmp;
	int						nDispIdx = 0;
    int                     nSize = 0;
	SlAeAlarmStateEnum		nAlarmState;
	
	for(int i = 0; i < rEventList.size(); ++i)
	{
		szAlarmLine = "";
		SlAeEvent* pAEEvent = rEventList.at(i);
		//Timp Alarma
		dt = pAEEvent->getTimestamp();
		szAlarmLine = szAlarmLine + dt.toString("dd.MM.yyyy ; hh:mm:ss");
		//Tip Eveniment Alarma
		nAlarmState = pAEEvent->getAlarmState();
		if(chk_alm_simpl->isChecked()){
			szTmp = szTmp.setNum(pAEEvent->getAlarmState());
		}
		else{
			if(SLAE_ALARM_STATE_ACTIVE == (SLAE_ALARM_STATE_ACTIVE & nAlarmState)){
				szTmp = "NEW";
			}
			else{
				szTmp = "CLR";
			}
		}
		szAlarmLine = szAlarmLine + "; " + szTmp;
		//Numar Alarma
		szTmp = szTmp.setNum(pAEEvent->getAlarmId());
		szAlarmLine = szAlarmLine + "; " + szTmp;
		//Text Alarma
		vTmp = pAEEvent->getAttribute(SLAE_EV_ATTR_MSGTEXT);
		szTmp = vTmp.toString();
		szAlarmLine = szAlarmLine + "; " + szTmp;
		if(chk_alm_simpl->isChecked()){
			//Sursa Alarma
			szTmp = szTmp.setNum(pAEEvent->getSourceId());
			szAlarmLine = szAlarmLine + "; [Source: " + szTmp + "]";
			//Categorie Alarma
			szTmp = szTmp.setNum(pAEEvent->getCategory());
			szAlarmLine = szAlarmLine + "; [Cat. " + szTmp + "]";
		}
		if(b_alm_active){
			//Linie noua in Jurnal
			pteAlarmLog->append(szAlarmLine);
		}
	}
}