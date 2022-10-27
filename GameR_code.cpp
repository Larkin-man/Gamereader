//---------------------------------------------------------------------------
#include <vector>
#include <vcl.h>
#pragma hdrstop

#include "GameR_code.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	OpenTextFileDialog1->InitialDir = GetCurrentDir();
	//Book = new TStringList;
	Book = NULL;
	//book = NULL;
	ParCount = 0;
	//Store->Width = 1000;
	RTF = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (RTF == false)
		delete Book;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OpenBtnClick(TObject *Sender)
{
	if (OpenTextFileDialog1->Execute() == ID_OK)
	{
		//Form1->Name = OpenDialog1->FileName;
		Path.clear();
		Path.reserve(20);
		Step = 0;
		BackStep = 0;

		//Out->Lines->LoadFromFile
		if (OpenTextFileDialog1->Options.Contains(ofExtensionDifferent))
		{      //Store->Lines
			//Book->LoadFromFile(OpenTextFileDialog1->FileName,
			//	(TEncoding *) (OpenTextFileDialog1->Encodings->Objects[OpenTextFileDialog1->EncodingIndex] ));
			//Book->Clear();
			//Book->AddStrings(Out->Lines);
			//Book->Assign(Store->Lines);
			Store->Lines->LoadFromFile(OpenTextFileDialog1->FileName,
				(TEncoding *) (OpenTextFileDialog1->Encodings->Objects[OpenTextFileDialog1->EncodingIndex] ));
			if (RTF == false)
				delete Book;
			Book = Store->Lines;
			RTF = true;
			//return;
		}
		else   //*/
		{
			RTF = false;
      	Book = new TStringList;
			Book->LoadFromFile(OpenTextFileDialog1->FileName,
				(TEncoding *) (OpenTextFileDialog1->Encodings->Objects[OpenTextFileDialog1->EncodingIndex] ));
		}
		RefreshBookName(OpenTextFileDialog1->FileName);
		ParStartStr.clear();
		ParStartStr.reserve(50);
		ParStartStr.push_back(-1);
		int par;
		int seek = 1;
		for (int i=0; i < Book->Count; i++)
		{
			par = Book->Strings[i].Trim().ToIntDef(0);
			if (par == seek)
			{
				ParStartStr.push_back(i);
				seek++;
				for (int j=i+1; j < Book->Count; j++)
					if (Book->Strings[j].IsEmpty())
					{
						Book->Delete(j);
						j--;
					}
			}
			//try  		{} catch (EConvertError&){ 	}
		}
		ParCount = seek-1;
		ParStartStr.push_back(Book->Count);
		ShowMessage("В книге "+IntToStr(ParCount)+" параграфов.");
		//Out->Lines->
		SelPar->Text = "0";
		GoBtnClick(Sender);
		SelPar->Text = "1";
		GoBtn->Enabled = ParCount > 0;
		PrintStepsBtn->Enabled = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OutClick(TObject *Sender)
{
	IfParSel = false;
	int Sel = Out->SelStart;
	if (Sel > 3500)
   	return;
   int E = 0;
	for (int i = 0; i < Out->Lines->Count; ++i)
	{
		E += Out->Lines->Strings[i].Length();
		if (Sel <= E)
			break;
		if (Out->Text[E + 2] == '\n')
		{
			Sel++;
			E += 2;
		}
	}
	//Sel += Out->CaretPos.y;
	if (Sel == 0)
		return;
	//Memo1->Lines->Add(Out->Text[Sel]);
	if (IsCipher(Out->Text[Sel]) == false)
		Sel++;
	int Start = Sel, End = Sel;
	for (int i = Sel; i <= Out->Text.Length(); ++i)
		if (IsCipher(Out->Text[i]))
			End++;
		else
			break;
	if (Sel > Out->Text.Length())
		Sel = Out->Text.Length();
	for (int i = Sel; i > 0; --i)
		if (IsCipher(Out->Text[i]))
			Start--;
		else
			break;
	//Memo1->Lines->Add(IntToStr(Start)+"<>"+IntToStr(End));
	Start++;
	//Memo1->Lines->Add(Out->Text.SubString(Start, End-Start));
	if (End-Start > 0)
	{
		SelPar->Text = Out->Text.SubString(Start, End-Start);
		IfParSel = true;
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::OutDblClick(TObject *Sender)
{
	if (IfParSel)
	{
		GoBtnClick(Sender);
		SelPar->SetFocus();
	}
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NEditClick(TObject *Sender)
{
	NEdit->Checked = Out->ReadOnly;
	Out->ReadOnly = !Out->ReadOnly;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NPrintBookClick(TObject *Sender)
{
	Out->Lines = Book;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FontBtnClick(TObject *Sender)
{
	FontDialog1->Font = Out->Font;
	if (FontDialog1->Execute() == ID_OK)
		Out->Font = FontDialog1->Font;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::GoBtnClick(TObject *Sender)
{
	int Par = SelPar->Text.ToInt();
	if (Par > ParCount)
   {
		Out->Lines->Add("В книге "+IntToStr(ParCount)+" параграфов.");
		return;
	}
	int Stop = ParStartStr[Par+1];
	Out->Lines->Clear();
	//int OutPos = 0;
	for (int i = ParStartStr[Par]+1; i < Stop; ++i)
	{
		//if (OutPos < Out->Lines->Count)
		//	Out->Lines->Strings[0] = Book->Strings[i];
		//else
			Out->Lines->Append(Book->Strings[i]);
     	//OutPos++;
	}
	Out->SelStart = 0;
	Out->SelLength = 0;
	Out->Invalidate();
	//while (Out->Lines->Count > OutPos)
	//	Out->Lines->Delete(Out->Lines->Count - 1);
	if (0)//RTF)
	{
		int curs = 0;
		Store->SelStart = 0;
		for (int i = ParStartStr[Par]+1; i < Stop; ++i)
		{
			Out->SelStart = 0;
			Out->SelLength = Out->Lines->Strings[curs].Length();
			Store->SelLength = Out->Text.Length();
			//Store->Lines->GetEnumerator()->
			curs++;
		}
		Out->SelAttributes->Assign(Store->SelAttributes);
	}

	if (Path.size() <= Step)
	{
		Path.push_back(Par);
	}
	else //p>s
	{
   	Path[Step] = Par;
	}
	Step++;
	BackBtn->Enabled = (Step > 1);
	if (BackStep > 0)
		BackStep--;
	ForwBtn->Enabled = (BackStep > 0);
	PrintStepsBtn->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SelParMouseEnter(TObject *Sender)
{
	SelPar->SetFocus();
	SelPar->SelectAll();
	//SelPar->SelStart = 0;
	//SelPar->SelLength = SelPar->Text.Length();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::BackBtnClick(TObject *Sender)
{
	BackStep += 2;
	Step -= 2;
	SelPar->Text = Path[Step];
	GoBtnClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ForwBtnClick(TObject *Sender)
{
	//BackStep--;
   SelPar->Text = Path[Step];
	GoBtnClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PrintStepsBtnClick(TObject *Sender)
{
	Out->Lines->Add("Сделано шагов: "+IntToStr((int)Step-1));
	UnicodeString Str = "0";
	for (unsigned int i = 1; i < Step; ++i)
		Str += "->"+IntToStr(Path[i]);
	Out->Lines->Add(Str);

}
//---------------------------------------------------------------------------
//Обновить заголовок окна
void TForm1::RefreshBookName(UnicodeString FileName)
{
	for (int i=FileName.Length(); i>1; --i)
		if (FileName[i] == '\\')
		{
			FileName = FileName.SubString(i+1, FileName.Length());
			break;
		}
	Form1->Caption = FileName.SubString(1, FileName.Length()-4) + " - Игрочиталка";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::NClearPathClick(TObject *Sender)
{
	Step = 1;
	BackBtn->Enabled = false;
	BackStep = 0;
	ForwBtn->Enabled = false;
}
//---------------------------------------------------------------------------


void __fastcall TForm1::NNoCheatClick(TObject *Sender)
{
	NNoCheat->Checked = true;
	NNoCheat->Enabled = false;
	BackBtn->Visible = false;
	ForwBtn->Visible = false;
	PrintStepsBtn->Visible = false;
}
//---------------------------------------------------------------------------
void TForm1::DebugFunction(int d)
{
	if (d == 0)
	{
	for (unsigned int i = 0; i < ParStartStr.size(); ++i)
		Out->Lines->Add(ParStartStr[i]);
	//Store->Visible = !Store->Visible;
	}
	//Out->Lines->Assign(Store->Lines);
	//Out->SelStart = 0;
	//Out->SelLength = Out->Lines->Strings[0].Length();
	//Store->SelStart = 0;
	//Store->SelLength = Store->Lines->Strings[0].Length();
	//Out-> = Out->Lines->Strings[1];
	//Store->SelText = Store->Lines->Strings[2];
	//Out->SelAttributes->Assign(Store->SelAttributes);


	//Out->Lines->Add(Store->Lines->Strings[0]);
	if (d == 2)     {
		 /*
		Store->Lines->Add(Out->Lines->Count);
	int E=0;
	int L=0;
	for (int i = 0; i < Out->Lines->Count; ++i)
	{
		E += Out->Lines->Strings[i].Length();
		if (E >= Out->Text.Length())
			break;
		if (Out->Text[E+2] == '\n')
		{
			L++;
			E+=2;
		}
	}
	 /*	if (Out->Lines->Strings[i][Out->Lines->Strings[i].Length()] == '\n')
			Store->Lines->Add('Y');
		else
			Store->Lines->Add((int)Out->Lines->Strings[i][Out->Lines->Strings[i].Length()]); //*/
/*		int N=0,g=0;
		for (int i = 1; i <= Out->Text.Length(); ++i)
		{
			if (Out->Text[i] == 10)
				N++;
			if (Out->Text[i] == 13)
				g++;
		}
	Store->Lines->Add("L="+IntToStr(L)+" E="+IntToStr(E)+" N="+IntToStr(N)+" g="+IntToStr(g));
	*/
	}
	if (d == 3) //Промотать вверх
   {
   	Out->SelStart = 0;
		Out->SelLength = 0;
		//Out->ScrollBars == ssNone;
		Out->Invalidate();
		//Out->ScrollBars == ssVertical;
	}
}
//---------------------------------------------------------------------------

