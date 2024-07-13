#pragma once

using namespace System::Runtime::InteropServices;

//Variabile globale
char cpu_op = 'K';
char fpga_op = 'K';
char out_op = 'S';

uint32_t decript_key[6];
uint32_t encript_key[6];

HANDLE port;
char com_port_buff[100];
uint32_t baud_rate = 9600;
int conected = 0;
char* recv_buff;
int recv_index = 0;

namespace CppCLRWinFormsProject {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			recv_index = 0;
			//alocare memorie pentru buffer
			recv_buff = new char[10000];

			cpuComboBox->Items->Add("None");
			cpuComboBox->Items->Add("DF key");
			cpuComboBox->Items->Add("Encript");
			cpuComboBox->Items->Add("Decript");

			fpgaComboBox->Items->Add("None");
			fpgaComboBox->Items->Add("DF key");
			fpgaComboBox->Items->Add("Encript");
			fpgaComboBox->Items->Add("Decript");

			outputComboBox->Items->Add("Hex");
			outputComboBox->Items->Add("String");

			dbgComboBox->Items->Add("Decode Key");
			dbgComboBox->Items->Add("Encode Key");
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete[] recv_buff;
				delete components;
			}
		}
		////////////////////////////////////////////////////////////////////
		//Crearea componentelor grafice
		////////////////////////////////////////////////////////////////////
	private: System::Windows::Forms::Button^ sendButton;
	protected:

	protected:
	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::TextBox^ inputTextBox;
	private: System::Windows::Forms::TextBox^ comTextBox;
	private: System::Windows::Forms::Button^ connectButton;
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TextBox^ speedTextBox;
	private: System::Windows::Forms::ComboBox^ cpuComboBox;
	private: System::Windows::Forms::Label^ label3;
	private: System::Windows::Forms::Label^ label4;
	private: System::Windows::Forms::ComboBox^ fpgaComboBox;
	private: System::Windows::Forms::Button^ clearButton;
	private: System::Windows::Forms::RichTextBox^ richTextBox1;
	private: System::Windows::Forms::ComboBox^ outputComboBox;
	private: System::Windows::Forms::Label^ label5;
	private: System::Windows::Forms::Label^ label6;
	private: System::Windows::Forms::ComboBox^ dbgComboBox;
	private: System::Windows::Forms::Button^ disconnectButton;
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->sendButton = (gcnew System::Windows::Forms::Button());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->inputTextBox = (gcnew System::Windows::Forms::TextBox());
			this->comTextBox = (gcnew System::Windows::Forms::TextBox());
			this->connectButton = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->speedTextBox = (gcnew System::Windows::Forms::TextBox());
			this->cpuComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->fpgaComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->clearButton = (gcnew System::Windows::Forms::Button());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->outputComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->dbgComboBox = (gcnew System::Windows::Forms::ComboBox());
			this->disconnectButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// sendButton
			// 
			this->sendButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
				(System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->sendButton->Location = System::Drawing::Point(388, 308);
			this->sendButton->Name = L"sendButton";
			this->sendButton->Size = System::Drawing::Size(78, 20);
			this->sendButton->TabIndex = 0;
			this->sendButton->Text = L"send";
			this->sendButton->UseVisualStyleBackColor = true;
			this->sendButton->Click += gcnew System::EventHandler(this, &Form1::sendButton_Click);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(476, 24);
			this->menuStrip1->TabIndex = 1;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// inputTextBox
			// 
			this->inputTextBox->AcceptsReturn = true;
			this->inputTextBox->AcceptsTab = true;
			this->inputTextBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((
				(System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->inputTextBox->Location = System::Drawing::Point(12, 308);
			this->inputTextBox->Name = L"inputTextBox";
			this->inputTextBox->Size = System::Drawing::Size(366, 20);
			this->inputTextBox->TabIndex = 2;
			// 
			// comTextBox
			// 
			this->comTextBox->Location = System::Drawing::Point(106, 12);
			this->comTextBox->Name = L"comTextBox";
			this->comTextBox->Size = System::Drawing::Size(73, 20);
			this->comTextBox->TabIndex = 4;
			this->comTextBox->Text = L"15";
			// 
			// connectButton
			// 
			this->connectButton->Location = System::Drawing::Point(302, 12);
			this->connectButton->Name = L"connectButton";
			this->connectButton->Size = System::Drawing::Size(80, 20);
			this->connectButton->TabIndex = 5;
			this->connectButton->Text = L"Connect";
			this->connectButton->UseVisualStyleBackColor = true;
			this->connectButton->Click += gcnew System::EventHandler(this, &Form1::connectButton_Click);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(9, 15);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(88, 13);
			this->label1->TabIndex = 6;
			this->label1->Text = L"Select COM port:";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(185, 15);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 13);
			this->label2->TabIndex = 7;
			this->label2->Text = L"Baud-rate";
			// 
			// speedTextBox
			// 
			this->speedTextBox->Location = System::Drawing::Point(239, 12);
			this->speedTextBox->Name = L"speedTextBox";
			this->speedTextBox->Size = System::Drawing::Size(57, 20);
			this->speedTextBox->TabIndex = 8;
			this->speedTextBox->Text = L"9600";
			this->speedTextBox->TextChanged += gcnew System::EventHandler(this, &Form1::speedTextBox_TextChanged);
			// 
			// cpuComboBox
			// 
			this->cpuComboBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
				(System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->cpuComboBox->FormattingEnabled = true;
			this->cpuComboBox->Location = System::Drawing::Point(106, 358);
			this->cpuComboBox->Name = L"cpuComboBox";
			this->cpuComboBox->Size = System::Drawing::Size(80, 21);
			this->cpuComboBox->TabIndex = 9;
			this->cpuComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::cpuComboBox_SelectedIndexChanged);
			// 
			// label3
			// 
			this->label3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
				(System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(106, 339);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(82, 13);
			this->label3->TabIndex = 10;
			this->label3->Text = L"CPU operation: ";
			// 
			// label4
			// 
			this->label4->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
				(System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(12, 339);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(88, 13);
			this->label4->TabIndex = 11;
			this->label4->Text = L"FPGA operation: ";
			// 
			// fpgaComboBox
			// 
			this->fpgaComboBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
				(System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->fpgaComboBox->FormattingEnabled = true;
			this->fpgaComboBox->Location = System::Drawing::Point(12, 358);
			this->fpgaComboBox->Name = L"fpgaComboBox";
			this->fpgaComboBox->Size = System::Drawing::Size(80, 21);
			this->fpgaComboBox->TabIndex = 12;
			this->fpgaComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::fpgaComboBox_SelectedIndexChanged);
			// 
			// clearButton
			// 
			this->clearButton->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
				(System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->clearButton->Location = System::Drawing::Point(388, 343);
			this->clearButton->Name = L"clearButton";
			this->clearButton->Size = System::Drawing::Size(78, 36);
			this->clearButton->TabIndex = 13;
			this->clearButton->Text = L"clear";
			this->clearButton->UseVisualStyleBackColor = false;
			this->clearButton->Click += gcnew System::EventHandler(this, &Form1::clearButton_Click);
			// 
			// richTextBox1
			// 
			this->richTextBox1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((
				(System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->richTextBox1->Location = System::Drawing::Point(12, 38);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->Size = System::Drawing::Size(454, 264);
			this->richTextBox1->TabIndex = 14;
			this->richTextBox1->Text = L"";
			// 
			// outputComboBox
			// 
			this->outputComboBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
				(System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->outputComboBox->FormattingEnabled = true;
			this->outputComboBox->Location = System::Drawing::Point(204, 358);
			this->outputComboBox->Name = L"outputComboBox";
			this->outputComboBox->Size = System::Drawing::Size(82, 21);
			this->outputComboBox->TabIndex = 15;
			this->outputComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::outputComboBox_SelectedIndexChanged);
			// 
			// label5
			// 
			this->label5->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
				(System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(201, 339);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(71, 13);
			this->label5->TabIndex = 16;
			this->label5->Text = L"Output format";
			// 
			// label6
			// 
			this->label6->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
				(System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(299, 339);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(76, 13);
			this->label6->TabIndex = 17;
			this->label6->Text = L"PC Debug info";
			// 
			// dbgComboBox
			// 
			this->dbgComboBox->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(
				(System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->dbgComboBox->FormattingEnabled = true;
			this->dbgComboBox->Location = System::Drawing::Point(301, 358);
			this->dbgComboBox->Name = L"dbgComboBox";
			this->dbgComboBox->Size = System::Drawing::Size(77, 21);
			this->dbgComboBox->TabIndex = 18;
			this->dbgComboBox->SelectedIndexChanged += gcnew System::EventHandler(this, &Form1::dbgComboBox_SelectedIndexChanged);
			// 
			// disconnectButton
			// 
			this->disconnectButton->Location = System::Drawing::Point(388, 12);
			this->disconnectButton->Name = L"disconnectButton";
			this->disconnectButton->Size = System::Drawing::Size(78, 20);
			this->disconnectButton->TabIndex = 19;
			this->disconnectButton->Text = L"Disconnect";
			this->disconnectButton->UseVisualStyleBackColor = true;
			this->disconnectButton->Click += gcnew System::EventHandler(this, &Form1::disconnectButton_Click);
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Center;
			this->ClientSize = System::Drawing::Size(476, 387);
			this->Controls->Add(this->disconnectButton);
			this->Controls->Add(this->dbgComboBox);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->outputComboBox);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->clearButton);
			this->Controls->Add(this->fpgaComboBox);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->cpuComboBox);
			this->Controls->Add(this->speedTextBox);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->connectButton);
			this->Controls->Add(this->comTextBox);
			this->Controls->Add(this->inputTextBox);
			this->Controls->Add(this->sendButton);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->MinimumSize = System::Drawing::Size(492, 426);
			this->Name = L"Form1";
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterScreen;
			this->Text = L"Present_Demo";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
		////////////////////////////////////////////////////////////////////
		//Funcții callback
		////////////////////////////////////////////////////////////////////

				   //Funcția buttonului de conectare
	private: System::Void connectButton_Click(
		System::Object^ sender,
		System::EventArgs^ e) {
		//
		String^ com_port = "\\\\.\\COM" + comTextBox->Text;
		for (int i = 0; i < com_port->Length && i < 100; i++) {
			com_port_buff[i] = com_port[i];
		}
		//creare port
		port = open_serial_port(com_port_buff, baud_rate);
		if (port == INVALID_HANDLE_VALUE) {
			richTextBox1->Text += "Could not connect to com port "
				+ comTextBox->Text + "\n";
		}
		else {
			conected = 1;
			richTextBox1->Text += "Conecction succesfull!\n";
		}
	}

		   //Funcția butonului de deconectare
	private: System::Void disconnectButton_Click(
		System::Object^ sender,
		System::EventArgs^ e) {
		if (port == INVALID_HANDLE_VALUE) { return; }
		CloseHandle(port);
		conected = 0;
		richTextBox1->Text += "Disconecction succesfull!\n";
	}

		   //Funcția de introducere baud-rate
	private: System::Void speedTextBox_TextChanged(
		System::Object^ sender,
		System::EventArgs^ e) {
		try {
			baud_rate = Convert::ToInt32(speedTextBox->Text);
		}
		catch (FormatException^ e) {
			richTextBox1->Text += "Baud-rate must be integer!\n";
		}
	}

		   //Funcția de curățare consolă
	private: System::Void clearButton_Click(
		System::Object^ sender,
		System::EventArgs^ e) {
		richTextBox1->Text = "";
	}

		   //Funcția meniului de debug
	private: System::Void dbgComboBox_SelectedIndexChanged(
		System::Object^ sender,
		System::EventArgs^ e) {
		//Pentru afișarea cheii de decriptare
		if (!System::String::Compare(dbgComboBox->Text, "Decode Key")) {
			recv_index += std::sprintf(&recv_buff[recv_index],
				"\nPC Decode key: ");
			//Actualizare cheie pana la K_32
			for (uint32_t counter = 0; counter < 31; counter++) {
				update_key(encript_key, counter + 1);
			}
			//Stocare K_32 in recv_buff
			print_as_hex(encript_key, 3);
			//Revenire la cheia inițială
			for (char counter = 31; counter > 0; counter--) {
				reverse_key(encript_key, counter);
			}
			//Afișare K_32
			richTextBox1->Text += gcnew String(recv_buff);
			recv_index = 0;
			return;
		}
		//Pentru afișarea cheii de criptare
		if (!System::String::Compare(dbgComboBox->Text, "Encode Key")) {
			recv_index += std::sprintf(&recv_buff[recv_index],
				"\nPC Encode key: ");
			print_as_hex(decript_key, 3);
			richTextBox1->Text += gcnew String(recv_buff);
			recv_index = 0;
			return;
		}
	}

		   //Funția butonului send
	private: System::Void sendButton_Click(
		System::Object^ sender,
		System::EventArgs^ e) {
		if (!conected) {
			richTextBox1->Text += "Connect to device to send data.\n";
			return;
		}
		//apelează algoritmul de stabilirea cheii secrete
		if (cpu_op == 'K' || fpga_op == 'K')
		{
			key_agreement(port, decript_key);
			cpy(encript_key, decript_key, data_size);
			richTextBox1->Text += gcnew String(recv_buff);
			recv_index = 0;
			return;
		}
		std::string sent_op = (fpga_op == 'D') ?
			" for decription" :
			((fpga_op == 'E') ? " for encription" : "");
		std::string recv_op = (cpu_op == 'D') ?
			" and decoded" :
			((cpu_op == 'E') ? " and encoded" : "");

		recv_index += std::sprintf(&recv_buff[recv_index],
			"\nText sent%0s.\nText recived%0s:\n",
			sent_op.c_str(), recv_op.c_str());

		//fir de execuție pentru funția recive_data
		std::thread thread_1(recive_data,
			port, cpu_op, (cpu_op == 'D' ? decript_key : encript_key));

		char* buff = (char*)(void*)Marshal::StringToHGlobalAnsi(inputTextBox->Text);
		send_data(buff, port, fpga_op);

		//la finalizarea scrierii și citirii se afișează în consolă
		thread_1.join();
		richTextBox1->Text += gcnew String(recv_buff);
		recv_index = 0;
	}

		   //Funcția meniului de operație pe fpga
	private: System::Void fpgaComboBox_SelectedIndexChanged(
		System::Object^ sender,
		System::EventArgs^ e) {
		if (!System::String::Compare(fpgaComboBox->Text, "None")) {
			fpga_op = 'N';
			return;
		}
		if (!System::String::Compare(fpgaComboBox->Text, "DF key")) {
			fpga_op = 'K';
			return;
		}
		if (!System::String::Compare(fpgaComboBox->Text, "Encript")) {
			fpga_op = 'E';
			return;
		}
		if (!System::String::Compare(fpgaComboBox->Text, "Decript")) {
			fpga_op = 'D';
			return;
		}
	}

		   //Funcția meniului de operație pe computer
	private: System::Void cpuComboBox_SelectedIndexChanged(
		System::Object^ sender,
		System::EventArgs^ e) {
		if (!System::String::Compare(cpuComboBox->Text, "None")) {
			cpu_op = 'N';
			return;
		}
		if (!System::String::Compare(cpuComboBox->Text, "DF key")) {
			cpu_op = 'K';
			return;
		}
		if (!System::String::Compare(cpuComboBox->Text, "Encript")) {
			cpu_op = 'E';
			return;
		}
		if (!System::String::Compare(cpuComboBox->Text, "Decript")) {
			cpu_op = 'D';
			return;
		}
	}

		   //Funcția meniului de output
	private: System::Void outputComboBox_SelectedIndexChanged(
		System::Object^ sender,
		System::EventArgs^ e) {
		if (!System::String::Compare(outputComboBox->Text, "Hex")) {
			out_op = 'H';
			return;
		}
		if (!System::String::Compare(outputComboBox->Text, "String")) {
			out_op = 'S';
			return;
		}
	}
	};
}