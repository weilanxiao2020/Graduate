namespace Client.UI
{
    partial class MainForm
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.Txt_Receiver = new System.Windows.Forms.Label();
            this.Tb_Receiver = new System.Windows.Forms.TextBox();
            this.Txt_Address = new System.Windows.Forms.Label();
            this.Tb_Address = new System.Windows.Forms.TextBox();
            this.Txt_Phone = new System.Windows.Forms.Label();
            this.Tb_Phone = new System.Windows.Forms.TextBox();
            this.Txt_Sender = new System.Windows.Forms.Label();
            this.Tb_Sender = new System.Windows.Forms.TextBox();
            this.Btn_Create = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // txtReceiver
            // 
            this.Txt_Receiver.AutoSize = true;
            this.Txt_Receiver.Location = new System.Drawing.Point(34, 24);
            this.Txt_Receiver.Name = "txtReceiver";
            this.Txt_Receiver.Size = new System.Drawing.Size(52, 15);
            this.Txt_Receiver.TabIndex = 0;
            this.Txt_Receiver.Text = "收件人";
            // 
            // Tb_Receiver
            // 
            this.Tb_Receiver.Location = new System.Drawing.Point(114, 14);
            this.Tb_Receiver.Name = "Tb_Receiver";
            this.Tb_Receiver.Size = new System.Drawing.Size(163, 25);
            this.Tb_Receiver.TabIndex = 1;
            // 
            // txtAddress
            // 
            this.Txt_Address.AutoSize = true;
            this.Txt_Address.Location = new System.Drawing.Point(34, 111);
            this.Txt_Address.Name = "txtAddress";
            this.Txt_Address.Size = new System.Drawing.Size(67, 15);
            this.Txt_Address.TabIndex = 2;
            this.Txt_Address.Text = "详细地址";
            // 
            // Tb_Address
            // 
            this.Tb_Address.Location = new System.Drawing.Point(114, 101);
            this.Tb_Address.Name = "Tb_Address";
            this.Tb_Address.Size = new System.Drawing.Size(259, 25);
            this.Tb_Address.TabIndex = 3;
            // 
            // txtPhone
            // 
            this.Txt_Phone.AutoSize = true;
            this.Txt_Phone.Location = new System.Drawing.Point(34, 66);
            this.Txt_Phone.Name = "txtPhone";
            this.Txt_Phone.Size = new System.Drawing.Size(67, 15);
            this.Txt_Phone.TabIndex = 4;
            this.Txt_Phone.Text = "联系方式";
            // 
            // Tb_Phone
            // 
            this.Tb_Phone.Location = new System.Drawing.Point(114, 56);
            this.Tb_Phone.MaxLength = 11;
            this.Tb_Phone.Name = "Tb_Phone";
            this.Tb_Phone.Size = new System.Drawing.Size(163, 25);
            this.Tb_Phone.TabIndex = 5;
            this.Tb_Phone.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.Tb_Phone_KeyPress);
            // 
            // label1
            // 
            this.Txt_Sender.AutoSize = true;
            this.Txt_Sender.Location = new System.Drawing.Point(460, 23);
            this.Txt_Sender.Name = "label1";
            this.Txt_Sender.Size = new System.Drawing.Size(52, 15);
            this.Txt_Sender.TabIndex = 6;
            this.Txt_Sender.Text = "寄件人";
            // 
            // Tb_Sender
            // 
            this.Tb_Sender.Location = new System.Drawing.Point(518, 13);
            this.Tb_Sender.Name = "Tb_Sender";
            this.Tb_Sender.Size = new System.Drawing.Size(172, 25);
            this.Tb_Sender.TabIndex = 7;
            // 
            // Btn_Create
            // 
            this.Btn_Create.Location = new System.Drawing.Point(114, 160);
            this.Btn_Create.Name = "Btn_Create";
            this.Btn_Create.Size = new System.Drawing.Size(75, 50);
            this.Btn_Create.TabIndex = 8;
            this.Btn_Create.Text = "创建";
            this.Btn_Create.UseVisualStyleBackColor = true;
            this.Btn_Create.Click += new System.EventHandler(this.Btn_Create_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(800, 450);
            this.Controls.Add(this.Btn_Create);
            this.Controls.Add(this.Tb_Sender);
            this.Controls.Add(this.Txt_Sender);
            this.Controls.Add(this.Tb_Phone);
            this.Controls.Add(this.Txt_Phone);
            this.Controls.Add(this.Tb_Address);
            this.Controls.Add(this.Txt_Address);
            this.Controls.Add(this.Tb_Receiver);
            this.Controls.Add(this.Txt_Receiver);
            this.Name = "MainForm";
            this.Text = "Main";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label Txt_Receiver;
        private System.Windows.Forms.TextBox Tb_Receiver;
        private System.Windows.Forms.Label Txt_Address;
        private System.Windows.Forms.TextBox Tb_Address;
        private System.Windows.Forms.Label Txt_Phone;
        private System.Windows.Forms.TextBox Tb_Phone;
        private System.Windows.Forms.Label Txt_Sender;
        private System.Windows.Forms.TextBox Tb_Sender;
        private System.Windows.Forms.Button Btn_Create;
    }
}