//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "CPortCtl.hpp"
#include "CPort.hpp"

#include "CSPIN.h"
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TComPort *ComPort1;
        TGroupBox *GroupBox1;
        TButton *Button7;
        TButton *Button8;
        TButton *Button19;
        TButton *Button20;
        TGroupBox *GroupBox2;
        TButton *Button3;
        TButton *Button5;
        TButton *Button13;
        TGroupBox *GroupBox3;
        TButton *Button1;
        TButton *Button9;
        TButton *Button11;
        TButton *Button4;
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TTabSheet *TabSheet2;
        TTabSheet *TabSheet3;
        TComTerminal *ComTerminal2;
        TComTerminal *ComTerminal1;
        TListBox *ListBox1;
        TCSpinEdit *CSpinEdit1;
        TButton *Button2;
        TButton *Button18;
        TComboBox *ComboBox1;
        TMemo *SMSText;
        TComPort *ComPort2;
        TProgressBar *ProgressBar1;
        TCheckBox *CheckBox1;
        TMemo *Log;
        TMemo *Memo2;
        TButton *Button6;
        TLabel *Label1;
        TButton *Button10;
        TCheckBox *AutoStart;
        TLabel *BalanceLabel;
        TOpenDialog *OpenDialog1;
        TButton *Button12;
        TButton *Button14;
        TEdit *BaseFname;
        TCSpinEdit *CountErrorsStop;
        TLabel *Label2;
        TTabSheet *TabSheet4;
        TButton *Button15;
        TMemo *Memo1;
        TMemo *Memo3;
        TButton *Button16;
        TMemo *Memo4;
        TMemo *Memo5;
        TCSpinEdit *SMS_NUM;
        TButton *Button17;
        TCheckBox *AutoDecode;
        TLabel *Label3;
        TComboBox *port1combo;
        TComboBox *port2combo;
        TButton *Button21;
        TLabel *Label4;
        TLabel *Label5;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall ComTerminal1StrRecieved(TObject *Sender,
          AnsiString &Str);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);
        void __fastcall Button9Click(TObject *Sender);
        void __fastcall Button11Click(TObject *Sender);
        void __fastcall Button8Click(TObject *Sender);
        void __fastcall Button13Click(TObject *Sender);
        void __fastcall Button18Click(TObject *Sender);
        void __fastcall Button19Click(TObject *Sender);
        void __fastcall Button20Click(TObject *Sender);
        void __fastcall ComPort1Ring(TObject *Sender);
        void __fastcall ComPort1RxFlag(TObject *Sender);
        void __fastcall ComPort1CTSChange(TObject *Sender, bool OnOff);
        void __fastcall ComPort1DSRChange(TObject *Sender, bool OnOff);
        void __fastcall ComPort1RLSDChange(TObject *Sender, bool OnOff);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall ListBox1Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall ComTerminal2StrRecieved(TObject *Sender,
          AnsiString &Str);
        void __fastcall Button10Click(TObject *Sender);
        void __fastcall LogChange(TObject *Sender);
        void __fastcall Button12Click(TObject *Sender);
        void __fastcall Button15Click(TObject *Sender);
        void __fastcall Button16Click(TObject *Sender);
        void __fastcall Button17Click(TObject *Sender);
        void __fastcall SMS_NUMChange(TObject *Sender);
        void __fastcall ComPort2Error(TObject *Sender, TComErrors Errors);
        void __fastcall ComPort1Error(TObject *Sender, TComErrors Errors);
        void __fastcall port1comboChange(TObject *Sender);
        void __fastcall port2comboChange(TObject *Sender);
        void __fastcall Button21Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
        void __fastcall TForm1::AppException(TObject *Sender, Exception *E);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
