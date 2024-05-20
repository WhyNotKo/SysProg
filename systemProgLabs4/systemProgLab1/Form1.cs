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
        static extern int Connect(int port);

        [DllImport("TransportDLL", CharSet = CharSet.Ansi)]
        static extern void Disconnect();

        [DllImport("TransportDLL", CharSet = CharSet.Ansi)]
        static extern int sendMessageTo(int addr, StringBuilder data);

        [DllImport("TransportDLL", CharSet = CharSet.Ansi)]
        static extern int ProcessMessages(int maxBufferLength, ref int mes_code, StringBuilder buffer);


        const int maxResponseLength = 100;
        StringBuilder responseBuilder = new StringBuilder(maxResponseLength);
        CancellationTokenSource cancelTokenSource;
        bool connectionInitialized = false;

        public Form1()
        {
            InitializeComponent();
        }
        private void updateReceivedMessages(int responseLength)
        {
            receivedMessageList.Invoke((MethodInvoker)(() =>
            {
                receivedMessageList.Items.Add(responseBuilder.ToString().Remove(responseLength, responseBuilder.ToString().Length - responseLength));
                responseBuilder.Clear();
            }));
            
        }


        private void updateThreadsList((int, string) parseString)
        {
            threadsBox.Invoke((MethodInvoker)(() =>
            {
                threadsBox.Text = String.Empty;
                threadsBox.Items.Clear();

                int nLength = parseString.Item1;
                string idString = parseString.Item2; ;

                if (parseString.Item1 == 0)
                    return;


                string[] ids = idString.Remove(nLength, idString.Length - nLength).Split(',');


                threadsBox.Items.Add(new ComboBoxItem { Text = "main", Value =  0 });
                foreach (var id in ids)
                {
                    ComboBoxItem item = new ComboBoxItem();
                    item.Text = id;
                    item.Value = Int32.Parse(id);
                    threadsBox.Items.Add(item);
                }
                threadsBox.Items.Add(new ComboBoxItem { Text = "All threads", Value = 50 });
            }));

            
        }

       
        private void connectButton_Click(object sender, EventArgs e)
        {
            if (connectionInitialized)
                return ;
            int netId = Connect(12345);
            myId.Text = netId.ToString();

            connectionInitialized = true;
            Thread.Sleep(1000);
            cancelTokenSource = new CancellationTokenSource();
            CancellationToken token = cancelTokenSource.Token;

            Task getMessages = Task.Run(() =>
                {
                    while (!token.IsCancellationRequested)
                    {
                        int messageCode = 0;
                        int length = ProcessMessages(maxResponseLength, ref messageCode, responseBuilder);
                        if (messageCode == 3)
                        {
                            updateReceivedMessages(length);        
                        }
                        else if (messageCode == 6)
                        {
                            updateThreadsList((length, responseBuilder.ToString()));
                        }
                        responseBuilder.Clear();
                        Thread.Sleep(5000);
                    }
                },
                token
            );

        }

        private int getIdFromThreadBox()
        {
            if (threadsBox.SelectedItem != null)
                return Convert.ToInt32((threadsBox.SelectedItem as ComboBoxItem).Value);
            return 0;
        }

        private void disconnectButton_Click(object sender, EventArgs e)
        {
            cancelTokenSource.Cancel();
            Disconnect();
            connectionInitialized = false;
        }

        private void buttonSend_Click(object sender, EventArgs e)
        {
            if (connectionInitialized && messageBox.Text != "")
            {
                sendMessageTo(getIdFromThreadBox(), new StringBuilder(messageBox.Text));
            }
        }

    
    }
}
