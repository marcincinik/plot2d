#pragma once

#include "CalculatorPresenter.h"
#include "ICalculatorView.h"

namespace calc_gui {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	/// </summary>
	public ref class CalculatorMainForm : public System::Windows::Forms::Form, public ICalculatorView
	{
	private:
		CalculatorPresenter^ presenter;
	public:
		CalculatorMainForm(void)
		{
			InitializeComponent();
			presenter = gcnew CalculatorPresenter(this);
		}

		virtual void updateFunctionValue(double v) {

		}

		virtual void updateGraph() {

		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~CalculatorMainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Panel^  panel1;
	protected: 
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::TextBox^  txtFuncArg;


	private: System::Windows::Forms::TextBox^  txtFunction;



	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  btnCalcValue;
	private: System::Windows::Forms::TextBox^  txtFuncValue;


	private: System::Windows::Forms::Button^  btnDraw;
	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::TextBox^  txtIntervalX2;
	private: System::Windows::Forms::TextBox^  txtIntervalX1;



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
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->btnDraw = (gcnew System::Windows::Forms::Button());
			this->txtFuncValue = (gcnew System::Windows::Forms::TextBox());
			this->btnCalcValue = (gcnew System::Windows::Forms::Button());
			this->txtFuncArg = (gcnew System::Windows::Forms::TextBox());
			this->txtFunction = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->txtIntervalX1 = (gcnew System::Windows::Forms::TextBox());
			this->txtIntervalX2 = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->panel2->SuspendLayout();
			this->SuspendLayout();
			// 
			// panel1
			// 
			this->panel1->Location = System::Drawing::Point(12, 162);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(806, 297);
			this->panel1->TabIndex = 0;
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->label3);
			this->panel2->Controls->Add(this->txtIntervalX2);
			this->panel2->Controls->Add(this->txtIntervalX1);
			this->panel2->Controls->Add(this->btnDraw);
			this->panel2->Controls->Add(this->txtFuncValue);
			this->panel2->Controls->Add(this->btnCalcValue);
			this->panel2->Controls->Add(this->txtFuncArg);
			this->panel2->Controls->Add(this->txtFunction);
			this->panel2->Controls->Add(this->label2);
			this->panel2->Controls->Add(this->label1);
			this->panel2->Location = System::Drawing::Point(12, 12);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(806, 144);
			this->panel2->TabIndex = 1;
			// 
			// btnDraw
			// 
			this->btnDraw->Location = System::Drawing::Point(298, 110);
			this->btnDraw->Name = L"btnDraw";
			this->btnDraw->Size = System::Drawing::Size(75, 23);
			this->btnDraw->TabIndex = 6;
			this->btnDraw->Text = L"Go";
			this->btnDraw->UseVisualStyleBackColor = true;
			// 
			// txtFuncValue
			// 
			this->txtFuncValue->Location = System::Drawing::Point(232, 71);
			this->txtFuncValue->Name = L"txtFuncValue";
			this->txtFuncValue->Size = System::Drawing::Size(141, 22);
			this->txtFuncValue->TabIndex = 5;
			// 
			// btnCalcValue
			// 
			this->btnCalcValue->Location = System::Drawing::Point(198, 70);
			this->btnCalcValue->Name = L"btnCalcValue";
			this->btnCalcValue->Size = System::Drawing::Size(28, 23);
			this->btnCalcValue->TabIndex = 4;
			this->btnCalcValue->Text = L">";
			this->btnCalcValue->UseVisualStyleBackColor = true;
			// 
			// txtFuncArg
			// 
			this->txtFuncArg->Location = System::Drawing::Point(92, 71);
			this->txtFuncArg->Name = L"txtFuncArg";
			this->txtFuncArg->Size = System::Drawing::Size(100, 22);
			this->txtFuncArg->TabIndex = 3;
			// 
			// txtFunction
			// 
			this->txtFunction->Location = System::Drawing::Point(92, 14);
			this->txtFunction->Name = L"txtFunction";
			this->txtFunction->Size = System::Drawing::Size(281, 22);
			this->txtFunction->TabIndex = 2;
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(54, 71);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(26, 17);
			this->label2->TabIndex = 1;
			this->label2->Text = L"x:=";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(40, 19);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(40, 17);
			this->label1->TabIndex = 0;
			this->label1->Text = L"f(x):=";
			// 
			// txtIntervalX1
			// 
			this->txtIntervalX1->Location = System::Drawing::Point(92, 43);
			this->txtIntervalX1->Name = L"txtIntervalX1";
			this->txtIntervalX1->Size = System::Drawing::Size(134, 22);
			this->txtIntervalX1->TabIndex = 7;
			// 
			// txtIntervalX2
			// 
			this->txtIntervalX2->Location = System::Drawing::Point(232, 43);
			this->txtIntervalX2->Name = L"txtIntervalX2";
			this->txtIntervalX2->Size = System::Drawing::Size(140, 22);
			this->txtIntervalX2->TabIndex = 8;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(28, 47);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(58, 17);
			this->label3->TabIndex = 9;
			this->label3->Text = L"Interval:";
			// 
			// CalculatorMainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(830, 471);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel1);
			this->Name = L"CalculatorMainForm";
			this->Text = L"Calculator";
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
	};



}

