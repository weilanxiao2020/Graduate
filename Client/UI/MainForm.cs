using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Diagnostics;

namespace Client.UI
{
    public partial class MainForm : Form
    {
        private string _sender;
        private string _receiver;
        private string _phone;
        private string _address;

        public MainForm()
        {
            InitializeComponent();
        }

        private void Tb_Phone_KeyPress(object sender, KeyPressEventArgs e)
        {
            if (e.KeyChar != '\b' && !Char.IsDigit(e.KeyChar))
            {
                Debug.WriteLine("请输入数字");
                e.Handled = true;
            }
            else if (e.KeyChar == '-' && ((TextBox)sender).Text.Length > 1)
            {
                e.Handled = true;
            }
        }

        private void Btn_Create_Click(object sender, EventArgs e)
        {
            _sender = Tb_Sender.Text.Trim();
            _receiver = Tb_Receiver.Text.Trim();
            _phone = Tb_Phone.Text.Trim();
            _address = Tb_Address.Text.Trim();
            Debug.WriteLine("寄件人:" + ("".Equals(_sender) ? "none": _sender));
            Debug.WriteLine("收件人:" + ("".Equals(_receiver) ? "none": _receiver));
            Debug.WriteLine("联系电话:" + ("".Equals(_phone) ? "none": _phone));
            Debug.WriteLine("收件地址:" + ("".Equals(_address) ? "none": _address));
        }
    }
}
