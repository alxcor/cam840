///////////////////////////////////////////////////////////////////////////////
/*! \file   cam840aform.h
 *  \author
 *  \date
 *  \brief  Definition file for class cam840aform
 *
 *
 */
///////////////////////////////////////////////////////////////////////////////

#ifndef CAM840AFORM_H_INCLUDED
#define CAM840AFORM_H_INCLUDED

#include <QtGui/QtGui>
#include "slgfwdialogform.h"
#include "slgfwlineedit.h"
#include "slqcap.h"
#include "slqcapnamespace.h"
#include "slaeqeventsink.h"
#include "chartxy.h"


/*! \class cam840aform
 *  \brief
 */
class cam840aform : public SlGfwDialogForm
{
	Q_OBJECT


public:

///////////////////////////////////////////////////////////////////////////
// CREATORS
///////////////////////////////////////////////////////////////////////////

/*!
*	\fn cam840aform(QWidget* pParent = 0, const QString& rszName = QString::null);
*
*  Constructor
*/
cam840aform(QWidget* pParent = 0, const QString& rszName = QString::null);

/*!
*
cam840aform(QWidget* pParent = 0, const QString& rszName = QString::null);
*
*  Destructor
*/
virtual ~cam840aform(void);


///////////////////////////////////////////////////////////////////////////////
// Overrides
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// public ACCESSORS
///////////////////////////////////////////////////////////////////////////////




///////////////////////////////////////////////////////////////////////////////
// MANIPULATORS - system overrides
///////////////////////////////////////////////////////////////////////////////


public slots:
	void readstatus(bool);
	void getinifil(void);
	void readinifil(void);
	void saveinifil(void);
	void set_ecran(int);

///////////////////////////////////////////////////////////////////////////////
// public SLOTS - system overrides
///////////////////////////////////////////////////////////////////////////////
/*!
*  \fn virtual void onFunction(const QString& rszFunction, const QString& rszArgs, bool& rbHandled);
*
*  Will be called by the framework, if a function should be executed, e.g. if a softkey
*  should execute a command
*/
virtual void onFunction (const QString& rszFunction, const QString& rszArgs, bool& rbHandled);

private:

///////////////////////////////////////////////////////////////////////////////
// member variables
///////////////////////////////////////////////////////////////////////////////
	//variabile interblocare pe timpul exec. functiilor
	bool			b_settingon;
	bool			b_readstaon;
	/*! SlQCap server pt. data access */
	SlQCap			m_capServerReadWrite;
	/*! SlQCap pt. hotlink intre var. din qt si var. nc */
	SlQCap			m_capServerHotlink1;
	SlQCap			m_capServerHotlink2;
	/*! SlQCapHandle pt. advise data */
	SlQCapHandle	m_capHandleHotlink1;
	SlQCapHandle	m_capHandleHotlink2;
	/*! SlQCapNamespace -> pt. citire NC_Data sau GUD*/
	SlQCapNamespace m_capNameSpaceNC;
	SlQCapNamespace m_capNameSpaceCH;
	SlQCapNamespace m_capNameSpaceAX;
	//fisier setari
	QString			m_sFilePath;
	//numele fisierului de setari (ini)
	QString			m_sSettingsFile;
	//Variabila eroare acces tinta
	bool			b_TargetOK;
	//Dimens. fereastra
	int				i_dispwidth;
	QGroupBox		*gbSetari;
	//Liste ce tin loc de jurnal
	QListWidget		*pListaLog1;
	QListWidget		*pListaLog2;
	int				iListaLog2;

	//Numele sistemului tinta (target) pt. mapare date de masina
	QString			m_sTargetSyst;

	//Ecran Sinoptic Canale/Axe/Arbori
	QGroupBox		*gbSinoptic;
	//Labels pt. stare canal
	QLabel			*pChChNo[10];		//numar canal
	QLabel			*pChState[10];		//stare canal
	QLabel			*pChParts[10];		//numar de piese realizat
	QLabel			*pChSelWP[10];		//program piesa selectat
	QLabel			*pChProgN[10];		//program piesa curent
	QLabel			*pChActBl[10];		//fraza actuala
	//Layout Axe
	QGridLayout		*axGridLayout;
	//Labels pt. stare axe (MCS / WCS)
	QLabel			*pAxNameLbl1[31];	//nr.  canal
	QLabel			*pAxNameLbl2[31];	//nr.  axa Geo
	QLabel			*pAxNameLbl3[31];	//nume axa Geo
	QLabel			*pAxNameLbl4[31];	//nr.  axa Canal
	QLabel			*pAxNameLbl5[31];	//nume axa Canal
	QLabel			*pAxNameLbl6[31];	//cota axa WCS
	bool			bAxNameLbl61[31];	//axa e rot.
	QLabel			*pAxNameLbl7[31];	//nr.  axa Mach
	QLabel			*pAxNameLbl8[31];	//nume axa Mach
	QLabel			*pAxNameLbl9[31];	//cota axa MCS
	QLabel			*pAxNameLbla[31];	//viteza
	//Layout Arbori
	QGridLayout		*spGridLayout;
	//Labels pt. stare arbori (MCS / WCS)
	QLabel			*pSpNameLbl1[10];	//nr.  canal
	QLabel			*pSpNameLbl2[10];	//nr. spindle
	QLabel			*pSpNameLbl3[10];	//turatie actuala
	QLabel			*pSpNameLbl4[10];	//pozitie MCS
	QLabel			*pSpNameLbl5[10];	//turatie prescr.
	QProgressBar	*pSpLoad[10];		//incarcare
	//Ecran Sinoptic: Setari
	QGroupBox		*gbSinopticSett;	//GroupBox Setari
	bool			bSinopticSett;
	QCheckBox		*chk_afis_chan;		//Afisare/Mascare Layout Canale
	QCheckBox		*chk_afis_nosp;		//Afisare Comprimata...
	QCheckBox		*chk_afis_log;		//Afisare/Mascare Layout Liste Jurnal
	QCheckBox		*chk_afis_axes;		//Afisare/Mascare Layout Axe
	QSpinBox		*spn_afis_axno;		//Numar axe afisate (-1 = toate)
	QCheckBox		*chk_afis_zoom;
	QCheckBox		*chk_afis_spin;		//Afisare/Mascare Layout Arbori
	QSpinBox		*spn_afis_spno;		//Numar arbori afisati (-1 = toti)
	QLineEdit		*led_afis_axes;
	QLineEdit		*led_afis_time;
	bool			bSinopticChart;

	//Ecran Monitor
	QGroupBox		*gbMonitor;
	QTableView			*table_ch2;		//tabel cu valori grafic
	ChartXY				*chart_ch2;		//grafic
	QStandardItemModel	*model_ch2;		//model grafic
	int					i_monitorpos;
	//Ecran Monitor: Variabile
	QGroupBox		*gbMonitorVar;		//GroupBox Variabile
	QLabel			*pLblMonVar[2];		//variabile de monitorizat
	QVariant		*pVarMonVar[2];

	//Ecran Chart
	QGroupBox		*gbChart;
	QTableView			*table_ch1;		//tabel cu valori grafic
	ChartXY				*chart_ch1;		//grafic
	QStandardItemModel	*model_ch1;		//model grafic
	int					i_chartpos;
	//Ecran Chart: Variabile
	QGroupBox		*gbChartVar;		//GroupBox Variabile
	QLabel			*pLblNcVar[4];		//variabile de monitorizat
	QVariant		*pVarNcVar[4];
	//Ecran Chart: Setari
	QGroupBox		*gbChartSett;		//GroupBox Setari
	bool			bChartSett;
	QCheckBox		*chk_ch1_afisv1;
	QCheckBox		*chk_ch1_afchart;
	QCheckBox		*chk_ch1_aftable;
	QLineEdit		*led_ch1_varM[2];
	bool			b_ch1_varM[2];
	bool			b_ch1_varMold[2];
	QLabel			*led_ch1_valM;
	QLineEdit		*led_ch1_var[4];
	QSpinBox		*spn_ch1_fx_min_y;
	QSpinBox		*spn_ch1_fx_max_y;
	QSpinBox		*spn_ch1_trd;
	QSpinBox		*spn_ch1_size;
	QLineEdit		*led_ch1_time[4];

	//Ecran Alarme
	QGroupBox		*gbAlarmLog;
	//Layout pt. jurnal alarme
	QLabel			*lbAlarmStat;
	QTextEdit		*pteAlarmLog;	//Text Edit pentru Alarm Log
	SlAeQEventSink*	m_pQEventSink;	//pointer to an SIAeQEventSink
	//Ecran Alarme: Setari
	QGroupBox		*gbAlarmSett;	//GroupBox Setari
	bool			bAlarmSett;
	QCheckBox		*chk_alm_typ[4];
	QLineEdit		*led_alm_varM[4];
	bool			b_alm_varM[4];
	bool			b_alm_varMold[4];
	bool			b_alm_active;
	QLineEdit		*led_alm_file;
	QLabel			*lbl_alm_file;
	QCheckBox		*chk_alm_fincr;
	QSpinBox		*spn_alm_fincr;
	QSpinBox		*spn_alm_fincd;
	QSpinBox		*spn_alm_finca;
	QCheckBox		*chk_alm_finca;
	QCheckBox		*chk_alm_simpl;
	QCheckBox		*chk_alm_clrstart;
	QCheckBox		*chk_alm_stopsave;

	//Timere citire Date si Afisare Stare
	QTimer			*t_readch1;
	QTimer			*t_readch2;
	QTimer			*t_status;
	QTime			*t_multi1;
	QTime			*t_multi2;
	QTime			*t_chart1;

	//numarul de canale/axe/arbori detectate in sistem
	int				nNrDatChan;
	int				nNrDatAxes;
	int				nNrDatSpindle;

///////////////////////////////////////////////////////////////////////////////
// private METHODS
///////////////////////////////////////////////////////////////////////////////
	void handleRetVal(SlCapErrorEnum eError, QString szSuccess = QString::null);
	void timer_start(void);
	void timer_stop(void);
	void monitor_start(void);
	void monitor_stop(void);
	void add_log2(QString);
	void almlog_save(void);
	void savealmindex(void);

///////////////////////////////////////////////////////////////////////////
// PRIVATE SLOTS
///////////////////////////////////////////////////////////////////////////
private slots:
	//varianta pt. o singura variabila
	//void adviseDataSlot(SlCapErrorEnum, const QVariant&, const SlCapSupplementInfoType&);
	//varianta pt. multi-data-link
	void multiAdviseDataSlot1(SlCapErrorEnum, const QVector<SlCapAdviseResultType>&);
	void multiAdviseDataSlot2(SlCapErrorEnum, const QVector<SlCapAdviseResultType>&);
	void readncvar(void);
	void readmonvar(void);
	//Afisare Data si Ora
	void show_caption(void);
	//slot pentru Alarme: EventSink
	void onNewEvent(const QList<SlAeEvent*>&);
	//slot pentru generare nume fisier alarme
	void almlog_file(void);
	void almlog_fileinc1(bool);
	void almlog_fileinc2(int);
	void almlog_fileinc3(const QString &);
};

#endif // CAM840AFORM_H_INCLUDED
