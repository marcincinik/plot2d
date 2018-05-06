#pragma once

#include "ICalculatorView.h"
#include "CalculatorPresenter.h"

namespace calc_gui {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Drawing::Drawing2D;

	/// <summary>
	/// Summary for CalcViewForm
	/// </summary>
	public ref class CalcViewForm 
		: public System::Windows::Forms::Form, 
		/* the view implements view's interface */
		public ICalculatorView
	{
	private:
		CalculatorPresenter^ presenter;
	private: System::Windows::Forms::Label^  lblError;
			 bool saveStateDirty;
	public:
		CalcViewForm(void)
		{
			InitializeComponent();
			//create the presenter
			presenter = gcnew CalculatorPresenter(this);

			//bind dimensions of the panel to the window
			pnlGraph->Anchor = AnchorStyles::Left | AnchorStyles::Top | 
                AnchorStyles::Right | AnchorStyles::Bottom;

			
	
		}

		/* fill-in the function's value for a single-value calculation*/
		virtual void updateFunctionValue(double v) {
			txtFunctionVal->Text = System::Convert::ToString(v);
		}

		virtual void updateFunctionText(String^ functionText) {
			txtFunctionText->Text = functionText;
		}

		/*redraw the graph*/
		virtual void updateGraph() {
			//just request to redraw asynchronously
			pnlGraph->Invalidate();
		}

		virtual void showError(String^ msg) {
			lblError->Text = msg;
		}

		virtual void clearError() {
			lblError->Text = "";
		}
	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CalcViewForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Button^  btnCalcFuncVal;
	private: System::Windows::Forms::TextBox^  txtFunctionText;
	private: System::Windows::Forms::TextBox^  txtFunctionVal;
	private: System::Windows::Forms::TextBox^  txtFunctionArg;
	private: System::Windows::Forms::Panel^  pnlGraph;
	private: System::Windows::Forms::Button^  btnDraw;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  txtX2;

	private: System::Windows::Forms::TextBox^  txtX1;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog;

	private: System::Windows::Forms::SaveFileDialog^  saveFileDialog;
	private: System::Windows::Forms::MenuStrip^  menuStrip;
	private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  openToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^  saveAsToolStripMenuItem;

	private: System::Windows::Forms::ToolStripMenuItem^  exitToolStripMenuItem;


	protected: 

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->lblError = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->txtX2 = (gcnew System::Windows::Forms::TextBox());
			this->txtX1 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->btnDraw = (gcnew System::Windows::Forms::Button());
			this->txtFunctionText = (gcnew System::Windows::Forms::TextBox());
			this->txtFunctionVal = (gcnew System::Windows::Forms::TextBox());
			this->txtFunctionArg = (gcnew System::Windows::Forms::TextBox());
			this->btnCalcFuncVal = (gcnew System::Windows::Forms::Button());
			this->pnlGraph = (gcnew System::Windows::Forms::Panel());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->saveFileDialog = (gcnew System::Windows::Forms::SaveFileDialog());
			this->menuStrip = (gcnew System::Windows::Forms::MenuStrip());
			this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->openToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->saveAsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->exitToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->panel1->SuspendLayout();
			this->menuStrip->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->lblError);
			this->panel1->Controls->Add(this->label3);
			this->panel1->Controls->Add(this->txtX2);
			this->panel1->Controls->Add(this->txtX1);
			this->panel1->Controls->Add(this->label2);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->btnDraw);
			this->panel1->Controls->Add(this->txtFunctionText);
			this->panel1->Controls->Add(this->txtFunctionVal);
			this->panel1->Controls->Add(this->txtFunctionArg);
			this->panel1->Controls->Add(this->btnCalcFuncVal);
			this->panel1->Location = System::Drawing::Point(12, 31);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(679, 164);
			this->panel1->TabIndex = 0;
			// 
			// lblError
			// 
			this->lblError->AutoSize = true;
			this->lblError->Location = System::Drawing::Point(73, 37);
			this->lblError->Name = L"lblError";
			this->lblError->Size = System::Drawing::Size(0, 17);
			this->lblError->TabIndex = 10;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(3, 68);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(62, 17);
			this->label3->TabIndex = 9;
			this->label3->Text = L"(x1,x2):=";
			// 
			// txtX2
			// 
			this->txtX2->Location = System::Drawing::Point(204, 63);
			this->txtX2->Name = L"txtX2";
			this->txtX2->Size = System::Drawing::Size(122, 22);
			this->txtX2->TabIndex = 8;
			this->txtX2->Text = L"1";
			// 
			// txtX1
			// 
			this->txtX1->Location = System::Drawing::Point(76, 63);
			this->txtX1->Name = L"txtX1";
			this->txtX1->Size = System::Drawing::Size(122, 22);
			this->txtX1->TabIndex = 7;
			this->txtX1->Text = L"-1";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(49, 94);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(15, 17);
			this->label2->TabIndex = 6;
			this->label2->Text = L"v";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(24, 15);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(40, 17);
			this->label1->TabIndex = 5;
			this->label1->Text = L"f(x):=";
			// 
			// btnDraw
			// 
			this->btnDraw->Location = System::Drawing::Point(6, 138);
			this->btnDraw->Name = L"btnDraw";
			this->btnDraw->Size = System::Drawing::Size(75, 23);
			this->btnDraw->TabIndex = 4;
			this->btnDraw->Text = L"Draw";
			this->btnDraw->UseVisualStyleBackColor = true;
			this->btnDraw->Click += gcnew System::EventHandler(this, &CalcViewForm::btnDraw_Click);
			// 
			// txtFunctionText
			// 
			this->txtFunctionText->Location = System::Drawing::Point(76, 12);
			this->txtFunctionText->Name = L"txtFunctionText";
			this->txtFunctionText->Size = System::Drawing::Size(250, 22);
			this->txtFunctionText->TabIndex = 3;
			this->txtFunctionText->Text = L"x";
			// 
			// txtFunctionVal
			// 
			this->txtFunctionVal->Location = System::Drawing::Point(226, 91);
			this->txtFunctionVal->Name = L"txtFunctionVal";
			this->txtFunctionVal->ReadOnly = true;
			this->txtFunctionVal->Size = System::Drawing::Size(100, 22);
			this->txtFunctionVal->TabIndex = 2;
			// 
			// txtFunctionArg
			// 
			this->txtFunctionArg->Location = System::Drawing::Point(76, 91);
			this->txtFunctionArg->Name = L"txtFunctionArg";
			this->txtFunctionArg->Size = System::Drawing::Size(100, 22);
			this->txtFunctionArg->TabIndex = 1;
			this->txtFunctionArg->Text = L"1";
			// 
			// btnCalcFuncVal
			// 
			this->btnCalcFuncVal->Location = System::Drawing::Point(182, 88);
			this->btnCalcFuncVal->Name = L"btnCalcFuncVal";
			this->btnCalcFuncVal->Size = System::Drawing::Size(38, 23);
			this->btnCalcFuncVal->TabIndex = 0;
			this->btnCalcFuncVal->Text = L"f(v)";
			this->btnCalcFuncVal->UseVisualStyleBackColor = true;
			this->btnCalcFuncVal->Click += gcnew System::EventHandler(this, &CalcViewForm::btnCalcFuncVal_Click);
			// 
			// pnlGraph
			// 
			this->pnlGraph->AutoSize = true;
			this->pnlGraph->BorderStyle = System::Windows::Forms::BorderStyle::FixedSingle;
			this->pnlGraph->Location = System::Drawing::Point(12, 201);
			this->pnlGraph->Name = L"pnlGraph";
			this->pnlGraph->Size = System::Drawing::Size(679, 262);
			this->pnlGraph->TabIndex = 1;
			this->pnlGraph->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &CalcViewForm::paintGraph);
			this->pnlGraph->Resize += gcnew System::EventHandler(this, &CalcViewForm::onPnlDrawingResize);
			// 
			// menuStrip
			// 
			this->menuStrip->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) {this->fileToolStripMenuItem});
			this->menuStrip->Location = System::Drawing::Point(0, 0);
			this->menuStrip->Name = L"menuStrip";
			this->menuStrip->Size = System::Drawing::Size(703, 28);
			this->menuStrip->TabIndex = 2;
			this->menuStrip->Text = L"menuStrip1";
			// 
			// fileToolStripMenuItem
			// 
			this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(4) {this->openToolStripMenuItem, 
				this->saveToolStripMenuItem, this->saveAsToolStripMenuItem, this->exitToolStripMenuItem});
			this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
			this->fileToolStripMenuItem->Size = System::Drawing::Size(44, 24);
			this->fileToolStripMenuItem->Text = L"File";
			// 
			// openToolStripMenuItem
			// 
			this->openToolStripMenuItem->Name = L"openToolStripMenuItem";
			this->openToolStripMenuItem->Size = System::Drawing::Size(135, 24);
			this->openToolStripMenuItem->Text = L"Open..";
			this->openToolStripMenuItem->Click += gcnew System::EventHandler(this, &CalcViewForm::openToolStripMenuItem_Click);
			// 
			// saveToolStripMenuItem
			// 
			this->saveToolStripMenuItem->Name = L"saveToolStripMenuItem";
			this->saveToolStripMenuItem->Size = System::Drawing::Size(135, 24);
			this->saveToolStripMenuItem->Text = L"Save";
			this->saveToolStripMenuItem->Click += gcnew System::EventHandler(this, &CalcViewForm::saveToolStripMenuItem_Click);
			// 
			// saveAsToolStripMenuItem
			// 
			this->saveAsToolStripMenuItem->Name = L"saveAsToolStripMenuItem";
			this->saveAsToolStripMenuItem->Size = System::Drawing::Size(135, 24);
			this->saveAsToolStripMenuItem->Text = L"Save As..";
			this->saveAsToolStripMenuItem->Click += gcnew System::EventHandler(this, &CalcViewForm::saveAsToolStripMenuItem_Click);
			// 
			// exitToolStripMenuItem
			// 
			this->exitToolStripMenuItem->Name = L"exitToolStripMenuItem";
			this->exitToolStripMenuItem->Size = System::Drawing::Size(135, 24);
			this->exitToolStripMenuItem->Text = L"Exit";
			this->exitToolStripMenuItem->Click += gcnew System::EventHandler(this, &CalcViewForm::exitToolStripMenuItem_Click);
			// 
			// CalcViewForm
			// 
			this->AcceptButton = this->btnDraw;
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(703, 465);
			this->Controls->Add(this->pnlGraph);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->menuStrip);
			this->MainMenuStrip = this->menuStrip;
			this->Name = L"CalcViewForm";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Function Calculator";
			this->WindowState = System::Windows::Forms::FormWindowState::Maximized;
			this->Shown += gcnew System::EventHandler(this, &CalcViewForm::shown);
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			this->menuStrip->ResumeLayout(false);
			this->menuStrip->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btnCalcFuncVal_Click(System::Object^  sender, System::EventArgs^  e) {
				 //calculate a single value for given function
				 String^ txt = txtFunctionText->Text->Trim();
				 if (txt->Length > 0) {
					 presenter->updateFunction(
						 txtFunctionText->Text);
					 double x;
					 if (Double::TryParse(txtFunctionArg->Text, x)) {
						 presenter->calculateValue(x);
					 }
				 }
			 }
			 /*redraw the function in the panel*/
private: System::Void paintGraph(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {
			 /* get pre-calculated values*/
			 array<System::Drawing::PointF>^ points = presenter->getModelCalculatedPoints();

			 /* calculate dimensions in the mathematical coordinates */
			 if (points != nullptr && points->Length > 0) {
				 double minX = points[0].X;
				 double maxX = points[points->Length-1].X;

				 double minY = presenter->getMinY();
				 double maxY = presenter->getMaxY();

				 /* scale X,Y is calculated by the presenter*/
				 double scaleX = presenter->getScaleX();
				 double scaleY = presenter->getScaleY(pnlGraph->Height);

				 /////apply affinic transformations
				 //move to half vertically 
				 e->Graphics->TranslateTransform(0, (float)(pnlGraph->Height));

				 //apply scaling
				 e->Graphics->ScaleTransform((float)scaleX,(float)(-scaleY));
				 //move again to fit the full graph
				 e->Graphics->TranslateTransform((float)(-points[0].X),(float)(-minY));

				 			 //create pens
				 Pen^ blackPen = gcnew Pen( Color::Black, 0.0f );
				 Pen^ bluePen = gcnew Pen( Color::Blue, 1.0f / (float)scaleX );
				 e->Graphics->SmoothingMode = 
						System::Drawing::Drawing2D::SmoothingMode::AntiAlias;
				 
				 //draw grid
				 //drawGrid(sender, e, minX, maxX, minY, maxY);
				 //draw coordinate system
				 e->Graphics->DrawLine(bluePen, (float)minX, 0.0f, (float)maxX, 0.0f);
				 e->Graphics->DrawLine(bluePen, 0.0f, (float)minY, 0.0f, (float)maxY);
				 e->Graphics->DrawLines(blackPen, points);
			 }
		 }

private: System::Void drawGrid(
			 System::Object^  sender, 
			 System::Windows::Forms::PaintEventArgs^  e,
			 double minX, double maxX, double minY, double maxY) {
				 Pen^ bluePen = gcnew Pen( Color::Blue, 0.0f);
				 array<Single>^ dashPattern = {1.0F, 3.0F};
				 bluePen->DashPattern  = dashPattern;
				 for (int i = (int)minX; i <= (int)maxX; i+=1) {
					 e->Graphics->DrawLine(bluePen, 
						 (float)i, (float)minY, (float)i, (float)maxY);
				 }
				 for (int i = (int)minY; i <= (int)maxY; i+=1) {
					 e->Graphics->DrawLine(bluePen, 
						 (float)minX, (float)i, (float)maxX, (float)i);
				 }
		 }

private: System::Void btnDraw_Click(System::Object^  sender, System::EventArgs^  e) {
			 String^ txt = txtFunctionText->Text->Trim();
			 if (txt->Length > 0) {
				 presenter->updateFunction(
					 txtFunctionText->Text);
				 double x1;
				 double x2;
				 if (Double::TryParse(txtX1->Text, x1)
					 && Double::TryParse(txtX2->Text, x2)) {
					 presenter->drawFunction(x1, x2, pnlGraph->Width);
				 }
			 }
		 }

private: System::Void onPnlDrawingResize(System::Object^  sender, System::EventArgs^  e) {
			 pnlGraph->Invalidate();
		 }
private: System::Void exitToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 this->Close();
		 }
private: System::Void openToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 openFileDialog->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
			 if ( openFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK ) {
				 if (presenter->openFile(openFileDialog->FileName)) {
					 this->btnDraw_Click(sender, e);
				 }
			 }
		 }
private: System::Void saveAsToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 saveFileDialog->Filter = "txt files (*.txt)|*.txt|All files (*.*)|*.*";
			 if ( saveFileDialog->ShowDialog() == System::Windows::Forms::DialogResult::OK ) {
				 this->btnDraw_Click(sender, e);
				 presenter->saveFile(saveFileDialog->FileName);
			 }

		 }
private: System::Void saveToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e) {
			 if (presenter->getFileName() != nullptr) {
				 presenter->saveFile();
			 } else {
				 saveAsToolStripMenuItem_Click(sender, e);
			 }
		 }
private: System::Void shown(System::Object^  sender, System::EventArgs^  e) {
			 txtFunctionText->Focus();
		 }
};
}

