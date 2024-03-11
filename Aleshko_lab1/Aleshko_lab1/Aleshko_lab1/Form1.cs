using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Aleshko_lab1
{
    public partial class Form1 : Form
    {
        Process childProcess = null;
        EventWaitHandle eventStart = new EventWaitHandle(false, EventResetMode.AutoReset, "StartEvent");
        EventWaitHandle eventConfirm = new EventWaitHandle(false, EventResetMode.AutoReset, "ConfirmEvent");
        EventWaitHandle eventClose = new EventWaitHandle(false, EventResetMode.AutoReset, "CloseProc");
        EventWaitHandle eventExit = new EventWaitHandle(false, EventResetMode.AutoReset, "ExitProc");
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
                listBox1.Items.Clear();
                childProcess = Process.Start("Aleshko_console.exe");
                eventConfirm.WaitOne();
                listBox1.Items.Add("Основной");
                listBox1.Items.Add("Все потоки");
                threadsCounter++;

            }
            else
            {
                int n = Convert.ToInt32(numericUpDown1.Value);

                for (int i = 0; i < n; ++i)
                {
                    eventStart.Set();
                    eventConfirm.WaitOne();
                    listBox1.Items.Add(threadsCounter++).ToString();
                }
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
                if (listBox1.Items.Count > 2)
                {
                    int n = Convert.ToInt32(numericUpDown1.Value);
                    if(n < threadsCounter)
                    {
                        for (int i = 0; i < n; ++i)
                        {
                            eventClose.Set();
                            eventConfirm.WaitOne();
                            listBox1.Items.RemoveAt(threadsCounter);
                            threadsCounter--;
                        }
                    }
                    else
                    {
                        eventClose.Set();
                        eventConfirm.WaitOne();
                        listBox1.Items.RemoveAt(threadsCounter);
                        threadsCounter--;
                    }
                    
                }
                else
                {
                    eventClose.Set();
                    eventConfirm.WaitOne();
                    listBox1.Items.Clear();
                    threadsCounter = 0;
                }
                
            }
        }

        private void numericUpDown1_ValueChanged(object sender, EventArgs e)
        {

        }
    }
}
