namespace GfxEditor
{
    partial class Form1
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
            this.firmware = new System.Windows.Forms.TextBox();
            this.firmwareBrowse = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.gfx = new System.Windows.Forms.ComboBox();
            this.m_address = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.m_width = new System.Windows.Forms.TextBox();
            this.m_height = new System.Windows.Forms.TextBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.label5 = new System.Windows.Forms.Label();
            this.button2 = new System.Windows.Forms.Button();
            this.imageTextBox = new System.Windows.Forms.TextBox();
            this.pngImage = new System.Windows.Forms.PictureBox();
            this.replace = new System.Windows.Forms.Button();
            this.save = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pngImage)).BeginInit();
            this.SuspendLayout();
            // 
            // firmware
            // 
            this.firmware.Location = new System.Drawing.Point(99, 12);
            this.firmware.Name = "firmware";
            this.firmware.ReadOnly = true;
            this.firmware.Size = new System.Drawing.Size(129, 20);
            this.firmware.TabIndex = 0;
            this.firmware.TextChanged += new System.EventHandler(this.firmware_TextChanged);
            // 
            // firmwareBrowse
            // 
            this.firmwareBrowse.Location = new System.Drawing.Point(234, 12);
            this.firmwareBrowse.Name = "firmwareBrowse";
            this.firmwareBrowse.Size = new System.Drawing.Size(38, 20);
            this.firmwareBrowse.TabIndex = 1;
            this.firmwareBrowse.Text = "...";
            this.firmwareBrowse.UseVisualStyleBackColor = true;
            this.firmwareBrowse.Click += new System.EventHandler(this.firmwareBrowse_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(13, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(49, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "Firmware";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(13, 40);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(44, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "Graphic";
            // 
            // gfx
            // 
            this.gfx.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.gfx.FormattingEnabled = true;
            this.gfx.Location = new System.Drawing.Point(99, 40);
            this.gfx.Name = "gfx";
            this.gfx.Size = new System.Drawing.Size(173, 21);
            this.gfx.TabIndex = 4;
            this.gfx.SelectedIndexChanged += new System.EventHandler(this.gfx_SelectedIndexChanged);
            // 
            // m_address
            // 
            this.m_address.Location = new System.Drawing.Point(99, 67);
            this.m_address.Name = "m_address";
            this.m_address.ScrollBars = System.Windows.Forms.ScrollBars.Horizontal;
            this.m_address.Size = new System.Drawing.Size(173, 20);
            this.m_address.TabIndex = 5;
            this.m_address.TextChanged += new System.EventHandler(this.m_address_TextChanged);
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(13, 67);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(45, 13);
            this.label3.TabIndex = 6;
            this.label3.Text = "Address";
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(13, 94);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(27, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "Size";
            // 
            // m_width
            // 
            this.m_width.Location = new System.Drawing.Point(99, 94);
            this.m_width.Name = "m_width";
            this.m_width.Size = new System.Drawing.Size(84, 20);
            this.m_width.TabIndex = 8;
            this.m_width.TextChanged += new System.EventHandler(this.sizeChanged);
            // 
            // m_height
            // 
            this.m_height.Location = new System.Drawing.Point(188, 94);
            this.m_height.Name = "m_height";
            this.m_height.Size = new System.Drawing.Size(84, 20);
            this.m_height.TabIndex = 9;
            this.m_height.TextChanged += new System.EventHandler(this.sizeChanged);
            // 
            // pictureBox1
            // 
            this.pictureBox1.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.pictureBox1.Location = new System.Drawing.Point(16, 128);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(256, 88);
            this.pictureBox1.TabIndex = 10;
            this.pictureBox1.TabStop = false;
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(13, 235);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(67, 13);
            this.label5.TabIndex = 13;
            this.label5.Text = "New graphic";
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(234, 235);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(38, 20);
            this.button2.TabIndex = 12;
            this.button2.Text = "...";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.imageBrowseClicked);
            // 
            // imageTextBox
            // 
            this.imageTextBox.Location = new System.Drawing.Point(99, 235);
            this.imageTextBox.Name = "imageTextBox";
            this.imageTextBox.ReadOnly = true;
            this.imageTextBox.Size = new System.Drawing.Size(129, 20);
            this.imageTextBox.TabIndex = 11;
            this.imageTextBox.TextChanged += new System.EventHandler(this.imageChanged);
            // 
            // pngImage
            // 
            this.pngImage.BackColor = System.Drawing.SystemColors.ActiveBorder;
            this.pngImage.Location = new System.Drawing.Point(16, 261);
            this.pngImage.Name = "pngImage";
            this.pngImage.Size = new System.Drawing.Size(256, 88);
            this.pngImage.TabIndex = 14;
            this.pngImage.TabStop = false;
            // 
            // replace
            // 
            this.replace.Location = new System.Drawing.Point(16, 366);
            this.replace.Name = "replace";
            this.replace.Size = new System.Drawing.Size(256, 23);
            this.replace.TabIndex = 15;
            this.replace.Text = "Replace";
            this.replace.UseVisualStyleBackColor = true;
            this.replace.Click += new System.EventHandler(this.replaceClicked);
            // 
            // save
            // 
            this.save.Location = new System.Drawing.Point(16, 395);
            this.save.Name = "save";
            this.save.Size = new System.Drawing.Size(256, 23);
            this.save.TabIndex = 16;
            this.save.Text = "Save";
            this.save.UseVisualStyleBackColor = true;
            this.save.Click += new System.EventHandler(this.saveClicked);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(284, 429);
            this.Controls.Add(this.save);
            this.Controls.Add(this.replace);
            this.Controls.Add(this.pngImage);
            this.Controls.Add(this.label5);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.imageTextBox);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.m_height);
            this.Controls.Add(this.m_width);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.m_address);
            this.Controls.Add(this.gfx);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.firmwareBrowse);
            this.Controls.Add(this.firmware);
            this.Name = "Form1";
            this.Text = "FlySky I6 editor";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pngImage)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox firmware;
        private System.Windows.Forms.Button firmwareBrowse;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox gfx;
        private System.Windows.Forms.TextBox m_address;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.TextBox m_width;
        private System.Windows.Forms.TextBox m_height;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.TextBox imageTextBox;
        private System.Windows.Forms.PictureBox pngImage;
        private System.Windows.Forms.Button replace;
        private System.Windows.Forms.Button save;
    }
}

