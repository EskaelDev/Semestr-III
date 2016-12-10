using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Piate_spotkanie_WinForm
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {

        }

        private void WriteLog(string message)
        {
            listBox1.Items.Insert(0,
                String.Format("{0} {1} {2}", DateTime.Now.TimeOfDay.ToString(),
                Thread.CurrentThread.ManagedThreadId,
                message));
        }

        private async void button2_Click(object sender, EventArgs e)
        {
            button2.Enabled = false;
            WriteLog("Startuje 1");
            for (int i = 0; i < 20; i++)
            {
                WriteLog(i.ToString());
                await Task.Run(() => Thread.Sleep(1000));
            }
            WriteLog("Koniec 1");
            button2.Enabled = true;
        }
    }
}
