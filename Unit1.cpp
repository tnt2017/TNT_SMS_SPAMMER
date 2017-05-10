//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include <IniFiles.hpp>
#include <stdio.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CPortCtl"
#pragma link "CPort"
#pragma link "CSPIN"
#pragma resource "*.dfm"
TForm1 *Form1;

AnsiString AppDir=ExtractFilePath(Application->ExeName);
AnsiString smstext;
const char CTRLZ=26;
int error_counter=0;

//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{

}
//---------------------------------------------------------------------------


void __fastcall TForm1::AppException(TObject *Sender, Exception *E)
{
 Log->Lines->Add(E->Message);
}


void __fastcall TForm1::Button1Click(TObject *Sender)
{
 ComPort1->WriteStr("ATI\r\n");
}
//---------------------------------------------------------------------------

int htoi(char c)
{
 int i;

 if(c>='0' && c<='9')
  return StrToInt(c);
 else
  {
   switch (c)
   {
     case 'A' : return 10;
     case 'B' : return 11;
     case 'C' : return 12;
     case 'D' : return 13;
     case 'E' : return 14;
     case 'F' : return 15;
   }
  }

 return i;
}

AnsiString PhoneConvert(AnsiString S)
{
  S=StringReplace(S,"-","",TReplaceFlags() << rfReplaceAll);
  S=StringReplace(S,"+","",TReplaceFlags() << rfReplaceAll);

  S[1]='7';

  AnsiString num=S+'F';
  AnsiString num2=S+'F';

  num2[1]=num[2];
  num2[2]=num[1];

  num2[3]=num[4];
  num2[4]=num[3];

  num2[5]=num[6];
  num2[6]=num[5];

  num2[7]=num[8];
  num2[8]=num[7];

  num2[9]=num[10];
  num2[10]=num[9];

  num2[11]=num[12];
  num2[12]=num[11];

  return num2;
}


AnsiString GetHeader(AnsiString phonenum)
{
  AnsiString s1="00"; // Длина и номер SMS центра (00 - использовать дефолтный)
  AnsiString s2="11"; // Сообщение SMS-Submit (сохраненная копия)
  AnsiString s3="00"; // TP-Message-Reference. Значение 0х00 указывает на то, что в качестве номера телефона отправителя будет использоваться номер.
  AnsiString s4="0B"; // Длина телефонного номера получателя (количество цифр в нем – в нашем случае 11)
  AnsiString s5="91"; // Тип телефонного номера получателя (у нас получается «международный тип с планом нумерации Е.164/E.163

  AnsiString phone=PhoneConvert(phonenum);  

  AnsiString p1="00"; // протокол SMS
  AnsiString p2="08"; // (08=UCS2,00-latin 7 bit etc. Старший байт – не сохранять в истории получателя
  AnsiString p3="C1"; // время жизни=неделя

  AnsiString header=s1+s2+s3+s4+s5+phone+p1+p2+p3;
  return header;
}

SendSMS(AnsiString phone,AnsiString msg, bool test)
{
 int PDULEN=msg.Length()+19;

 if(!test)
 Form1->ComPort1->WriteStr("AT+CMGS=" + IntToStr(PDULEN) + "\r\n");

 AnsiString header=GetHeader(phone);

 WideString WS=msg;
 WS=WS.SubString(0,WS.Length()-2);

 AnsiString out;

 for(int i=1;i<WS.Length()+1;i++)
 {
  wchar_t c=WS[i];
  char c1=c;
  char c2=c>>8;
  AnsiString str;
  str.sprintf("%0.2X%0.2X",c2,c1);
  out+=str;
 }

 if(test)
  Form1->Memo5->Text=out;

 AnsiString p4;
 p4=p4.sprintf("%0.2X",WS.Length()*2);

 AnsiString FinalString=header + p4 + out + CTRLZ;
 Form1->Memo2->Lines->Add(FinalString);
 Form1->ComPort1->WriteStr(FinalString);
}

void __fastcall TForm1::Button2Click(TObject *Sender)
{
 ComPort1->WriteStr("AT+CMGS=\"" + ComboBox1->Text + "\",145\r\n");
 ComPort1->WriteStr(SMSText->Text + CTRLZ);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
 ComPort1->WriteStr("AT+CMGF=1\r\n");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
 ComPort1->WriteStr("AT+CPIN=0000\r\n");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComTerminal1StrRecieved(TObject *Sender,
      AnsiString &Str)
{
 if(Str.AnsiPos("+CMGS") && Str.AnsiPos("OK"))
 {
  AnsiString DT=DateToStr(Date()) + " " + TimeToStr(Time());
  AnsiString s1=Str;
  s1=StringReplace(s1,"\r\n","...",TReplaceFlags()<<rfReplaceAll);
  Log->Lines->Add(DT + " :: " + ListBox1->Items->Strings[ListBox1->ItemIndex] + " :: успешно отправлено :: " + s1 + " (" + Label1->Caption + ")");
  if(CheckBox1->Checked)
  {
   ListBox1->ItemIndex++;
   CSpinEdit1->Value++;
   Label1->Caption=IntToStr(CSpinEdit1->Value) + " из " + IntToStr(ListBox1->Count);
   ProgressBar1->Position=ListBox1->ItemIndex;
   error_counter=0;
   Button18Click(Sender);
  }
 }
 if(Str.AnsiPos("+CMS") && Str.AnsiPos("500"))
 {
   Log->Lines->Add(ListBox1->Items->Strings[ListBox1->ItemIndex] + " :: ошибка 500 :: " + Str);
   ListBox1->ItemIndex++;
   CSpinEdit1->Value++;
   Label1->Caption=IntToStr(CSpinEdit1->Value) + " из " + IntToStr(ListBox1->Count);
   ProgressBar1->Position=ListBox1->ItemIndex;
   error_counter++;

   if(error_counter<CountErrorsStop->Value)
    Button18Click(Sender);
   else
    ShowMessage("хуйня какаято");
 }

 if(Str.AnsiPos("+CMGR"))
 {
   Memo1->Text=Str;
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
 ComPort1->WriteStr("AT+CMGF=0\r\n");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button7Click(TObject *Sender)
{
 ComPort1->WriteStr("AT+CSCS=\"UCS2\"\r\n"); // Установка кодировки
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button9Click(TObject *Sender)
{
 ComPort1->WriteStr("AT+CSCA=\"+79043490000\"\r\n");
 ComPort1->WriteStr("AT+CSCA=\"+79529480100\"\r\n"); //настоящий
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button11Click(TObject *Sender)
{
 ComPort1->WriteStr("AT+CSCA?\r\n");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button8Click(TObject *Sender)
{
 ComPort1->WriteStr("AT+CSCS?\r\n"); // Запрос кодировки
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button13Click(TObject *Sender)
{
 ComPort1->WriteStr("AT+CMGF?\r\n");
}
//---------------------------------------------------------------------------


MySleep(int interval)
{
 DWORD end=GetTickCount()+5000;
 while(GetTickCount()<end)
 {

 }
}

void __fastcall TForm1::Button18Click(TObject *Sender)
{
 AnsiString CurrentPhone=ComboBox1->Text;
 if(CheckBox1->Checked)
 {
  //if(ListBox1->ItemIndex%10==0)
  //{
  //  MySleep(5000);
  //  Button6Click(Sender);
  //  MySleep(5000);
  //}

  if(ListBox1->ItemIndex<ListBox1->Count-1)
   {
    CurrentPhone=ListBox1->Items->Strings[ListBox1->ItemIndex];
   }
  else
   {
      CurrentPhone=""; ShowMessage("Рассылка закончена");
      return;
   }
   SendSMS(CurrentPhone,SMSText->Text,0);
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button19Click(TObject *Sender)
{
 ComPort1->WriteStr("AT+CSCS=\"IRA\"\r\n"); // Установка кодировки
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button20Click(TObject *Sender)
{
 ComPort1->WriteStr("AT+CSCS=\"GSM\"\r\n"); // Установка кодировки
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComPort1Ring(TObject *Sender)
{
 ShowMessage("RING");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComPort1RxFlag(TObject *Sender)
{
 ShowMessage("RING");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComPort1CTSChange(TObject *Sender, bool OnOff)
{
// Log->Lines->Add("CTS");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComPort1DSRChange(TObject *Sender, bool OnOff)
{
// Log->Lines->Add("DSR");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComPort1RLSDChange(TObject *Sender, bool OnOff)
{
// Log->Lines->Add("RLSD");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCreate(TObject *Sender)
{
 Application->OnException=AppException;
 
 if(FileExists(AppDir + "options.ini"))
 {
   TIniFile* RecFileIni;
    RecFileIni = new TIniFile(AppDir + "options.ini");
    CSpinEdit1->Value=RecFileIni->ReadInteger("OPTIONS","CURRENT-POSITION",10);
    BaseFname->Text=RecFileIni->ReadString("OPTIONS","CURRENT-BASE","");
    smstext=RecFileIni->ReadString("OPTIONS","SMSTEXT",AppDir + "text.txt");

    port1combo->Text=RecFileIni->ReadString("OPTIONS","COMPORT1","COM3");
    port1comboChange(Sender);
    port2combo->Text=RecFileIni->ReadString("OPTIONS","COMPORT2","COM4");
    port2comboChange(Sender);
    
    if(FileExists(BaseFname->Text))
    ListBox1->Items->LoadFromFile(BaseFname->Text);
   delete RecFileIni;
 }

 ListBox1->ItemIndex=CSpinEdit1->Value;
 ProgressBar1->Max=ListBox1->Count;
 ProgressBar1->Position=CSpinEdit1->Value;

 if(FileExists(AppDir + "log.txt"))
  Log->Lines->LoadFromFile(AppDir + "log.txt");

 if(FileExists(smstext))
  SMSText->Lines->LoadFromFile(smstext);

 Label1->Caption=IntToStr(CSpinEdit1->Value) + " из " + IntToStr(ListBox1->Count);

 if(AutoStart->Checked)
  Button18Click(Sender);

 PageControl1->ActivePageIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormCloseQuery(TObject *Sender, bool &CanClose)
{
 TIniFile* RecFileIni;
  RecFileIni = new TIniFile(AppDir + "options.ini");
  if(FileExists(BaseFname->Text))
  {
  RecFileIni->WriteString("OPTIONS","CURRENT-BASE",BaseFname->Text);
  RecFileIni->WriteInteger("OPTIONS","CURRENT-POSITION",CSpinEdit1->Value);

  if(SMSText->Modified)
  {
    SMSText->Lines->SaveToFile(smstext);
  }
  }
 delete RecFileIni;

 Log->Lines->SaveToFile(AppDir + "log.txt");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ListBox1Click(TObject *Sender)
{
 CSpinEdit1->Value=ListBox1->ItemIndex;
 Label1->Caption=IntToStr(CSpinEdit1->Value) + " из " + IntToStr(ListBox1->Count);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
 ComPort1->WriteStr("AT+CUSD=1,*105#\r\n");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComTerminal2StrRecieved(TObject *Sender,
      AnsiString &Str)
{
 if(Str.AnsiPos("+CUSD"))
 {
  //AnsiString balance=Str.SubString(13,Str.Length());
  //balance=balance.SubString(0,balance.Length()-5);
  AnsiString balance=Str.SubString(Str.AnsiPos("\"")+1,Str.Length());
  balance=balance.SubString(0,balance.AnsiPos("\"")-1);
  //MessageBox(NULL,balance.c_str(),"Баланс",NULL);
  BalanceLabel->Caption=balance;
 }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button10Click(TObject *Sender)
{
  ComPort1->WriteStr("AT+CUSD=1,*140#\r\n");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::LogChange(TObject *Sender)
{
 Log->Perform(EM_LINESCROLL, 1, Log->Lines->Count);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button12Click(TObject *Sender)
{
 if(OpenDialog1->Execute())
 {
    ListBox1->Items->LoadFromFile(OpenDialog1->FileName);
    ListBox1->ItemIndex=0;
    CSpinEdit1->Value=0;
    ProgressBar1->Max=ListBox1->Count;
    ProgressBar1->Position=CSpinEdit1->Value;
    Label1->Caption=IntToStr(CSpinEdit1->Value) + " из " + IntToStr(ListBox1->Count);
    BaseFname->Text=OpenDialog1->FileName;
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button15Click(TObject *Sender)
{
 ComPort1->WriteStr("AT+CMGR=" + IntToStr(SMS_NUM->Value) + "\r\n");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button16Click(TObject *Sender)
{
 AnsiString str=Memo1->Text;
 Memo3->Clear();
 Memo4->Clear();
 Memo5->Clear();

 // "07919720150110F0040B919720672934F50008010112613384633604210442043E0438043C043E04410442044C0020041A043E043D04410443043B044C04420430043D0442002D043F043B044E0441003F";
          ///          SMS-центр ..      79027692435F0080101021 16-33-48-3663
                                                       //дата   //время
 AnsiString str2;
 char c1,c2;

 for(int i=1;i<str.Length();i+=2) /// переворачиваем байты в заголовке
 {
  c1=str[i];
  c2=str[i+1];
  str2=str2+c2+c1;
 }

 AnsiString SMSC=str2.SubString(5,13);
 AnsiString NUMER=str2.SubString(23,13);
 AnsiString DATETIME=str2.SubString(39,12);
 AnsiString DATE=DATETIME.SubString(0,6);
 AnsiString TIME=DATETIME.SubString(7,12);
 AnsiString TEXT=str.SubString(55,str.Length());

 Memo3->Lines->Add("SMS-CENTER: " + SMSC);
 Memo3->Lines->Add("PHONE: " + NUMER);
 Memo3->Lines->Add("DATE: " + DATE + " " + TIME);
 Memo3->Lines->Add("");
// Memo3->Lines->Add(TEXT);
 Memo4->Text=TEXT;

 wchar_t x[80];
 for(int i=1;i<TEXT.Length();i+=4)
 {
 char s[10];
 strcpy(s,TEXT.SubString(i,4).c_str());
 int n=htoi(s[0])*16*16*16 + htoi(s[1])*16*16 + htoi(s[2])*16 + htoi(s[3]);
 x[(i-1)/4]=n;
 //
 }

 Memo5->Text=x;
// MessageBoxW(NULL,,L"",NULL);
}
//---------------------------------------------------------------------------


int hextostr(string, hexa, length)
char *string;
const char *hexa;
int length;
{
	while (length--) {
		sprintf(string, "%02X", (unsigned char) *hexa++);
		string += 2;
	}

	return 0;
}

void __fastcall TForm1::Button17Click(TObject *Sender)
{
  Memo1->Text=Memo1->Lines->Strings[2];
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SMS_NUMChange(TObject *Sender)
{
 if(AutoDecode->Checked)
 {
   Button15Click(Sender);
   Sleep(500);
   Button17Click(Sender);
   Button16Click(Sender);
 }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComPort2Error(TObject *Sender, TComErrors Errors)
{
 Log->Lines->Add("ошибка");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ComPort1Error(TObject *Sender, TComErrors Errors)
{
 Log->Lines->Add("ошибка");        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::port1comboChange(TObject *Sender)
{
    AnsiString *port = &(ComPort1->Port);
    *port = port1combo->Text;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::port2comboChange(TObject *Sender)
{
    AnsiString *port = &(ComPort2->Port);
    *port = port2combo->Text;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button21Click(TObject *Sender)
{
    ComPort1->Open();
    ComPort2->Open();
}
//---------------------------------------------------------------------------

