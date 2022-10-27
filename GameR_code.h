//---------------------------------------------------------------------------

#ifndef GameR_codeH
#define GameR_codeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *Out;
	TFontDialog *FontDialog1;
	TOpenTextFileDialog *OpenTextFileDialog1;
	TPopupMenu *PopupMenu1;
	TMenuItem *NEdit;
	TMenuItem *NPrintBook;
	TToolBar *ToolBar1;
	TToolButton *OpenBtn;
	TImageList *ImageList1ger;
	TToolButton *FontBtn;
	TToolButton *BackBtn;
	TToolButton *ForwBtn;
	TEdit *SelPar;
	TToolButton *ToolButton1;
	TToolButton *ToolButton2;
	TSpeedButton *GoBtn;
	TImageList *DisImageList;
	TToolButton *ToolButton3;
	TToolButton *PrintStepsBtn;
	TMenuItem *NClearPath;
	TMenuItem *NNoCheat;
	TRichEdit *Store;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall OutClick(TObject *Sender);
	void __fastcall OutDblClick(TObject *Sender);
	void __fastcall NEditClick(TObject *Sender);
	void __fastcall NPrintBookClick(TObject *Sender);
	void __fastcall OpenBtnClick(TObject *Sender);
	void __fastcall FontBtnClick(TObject *Sender);
	void __fastcall GoBtnClick(TObject *Sender);
	void __fastcall SelParMouseEnter(TObject *Sender);
	void __fastcall BackBtnClick(TObject *Sender);
	void __fastcall ForwBtnClick(TObject *Sender);
	void __fastcall PrintStepsBtnClick(TObject *Sender);
	void __fastcall NClearPathClick(TObject *Sender);
	void __fastcall NNoCheatClick(TObject *Sender);

private:	// User declarations
int FnStartLine;
int FnStartCol;
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);

	//TStringList *book;
	TStrings *Book;
	bool RTF;
	std::vector<int> ParStartStr;
	int ParCount;
	bool IfParSel;
	bool IsCipher(wchar_t Symb) {	return ((Symb >= '0')&&(Symb <= '9'));	}
	std::vector<int> Path;
	unsigned int Step;
	unsigned int BackStep;
	void RefreshBookName(UnicodeString FileName);
	void DebugFunction(int d);

};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
