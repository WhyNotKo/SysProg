namespace WindowsFormsApp1
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.start_but = new System.Windows.Forms.Button();
            this.stop_but = new System.Windows.Forms.Button();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.SuspendLayout();
            // 
            // listBox1
            // 
            this.listBox1.FormattingEnabled = true;
            this.listBox1.Location = new System.Drawing.Point(22, 37);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(183, 225);
            this.listBox1.TabIndex = 0;
            // 
            // start_but
            // 
            this.start_but.Location = new System.Drawing.Point(265, 73);
            this.start_but.Name = "start_but";
            this.start_but.Size = new System.Drawing.Size(75, 23);
            this.start_but.TabIndex = 1;
            this.start_but.Text = "Start";
            this.start_but.UseVisualStyleBackColor = true;
            this.start_but.Click += new System.EventHandler(this.start_but_Click);
            // 
            // stop_but
            // 
            this.stop_but.Location = new System.Drawing.Point(265, 119);
            this.stop_but.Name = "stop_but";
            this.stop_but.Size = new System.Drawing.Size(75, 23);
            this.stop_but.TabIndex = 2;
            this.stop_but.Text = "Stop";
            this.stop_but.UseVisualStyleBackColor = true;
            this.stop_but.Click += new System.EventHandler(this.stop_but_Click);
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Location = new System.Drawing.Point(265, 165);
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(75, 20);
            this.numericUpDown1.TabIndex = 3;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(389, 275);
            this.Controls.Add(this.numericUpDown1);
            this.Controls.Add(this.stop_but);
            this.Controls.Add(this.start_but);
            this.Controls.Add(this.listBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.ListBox listBox1;
        private System.Windows.Forms.Button start_but;
        private System.Windows.Forms.Button stop_but;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
    }
}

