using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace systemProgLab1
{
    internal static class Program
    {
        /*[DllImport("TransportDLL", CharSet = CharSet.Ansi)]
        static extern void Disconnect();*/
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new Form1());
            
            //Disconnect();
        }
    }
}
