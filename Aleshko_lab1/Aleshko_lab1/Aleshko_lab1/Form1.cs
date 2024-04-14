using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Aleshko_lab1
{
    public partial class Form1 : Form
    {
        [DllImport("TransferDLL", CharSet = CharSet.Ansi)]
        static extern bool startApp();


        [DllImport("TransferDLL", CharSet = CharSet.Ansi)]
        static extern void startThread();

        [DllImport("TransferDLL", CharSet = CharSet.Ansi)]
        static extern void stopThread();

        [DllImport("TransferDLL", CharSet = CharSet.Ansi)]
        static extern void stopAllThreads();

        [DllImport("TransferDLL", CharSet = CharSet.Ansi)]
        static extern void sendMessage(int addr, StringBuilder sb);

        Process childProcess = null;
/*        EventWaitHandle eventStart = new EventWaitHandle(false, EventResetMode.AutoReset, "StartEvent");
        EventWaitHandle eventConfirm = new EventWaitHandle(false, EventResetMode.AutoReset, "ConfirmEvent");
        EventWaitHandle eventClose = new EventWaitHandle(false, EventResetMode.AutoReset, "CloseProc");
        EventWaitHandle eventExit = new EventWaitHandle(false, EventResetMode.AutoReset, "ExitProc");*/
        public Form1()
        {
            InitializeComponent();
        }

        int threadsCounter = 0;
        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void listBox1_SelectedIndexChanged(object sender, EventArgs e)
        {

        }

        private void btn_start_Click(object sender, EventArgs e)
        {
            
            if (childProcess == null || childProcess.HasExited)
            {
                threadsCounter = 2;
                listBox1.Items.Clear();
                childProcess = Process.Start("AleshkoConsoleApp.exe");

                listBox1.Items.Add("Основной");

            }
            else
            {
                int n = Convert.ToInt32(numericUpDown1.Value);
                if(listBox1.Items.Count != 1)
                    listBox1.Items.RemoveAt(listBox1.Items.Count - 1);
                for (int i = 0; i < n; ++i)
                {
                    startThread();
                    listBox1.Items.Add(threadsCounter++).ToString();
                }
                listBox1.Items.Add("Все потоки");
            }
        }

        private void btn_stop_Click(object sender, EventArgs e)
        {
            if (childProcess == null || childProcess.HasExited)
            {
                listBox1.Items.Clear();
            }
            else
            {

                stopThread();

                if (listBox1.Items.Count > 3)
                {
                    listBox1.Items.RemoveAt(listBox1.Items.Count - 2);
                }
                else if (listBox1.Items.Count == 3)
                {
                    listBox1.Items.RemoveAt(listBox1.Items.Count - 1);
                    listBox1.Items.RemoveAt(listBox1.Items.Count - 1);
                }
                else
                {
                    listBox1.Items.Clear();
                }

            }
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {

        }

        private void btn_send_Click(object sender, EventArgs e)
        {
            int index = listBox1.SelectedIndex;
            string message = textBox.Text;


            if (index == listBox1.Items.Count - 1)
            {
                sendMessage(-1, new StringBuilder(message));
            }
            else
            {
                sendMessage(index, new StringBuilder(message));
            }
        }

        private void textBox_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
