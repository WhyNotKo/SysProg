using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Windows.Forms.Design;
using System.Xml.Schema;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace systemProgLab1
{
    public partial class Form1 : Form
    {
        [DllImport("TransportDLL", CharSet = CharSet.Ansi)]
        static extern int startThread(StringBuilder responseMessage);

        [DllImport("TransportDLL", CharSet = CharSet.Ansi)]
        static extern int stopThread(int addr, StringBuilder responseMessage);

        [DllImport("TransportDLL", CharSet = CharSet.Ansi)]
        static extern int Connect(int port);

        [DllImport("TransportDLL", CharSet = CharSet.Ansi)]
        static extern int sendMessageTo(int addr, StringBuilder data, StringBuilder responseMessage);

        [DllImport("TransportDLL", CharSet = CharSet.Ansi)]

        static extern int getWorkThreads(int maxLength, StringBuilder lpBuffer);

        int connected = 0;

        const int maxResponseLength = 64;
        StringBuilder responseBuilder = new StringBuilder(maxResponseLength);

        public Form1()
        {
            InitializeComponent();
        }
        private void updateResponseField(int responseLength)
        {
            responseList.Items.Add(responseBuilder.ToString().Remove(responseLength, responseBuilder.ToString().Length - responseLength));
            responseList.SelectedIndex = responseList.Items.Count - 1;
            responseBuilder.Clear();
        }

        private (int, string) getWorkThreadsId()
        { 
            const int maxPathLength = 8192;
            StringBuilder builder = new StringBuilder(maxPathLength);
            int nLength = getWorkThreads(maxPathLength, builder);
            return (nLength, builder.ToString());
        }

        private void updateThreadsList((int, string) parseString)
        {

            ThreadsList.Items.Clear();

            int nLength = parseString.Item1;
            string idString = parseString.Item2; ;

            if (parseString.Item1 == 0)
                return;


            string[] ids = idString.Remove(nLength, idString.Length - nLength).Split(',');


            //ThreadsList.Items.Add("main");
            foreach (var id in ids)
            {
                ThreadsList.Items.Add(id);
            }
            ThreadsList.Items.Add("All");
        }

        private void startButton_Click(object sender, EventArgs e)
        {  
            if (connected == 0)
            {
                connected = Connect(12345);
            }
            else
            {          
                int n = Convert.ToInt32(numericUpDown1.Value);

                for (int i = 0; i < n; ++i)
                {            
                    updateResponseField(startThread(responseBuilder));                
                }
                updateThreadsList(getWorkThreadsId());
            }

        }


        private void stopButton_Click(object sender, EventArgs e)
        {
            if (Int32.TryParse(ThreadsList.SelectedItem.ToString(), out int id))
            {
                updateResponseField(stopThread(id, responseBuilder));
            }
            else return;

            updateThreadsList(getWorkThreadsId());
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            string message = messageBox.Text;
            if (!Int32.TryParse(ThreadsList.SelectedItem.ToString(), out int id))
            {
                id = -1;
            }
            updateResponseField(sendMessageTo(id, new StringBuilder(message), responseBuilder));
        }

        private void receiveThreadsButton_Click(object sender, EventArgs e)
        {
            updateThreadsList(getWorkThreadsId());
        }

    }
}
